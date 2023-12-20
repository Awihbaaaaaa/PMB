#include "../Headers/declarations.hpp"

/**
 * @brief This function will calculate the likelihood that a tracked object got missed.
 *
 * It include two posibilities:
 * 1. It is propable that the object didn't generate any measurements at all.
 * 2. The object is assumed to be tracked but actually it is clutter and so it didn't generate measurements.
 *
 * @param currTrackedObj Pointer to a single potentially tracked object.
 * @param currMeasurements Pointer to a struct that includes the raw measurements data, a matrix called ingated,
 *                         that includes measurements located inside the gates around MBs.
                           and a matrix for outgated measurements
 * @param radar Pointer to the radarDefinition providing radar-related parameters.
 * @param extObj Pointer to the ExtendedObjectDefinition providing object-related parameters.
 * 
 * @return A struct that include a TrackedObj and likelihod for the input object
 */
std::vector<GGIW_result> updateMissedObjs(std::vector<TrackedObj>* currTrackedObjs, measurements* currMeasurements, radarDefinition* radar, ExtendedObjectDefinition* extObj){
    double qd;
    double currentBBeta,currentBAlpha;
    double Pd = radar->getRadarDetectionProbability();
    
    double w1, w2, l;
    TrackedObj psi1, psi2;
    std::vector<double> tempWeights;
    std::vector<TrackedObj> tempMBs;
    GGIW_result missedObj;
    std::vector<GGIW_result> missedObjs;

    TrackedObj tmpObj;

    for(TrackedObj currObj:*currTrackedObjs){
        currentBBeta = currObj.beta;
        currentBAlpha = currObj.alpha;
        qd = 1-Pd+Pd*pow((currentBBeta/(currentBBeta+1)),currentBAlpha);

        w1 = (1-Pd)/qd;
        w2 = Pd*pow((currentBBeta/(currentBBeta+1)),currentBAlpha)/qd;
        
        tempWeights.push_back(w1);
        tempWeights.push_back(w2);

        psi1 = currObj;
        psi2 = currObj;

        /* std::cout << currObj.X;
        std::cout << psi1.X; */
        psi2.beta = psi2.beta+1;

        /* std::cout << currObj;
        std::cout << psi1; */
        
        tempMBs.push_back(psi1);
        tempMBs.push_back(psi2);

        std::cout << "Merging the hypotheses that an object is missed with/without generating measurements ... " << std::endl;
        tmpObj = merge(&tempWeights, &tempMBs,extObj);

        tmpObj.r_MB = currObj.r_MB*qd/(1-currObj.r_MB+currObj.r_MB*qd)*radar->getDeathForce();

        l = log(1 - currObj.r_MB + currObj.r_MB*qd);

        missedObj.newMB = tmpObj;
        missedObj.L = l;

        missedObjs.push_back(missedObj);
    }

    return missedObjs;
}


/**
 * @brief Update the state space and existance probability of detected objects MBs.
 *
 * It consists of two major steps:
 * 1. Tracked object might be missed at this step.
 * 2. Tracked objectts might generate measurements and be tracked again.
 * 3. The tracked objects might be overlaping objects
 * 
 * These three cases will be studied and the probability of each case will be calculated.
 * Objects with low existance probability should be removed, potential  objects belong to 
 * the same real object should be merged and the state space of the tracked objects we are
 * sure they are still tracked will be updated.
 *
 * @param collection ObjectsCollection containing both tracked (MB) and untracked (PPP) objects.
 * @param currMeasurements Pointer to a struct that includes the raw measurements data, a matrix ,ingated,
 *                         that includes measurements located inside the gates around MBs.
                           and a matrix for outgated measurements
 * @param radar Pointer to the radarDefinition providing radar-related parameters.
 * @param extObj Pointer to the ExtendedObjectDefinition providing object-related parameters.
 * 
 * @return No returned values are needed, the input objects collection will be updated.
 */
void MB_update(ObjectsCollection& collection, measurements* currMeasurements, radarDefinition* radar, ExtendedObjectDefinition* extObj){
    std::cout << "Creating the hypotheses for the tracked objects ... " << std::endl;
    double maxNrHyp = 100;
    bool maximized = false;

    double prunThr = extObj->getMB_PruningThreshold();
    double exProb = extObj->getExistanceThreshold();

    // Extract the necicerily parameters for the clustering algorithm
    std::vector<double> eps = {radar->getEpsCluster()};
    int clusterNrPnt = radar->getMinNrPntsCluster();

    double state_dim = 3;
    double extDim = extObj->getExtensionDimension();

    if(collection.MB.size() == 0){
        return;
    }

    // Missdetected an object? Lower the existance probability!
    if(collection.PPP.size() == 0){
        throw std::runtime_error("No potential objects available, try to decrease the pruning threshold.");
    }

    // Calculate the missedetection probability for all the tracked objects.
    std::cout << "Finding likelihood that MBs are missed ... " << std::endl;
    std::vector<GGIW_result> missedObjUpd = updateMissedObjs(&collection.MB, currMeasurements, radar, extObj);
    
    DBSCAN clustersResult = dbscan::run(*(currMeasurements->z), eps, clusterNrPnt);
//    std::cout << clustersResult;

    std::cout << "For each available cluster, creating a potential new tracked object and calculate its likelihood as well as update the tracked objects ... ";
    std::vector<GGIW_result> newObjs;
    std::vector<GGIW_result> updateObjs;

    GGIW_result* tmpNewTrackedObjs;
    GGIW_result* tmpUpdateTrackedObj = new GGIW_result;
    double tempL;

    // Loop over the clusters and calculate the likelihood for getting new objects from each cluster.
    for(std::vector<int> c:clustersResult.c){
        Matrix currentClusterMeas = getClusterMeas(currMeasurements->z,&c);
        tmpNewTrackedObjs = newBernoulliBirth(collection,
                                            &currentClusterMeas,
                                            radar,
                                            extObj);
        newObjs.push_back(*tmpNewTrackedObjs);
        
        measurements currCluster(&currentClusterMeas);

        // For each cluster, loop over all the already tracked objects, update their states, and calculate the likelihood that they should be updated.
        for(TrackedObj obj:collection.MB){
            *tmpUpdateTrackedObj = ggiwUpdate(&obj, &currCluster,radar,extObj);
//          std::cout << tmpUpdateTrackedObj->newMB;
            tempL = tmpUpdateTrackedObj->L;
//          std::cout << tmpUpdateTrackedObj->L << std::endl;
            tmpUpdateTrackedObj->newMB.r_MB = 1;
            tmpUpdateTrackedObj->L = obj.r_MB*radar->getRadarDetectionProbability()*tmpUpdateTrackedObj->L;
//          std::cout << tmpUpdateTrackedObj->L << std::endl;
            updateObjs.push_back(*tmpUpdateTrackedObj);
        }
    }

    // Following is the 
    //|--------------------------------------------------------------|
    //|object_updates |object missed diagonal|           inf         | {object count}
    //|new MB diagonal|         inf          | ignore new MB diagonal| {cluster count}
    //|--------------------------------------------------------------|
    //|{cluster count}| {object count}       | {cluster count}
    
    

    
};
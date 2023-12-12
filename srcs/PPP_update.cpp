#include "../Headers/declarations.hpp"

/**
 * @brief Update undetected objects based on radar detections.
 *
 * This function updates the existence probabilities of untracked objects in the PPP (Poisson Point Process) model.
 * Untracked objects that remain untracked have their probabilities reduced, and those generating measurements have adjusted probabilities.
 * Objects with probabilities below a pruning threshold are removed from the collection.
 *
 * @param ObjColl ObjectsCollection containing a vector of untracked objects (PPP).
 * @param radar Pointer to the radarDefinition providing radar-related parameters.
 * @param extObj Pointer to the ExtendedObjectDefinition providing object-related parameters.
 */
void updateUndetectedObjs(ObjectsCollection& ObjColl,
                          radarDefinition* radar,
                          ExtendedObjectDefinition* extObj){
    double Pd = radar->getRadarDetectionProbability();

    std::vector<UntrackedObj> tempPPP = ObjColl.PPP;

    double PPP_threshold = extObj->getPPP_PruningThreshold();
    double updateWppp1 = 0; 
    double updateWppp2 = 0;
    int temp =0;
    for(int i = ObjColl.PPP.size() - 1; i >= 0; --i){
        updateWppp1 = (1 - Pd)*ObjColl.PPP[i].w_ppp;
        if(updateWppp1 > PPP_threshold){
            ObjColl.PPP[i].w_ppp = updateWppp1;
        }else{
            //std::cout << "object erased." <<std::endl;
            ObjColl.PPP.erase(ObjColl.PPP.begin() + i);
            //std::cout << PPP.size() << std::endl;
            temp++;
        }

        updateWppp2 = Pd*pow(tempPPP[i].beta/(tempPPP[i].beta + 1),tempPPP[i].alpha);
        if(updateWppp2 > PPP_threshold){
            tempPPP[i].w_ppp = updateWppp2;
            tempPPP[i].beta = tempPPP[i].beta + 1;
        }else{
            //std::cout << "temp object erased."<< std::endl;
            tempPPP.erase(tempPPP.begin() + i);
        }
    }

    combinePPPs(ObjColl.PPP, tempPPP);

    if(ObjColl.PPP.empty()){
        std::cerr << "All the PPP components got pruned!\n " 
                  << "After update we prune PPP components with low weights."
                  << " If no new PPPs are added for the next recursion, we cant"
                  << " create new objects in the MB update loop.\n "
                  << "Try changing initial weights of PPP or lowering Pd\n";
    }
}

/**
 * @brief Create new MBs (Tracked Objects) based on clustered measurements.
 *
 * This function processes the outgated measurements, performs ellipsoidal gating around the untracked objects to check
 * there is measurements close to any of them, and clusters the ingated measurements using the DBSCAN algorithm.
 * New MB components are created for each cluster, representing potential new tracked objects.
 *
 * @param ObjColl ObjectsCollection containing both tracked (MB) and untracked (PPP) objects.
 * @param CurrentMeasurements Pointer to measurements struct containing raw sensor data and gating information.
 * @param radar Pointer to the radarDefinition providing radar-related parameters.
 * @param extObj Pointer to the ExtendedObjectDefinition providing object-related parameters.
 */
void createNewMBs(ObjectsCollection& ObjColl,
                  measurements* CurrentMeasurements,
                  radarDefinition* radar,
                  ExtendedObjectDefinition* extObj){
    /*
    CurrentMeasurements includes measurements around MBs
    */
    std::vector<double> eps = {radar->getEpsCluster()};
    int minNrPnts = radar->getMinNrPntsCluster();

    // Local copy for the local ingating.
    measurements localCopy = *CurrentMeasurements;

    // We should extract the measurements that are outgated.
    int measRemoved = 0;
    //std::cout << *(localCopy.z);

    //std::cout << localCopy.outGated;
    // outgated raw measurements
    Matrix ogrm = *(localCopy.z);
    for(int i=0; i<localCopy.outGated.nrCols(); i++){
        //std::cout << localCopy.outGated(0,i) << "\n";
        if(localCopy.outGated(0,i)!=0){
            //std::cout << "RawMeasInsideGates " << RawMeasInsideGates.nrCols() << ", column "<< measRemoved << "\n";
            ogrm.removeColumn(measRemoved);
            
            //measRemoved--;
            //std::cout << ogrm;
            continue;
        }
        measRemoved++;
    }

    // To check which outgated measurements are located in the 
    // PPP gate.
    measurements ogrmMeas(&ogrm);
    elipsoidalGating(radar, &ObjColl,'P', ogrmMeas);
    
    /* std::cout << *(ogrmMeas.z);
    std::cout << ogrmMeas.inGated;
    std::cout << ogrmMeas.outGated; */
    /* 
    The following loop is to extract measurements located in the gates.
    Before the loop, the measurements are copied to RawMeasInsideGates.
    We assume that all measurements are inside the gates. We loop then
    over the matrix outGated. We take the index of the measurements located 
    outside the gate, then this column is removed from RawMeasInsideGates.
    To do that, two trackers are used. i to loop over outGated and measRemoved.
    measRemoved will follow i if no measurements removed. If measurements are removed
    measRemoved is decreased, because the RawMeasInsideGates is shrinked.
    */
    Matrix RawMeasInsideGates = *(CurrentMeasurements->z);
    measRemoved = 0;

    for(int i=0; i<localCopy.outGated.nrCols(); i++){
        //std::cout << localCopy.outGated(0,i) << "\n";
        if(localCopy.outGated(0,i)==0){
            //std::cout << "RawMeasInsideGates " << RawMeasInsideGates.nrCols() << ", column "<< measRemoved << "\n";
            RawMeasInsideGates.removeColumn(measRemoved);
            //measRemoved--;
            //std::cout << RawMeasInsideGates;
            continue;
        }
        measRemoved++;
    }

    DBSCAN result = dbscan::run(RawMeasInsideGates, eps, minNrPnts);

    // Check that we have any measurement that belong to a cluster
    if(!result.id.empty()){
        // Loop over the clusters
        for(std::vector<int> row:result.c){
            //Extract measurements belong to each cluster
            Matrix clusterMeasurements(3,row.size(),0.0);
            for(int i=0; i<row.size(); i++){
                clusterMeasurements.setColumn(i,RawMeasInsideGates.getColumn(row[i]));
            }
            // Find the probability of new births
            newBernoulliBirth(ObjColl,
                              &clusterMeasurements,
                              radar,
                              extObj);
        }
    }


}


/**
 * @brief Update undetected objects and create potentially new MBs based on current measurements.
 *
 * It consists of two main steps:
 * 1. Update undetected objects: Untracked objects can either remain untracked or get tracked.
 *    a. Untracked objects that remain untracked have their existence probabilities reduced.
 *    b. Untracked objects that generate measurements have their probabilities adjusted.
 * 2. Create new MBs: The probability of an object being detected is calculated, and new objects are added to the collection if detected.
 *    Detected measurements are clustered using the DBSCAN algorithm, and new Bernoulli components are created for each cluster.
 *
 * @param collection ObjectsCollection containing both tracked (MB) and untracked (PPP) objects.
 * @param currMeasurements Pointer to a struct that includes the raw measurements data, a matrix ,ingated,
 *                         that includes measurements located inside the gates around MBs.
                           and a matrix for outgated measurements
 * @param radar Pointer to the radarDefinition providing radar-related parameters.
 * @param extObj Pointer to the ExtendedObjectDefinition providing object-related parameters.
 */
void PPP_update(ObjectsCollection& collection,
                measurements* currMeasurements,
                radarDefinition* radar,
                ExtendedObjectDefinition* extObj){
                    updateUndetectedObjs(collection,
                                         radar,
                                         extObj);
                    
                    if(currMeasurements->z->nrCols() > 0){
                        createNewMBs(collection,
                                     currMeasurements,
                                     radar,
                                     extObj);
                    }

                };
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
GGIW_result updateMissedObjs(TrackedObj* currTrackedObj, measurements* currMeasurements, radarDefinition* radar, ExtendedObjectDefinition* extObj){
    double qd;
    double currentBBeta = currTrackedObj->beta;
    double currentBAlpha = currTrackedObj->alpha;
    double Pd = radar->getRadarDetectionProbability();

    qd = 1-Pd+Pd*pow((currentBBeta/(currentBBeta+1)),currentBAlpha);

    double w1 = (1-Pd)/qd;
    double w2 = Pd*pow((currentBBeta/(currentBBeta-1)),currentBAlpha)/qd;

    TrackedObj psi1 = currTrackedObj;
    TrackedObj psi2 = currTrackedObj;
    psi2.beta = psi2.beta+1;

    TrackedObj mergedObj = merge()
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
    double maxNrHyp = 100;
    bool maximized = false;

    double prunThr = extObj->getMB_PruningThreshold();
    double exProb = extObj->getExistanceThreshold();

    // Extract the necicerily parameters for the clustering algorithm
    double eps = radar->getEpsCluster();
    double nrPnt = radar->getMinNrPntsCluster();

    double state_dim = 3;
    double extDim = extObj->getExtensionDimension();

    if(collection.MB.size() == 0){
        return;
    }

    // Missdetected an object? Lower the existance probability!
    if(collection.PPP.size() == 0){
        throw std::runtime_error("No potential objects available, try to decrease the pruning threshold.");
    }

    GGIW_result missedObjUpd = updateMissedObjs(collection, currMeasurements, radar, extObj);

};
#include "../Headers/declarations.hpp"

GGIW_result* newBernoulliBirth(ObjectsCollection& PPP_objs, const Matrix* clusterMeas, radarDefinition* radar, ExtendedObjectDefinition* extObj){
    std::vector<double> MB_Likelihood;
    measurements currMeas(clusterMeas);
    GGIW_result* result = new GGIW_result;

    std::cout << "Elipsoidal gating on measurements around the PPPs ... " << std::endl;
    elipsoidalGating(radar,&PPP_objs,'P',currMeas);

    Matrix validInGatedObjs = currMeas.inGated.sum(2);
    
    if(validInGatedObjs.sum(1)(0,0)>0){
        std::vector<TrackedObj> newMBs;
        std::vector<double> likelihoods;
        std::vector<UntrackedObj> validPPP;
        std::vector<double> PPPWeights;
        double tempLikelihood;

        for(int i = 0; i < validInGatedObjs.nrRows(); i++){
            if(validInGatedObjs(i,0) !=0){
                std::cout << "GGIW update for object " << i << std::endl;
                GGIW_result ggiw_update = ggiwUpdate(&PPP_objs.PPP[i],
                                            &currMeas,
                                            radar,
                                            extObj);
                newMBs.push_back(ggiw_update.newMB);   
                tempLikelihood = radar->getRadarDetectionProbability()*PPP_objs.PPP[i].w_ppp*exp(ggiw_update.L);
                likelihoods.push_back(tempLikelihood);
                validPPP.push_back(PPP_objs.PPP[i]);
                PPPWeights.push_back(PPP_objs.PPP[i].w_ppp);
            }
        }
        double sum = std::accumulate(likelihoods.begin(),likelihoods.end(),0.0);
        result->L = sum;

        std::cout << "Merging the objects created from the same PPP and same set of measurements ... " << std::endl;
        TrackedObj mergedComponent = merge(&PPPWeights,
                                        &newMBs,
                                        extObj);

        // Update the new object existing probability based on how many measurements we have in a cluster
        if(currMeas.z->nrCols() == 1){
            mergedComponent.r_MB = sum/(radar->getCIClutterIntensity()+sum);
        }else{
            mergedComponent.r_MB = 1;
        }
        // std::cout << "Components got merged to: \n" << mergedComponent.X;
        if(mergedComponent.r_MB>extObj->getMB_PruningThreshold()){
            result->newMB = mergedComponent;
        }

    }else{
        std::cout << "No valid component found. Checking the next cluster ..." << std::endl;
    }
    return result;
};
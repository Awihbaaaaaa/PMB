#include "../Headers/declarations.hpp"

void newBernoulliBirth(ObjectsCollection& PPP_objs,Matrix* clusterMeas, radarDefinition* radar, ExtendedObjectDefinition* extObj){
    std::vector<double> MB_Likelihood;
    measurements currMeas(clusterMeas);
    //std::cout << *(currMeas.z);
    elipsoidalGating(radar,&PPP_objs,'P',currMeas);
    std::cout << "Measurements in the current cluster are :\n";
    std::cout << *(clusterMeas);

    /* std::cout << "Ingated measurements :\n";
    std::cout << currMeas.inGated; */

    Matrix validInGatedObjs = currMeas.inGated.sum(2);
    //std::cout << validInGatedObjs;
    /* std::cout << validInGatedObjs.nrRows() << std::endl;
    std::cout << PPP_objs.PPP.size() << std::endl; */
    
    if(validInGatedObjs.sum(1)(0,0)>0){
        std::vector<TrackedObj> newMBs;
        std::vector<double> likelihoods;
        std::vector<UntrackedObj> validPPP;
        std::vector<double> PPPWeights;
        double tempLikelihood;

        for(int i = 0; i < validInGatedObjs.nrRows(); i++){
            if(validInGatedObjs(i,0) !=0){
                GGIW_result ggiw_update = ggiwUpdate(&PPP_objs.PPP[i],
                                            &currMeas,
                                            radar,
                                            extObj);
                std::cout << PPP_objs.PPP[i].V;
                newMBs.push_back(ggiw_update.newMB);   
                tempLikelihood = radar->getRadarDetectionProbability()*PPP_objs.PPP[i].w_ppp*exp(ggiw_update.L);
                likelihoods.push_back(tempLikelihood);
                validPPP.push_back(PPP_objs.PPP[i]);
                PPPWeights.push_back(PPP_objs.PPP[i].w_ppp);
            }
        }
        double sum = std::accumulate(likelihoods.begin(),likelihoods.end(),0.0);
        TrackedObj mergedComponent = merge(&PPPWeights,
                                        &newMBs,
                                        extObj);
        
        // Update the new object existing probability based on how many measurements we have in a cluster
        if(currMeas.z->nrCols() == 1){
            mergedComponent.r_MB = sum/(radar->getCIClutterIntensity()+sum);
        }else{
            mergedComponent.r_MB = 1;
        }
        std::cout << "Components got merged to: \n" << mergedComponent.X;
        std::cout << mergedComponent.alpha << std::endl;
        if(mergedComponent.r_MB>extObj->getMB_PruningThreshold()){
            PPP_objs.MB.push_back(mergedComponent);
        }

    }else{
        std::cout << "No valid component found. Checking the next cluster." << std::endl;
    }


};
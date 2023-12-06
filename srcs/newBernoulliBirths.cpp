#include "../Headers/declarations.hpp"

void newBernoulliBirth(ObjectsCollection& PPP_objs,Matrix* clusterMeas, radarDefinition* radar, ExtendedObjectDefinition* extObj){
    std::vector<double> MB_Likelihood;
    measurements currMeas(clusterMeas);
    //std::cout << *(currMeas.z);
    elipsoidalGating(radar,&PPP_objs,'P',currMeas);
    std::cout << "Measurements in the current cluster are :\n";
    std::cout << *(clusterMeas);

    std::cout << "Ingated measurements :\n";
    std::cout << currMeas.inGated;

    Matrix validInGatedObjs = currMeas.inGated.sum(2);
    std::cout << validInGatedObjs;
    /* std::cout << validInGatedObjs.nrRows() << std::endl;
    std::cout << PPP_objs.PPP.size() << std::endl; */
    std::vector<TrackedObj> newMBs;
    std::vector<double> likelihoods;
    std::vector<UntrackedObj> validPPP;
    double tempLikelihood;
    for(int i = 0; i < validInGatedObjs.nrRows(); i++){
        if(validInGatedObjs(i,0) !=0){
            GGIW_result ggiw_update = ggiwUpdate(&PPP_objs.PPP[i],
                                        &currMeas,
                                        radar,
                                        extObj);
            newMBs.push_back(ggiw_update.newMB);   
            tempLikelihood = radar->getRadarDetectionProbability()*PPP_objs.PPP[i].w_ppp*exp(ggiw_update.L);
            likelihoods.push_back(tempLikelihood);
            validPPP.push_back(PPP_objs.PPP[i]);
        }
    }
    
    double sum = std::accumulate(likelihoods.begin(),likelihoods.end(),0.0);
    TrackedObj mergedComponent = merge(&validPPP,
                                       &newMBs,
                                       extObj);
};
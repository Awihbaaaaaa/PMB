#include "../Headers/declarations.hpp"

void newBernoulliBirth(ObjectsCollection& PPP_objs,Matrix* clusterMeas, radarDefinition* radar, ExtendedObjectDefinition* extObj){
    std::vector<double> MB_Likelihood;
    measurements currMeas(clusterMeas);
    //std::cout << *(currMeas.z);
    elipsoidalGating(radar,&PPP_objs,'P',currMeas);
    std::cout << *(clusterMeas);
    std::cout << currMeas.outGated;
    std::vector<TrackedObj> newMBs;
    std::vector<double> likelihoods;
    double tempLikelihood;
    for(UntrackedObj obj:PPP_objs.PPP){
        GGIW_result ggiw_update = ggiwUpdate(&obj,
                                      &currMeas,
                                      radar,
                                      extObj);
        newMBs.push_back(ggiw_update.newMB);   
        tempLikelihood = radar->getRadarDetectionProbability()*obj.w_ppp*exp(ggiw_update.L);
        likelihoods.push_back(tempLikelihood);
    }
    
    double sum = std::accumulate(likelihoods.begin(),likelihoods.end(),0.0);
    merge(PPP_objs,
          &newMBs,
          extObj);
};
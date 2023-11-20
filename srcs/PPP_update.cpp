#include "../Headers/declarations.hpp"
//#include "utilities.cpp"

void updateUndetectedObjs(std::vector<UntrackedObj>& PPP,
                          radarDefinition* radar,
                          ExtendedObjectDefinition* extObj){
    double Pd = radar->getRadarDetectionProbability();
    std::cout << PPP.size() << std::endl;

    std::vector<UntrackedObj> tempPPP = PPP;
    std::cout << "Obj 1" << ": alpha: " << PPP[0].alpha << " beta: " << PPP[0].beta << " weight: " << PPP[0].w_ppp << " " << PPP[0].X << std::endl;
    std::cout << "Obj 2" << ": alpha: " << PPP[1].alpha << " beta: " << PPP[1].beta << " weight: " << PPP[1].w_ppp << " " << PPP[1].X << std::endl;
    std::cout << "Obj 3" << ": alpha: " << PPP[2].alpha << " beta: " << PPP[2].beta << " weight: " << PPP[2].w_ppp << " " << PPP[2].X << std::endl;
    std::cout << "TempObj 1" << ": alpha: " << tempPPP[0].alpha << " beta: " << tempPPP[0].beta << " weight: " << tempPPP[0].w_ppp << " " << tempPPP[0].X << std::endl;
    std::cout << "TempObj 2" << ": alpha: " << tempPPP[1].alpha << " beta: " << tempPPP[1].beta << " weight: " << tempPPP[1].w_ppp << " " << tempPPP[1].X << std::endl;
    std::cout << "TempObj 3" << ": alpha: " << tempPPP[2].alpha << " beta: " << tempPPP[2].beta << " weight: " << tempPPP[2].w_ppp << " " << tempPPP[2].X << std::endl;
    
    double PPP_threshold = extObj->getPPP_PruningThreshold();
    double updateWppp1 = 0; 
    double updateWppp2 = 0;
    int temp =0;
    for(int i = PPP.size() - 1; i >= 0; --i){
        updateWppp1 = (1 - Pd)*PPP[i].w_ppp;
        if(updateWppp1 > PPP_threshold){
            PPP[i].w_ppp = updateWppp1;
        }else{
            std::cout << "object erased." <<std::endl;
            PPP.erase(PPP.begin() + i);
            std::cout << PPP.size() << std::endl;
            temp++;
        }

        updateWppp2 = Pd*pow(tempPPP[i].beta/(tempPPP[i].beta + 1),tempPPP[i].alpha);
        if(updateWppp2 > PPP_threshold){
            tempPPP[i].w_ppp = updateWppp2;
            tempPPP[i].beta = tempPPP[i].beta + 1;
        }else{
            std::cout << "temp object erased."<< std::endl;
            tempPPP.erase(tempPPP.begin() + i);
        }
    }
    std::cout << "Obj 1" << ": alpha: " << PPP[0].alpha << " beta: " << PPP[0].beta << " weight: " << PPP[0].w_ppp << " " << PPP[0].X << std::endl;
    std::cout << "Obj 2" << ": alpha: " << PPP[1].alpha << " beta: " << PPP[1].beta << " weight: " << PPP[1].w_ppp << " " << PPP[1].X << std::endl;
    std::cout << "Obj 3" << ": alpha: " << PPP[2].alpha << " beta: " << PPP[2].beta << " weight: " << PPP[2].w_ppp << " " << PPP[2].X << std::endl;
    std::cout << "TempObj 1" << ": alpha: " << tempPPP[0].alpha << " beta: " << tempPPP[0].beta << " weight: " << tempPPP[0].w_ppp << " " << tempPPP[0].X << std::endl;
    std::cout << "TempObj 2" << ": alpha: " << tempPPP[1].alpha << " beta: " << tempPPP[1].beta << " weight: " << tempPPP[1].w_ppp << " " << tempPPP[1].X << std::endl;
    std::cout << "TempObj 3" << ": alpha: " << tempPPP[2].alpha << " beta: " << tempPPP[2].beta << " weight: " << tempPPP[2].w_ppp << " " << tempPPP[2].X << std::endl;
    combinePPPs(PPP, tempPPP);
    std::cout << PPP.size() << std::endl;
    std::cout << "Obj 1" << ": alpha: " << PPP[0].alpha << " beta: " << PPP[0].beta << " weight: " << PPP[0].w_ppp << " " << PPP[0].X << std::endl;

    if(PPP.empty()){
        std::cerr << "All the PPP components got pruned!\n " 
                  << "After update we prune PPP components with low weights."
                  << " If no new PPPs are added for the next recursion, we cant"
                  << " create new objects in the MB update loop.\n "
                  << "Try changing initial weights of PPP or lowering Pd\n";
    }
}

/* 
PPP_update will takes in a collection of objects, tracked and untracked.
Untracked objects will either remain untracked or get tracked. 
1: Untracked objects remain untracked.
   updateUndetectedObjs();
   The existance probabilities for objects that were untracked and still untracked 
   should be reduced.
   This should be done in updateUndetectedObjs.
   Untracked objects are either:
        a: Untracked objects generated some measurements.
        b: Untracked objects didn't generate measurements.

2: The probability that we an object will get detected will be calculated,
thus new objects will be added.
   createNewMB();

*/
void PPP_update(ObjectsCollection& collection,
                measurements* currMeasurements,
                radarDefinition* radar,
                ExtendedObjectDefinition* extObj){
                    updateUndetectedObjs(collection.PPP,
                                         radar,
                                         extObj);

                };
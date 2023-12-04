#include "../Headers/declarations.hpp"
//#include "utilities.cpp"

void updateUndetectedObjs(ObjectsCollection& PPP_objs,
                          radarDefinition* radar,
                          ExtendedObjectDefinition* extObj){
    double Pd = radar->getRadarDetectionProbability();
    //std::cout << PPP_objs.PPP.size() << std::endl;

    std::vector<UntrackedObj> tempPPP = PPP_objs.PPP;
    /* 
    std::cout << "Obj 1" << ": alpha: " << PPP[0].alpha << " beta: " << PPP[0].beta << " weight: " << PPP[0].w_ppp << " " << PPP[0].X << std::endl;
    std::cout << "Obj 2" << ": alpha: " << PPP[1].alpha << " beta: " << PPP[1].beta << " weight: " << PPP[1].w_ppp << " " << PPP[1].X << std::endl;
    std::cout << "Obj 3" << ": alpha: " << PPP[2].alpha << " beta: " << PPP[2].beta << " weight: " << PPP[2].w_ppp << " " << PPP[2].X << std::endl;
    std::cout << "TempObj 1" << ": alpha: " << tempPPP[0].alpha << " beta: " << tempPPP[0].beta << " weight: " << tempPPP[0].w_ppp << " " << tempPPP[0].X << std::endl;
    std::cout << "TempObj 2" << ": alpha: " << tempPPP[1].alpha << " beta: " << tempPPP[1].beta << " weight: " << tempPPP[1].w_ppp << " " << tempPPP[1].X << std::endl;
    std::cout << "TempObj 3" << ": alpha: " << tempPPP[2].alpha << " beta: " << tempPPP[2].beta << " weight: " << tempPPP[2].w_ppp << " " << tempPPP[2].X << std::endl;
     */
    double PPP_threshold = extObj->getPPP_PruningThreshold();
    double updateWppp1 = 0; 
    double updateWppp2 = 0;
    int temp =0;
    for(int i = PPP_objs.PPP.size() - 1; i >= 0; --i){
        updateWppp1 = (1 - Pd)*PPP_objs.PPP[i].w_ppp;
        if(updateWppp1 > PPP_threshold){
            PPP_objs.PPP[i].w_ppp = updateWppp1;
        }else{
            //std::cout << "object erased." <<std::endl;
            PPP_objs.PPP.erase(PPP_objs.PPP.begin() + i);
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
    
    /* std::cout << "Obj 1" << ": alpha: " << PPP[0].alpha << " beta: " << PPP[0].beta << " weight: " << PPP[0].w_ppp << " " << PPP[0].X << std::endl;
    std::cout << "Obj 2" << ": alpha: " << PPP[1].alpha << " beta: " << PPP[1].beta << " weight: " << PPP[1].w_ppp << " " << PPP[1].X << std::endl;
    std::cout << "Obj 3" << ": alpha: " << PPP[2].alpha << " beta: " << PPP[2].beta << " weight: " << PPP[2].w_ppp << " " << PPP[2].X << std::endl;
    std::cout << "TempObj 1" << ": alpha: " << tempPPP[0].alpha << " beta: " << tempPPP[0].beta << " weight: " << tempPPP[0].w_ppp << " " << tempPPP[0].X << std::endl;
    std::cout << "TempObj 2" << ": alpha: " << tempPPP[1].alpha << " beta: " << tempPPP[1].beta << " weight: " << tempPPP[1].w_ppp << " " << tempPPP[1].X << std::endl;
    std::cout << "TempObj 3" << ": alpha: " << tempPPP[2].alpha << " beta: " << tempPPP[2].beta << " weight: " << tempPPP[2].w_ppp << " " << tempPPP[2].X << std::endl;
     */
    combinePPPs(PPP_objs.PPP, tempPPP);
    //std::cout << PPP_objs.PPP.size() << std::endl;
    //std::cout << "Obj 1" << ": alpha: " << PPP[0].alpha << " beta: " << PPP[0].beta << " weight: " << PPP[0].w_ppp << " " << PPP[0].X << std::endl;

    if(PPP_objs.PPP.empty()){
        std::cerr << "All the PPP components got pruned!\n " 
                  << "After update we prune PPP components with low weights."
                  << " If no new PPPs are added for the next recursion, we cant"
                  << " create new objects in the MB update loop.\n "
                  << "Try changing initial weights of PPP or lowering Pd\n";
    }
}

void createNewMBs(ObjectsCollection& PPP_objs,
                  measurements* CurrentMeasurements,
                  radarDefinition* radar,
                  ExtendedObjectDefinition* extObj){
    std::vector<double> eps = {radar->getEpsCluster()};
    int minNrPnts = radar->getMinNrPntsCluster();

    // Local copy for the local ingating.
    measurements localCopy = *CurrentMeasurements;
    /* localCopy.outGated = CurrentMeasurements->outGated;
    localCopy.inGated = CurrentMeasurements->inGated;
    std::cout << "\n CurrentMeasurements -> z is \n";
    std::cout << *(CurrentMeasurements->z);
    std::cout << "Local copy z\n";
    std::cout << *(localCopy.z);
    std::cout << "CurrentMeasurements outgated:\n"; 
    std::cout << CurrentMeasurements->outGated;
    std::cout << "localCopy outgated:\n";
    std::cout << localCopy.outGated; */

    // To check which outgated measurements are located in the 
    // PPP gate.
    elipsoidalGating(radar, &PPP_objs,'P', localCopy);
    
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
    int measRemoved = 0;
    //std::cout << RawMeasInsideGates;

    //std::cout << localCopy.outGated;
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
    std::cout << "Meas ingate:\n";
    std::cout << RawMeasInsideGates;

    DBSCAN result = dbscan::run(RawMeasInsideGates, eps, minNrPnts);
    std::cout << result;
    std::cout << std::endl;

    /* 
    New Bernoulli Birth:
    In the following part, the likelihood that we will get new objects will 
    be calculated.
    */
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
            newBernoulliBirth(PPP_objs,
                              &clusterMeasurements,
                              radar,
                              extObj);
        }
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
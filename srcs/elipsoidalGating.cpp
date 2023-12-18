#include "../Headers/declarations.hpp"

/**
 * @brief Perform ellipsoidal gating to determine measurements within the Mahalanobis distance from a given object.
 *
 * This function evaluates the Mahalanobis distance for each measurement and determines whether it falls within the gating threshold.
 * The gating threshold defined in the radar object. The results are updated in the `currMeasurements` structure.
 *
 * @param radar A radarDefinition pointer representing the radar configuration.
 * @param objs An ObjectsCollection pointer containing tracked or untracked objects.
 * @param c A character indicating whether to perform gating on Markovian Birth (M) or Probabilistic Point Process (P) objects.
 * @param currMeasurements A measurements structure containing the current measurements and gating information.
 *        - inGated: A matrix of size nxm where n is the number of objects and m is the number of measurements collected at the current time step. 
 *                   Each element indicats whether the object at row n fall close to the measurement at column m. If we have a match the the element
 *                   is (1) or outside (0) the Mahalanobis distance gate for each object.
 *        - outGated: A matrix of size 1xm. Each column denotes the gating outcome: 0 (outside), otherwise inside.
 */
void elipsoidalGating(radarDefinition* radar,
                      ObjectsCollection* objs,
                      const char c,
                      measurements& currMeasurements){
    int rows;
    if(c=='M'){
        std::cout << "Gating around MBs ... " << std::endl;
        rows = objs->MB.size();
    }else{
        std::cout << "Gating around PPPs ... " << std::endl;
        rows = objs->PPP.size();
    }
    int cols = currMeasurements.z->nrCols();

    Matrix measInGate(rows, cols);

    Matrix invMeasCovariance = radar->getMeasurementsCovariance().inv();
    Matrix measPred;
    
    int nrMeasPnts = currMeasurements.z->nrCols();
    Matrix currMeas;

    Matrix currInnovation(3,1);
    
    Matrix tempMatrix;

    for(int i=0; i<rows; i++){
        if(c=='M'){
            measPred = radar->measurementPrediction(&(objs->MB.at(i).X));
        }else{
            measPred = radar->measurementPrediction(&(objs->PPP.at(i).X));
        }
        for(int t =0; t < nrMeasPnts;t++){
            currMeas = currMeasurements.z->getColumn(t);
            currInnovation = currMeas - measPred;            
            tempMatrix = currInnovation.transpose()*invMeasCovariance*currInnovation;
            measInGate(i,t) = tempMatrix(0,0)<radar->getGatingThreshold();
        }
    }

    Matrix sumRows = measInGate.sum(1);
    currMeasurements.inGated = measInGate;
    currMeasurements.outGated = sumRows;
}
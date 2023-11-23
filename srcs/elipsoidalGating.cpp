#include "../Headers/declarations.hpp"

/* 
This is a function that performs ellipsoidal gating
to find which measurements are located in the Mahalanobis  
distance from our objects. 
currMeasurements parameters will be updated based on
a gating threshold decided on earlier step. 
1: inGated:
   a matrix that is of size similar to the amount of 
   measurements we have. If a measurement is located 
   in the Mahalanobis distance, its position is donated with 1,
   otherwise it is 0.

2: outGated: 
   A matrix of size 1xt. 
   0 will be denoted if no measurement is located in 
   the objects gates.
   1 if x or y is located in the gate.
   2 if x and y are located in the gate.
 */
void elipsoidalGating(radarDefinition* radar,
                      ObjectsCollection* objs,
                      measurements& currMeasurements){
    int rows = currMeasurements.z->nrRows();
    int cols = currMeasurements.z->nrCols();

    Matrix measInGate(rows, cols);

    Matrix invMeasCovariance = radar->getMeasurementsCovariance().inv();
    Matrix measPred;
    
    int nrTimeSteps = currMeasurements.z->nrCols();
    Matrix currMeas;

    Matrix currInnovation(3,1);
    
    Matrix tempMatrix;
    //std::cout << objs->MB.size() << std::endl;
    //std::cout << objs->PPP.size() << std::endl;

    for(int i=0; i<objs->MB.size(); i++){
        measPred = radar->measurementPrediction(&(objs->MB.at(i).X));
        for(int t =0; t < nrTimeSteps;t++){
            currMeas = currMeasurements.z->getColumn(t);
            currInnovation = currMeas - measPred;            
            tempMatrix = currInnovation.transpose()*invMeasCovariance*currInnovation;
            measInGate(i,t) = tempMatrix(0,0)<radar->getGatingThreshold();
        }
        //std::cout << measInGate;
    }

    //std::cout << measInGate;
    Matrix sumRows = measInGate.sumRows();
    //Matrix measOutGate(1,measInGate.nrCols());
    //// SUM ROWS WILL RETURN HOW MANY MEAS WE HAVE, 0 -> outgate, 2 -> ingate
    /* std::cout << sumRows;
    for(int i = 0; i<sumRows.nrCols(); i ++){
        measOutGate(0,i) = (sumRows(0,i) == 0);
    } */
    currMeasurements.inGated = measInGate;
    currMeasurements.outGated = sumRows;
}
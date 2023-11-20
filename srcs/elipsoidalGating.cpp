#include "../Headers/declarations.hpp"


void elipsoidalGating(radarDefinition* radar,
                      std::vector<TrackedObj>* MB,
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

    for(int i=0; i<MB->size(); i++){
        measPred = radar->measurementPrediction(&(MB->at(i).X));
        for(int t =0; t < nrTimeSteps;t++){
            currMeas = currMeasurements.z->getColumn(t);
            currInnovation = currMeas - measPred;            
            tempMatrix = currInnovation.transpose()*invMeasCovariance*currInnovation;
            measInGate(i,t) = tempMatrix(0,0)<radar->getGatingThreshold();
        }
    }

    Matrix sumRows = measInGate.sumRows();
    Matrix measOutGate(1,measInGate.nrCols());

    for(int i = 0; i<sumRows.nrCols(); i ++){
        measOutGate(0,i) = (sumRows(0,i) == 0);
    }
    currMeasurements.inGated = measInGate;
    currMeasurements.outGated = measOutGate;
}
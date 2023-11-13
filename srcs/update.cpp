#include "../Headers/obj.hpp"
#include "../Headers/radarDefinition.hpp"
#include "../Headers/ExtendedObjectDefinition.hpp"

class measurements{
    public:
        Matrix* z;
        Matrix inGated;
        Matrix outGated;
        measurements(Matrix* measurements):z(measurements){};
};

void elipsoidalGating(radarDefinition* radar, std::vector<TrackedObj> MB, measurements& currMeasurements){
    int rows = currMeasurements.z->nrRows();
    int cols = currMeasurements.z->nrCols();

    Matrix measInGate(rows, cols);
    std::cout << measInGate;

    Matrix invMeasCovariance = radar->getMeasurementsCovariance().inv();
    Matrix measPred;
    
    int nrTimeSteps = currMeasurements.z->nrCols();
    Matrix currMeas;

    Matrix currInnovation(3,1);
    
    Matrix tempMatrix;

    for(int i=0; i<MB.size(); i++){
        measPred = radar->measurementPrediction(&MB[i].X);
        std::cout << measPred;
        for(int t =0; t < nrTimeSteps;t++){
            currMeas = currMeasurements.z->getColumn(t);
            std::cout << currMeas;
            currInnovation = currMeas - measPred;
            std::cout << currInnovation;
            std::cout << invMeasCovariance;
            std::cout << currInnovation.transpose();
            
            tempMatrix = currInnovation.transpose()*invMeasCovariance*currInnovation;
            measInGate(i,t) = tempMatrix(0,0)<radar->getGatingThreshold();
        }
    }

    std::cout << measInGate << std::endl;

    Matrix summedCols = measInGate.sumColumns();
    Matrix measOutGate(1,measInGate.nrCols());

    for(int i = 0; i<summedCols.nrCols(); i ++){
        measOutGate(0,i) = (summedCols(0,i) == 0);
    }
    std::cout << measOutGate << std::endl;
    currMeasurements.inGated = measInGate;
    currMeasurements.outGated = measOutGate;
}

void update(ObjectsCollection* collection,
            Matrix* data,
            radarDefinition* radar,
            ExtendedObjectDefinition* extObj){

                measurements currentMeasurements(data);
                std::vector<TrackedObj> tempppppp = collection->MB;
                elipsoidalGating(radar, collection->MB, currentMeasurements);
                std::cout <<currentMeasurements.inGated;
                std::cout <<currentMeasurements.outGated;
            };



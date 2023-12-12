#include <iostream>
using namespace std;
#include "Headers/declarations.hpp"
#include <random>

int main() {
    double T = 0.1, sigmaV = 1.0, sigmav = M_PI/15; 
    ctModel motionModel(T,sigmaV, sigmav);

    // Extended object representation parameters
    double extentDim = 3, survivalProbability = 0.35, shapeUncertainty = 0.1;
    double forgettingFactor = 8;
    double existanceThreshold = 0.3, PPP_pruningThreshold = 0.034, MB_pruningThreshold = 0.4;
    ExtendedObjectDefinition extendedObjDef(extentDim,
                                            motionModel,
                                            survivalProbability,
                                            shapeUncertainty,
                                            forgettingFactor,
                                            existanceThreshold,
                                            PPP_pruningThreshold,
                                            MB_pruningThreshold);

    // Radar parameters
    double detectionProbability = 0.9, gatingThreshold = 100, ciClutterIntensity = 1.5;
    double expectedObjects = 5, objectsRate = 10, extentDegreesOfFreedom = 150, extentCov = 60;

    // Clustering parameters
    double epsClusteringThreshold = 1.5, minNrPntsCluster = 1;

    ObjectsCollection testPPP = initPPP(expectedObjects, objectsRate, extentDegreesOfFreedom);
    radarDefinition radar(detectionProbability,
                          gatingThreshold,
                          ciClutterIntensity,
                          expectedObjects,
                          objectsRate,
                          extentDegreesOfFreedom,
                          extentCov,
                          epsClusteringThreshold,
                          minNrPntsCluster);
    
    TrackedObj obj1;
    obj1.X.x = 1.1;
    obj1.X.y = 2.1;
    std::cout << "Obj1:\n" << obj1.X; 
    testPPP.MB.push_back(obj1);

    TrackedObj obj2;
    obj2.X.x = -23;
    obj2.X.y = 23;
    std::cout << "Obj2:\n" << obj2.X ;
    testPPP.MB.push_back(obj2);

    Matrix meas(3,10);
    meas = {{1.1, 1.0, 0.9, 7,  0.5,  12, -22.9, -23, -23.2}, 
            {2.1, 2.1, 2.0, 1, -0.5, -12,  22.9,  23,  23.2},
            {5  , 3  , -6 , 1,    2,   3,   1  ,  2 ,    5 }};
        
    std::cout << "Test mesurements are:" << std::endl;
    std::cout << meas;
    
    predict(testPPP, extendedObjDef);

    update(testPPP, &meas, &radar, &extendedObjDef);
    
    cin.get();

    return 0;
} 
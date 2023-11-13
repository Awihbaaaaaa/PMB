#include <iostream>
using namespace std;
#include <cmath> 
#include <vector>
#include <limits>
#include "Headers/declarations.hpp"
//#include "Headers/ctModel.hpp"
//#include "Headers/objectsStack.hpp"
//#include "Headers/ExtendedObjectDefinition.hpp"
//#include "Headers/radarDefinition.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
//#include "Headers/obj.hpp"
#define MIN_DOUBLE -std::numeric_limits<double>::max()
#include <random>
//#include <cmath>

int main() {
    // Motion model parameters
    double T = 0.1, sigmaV = 1.0, sigmav = M_PI/15; 
    ctModel motionModel(T,sigmaV, sigmav);

    // Extended object representation parameters
    double extentDim = 3, survivalProbability = 0.35, shapeUncertainty = 0.1;
    double forgettingFactor = 8, ciClusteringThreshold = 1.5, minNrPnts= 3, objectsRate = 10;
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
    double detectionProbability = 0.9, gatingThreshold = 100, expectedObjects = 5, extentDegreesOfFreedom = 150, extentCov = 60;
    ObjectsCollection testPPP = initPPP(expectedObjects, objectsRate, extentDegreesOfFreedom);
    radarDefinition radar(detectionProbability,
                          gatingThreshold,
                          ciClusteringThreshold,
                          expectedObjects,
                          objectsRate,
                          extentDegreesOfFreedom,
                          extentCov);

    /* runRadarTest();
    testMatrix_cpp();
    objectCreationTest(); */
    
    cout << "Obj 1" << ": alpha: " << testPPP.PPP[0].alpha << " beta: " << testPPP.PPP[0].beta << " weight: " << testPPP.PPP[0].w_ppp << " " << testPPP.PPP[0].X << endl;
    cout << "Obj 2" << ": alpha: " << testPPP.PPP[1].alpha << " beta: " << testPPP.PPP[1].beta << " weight: " << testPPP.PPP[1].w_ppp << " " << testPPP.PPP[1].X << endl;
    cout << "Obj 3" << ": alpha: " << testPPP.PPP[2].alpha << " beta: " << testPPP.PPP[2].beta << " weight: " << testPPP.PPP[2].w_ppp << " " << testPPP.PPP[2].X << endl;
    
    // combineComponents(currentPPP, newPPP);
    Matrix meas(3,10);
    TrackedObj obj1;
    obj1.X.x = 1.1;
    obj1.X.y = 2.1;

    testPPP.MB.push_back(obj1);

    TrackedObj obj2;
    obj2.X.x = -23;
    obj2.X.y = 23;

    testPPP.MB.push_back(obj2);

    meas = {{1.1, 1.0, 0.9, 7,  0.5,  12, -22.9, -23, -23.2}, 
            {2.1, 2.1, 2.0, 1, -0.5, -12,  22.9,  23,  23.2},
            {5  , 3  , -6 , 1,    2,   3,   1  ,  2 ,    5 }};
    
    
    predict(testPPP, extendedObjDef);
    update(&testPPP, &meas, &radar, &extendedObjDef);

    cout << testPPP.PPP.size();

    /* ObjectsCollection Rad;

    double values[] = {MIN_DOUBLE,MIN_DOUBLE,MIN_DOUBLE,MIN_DOUBLE,MIN_DOUBLE};
    

    UntrackedObj initPPPObject;
    TrackedObj initMBObject;
    if(!initMBObject.r_MB){
        std::cout << initMBObject.alpha;
    }

    /* objStateSpace untrackedState(values);   
    initPPPObject.X = untrackedState;

    Matrix v(3, 1);
    v = {{1.0}, {2.0}, {3.0}};

    initPPPObject.v = v;
    initMBObject.v = v;

    Matrix V(3, 3);
    V(0, 0) = 0.1;
    V(1, 1) = 0.1;
    V(2, 2) = 0.1;
    initPPPObject.V = V;
    initMBObject.V = V;

    Matrix alpha(1, 1);
    alpha(0, 0) = 1.0;
    initPPPObject.alpha = alpha;
    initMBObject.alpha = alpha;

    Matrix beta(1, 1);
    beta(0, 0) = 10.0;
    initPPPObject.beta = beta;
    initMBObject.beta = beta;

    Matrix w_ppp(1, 1);
    w_ppp(0, 0) = 0.1;
    initPPPObject.w_ppp = w_ppp;

    Matrix r_MB(1, 1);
    r_MB(0, 0) = 0.9;
    initMBObject.r_MB = r_MB;

    initPPPObject.P = Matrix(5,5);
    initMBObject.P = Matrix(5,5);

    Rad.PPP.push_back(initPPPObject);
    Rad.MB.push_back(initMBObject); 
    std::cout << Rad.PPP[0].X;
    std::cout << Rad.MB[0].X;

    readCSV(); */
    
    /* objectsStack testStack;

    double testObject1[5] = {5,5,3,0,0};
    double testObject2[5] = {0,0,3,0,0};
    double testObject3[5] = {-5,5,3,0,0};

    testStack.push(testObject1);
    testStack.push(testObject2);
    testStack.push(testObject3);

    testStack.displayStack(); */

    ctModel model(0.1, 0.01, 0.01);
    Matrix rotationMatrix(3,3);
    ExtendedObjectDefinition objPars(3,model,0.1,0.2,0.3,0.4,0.5,0.6);
    cout << objPars.getExtensionDimension() << endl;
    cout << objPars.getSurvivalProbability() << endl;
    cout << objPars.getExistanceThreshold() << endl;
    cout << objPars.getForgettingFactor() << endl;
    cout << objPars.getMB_PruningThreshold() << endl;
    cout << objPars.getPPP_PruningThreshold() << endl; 
    /* objStateSpace poppedObject = testStack.pop();   
    cout << objPars.rotationMatrix(&poppedObject);
        
    objStateSpace* currObj = testStack.head;
    while(currObj){
        model.f(*currObj);
        currObj = currObj -> nextObj;
    }
    testStack.displayStack();

    testStack.push(testObject2);

    testStack.displayStack(); */

    cin.get();

    return 0;
} 
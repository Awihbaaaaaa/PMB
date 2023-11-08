#include <iostream>
using namespace std;
#include <cmath> 
#include <vector>
#include <limits>
//#include "Headers/Matrix.hpp"
//#include "Headers/ctModel.hpp"
//#include "Headers/objectsStack.hpp"
#include "Headers/ExtendedObjectDefinition.hpp"
#include "Headers/radarDefinition.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include "Headers/obj.hpp"
#define MIN_DOUBLE -std::numeric_limits<double>::max()
#include <random>
#include <cmath>

void runRadarTest();
void testMatrix_cpp();
void objectCreationTest();
void readCSV();
double deg2rad(double degrees);
double genRanNr(double min, double max); 
ObjectsCollection initPPP(const double tot_exp_objs,
                        const double objs_rate,
                        const double dof); 

int main() {
    runRadarTest();
    testMatrix_cpp();
    objectCreationTest();
    Matrix V(3,3);
    ObjectsCollection testInit = initPPP(5.0, 10.0, 160.0);
    for(const UntrackedObj obj: testInit.PPP){
        cout << obj.w_ppp << endl;
    }
    cout << testInit.PPP.size();

    ObjectsCollection Rad;

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
    Rad.MB.push_back(initMBObject); */

    std::cout << Rad.PPP[0].X;
    std::cout << Rad.MB[0].X;

    readCSV();
    
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
    ExtendedObjectDefinition objPars(3,model,0.1,0.2,0.3,rotationMatrix,0.4,0.5,0.6);
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
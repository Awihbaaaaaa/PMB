#include <iostream>
using namespace std;
#include "Headers/declarations.hpp"

#define MIN_DOUBLE -std::numeric_limits<double>::max()
#include <random>
//#include <cmath>

int main() {
    /* Matrix A(3, 3);
    A ={{4, 12, -16},
        {12, 37, -43},
        {-16, -43, 98}};

    // Perform Cholesky factorization
    Matrix L = A.chol();

    std::cout << "Original matrix A:\n" << A << "\n\n";
    std::cout << "Lower triangular matrix L (Cholesky factorization):\n" << L << "\n\n"; */
    /* Matrix Z(3,4);
    Z = {{15.0, 2.0, 2.5, 10.0},
        {50.0, 6.0, 7.0, 9.0},
        {0.1, 0.2, 0.3, 0.4}};

    std::vector<double> eps = {1.5}; // You can have multiple eps values
    int minPts = 1;

    DBSCANResult result = DBSCAN::run(Z, eps, minPts);

    // Print the results
    std::cout << "IDs: ";
    for (int id : result.id) {
        std::cout << id << " " ;
    }
    std::cout << std::endl;
    std::cout << result.c[0].size() << std::endl;
    std::cout << result.c[1].size() << std::endl;
    std::cout << result.c[2].size() << std::endl;
    
    for(int i=0; i<result.c.size();i++){
        for(int j=0; j<result.c[i].size();j++){
            std::cout << result.c[i][j] << " ";
        }
        std::cout << std::endl;
    } */
    // Motion model parameters
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
    
    /* cout << "Obj 1" << ": alpha: " << testPPP.PPP[0].alpha << " beta: " << testPPP.PPP[0].beta << " weight: " << testPPP.PPP[0].w_ppp << " " << testPPP.PPP[0].X << endl;
    cout << "Obj 2" << ": alpha: " << testPPP.PPP[1].alpha << " beta: " << testPPP.PPP[1].beta << " weight: " << testPPP.PPP[1].w_ppp << " " << testPPP.PPP[1].X << endl;
    cout << "Obj 3" << ": alpha: " << testPPP.PPP[2].alpha << " beta: " << testPPP.PPP[2].beta << " weight: " << testPPP.PPP[2].w_ppp << " " << testPPP.PPP[2].X << endl;
     */
    // combineComponents(currentPPP, newPPP);
    
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
    /* std::cout << "The size of the predicted objets is: " << testPPP.PPP.size() << std::endl;
    std::cout << testPPP.PPP[80].X; */

    update(testPPP, &meas, &radar, &extendedObjDef);
    /* std::cout << "The size of the updated objets is: " << testPPP.PPP.size();

    std::cout << testPPP.PPP.size(); */
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

    ctModel model(0.1, 0.01, 0.01);
    Matrix rotationMatrix(3,3);
    ExtendedObjectDefinition objPars(3,model,0.1,0.2,0.3,0.4,0.5,0.6);

    cin.get();

    return 0;
} 
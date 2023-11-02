#include <iostream>
using namespace std;
#include <cmath> 
#include <vector>
//#include "Headers/Matrix.hpp"
//#include "Headers/ctModel.hpp"
//#include "Headers/objectsStack.hpp"
#include "Headers/readCSV.hpp"
#include "Headers/ExtendedObjectDefinition.hpp"
#include "Headers/radarDefinition.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
void runRadarTest();
void testMatrix_cpp();

int main() {
    runRadarTest();
    testMatrix_cpp();

    std::vector<std::vector<double>> x_values, y_values, vel_values;
    readCSV1("ros_data.csv", x_values, y_values, vel_values);

    std::cout << x_values.size() << " ";
    std::cout << y_values.size() << " ";
    std::cout << vel_values.size() << " ";
    
    std::cout.precision(10);
    for (const std::vector<double>& row : x_values) {
        for (double value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;  // Print a newline after each row
    }
    
    objectsStack testStack;

    double testObject1[5] = {5,5,3,0,0};
    double testObject2[5] = {0,0,3,0,0};
    double testObject3[5] = {-5,5,3,0,0};

    testStack.push(testObject1);
    testStack.push(testObject2);
    testStack.push(testObject3);

    testStack.displayStack();

    ctModel model(0.1, 0.01, 0.01);
    Matrix rotationMatrix(3,3);
    ExtendedObjectDefinition objPars(3,model,0.1,0.2,0.3,rotationMatrix,0.4,0.5,0.6);
    cout << objPars.getExtensionDimension() << endl;
    cout << objPars.getSurvivalProbability() << endl;
    cout << objPars.getExistanceThreshold() << endl;
    cout << objPars.getForgettingFactor() << endl;
    cout << objPars.getMB_PruningThreshold() << endl;
    cout << objPars.getPPP_PruningThreshold() << endl; 
    objStateSpace poppedObject = testStack.pop();   
    cout << objPars.rotationMatrix(&poppedObject);
        
    objStateSpace* currObj = testStack.head;
    while(currObj){
        model.f(*currObj);
        currObj = currObj -> nextObj;
    }
    testStack.displayStack();

    testStack.push(testObject2);

    testStack.displayStack();

    cin.get();

    return 0;
} 
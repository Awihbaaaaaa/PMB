#include <iostream>
using namespace std;
#include <cmath> 
#include <vector>
#include "Headers/Matrix.hpp"
#include "Headers/ctModel.hpp"
#include "Headers/objectsStack.hpp"
#include "Headers/readCSV.hpp"
#include <fstream>
#include <sstream>
#include <string>

/* class CtModel{
private:
    int state_dimension;
    double sampling_time;
    vector<vector<double>> Q;

    CtModel():state_dimension(5),sampling_time(0.1){
        
    };
};
 */


int main() {
    /* int n = 100;      // Number of data points
    double step = 0.1; // Time step in seconds
    double initial_x = 0.0; // Initial position
    double velocity = 0.3; // Constant velocity (1 unit per second)

    for (int i = 0; i < n; i++) {
        double time = i * step; // Elapsed time
        double x = initial_x + velocity * time; // Calculate position
        std::cout << time << " " << x << std::endl;
    } */
    std::vector<std::vector<double>> x_values, y_values, vel_values;
    readCSV1("ros_data1.csv", x_values, y_values, vel_values);

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

    obj* currObj = testStack.head;
    while(currObj){
        model.f(*currObj);
        currObj = currObj -> nextObj;
    }
    testStack.displayStack();

    testStack.push(testObject2);

    testStack.displayStack();

    /* 
    // Testing declaration of a matrix A
    Matrix A(4,4);

    // Testing initialization of the declared matrix A
    A = {{3, 5, 1, 7}, 
        {2, 8, -3, 1},
        {5, 3, -6, 1},
        {3, 1, 6, 2}};

    // Testing another way for initialization of the declared matrix A
    A(0, 0) = 3.0;
    A(0, 1) = 5.0;
    A(0, 2) = 1.0;
    A(0, 3) = 7.0;
    A(1, 0) = 2.0;
    A(1, 1) = 8.0;
    A(1, 2) = -3.0;
    A(1, 3) = 1.0;
    A(2, 0) = 5.0;
    A(2, 1) = 3.0;
    A(2, 2) = -6.0;
    A(2, 3) = 1.0;
    A(3, 0) = 3.0;
    A(3, 1) = 1.0;
    A(3, 2) = 6.0;
    A(3, 3) = 2.0;
 
    // Testing overiding the operator <<
    cout << "Matrix A is:\n" << A << endl;
    std::vector<std::vector<double>> temp = {{9.0, 8.0, 7.0, 6.0},
                                            {2, 9.0, 8.0, 7.0},
                                            {9.0, 2.3, 8.0, 7.0},
                                            {3.0, 1.0, 6.0, 2.0}};

    // Updating the matrix data
    A.updateData(temp);

    cout << "Matrix A is updated to:\n" << A << endl;

    Matrix B(4, 4);
    B = {{9.0, 8.0, 7.0, 6.0},
        {2, 9.0, 8.0, 7.0},
        {9.0, 2.3, 8.0, 7.0},
        {3.0, 1.0, 6.0, 2.0}};

    cout << "Matrix B is:\n" << B << endl;

    cout << B << endl;
    
    Matrix A__B = A + B;
    cout << "Matrix addition \n" << A__B;

    Matrix B_A = A - B;
    cout << "Matrix subtraction \n" << B_A;

    Matrix AB = A * B;
    cout << "Matrix multiplication \n" << AB;


    cout << "A transpose is \n" << A.transpose() << endl;

    cout << "A transpose is \n" << A << endl;

    cout << "A determinant is \n" << A.determinant() << endl;

    cout << "The inverse of A is \n" << A.inv(); */

    cin.get();

    return 0;
} 
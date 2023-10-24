#include <iostream>
using namespace std;
#include <cmath> 
#include <vector>
#include "Matrix.hpp"

class CtModel{
private:
    int state_dimension;
    double sampling_time;
    vector<vector<double>> Q;

    CtModel():state_dimension(5),sampling_time(0.1){
        
    };
};

int main() {
    int n = 100;      // Number of data points
    double step = 0.1; // Time step in seconds
    double initial_x = 0.0; // Initial position
    double velocity = 1.0; // Constant velocity (1 unit per second)

    for (int i = 0; i < n; i++) {
        double time = i * step; // Elapsed time
        double x = initial_x + velocity * time; // Calculate position
        std::cout << time << " " << x << std::endl;
    }


    Matrix A(4,4);
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
 
    cout << "Matrix A is:\n" << A << endl;

    Matrix B(3, 3);
    B(0, 0) = 9.0;
    B(0, 1) = 8.0;
    B(0, 2) = 7.0;
    B(1, 0) = 6.0;
    B(1, 1) = 5.0;
    B(1, 2) = 4.0;
    B(2, 0) = 3.0;
    B(2, 1) = 2.0;
    B(2, 2) = 1.0;

    cout << "Matrix B is:\n" << B << endl;

    cout << B << endl;
    
    /* Matrix AB = A + B;
    cout << "Matrix addition \n " << AB;

    Matrix BA = A - B;
    cout << "Matrix subtraction \n " << BA;
  */
    cout << "A transpose is \n" << A.transpose() << endl;

    cout << "A transpose is \n" << A << endl;

    cout << "A determinant is \n" << A.determinant() << endl;

    cout << "The inverse of A is \n" << A.inv();

    cin.get();

    return 0;
} 
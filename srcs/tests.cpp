#include "../Headers/radarDefinition.hpp"
#include "../Headers/Matrix.hpp"
#include "../Headers/obj.hpp"
#include <assert.h>
#include <cmath>
#include "../Headers/readCSV.hpp"
#include "../Headers/dbscan.hpp"

/* 
This file include some test functions definitions. To run the test,
call the function handle in main.
 */

void runRadarTest(){
    // Create a radarDefinition object
    radarDefinition radarDefinition(0.9,100,0.6,5,10,150,60,1.5,1);

    // Test measurementModel
    objStateSpace testObject;
    testObject.x = 10.0;
    testObject.y = 5.0;
    testObject.v = 20.0;
    testObject.theta = 0.0;
    testObject.w = 0.1;

    // Test measurementPrediction
    Matrix Z = radarDefinition.measurementPrediction(&testObject);
    Matrix H = radarDefinition.measurementModel(&testObject);

    Matrix expectedMeasurementsPrediction(3, 1);
    expectedMeasurementsPrediction = {{11.9999}, {5.02}, {17.957}};

    Matrix expectedMeasurementModel(3, 5);
    expectedMeasurementModel = {{1.0     , 0.0      , 0.099995, -0.00199997, -0.0199997},
                        {0.0     , 1.0      , 0.000999, 0.19999    , 1.9999    },
                        {0.364908, -0.729817, 0.88991 , -9.12271   , -0.912271 }};
    double tolerance = 1e-3;  // Adjust as needed

    for (int i = 0; i < Z.nrRows(); i++) {
        for (int j = 0; j < Z.nrCols(); j++) {
            assert(std::abs(H(i, j) - expectedMeasurementModel(i, j)) < tolerance);
        }
    }

    std::cout << "Expected measurement model test, passed.\n";

}

void testMatrix_cpp(){
    // Testing declaration of a matrix A
    Matrix A(4,4);

    // Testing initialization of the declared matrix A
    A = {{3, 5, 1, 7}, 
        {2, 8, -3, 1},
        {5, 3, -6, 1},
        {3, 1, 6, 2}};
    std::cout << A;
    std::cout << "Colomn 1 is to be removed" << std::endl;
    A.removeColumn(1);
    std::cout << A;

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
    std::cout << "Matrix A is:\n" << A << std::endl;
    std::vector<std::vector<double>> temp = {{9.0, 8.0, 7.0, 6.0},
                                            {2, 9.0, 8.0, 7.0},
                                            {9.0, 2.3, 8.0, 7.0},
                                            {3.0, 1.0, 6.0, 2.0}};

    // Updating the matrix data
    A.updateData(temp);

    std::cout << "Matrix A is updated to:\n" << A << std::endl;

    Matrix B(4, 4);
    B = {{9.0, 8.0, 7.0, 6.0},
        {2, 9.0, 8.0, 7.0},
        {9.0, 2.3, 8.0, 7.0},
        {3.0, 1.0, 6.0, 2.0}};

    std::cout << "Matrix B is:\n" << B << std::endl;

    std::cout << B << std::endl;

    Matrix A__B = A + B;
    std::cout << "Matrix addition \n" << A__B;

    Matrix B_A = A - B;
    std::cout << "Matrix subtraction \n" << B_A;

    Matrix AB = A * B;
    std::cout << "Matrix multiplication \n" << AB;


    std::cout << "A transpose is \n" << A.transpose() << std::endl;

    std::cout << "A transpose is \n" << A << std::endl;

    std::cout << "A determinant is \n" << A.determinant() << std::endl;

    std::cout << "The inverse of A is \n" << A.inv();

}

void objectCreationTest(){
    ObjectsCollection Rad;
    
    double values[] = {10.0, 10.0, 20.0, 0.1, 3.0};
    objStateSpace untrackedState(values);

    UntrackedObj extendedObject;
    extendedObject.X = untrackedState;
    double v = 130;

    extendedObject.v = v;

    Matrix V(3, 3);
    V(0, 0) = 0.1;
    V(1, 1) = 0.1;
    V(2, 2) = 0.1;
    extendedObject.V = V;

    double alpha = 0.1;
    extendedObject.alpha = alpha;

    double beta = 0.1;
    extendedObject.beta = beta;

    double w_ppp = 0.1;
    extendedObject.w_ppp = w_ppp;

    /* Matrix r_MB(1, 1);
    r_MB(0, 0) = 0.9;
    extendedObject.r_MB = r_MB;
 */
    extendedObject.P = Matrix(5,5);

    Rad.PPP.push_back(extendedObject);

    std::cout << Rad.PPP[0].X;
}

void readCSV(){
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
}


void TestDBSCAN(){
    Matrix Z(3,4);
    Z = {{15.0, 2.0, 2.5, 10.0},
        {50.0, 6.0, 7.0, 9.0},
        {0.1, 0.2, 0.3, 0.4}};

    std::cout << "The set of measurements we have is:\n" << Z;
    std::cout << "eps is: " << "1.5" << " and the minimum number of points in each cluster is: 1 " << std::endl;
    std::vector<double> eps = {1.5}; // You can have multiple eps values
    int minPts = 1;

    DBSCAN result = dbscan::run(Z, eps, minPts);

    // Print the results
    std::cout << "Clusters' IDs found: ";
    for (int id : result.id) {
        std::cout << id << " " ;
    }
    std::cout << std::endl;
    /*
    std::cout << result.c[0].size() << std::endl;
    std::cout << result.c[1].size() << std::endl;
    std::cout << result.c[2].size() << std::endl;
     */
    for(int i=0; i<result.c.size();i++){
        std::cout << "The size of cluster " << i+1 << " is " << result.c[i].size() << std::endl;
        std::cout << "The corresponding measurements are ";
        for(int j=0; j<result.c[i].size();j++){
            std::cout << result.c[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

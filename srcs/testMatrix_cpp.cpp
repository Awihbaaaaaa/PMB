#include "../Headers/Matrix.hpp"

void testMatrix_cpp(){
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
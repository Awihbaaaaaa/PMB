#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include "../Headers/Matrix.hpp"
#include <cmath>

std::ostream& operator<<(std::ostream &os, const Matrix &m) {
    int rows = m.nrRows();
    int cols = m.nrCols();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            os << m(i, j) << " ";
        }
        os << std::endl;
    }

    return os;
}

int Matrix::nrRows() const{
    return data.size();
}

int Matrix::nrCols() const{
    return data[0].size();
}

void Matrix::resize(int newRows, int cols){
    data.resize(newRows, std::vector<double>(cols,0.0));
}

void Matrix::updateData(const std::vector<std::vector<double>>& newData) {
    if (newData.size() == nrRows() && newData[0].size() == nrCols()) {
        data = newData;
    } else {
        std::cout << "The new data dimensions don't match. Hit enter to exit.";
        std::cin.get();
        throw std::invalid_argument("Matrices dimensions doesn't match.");
    }
} 

double& Matrix::operator()(int row, int col){
    return data[row][col];
}

double Matrix::operator()(int row, int col) const{
    return data[row][col];
}

Matrix Matrix::operator*(const Matrix& other) const{
    if(nrRows() != other.nrRows() || nrCols() != other.nrCols()){
        std::cout << "The matrices dimensions don't match. Hit enter to exit.";
        std::cin.get();
        throw std::invalid_argument("Matrices dimensions doesn't match.");
    }

    Matrix res(nrRows(), nrCols());
    double c = 0, r = 0, temp = 0;
    for(int i = 0; i < nrRows(); i++){
        for(int j = 0; j < nrCols(); j++){
            for(c=0,r=0;r<nrRows(); c++,r++){
                temp += data[i][c] * other(r,j);
            }
            res(i,j) = temp;
            temp = 0;
        }
    }

    return res;
}

Matrix Matrix::operator+(const Matrix& other) const{
    if(nrRows() != other.nrRows() || nrCols() != other.nrCols()){
        std::cout << "The matrices dimensions don't match. Hit enter to exit.";
        std::cin.get();
        throw std::invalid_argument("Matrices dimensions doesn't match.");
    }

    Matrix res(nrRows(), nrCols());

    for(int i = 0; i < nrRows(); i++){
        for(int j = 0; j < nrCols(); j++){
            res(i,j) = data[i][j] + other(i,j);
        }
    }

    return res;
}

Matrix Matrix::operator-(const Matrix& other) const{
    if(nrRows() != other.nrRows() || nrCols() != other.nrCols()){
        std::cout << "The matrices dimensions don't match. Hit enter to exit.";
        std::cin.get();
        throw std::invalid_argument("Matrices dimensions doesn't match.");
    }

    Matrix res(nrRows(), nrCols());

    for(int i = 0; i < nrRows(); i++){
        for(int j = 0; j < nrCols(); j++){
            res(i,j) = data[i][j] - other(i,j);
        }
    }

    return res;
}

Matrix& Matrix::operator=(std::initializer_list<std::initializer_list<double>> values){
    int rows = nrRows();
    int cols = nrCols();
    int i = 0;
    int j = 0;

    for(const auto& row : values){
        for(double value : row){
            if (i < rows && j < cols) {
                data[i][j] = value;
            }
            j++;
        }
        j = 0;
        i++;
    }

    return *this;
}

Matrix Matrix::transpose() const{
    Matrix res(nrRows(), nrCols());
    for(int i = 0; i < nrRows(); i++){
        for(int j = 0; j < nrCols(); j++){
            res(i,j) = data[j][i];
            //res(j,i) = data[i][j];
        }
    }
    return res;
}

void Matrix::luDecomposition(Matrix& L, Matrix& U) const {
    int n = nrRows();

    // Initialize L and U as the zero matrices
    L = Matrix(n, n, 0.0);
    U = Matrix(n, n, 0.0);

    // Loop through the matrix rows
    for (int i = 0; i < n; i++) {
        // Loop through the matrix columns of the upper triangular matrix
        for (int k = i; k < n; k++) { 
            double sum = 0.0;

            // Calculate the sum of the products of elements in the lower triangular matrix L
            // and the upper triangular matrix U to find the elements of U
            for (int j = 0; j < i; j++) {
                sum += (L(i, j) * U(j, k));
            }

            // Compute the elements of the upper triangular matrix U
            U(i, k) = data[i][k] - sum;
        }

        // Loop through columns of the lower triangular matrix
        for (int k = i; k < n; k++) {
            if (i == k) {
                // Set the diagonal elements of L to 1
                L(i, i) = 1.0;
            } else {
                double sum = 0.0;

                // Calculate the sum of the products of elements in the lower triangular matrix L
                // and the upper triangular matrix U to find the elements of L
                for (int j = 0; j < i; j++) {
                    sum += (L(k, j) * U(j, i));
                }

                // Compute the elements of the lower triangular matrix L
                L(k, i) = (data[k][i] - sum) / U(i, i);
            }
        }
    }
}

bool Matrix::operator==(const Matrix& other) const {
    if (data.size() != other.data.size() || data[0].size() != other.data[0].size()) {
        return false;
    }

    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[0].size(); ++j) {
            if (data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }

    return true;
}

// Function to calculate the determinant using LU decomposition
double Matrix::determinant() const {
    int n = nrRows();
    if (n != nrCols()) {
        throw std::runtime_error("Determinant is defined only for square matrices.");
    }

    //If it is a single element matrix
    if (n == 1) {
        return data[0][0];
    }

    // Initialize two matrices to store the upper and lower matrices
    Matrix L, U;
    luDecomposition(L, U);

    // Singularity check
    for (int i = 0; i < n; i++) {
        if (std::abs(U(i, i)) < 1e-10) {
            std::cout << "The matrix is singular, determinant is 0" << std::endl;
            return 0.0;
        }
    }

    double det = 1.0;
    for (int i = 0; i < n; i++) {
        det *= U(i, i);
    }

    return det;
}

/* Matrix Matrix::forwardSubstitution(const Matrix& L, const Matrix& b) {
    int n = L.nrRows();
    Matrix y(n, 1);

    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < i; j++) {
            sum += L(i, j) * y(j, 0);
        }
        y(i, 0) = (b(i, 0) - sum) / L(i, i);
    }

    return y;
}

Matrix Matrix::backwardSubstitution(const Matrix& U, const Matrix& b) {
    int n = U.nrRows();
    Matrix x(n, 1);

    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum += U(i, j) * x(j, 0);
        }
        x(i, 0) = (b(i, 0) - sum) / U(i, i);
    }

    return x;
}


void Matrix::setColumn(int col, const Matrix& columnData) {
    if (col >= nrCols() || columnData.nrRows() != nrRows() || columnData.nrCols() != 1) {
        // Handle invalid input or dimension mismatch
        throw std::invalid_argument("Invalid column or dimension mismatch");
    }

    for (int i = 0; i < nrRows(); i++) {
        data[i][col] = columnData(i, 0);
    }
}

Matrix Matrix::getColumn(int col) const {
    int n = nrRows();
    if (col < 0 || col >= nrCols()) {
        // Handle invalid column index
        throw std::out_of_range("Invalid column index");
    }

    Matrix column(n, 1);
    for (int i = 0; i < n; i++) {
        column(i, 0) = data[i][col];
    }
    return column;
}

Matrix Matrix::inv() {
    int n = nrRows();
    if (n != nrCols()) {
        std::cout << "The matrix doesn't have an inverse, not a square matrix. Press enter to exit!";
        std::cin.get();
        throw std::runtime_error("Determinant is defined only for square matrices.");
    }

    // Perform LU decomposition
    Matrix L, U;
    luDecomposition(L, U);

    // Initialize identity matrix I
    Matrix I(n, n);
    for (int i = 0; i < n; i++) {
        I(i, i) = 1.0;
    }

    // Initialize the inverse matrix
    Matrix inverse(n, n);

    // Forward Substitution: Solve Ly = I
    for (int j = 0; j < n; j++) {
        Matrix y = forwardSubstitution(L, I.getColumn(j));
        inverse.setColumn(j, y);
    }

    // Backward Substitution: Solve Ux = y
    for (int j = 0; j < n; j++) {
        Matrix x = backwardSubstitution(U, inverse.getColumn(j));
        inverse.setColumn(j, x);
    }

    return inverse;
} */

Matrix Matrix::inv(){
    int n = nrRows();
    if (n != nrCols()) {
        std::cout << "The matrix doesn't have an inverse, not square matrix. Press enter to exit!";
        std::cin.get();
        throw std::runtime_error("Determinant is defined only for square matrices.");
    }

    // Perfrom LU Decomposition
    // A = L*U
    Matrix L, U;
    luDecomposition(L, U);
    
    // Singularity check
    for (int i = 0; i < n; i++) {
        if (std::abs(U(i, i)) < 1e-10) { 
            std::cout << "The matrix is not invertible, determinant is 0" << std::endl;
            return Matrix(n,n);
        }
    }

    // Forward Substitution for lower triangular
    // Ly = I
    // y0 = I0/L00
    // y1 = (I1 - L10*y0)/L11
    // I is the identitiy matrix
    Matrix I(n, n);
    for (int i = 0; i < n; i++) {
        I(i, i) = 1.0;
    }

    Matrix inverse(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inverse(i, j) = I(i, j);
            for (int k = 0; k < i; k++) {
                inverse(i, j) -= L(i, k) * inverse(k, j);  // Corrected index here
            }
        }
    }

    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j < n; j++) {
            for (int k = i + 1; k < n; k++) {
                inverse(i, j) -= U(i, k) * inverse(k, j);  // Corrected index here
            }
            inverse(i, j) /= U(i, i);
        }
    }

    return inverse;
}


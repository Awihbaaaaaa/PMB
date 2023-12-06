#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include "../Headers/Matrix.hpp"
#include "../Headers/obj.hpp"
#include <cmath>

Matrix::Matrix(objStateSpace obj):data(5, std::vector<double>(1)){
    data[0][0] = obj.x;
    std::cout << obj.y << "\n";
    data[1][0] = obj.y;
    std::cout << data[0][1];
    data[2][0] = obj.v;
    data[3][0] = obj.theta;
    data[4][0] = obj.w;
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    int rows = m.nrRows();
    int cols = m.nrCols();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (j > 0) {
                os << " | ";
            }
            os << std::setw(8) << std::setprecision(4) << m(i, j);
        }
        os << std::endl;
    }

    return os;
}

std::vector<std::vector<double>> Matrix::getData() const {
    return data;
}

int Matrix::nrRows() const{
    return data.size();
}

int Matrix::nrCols() const{
    return data[0].size();
}

Matrix Matrix::identity(int size) {
    Matrix identityMatrix(size, size, 0.0);

    for (int i = 0; i < size; i++) {
        identityMatrix(i, i) = 1.0;
    }

    return identityMatrix;
}

void Matrix::resize(int newRows, int newCols){
    //data.resize(newRows, std::vector<double>(cols,0.0));
    if (newRows < 0 || newCols < 0) {
        throw std::invalid_argument("Number of rows and columns must be non-negative");
    }

    // Shrink rows if needed
    if (newRows < nrRows()) {
        data.resize(newRows);
    }

    // Shrink columns if needed
    if (newCols < nrCols()) {
        for (auto& row : data) {
            row.resize(newCols);
        }
    }

    // Expand rows if needed
    if (newRows > nrRows()) {
        data.resize(newRows, std::vector<double>(newCols, 0.0));
    }

    // Expand columns if needed
    if (newCols > nrCols()) {
        for (auto& row : data) {
            row.resize(newCols, 0.0);
        }
    }
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
    if(nrCols() != other.nrRows()){
        std::cout << "The matrices dimensions don't match. Multiplication is not possible. Hit enter to exit.";
        std::cin.get();
        throw std::invalid_argument("Matrices dimensions doesn't match.");
    }

    Matrix res(nrRows(), other.nrCols(), 0.0);
    double c = 0, r = 0, temp = 0;
    for(int i = 0; i < this->nrRows(); i++){
        for(int j = 0; j < other.nrCols(); j++){
            double sum = 0.0;
            for(int k=0; k< this->nrCols(); k++){
                /* std::cout << "data at "<< i << ", " << k << " " << data[i][k] << std::endl;
                std::cout << "other at "<< k << ", " << j << " " << other(k,j) << std::endl;
                std::cout << data[i][k] * other(k,j) << std::endl; */
                sum += this->data[i][k] * other(k,j);
                //std::cout << sum << std::endl;
            }
            res(i,j) = sum;
        }
    }
    return res;
}

Matrix Matrix::operator*(const double scaler) const{
    Matrix result(nrRows(), nrCols());

    for(int i = 0; i<nrRows(); i++){
        for(int j = 0; j<nrCols(); j++){
            result(i,j) = data[i][j]*scaler;
        }
    }
    return result;
}

Matrix operator*(const double scaler, const Matrix& matrix) {
    Matrix result(matrix.nrRows(), matrix.nrCols());

    for (int i = 0; i < matrix.nrRows(); i++) {
        for (int j = 0; j < matrix.nrCols(); j++) {
            result(i, j) = matrix(i, j) * scaler;
        }
    }

    return result;
}


Matrix Matrix::operator/(const double scaler) const {
    if (scaler == 0.0) {
        throw std::invalid_argument("Division by zero.");
    }

    Matrix result(nrRows(), nrCols());
    for (int i = 0; i < nrRows(); ++i) {
        for (int j = 0; j < nrCols(); ++j) {
            result(i, j) = data[i][j] / scaler;
        }
    }
    return result;
}

Matrix Matrix::operator*(const objStateSpace& obj) const {
    Matrix result(3, 1);

    result(0, 0) = this->data[0][0] * obj.x + this->data[0][1] * obj.y + this->data[0][2] * obj.v + this->data[0][3] * obj.theta + this->data[0][4] * obj.w;
    result(1, 0) = this->data[1][0] * obj.x + this->data[1][1] * obj.y + this->data[1][2] * obj.v + this->data[1][3] * obj.theta + this->data[1][4] * obj.w;
    result(2, 0) = this->data[2][0] * obj.x + this->data[2][1] * obj.y + this->data[2][2] * obj.v + this->data[2][3] * obj.theta + this->data[2][4] * obj.w;

    return result;
}

Matrix Matrix::operator+(const objStateSpace& obj) const {
    Matrix result(*this);  

    result(0, 0) += obj.x;
    result(1, 0) += obj.y;
    result(2, 0) += obj.v;
    result(3, 0) += obj.theta;
    result(4, 0) += obj.w;

    return result;
}

Matrix Matrix::operator+(const Matrix& other) const{
    if(nrRows() != other.nrRows() || nrCols() != other.nrCols()){
        std::cout << "The matrices dimensions don't match. Addition is not possible. Hit enter to exit.";
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
    if((nrRows() != other.nrRows() && ((nrCols() != other.nrCols()) || (nrCols() != 1) || (other.nrCols()!=1)))){
        std::cout << "The matrices dimensions don't match. Subtraction is not possible. Hit enter to exit.";
        std::cin.get();
        throw std::invalid_argument("Matrices dimensions doesn't match.");
    }
    Matrix res(nrRows(), std::max(nrCols(),other.nrCols()));

    for(int i = 0; i < nrRows(); i++){
        for(int j = 0; j < nrCols(); j++){
            if (nrCols() == 1) {
                res(i, j) = data[i][0] - other(i, j);
            } else if (other.nrCols() == 1) {
                res(i, j) = data[i][j] - other(i, 0);
            } else {
                res(i, j) = data[i][j] - other(i, j);
            }
        }
    }

    return res;
}

Matrix& Matrix::operator=(std::initializer_list<std::initializer_list<double>> values){
    int newRows = values.size();
    int newCols = (newRows > 0) ? values.begin()->size() : 0;

    if (newRows != nrRows() || newCols != nrCols()) {
        resize(newRows, newCols);
    }

    int i = 0;
    int j = 0;

    for(const auto& row : values){
        for(double value : row){
            if (i < newRows && j < newCols) {
                data[i][j] = value;
            }
            j++;
        }
        j = 0;
        i++;
    }

    return *this;
}

bool Matrix::isempty()const{
    return data.empty();
}

Matrix Matrix::transpose() const{
    Matrix res(nrCols(),nrRows());
    for(int i = 0; i < nrRows(); i++){
        for(int j = 0; j < nrCols(); j++){
            res(j,i) = data[i][j];
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
                //std::cout << "L(" << i <<","<< j << ") is "<< L(i,j) << std::endl;
                //std::cout << "U(" << j <<","<< k << ") is "<< U(j, k) << std::endl;
                sum += (L(i, j) * U(j, k));
            }
            //std::cout << "data[" << i << "," << k <<"] = " << data[i][k] << " and sum is " << sum << std::endl;
            // Compute the elements of the upper triangular matrix U
            
            U(i, k) = data[i][k] - sum;
            //std::cout << "U("<<i<<","<<k<<")="<<U(i,k)<<std::endl;
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
                //std::cout << L(k,i) << std::endl;
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

    const double singularityTolerance = 1e-25;

    // Singularity check
    for (int i = 0; i < n; i++) {
        if (std::abs(U(i, i)) < singularityTolerance) {
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

void Matrix::setColumn(int col, const Matrix& columnData) {
    if (col >= nrCols() || columnData.nrRows() != nrRows() || columnData.nrCols() != 1) {
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

Matrix Matrix::getRow(int row) const {
    int n = nrCols();
    if (row < 0 || row >= n) {
        throw std::out_of_range("Invalid row index");
    }

    Matrix rowMatrix(1, n);
    for (int i = 0; i < n; i++) {
        std::cout << data[row][i];
        rowMatrix(0, i) = data[row][i];
    }
    std::cout << rowMatrix;
    return rowMatrix;
} 

void Matrix::removeColumn(int col){
    if(col < 0 || col>nrCols()){
        throw std::invalid_argument("Invalid column or dimension mismatch");
    }
    Matrix tempColumn(3,1);
    for(int i = col+1; i<nrCols();++i){
        tempColumn = this->getColumn(i);
        setColumn(i-1,tempColumn);
    }
    resize(nrRows(), nrCols()-1);
}

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

    Matrix I(n, n);
    for (int i = 0; i < n; i++) {
        I(i, i) = 1.0;
    }

    Matrix Y = forwardSubstitution(L, I);

    // Backward Substitution for upper triangular matrix U: Ux = Y
    Matrix inverse = backwardSubstitution(U, Y);

    return inverse;
}

Matrix Matrix::forwardSubstitution(const Matrix& L, const Matrix& B) const {
    int n = L.nrRows();
    Matrix Y(n, B.nrCols(), 0.0);

    for (int j = 0; j < B.nrCols(); j++) {
        for (int i = 0; i < n; i++) {
            Y(i, j) = B(i, j);
            for (int k = 0; k < i; k++) {
                Y(i, j) -= L(i, k) * Y(k, j);
            }
            Y(i, j) /= L(i, i);
        }
    }

    return Y;
}

Matrix Matrix::backwardSubstitution(const Matrix& U, const Matrix& B) const {
    int n = U.nrRows();
    Matrix X(n, B.nrCols(), 0.0);

    for (int j = 0; j < B.nrCols(); j++) {
        for (int i = n - 1; i >= 0; i--) {
            X(i, j) = B(i, j);
            for (int k = i + 1; k < n; k++) {
                X(i, j) -= U(i, k) * X(k, j);
            }
            X(i, j) /= U(i, i);
        }
    }

    return X;
}

/**
 * @brief Calculate the sum of all elements in one dimension of the matrix.
 *
 * This function computes the sum of all rows/columns in the matrix based on the parameter given.
 * It takes a parameter 'par' that can be used to specify if the requied sum should be through rows or columns.
 *
 * @param par A that controls the summation process.
 *            Use 1 if the sum should be done on rows.
 *            Use 2 if the sum should be done on rows.
 *            Otherwise, invalid argument.
 *
 * @return A matrix of size 1xnrCols() if the sum performes on rows.
 *         A matrix of size nrRows()x1 if the sum performes on cols.
 */
Matrix Matrix::sum(int par) const{
    if (data.empty() || data[0].empty()) {
        return Matrix();
    }

    if(par == 1){
        Matrix result(1, nrCols(), 0.0);

        for (int col = 0; col < nrCols(); ++col) {
            for (int row = 0; row < nrRows(); ++row) {
                result(0, col) += data[row][col];
            }
        }
        return result;
    }else if(par == 2){
        Matrix result(nrRows(), 1, 0.0);

        for (int row = 0; row < nrRows(); ++row) {
            for (int col = 0; col < nrCols(); ++col) {
                result(row, 0) += data[row][col];
            }
        }
        return result;
    }else{
        throw std::invalid_argument("Invalid argument. Please choose 1 or 2.");
    }
    
}

Matrix Matrix::mean(int dim) const {
    if (dim < 1 || dim > 2) {
        throw std::invalid_argument("Invalid dimension. Valid dimensions are 1 (mean along rows) or 2 (mean along columns).");
    }

    int rows = nrRows();
    int cols = nrCols();

    // Calculate mean along rows (dimension 1)
    if (dim == 1) {
        Matrix result(1, cols, 0.0);
        for (int col = 0; col < cols; ++col) {
            for (int row = 0; row < rows; ++row) {
                result(0, col) += data[row][col];
            }
            result(0, col) /= static_cast<double>(rows);
        }
        return result;
    }
    // Calculate mean along columns (dimension 2)
    else {
        Matrix result(rows, 1, 0.0);
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                result(row, 0) += data[row][col];
            }
            result(row, 0) /= static_cast<double>(cols);
        }
        return result;
    }
}

Matrix Matrix::chol(){
    int n = nrRows();

    if (n != nrCols()) {
        throw std::runtime_error("Cholesky factorization is defined only for square matrices.");
    }

    Matrix chol(this->nrRows(), this->nrCols(), 0.0);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            double sum = 0.0;
            if (i == j) {
                for (int k = 0; k < j; k++) {
                    sum += chol(i, k) * chol(i, k);
                }
                chol(i, j) = std::sqrt(data[i][i] - sum);
            } else {
                for (int k = 0; k < j; k++) {
                    sum += chol(i, k) * chol(j, k);
                }
                //std::cout << (1.0 / chol(j, j)) * (data[i][j] - sum) << std::endl;
                chol(i, j) = (1.0 / chol(j, j)) * (data[i][j] - sum);
            }
        }
    }
    return chol;
}


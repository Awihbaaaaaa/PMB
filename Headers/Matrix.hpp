#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <iostream>
#include <vector>


struct objStateSpace; // Forward declaration of objStateSpace for the Matrix constructor

/**
 * @brief A class represents a mathematical matrix and includes various operations such as
 *  matrix multiplication, addition, subtraction, inversion, LU decomposition, and more. The
 *  class is designed to handle different matrix operations efficiently. 
 */
class Matrix{
    private:
        std::vector<std::vector<double>> data; ///< 2D vector storing matrix elements

    public:
        /**
         * @brief Constructs a matrix with specified dimensions and optional initial value.
         * @param rows Number of rows in the matrix.
         * @param cols Number of columns in the matrix.
         * @param val Initial value for all elements (default is 0.0).
         */
        Matrix(int rows, int cols, double val = 0.0):data(rows, std::vector<double>(cols, val)){}
        
        /**
         * @brief Default constructor, creates an empty matrix.
         */
        Matrix():data(0, std::vector<double>(0, 0.0)){};

        /**
         * @brief Constructor to initialize the matrix from an objStateSpace object.
         *
         * This constructor takes an objStateSpace object and initializes a column matrix with its x, y, v, theta, and w values.
         *
         * @param obj The objStateSpace object used to initialize the matrix.
         */
        Matrix(objStateSpace obj);
        
        /**
         * @brief Getter function to return the number of rows in the matrix.
         *
         * This function returns the number of rows in the matrix.
         *
         * @return The number of rows in the matrix.
         */
        int nrRows() const;

        /**
         * @brief Getter function to return the number of columns in the matrix.
         *
         * This function returns the number of columns in the matrix.
         *
         * @return The number of columns in the matrix.
         */
        int nrCols() const;        

        /**
         * @brief Computes the inverse of the matrix.
         * @return Inverse matrix.
         */
        Matrix inv();

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
        Matrix sum(int par) const;

        /**
         * @brief Performs backward substitution using a lower triangular matrix and a column matrix.
         * @param U Upper triangular matrix.
         * @param b Column matrix.
         * @return Solution matrix.
         */
        Matrix backwardSubstitution(const Matrix& U, const Matrix& b) const;

        /**
         * @brief Performs forward substitution using a lower triangular matrix and a column matrix.
         * @param L Lower triangular matrix.
         * @param B Column matrix.
         * @return Solution matrix.
         */
        Matrix forwardSubstitution(const Matrix& L, const Matrix& B) const ;

        /**
         * @brief Function to resize the matrix.
         *
         * This function resizes the matrix to the specified number of rows and columns, handling both shrinking and expanding.
         *
         * @param newRows The new number of rows.
         * @param newCols The new number of columns.
         */
        void resize(int newRows, int cols);

        /**
         * @brief Function to set a column in the matrix with specified data.
         *
         * This function sets a specified column in the matrix with data from another matrix.
         *
         * @param col The column index to set.
         * @param columnData The matrix containing data to set in the specified column.
         */
        void setColumn(int col, const Matrix& columnData);

        /**
         * @brief Removes a specified column from the matrix.
         * @param col Column index to be removed.
         */
        void removeColumn(int col);

        /**
         * @brief Getter function to return the matrix data.
         *
         * This function returns a copy of the internal data vector representing the matrix.
         *
         * @return A vector of vectors containing the matrix elements.
         */
        std::vector<std::vector<double>> getData() const;

        /**
         * @brief Function to update the matrix with new data, checking dimensions.
         *
         * This function updates the matrix with new data only if the dimensions match the existing matrix.
         *
         * @param newData The new data to update the matrix.
         */
        void updateData(const std::vector<std::vector<double>>& newData);

        /**
         * @brief Function to get a column from the matrix.
         *
         * This function returns a specified column from the matrix as a separate matrix.
         *
         * @param col The column index to retrieve.
         *
         * @return The matrix containing the specified column.
         */
        Matrix getColumn(int col) const;

        /**
         * @brief Function to get a row from the matrix.
         *
         * This function returns a specified row from the matrix as a separate matrix.
         *
         * @param row The row index to retrieve.
         *
         * @return The matrix containing the specified row.
         */
        Matrix getRow(int row) const;

        /**
         * @brief Assigns matrix values using initializer lists.
         * @param values Initializer list of initializer lists representing matrix values.
         * @return Reference to the modified matrix.
         */
        Matrix& operator=(std::initializer_list<std::initializer_list<double>> values);
        
        /**
         * @brief Overloaded operator to multiply two matrices.
         *
         * This operator multiplies two matrices and returns the result.
         *
         * @param other The matrix to be multiplied with.
         *
         * @return The result of the matrix multiplication.
         */
        Matrix operator*(const Matrix& other) const;

        /**
         * @brief Overloaded operator to multiply the matrix by a scalar.
         *
         * This operator multiplies each element of the matrix by a scalar and returns the result.
         *
         * @param scaler The scalar value to multiply with.
         *
         * @return The result of the scalar multiplication.
         */
        Matrix operator*(const double scaler) const;

        /**
         * @brief Overloaded operator to multiply the matrix by an objStateSpace object.
         *
         * This operator multiplies the matrix by an objStateSpace object and returns the result.
         *
         * @param obj The objStateSpace object to be multiplied with.
         *
         * @return The result of the matrix multiplication with the objStateSpace object.
         */
        Matrix operator*(const objStateSpace& obj) const;

        /**
         * @brief Overloaded operator to divide the matrix by a scalar.
         *
         * This operator divides each element of the matrix by a scalar and returns the result.
         *
         * @param scaler The scalar value to divide by.
         *
         * @return The result of the scalar division.
         */
        Matrix operator/(const double scaler) const;

        /**
         * @brief Overloaded operator to add two matrices.
         *
         * This operator adds two matrices and returns the result.
         *
         * @param other The matrix to be added.
         *
         * @return The result of adding the two matrices.
         */
        Matrix operator+(const Matrix& other) const;

        /**
         * @brief Overloaded operator to add an objStateSpace object to the matrix.
         *
         * This operator adds an objStateSpace object to the matrix and returns the result.
         *
         * @param obj The objStateSpace object to be added.
         *
         * @return The result of adding the objStateSpace object to the matrix.
         */
        Matrix operator+(const objStateSpace& obj) const;

        /**
         * @brief Overloaded operator to implement scalar addition to a matrix (matrix+scalar).
         *
         * This operator adds a matrix and a scaler and returns the result in the form matrix + scalar.
         *
         * @param scalar The double value to be added to a matrix.
         *
         * @return The result of adding a double value to a matrix.
         */
        Matrix operator+(const double scalar) const;

        /**
         * @brief Overloaded operator to implement scalar subtraction from a matrix (matrix-scalar).
         *
         * This operator subtracts a scalar from a matrix and returns the result as a matrix.
         *
         * @param scalar The double value to be removed to a matrix.
         *
         * @return The result of adding a double value to a matrix.
         */
        Matrix operator-(const double scalar) const;
        
        /**
         * @brief Overloaded operator to subtract two matrices.
         *
         * This operator subtracts one matrix from another and returns the result.
         *
         * @param other The matrix to be subtracted.
         *
         * @return The result of subtracting the two matrices.
         */
        Matrix operator-(const Matrix& other) const;

        /**
         * @brief Function to check if two matrices are equal.
         *
         * This function checks if two matrices are equal by comparing their elements.
         *
         * @param other The matrix to compare with.
         *
         * @return True if the matrices are equal, false otherwise.
         */
        bool operator==(const Matrix& other) const;

        /**
         * @brief Function to transpose the matrix.
         *
         * This function transposes the matrix, swapping rows with columns.
         *
         * @return The transposed matrix.
         */
        Matrix transpose() const;

        /**
         * @brief Function to calculate the determinant using LU decomposition.
         *
         * This function calculates the determinant of the matrix using LU decomposition.
         *
         * @return The determinant of the matrix.
         */
        double determinant() const;

        
        /**
         * @brief Function to perform LU decomposition of the matrix.
         *
         * This function performs LU decomposition of the matrix and returns the decomposed matrices L and U.
         *
         * @param L The lower triangular matrix.
         * @param U The upper triangular matrix.
         */
        void luDecomposition(Matrix& L, Matrix& U) const;

        /**
         * @brief Function to check if the matrix is empty.
         *
         * This function checks if the matrix is empty, i.e., if it has zero rows or columns.
         *
         * @return True if the matrix is empty, false otherwise.
         */
        bool isempty() const;

        /**
         * @brief Static function to create an identity matrix of a given size.
         *
         * This function creates and returns an identity matrix of the specified size.
         *
         * @param size The size of the identity matrix.
         *
         * @return An identity matrix of size x size.
         */
        static Matrix identity(int size);

        /**
         * @brief Overloaded operator to access and modify matrix elements.
         *
         * This operator allows access and modification of individual elements in the matrix using the () notation.
         *
         * @param row The row index of the element.
         * @param col The column index of the element.
         *
         * @return A reference to the specified matrix element.
         */
        double& operator()(int row, int col);

        /**
         * @brief Overloaded operator to access matrix elements (const version).
         *
         * This operator allows read-only access to individual elements in the matrix using the () notation.
         *
         * @param row The row index of the element.
         * @param col The column index of the element.
         *
         * @return The value of the specified matrix element.
         */
        double operator()(int row, int col) const;

        /**
         * @brief Computes the mean along a specified dimension.
         * @param dim Dimension (1 for rows, 2 for columns).
         * @return Matrix containing the mean values.
         */
        Matrix mean(int dim) const;

        /**
         * @brief Computes the Cholesky decomposition of the matrix.
         * @return Lower triangular matrix representing the Cholesky decomposition.
         */
        Matrix chol();
        
        double max() const;

        double min() const;

        /**
         * @brief Overloaded stream insertion operator to print the matrix.
         *
         * This operator is used to output the matrix to the standard output stream with proper formatting.
         *
         * @param os The output stream.
         * @param m The matrix to be printed.
         *
         * @return The modified output stream.
         */
        friend std::ostream& operator<<(std::ostream &os, const Matrix &m);
};

/**
 * @brief Overloaded operator to multiply a scalar by a matrix.
 *
 * This operator multiplies a scalar by each element of the matrix and returns the result.
 *
 * @param scaler The scalar value to multiply with.
 * @param matrix The matrix to be multiplied.
 *
 * @return The result of the scalar multiplication.
 */
Matrix operator*(const double scaler, const Matrix& matrix);

#endif
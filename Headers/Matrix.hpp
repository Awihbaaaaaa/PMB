#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <iostream>
#include <vector>

class Matrix{
    private:
        std::vector<std::vector<double>> data;
        // The following is 5 rows and 1 column
        // std::vector<std::vector<double>> temp(5, std::vector<double>(1, 0.0));

    public:

        Matrix(int rows, int cols, double val = 0.0):data(rows,std::vector<double>(cols, val)){}
        
        Matrix():data(0, std::vector<double>(0, 0.0)){};
        int nrRows() const;
        int nrCols() const;        

        Matrix inv();
        Matrix sumColumns() const;
        Matrix backwardSubstitution(const Matrix& U, const Matrix& b);
        Matrix forwardSubstitution(const Matrix& L, const Matrix& b);
        void resize(int newRows, int cols);
        void setColumn(int col, const Matrix& columnData);
        std::vector<std::vector<double>> getData() const;
        void updateData(const std::vector<std::vector<double>>& newData);
        Matrix getColumn(int col) const;
        Matrix& operator=(std::initializer_list<std::initializer_list<double>> values);
        Matrix operator*(const Matrix& other) const;
        Matrix operator*(const double scaler) const;
        Matrix operator+(const Matrix& other) const;
        Matrix operator-(const Matrix& other) const;
        bool operator==(const Matrix& other) const;
        Matrix transpose() const;
        double determinant() const;
        void luDecomposition(Matrix& L, Matrix& U) const;
        bool isempty() const;
        static Matrix identity(int size);
        double& operator()(int row, int col);
        double operator()(int row, int col) const;
        
        friend std::ostream& operator<<(std::ostream &os, const Matrix &m);
};

#endif
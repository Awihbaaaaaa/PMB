#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <iostream>
#include <vector>

class Matrix{
    private:
        std::vector<std::vector<double>> data;

    public:
        Matrix(int rows, int cols, double val = 0.0):data(rows,std::vector<double>(cols, val)){}
        
        Matrix(){};
        int nrRows() const;
        int nrCols() const;        

        Matrix inv();
        Matrix backwardSubstitution(const Matrix& U, const Matrix& b);
        Matrix forwardSubstitution(const Matrix& L, const Matrix& b);
        void setColumn(int col, const Matrix& columnData);
        Matrix getColumn(int col) const;

        Matrix operator+(const Matrix& other) const;
        Matrix operator-(const Matrix& other) const;
        Matrix transpose() const;
        double determinant() const;
        void luDecomposition(Matrix& L, Matrix& U) const;

        double& operator()(int row, int col);
        double operator()(int row, int col) const;

        friend std::ostream& operator<<(std::ostream &os, const Matrix &m);
};

#endif
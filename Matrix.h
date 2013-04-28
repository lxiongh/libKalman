#ifndef MATRIX_H
#define MATRIX_H

#include <cstddef>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

using namespace std;
class Matrix
{
    public:
        Matrix();
        Matrix(int r, int c);
        Matrix(int r, int c, double* data);
        virtual ~Matrix();
        Matrix(const Matrix& other);
        Matrix& operator=(const Matrix& other);
        Matrix operator+(const Matrix& val);
        Matrix operator+(const double val);
        Matrix operator-(const Matrix& val);
        Matrix operator*(const Matrix& val);
        int Getrows() const{ return m_rows; }
        int Getcolumns() const{ return m_columns; }
        double* Getdata() { return m_data; }
        double Get(int r, int c) const;
        void Set(int r, int c, double val);
        void DisplayMtr();
        Matrix InverseMtr();
        Matrix TransposeMtr();
        void Empty();
    protected:
    private:
        int m_rows;
        int m_columns;
        double* m_data;
        void inv(double *a,int n);
        void Setdata(double* val, int len);
        void ResizeMtr(int r, int c);
        void ClearMtr();
};

#endif // MATRIX_H

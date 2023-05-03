#pragma once
#include <malloc.h>
#include <stdio.h>

class Matrix {
private:
	double** mat;
	int n;
	int m;
	bool is_diag = false;
	bool is_1v = false;
	bool is_1vT = false;
	double** CreateMat(int sizeN, int sizeM);
public:
	Matrix();
	Matrix(int n, int m);	
	Matrix(const Matrix& M);
	Matrix(Matrix&& M);
	Matrix L();
	Matrix U();
	Matrix D();
	Matrix Inv();
	void Print();
	void factorizeLU(Matrix* L, Matrix* U);
	double norm();
	Matrix& operator=(Matrix&& right);
	Matrix& operator=(const Matrix& right);
	Matrix operator-();
	friend Matrix RandomMatrix(int n, int m);
	friend Matrix CreateMatrix(int n, int a1, int a2, int a3);
	friend Matrix CreateVector(int n, int f);
	friend Matrix Ones(int n, int m);
	friend Matrix Zeros(int n, int m);
	friend Matrix Identity(int n, int m);
	friend Matrix operator+(const Matrix& A, const Matrix& B);
	friend Matrix operator-(const Matrix& A, const Matrix& B);
	friend Matrix operator*(const Matrix& A, const Matrix& B);	
	//forward substitution
	friend Matrix operator>>(const Matrix& A, const Matrix& b);
	//backward substitution
	friend Matrix operator<<(const Matrix& A, const Matrix& b);
	~Matrix();
};
Matrix RandomMatrix(int n, int m);
Matrix CreateMatrix(int n, int a1, int a2, int a3);
Matrix CreateVector(int n, int f);
Matrix Ones(int n, int m);
Matrix Zeros(int n, int m);
Matrix Identity(int n, int m);
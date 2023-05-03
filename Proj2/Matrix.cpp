#pragma once
#include "Matrix.h"
#include <random>
using namespace std;

Matrix RandomMatrix(int n, int m) {
	Matrix temp(n, m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			temp.mat[i][j] = rand() % 10;
		}
	}
	return temp;
}
double** Matrix::CreateMat(int sizeN, int sizeM) {
	double** temp;
	temp = (double**)malloc(sizeof(double**) * sizeN);
	for (int i = 0; i < sizeN; i++) {
		temp[i] = (double*)calloc(sizeM, sizeof(double));
	}
	return temp;
}

Matrix::Matrix() {
	mat = NULL;
	n = 0;
	m = 0;
}

Matrix::~Matrix() {
	if (mat == NULL)
		return;
	for (int i = 0; i < n; i++) {
		free(mat[i]);
	}
	free(mat);
}

Matrix::Matrix(int n, int m) {
	this->n = n;
	this->m = m;
	mat = CreateMat(n, m);
}

Matrix::Matrix(const Matrix& M) {
	n = M.n;
	m = M.m;
	this->is_1v = M.is_1v;
	this->is_1vT = M.is_1vT;
	this->is_diag = M.is_diag;
	mat = CreateMat(n, m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			mat[i][j] = M.mat[i][j];
		}
	}
}

Matrix::Matrix(Matrix&& M) {
	n = M.n;
	m = M.m;
	this->is_1v = M.is_1v;
	this->is_1vT = M.is_1vT;
	this->is_diag = M.is_diag;
	mat = M.mat;

	M.mat = NULL;
}

Matrix Matrix::L() {
	Matrix temp(n, m);
	for (int i = 0; i < n; i++) {
		for(int j = 0; j < i; j++){
			temp.mat[i][j] = mat[i][j];
		}
	}
	return temp;
}

Matrix Matrix::U() {
	Matrix temp(n, m);
	for (int i = 0; i < n; i++) {
		for (int j = i+1; j < m; j++) {
			temp.mat[i][j] = mat[i][j];
		}
	}
	return temp;
}

Matrix Matrix::D() {
	Matrix temp(n, m);
	for (int i = 0; i < n; i++) {
		temp.mat[i][i] = mat[i][i];
	}
	temp.is_diag = true;
	return temp;
}

Matrix Matrix::Inv() {
	if (!is_diag) {
		fprintf(stderr, "Nie diag!");
		exit(1);
	}
	Matrix temp(n, m);
	for (int i = 0; i < n; i++) {
		temp.mat[i][i] = 1 / mat[i][i];
	}
	return temp;
}

void Matrix::Print() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			printf("%.4lf ", this->mat[i][j]);
		}
		printf("\n");
	}
}

void Matrix::factorizeLU(Matrix* L, Matrix* U) {
	*U = *this;
	*L = Identity(n, m);
	for (int k = 0; k < m - 1; k++) {
		for (int j = k + 1; j < m; j++) {
			L->mat[j][k] = U->mat[j][k] / U->mat[k][k];
			for (int i = k; i < m; i++) {
				U->mat[j][i] = U->mat[j][i] - (L->mat[j][k] * U->mat[k][i]);
			}
		}
	}
}

double Matrix::norm() {
	if (m != 1) {
		fprintf(stderr, "To nie wektor!");
		exit(1);
	}
	double res = 0.0;
	for (int i = 0; i < n; i++) {
		res += mat[i][0] * mat[i][0];
	}
	return sqrt(res);
}

Matrix& Matrix::operator=(Matrix&& right) {
	swap(mat, right.mat);
	swap(n, right.n);
	swap(m, right.m);
	swap(is_1v, right.is_1v);
	swap(is_1vT, right.is_1vT);
	swap(is_diag, right.is_diag);
	return *this;
}

Matrix& Matrix::operator=(const Matrix& right) {
	Matrix tmp(right);
	swap(mat, tmp.mat);
	swap(n, tmp.n);
	swap(m, tmp.m);
	swap(is_1v, tmp.is_1v);
	swap(is_1vT, tmp.is_1vT);
	swap(is_diag, tmp.is_diag);
	return *this;
}

Matrix operator+(const Matrix& A, const Matrix& B) {
	if (A.n != B.n && A.m != B.m) {
		fprintf(stderr, "Niezgodne wymiary");
		exit(1);
	}
	Matrix temp(A.n, A.m);
	for (int i = 0; i < A.n; i++) {
		for (int j = 0; j <A. m; j++) {
			temp.mat[i][j] = A.mat[i][j] + B.mat[i][j];
		}
	}
	return temp;
}

Matrix Matrix::operator-() {
	Matrix temp(n, m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			temp.mat[i][j] = -mat[i][j];
		}
	}
	return temp;
}

Matrix operator-(const Matrix& A, const Matrix& B) {
	if (A.n != B.n && A.m != B.m) {
		fprintf(stderr, "Niezgodne wymiary");
		exit(1);
	}
	Matrix temp(A.n, A.m);
	for (int i = 0; i < A.n; i++) {
		for (int j = 0; j < A.m; j++) {
			temp.mat[i][j] = A.mat[i][j] - B.mat[i][j];
		}
	}
	return temp;
}

Matrix operator*(const Matrix& A, const Matrix& B) {
	if (A.m != B.n) {
		fprintf(stderr, "Niezgodne wymiary");
		exit(1);
	}
	if (A.is_diag && B.is_diag && A.n == B.n && B.m == A.m) {
		Matrix temp(A.n, B.n);
		for (int i = 0; i < A.n; i++) {
			temp.mat[i][i] = A.mat[i][i] * B.mat[i][i];
		}
		temp.is_diag = true;
		return temp;
	}
	Matrix temp(A.n, B.m);
	for (int i = 0; i < A.n; i++) {
		for (int j = 0; j < B.m; j++) {
			for (int k = 0; k < B.n; k++) {
				temp.mat[i][j] += A.mat[i][k] * B.mat[k][j];
			}
		}
	}
	if (temp.n == 1 && temp.m >= 1)
		temp.is_1v = true;
	if (temp.m == 1 && temp.n >= 1)
		temp.is_1vT = true;
	return temp;
}

Matrix operator>>(const Matrix& A, const Matrix& b) {
	Matrix x = Ones(b.n, 1);
	x.mat[0][0] = b.mat[0][0] / A.mat[0][0];
	for (int i = 1; i < A.n; i++) {
		double sum = b.mat[i][0];
		for (int j = 0; j < i; j++) {
			sum -= A.mat[i][j] * x.mat[j][0];
		}
		x.mat[i][0] = sum / A.mat[i][i];
	}
	return x;
}

Matrix operator<<(const Matrix& A, const Matrix& b) {
	Matrix x = Ones(b.n, 1);
	x.mat[b.n - 1][0] = b.mat[b.n - 1][0] / A.mat[A.n - 1][A.n - 1];
	for (int i = A.n-2; i >=0; i--) {
		double sum = b.mat[i][0];
		for (int j = i+1; j < A.n; j++) {
			sum -= A.mat[i][j] * x.mat[j][0];
		}
		x.mat[i][0] = sum / A.mat[i][i];
	}
	return x;
}

Matrix CreateMatrix(int n, int a1, int a2, int a3) {
	Matrix temp(n, n);
	for (int i = 0; i < n; i++) {
		temp.mat[i][i] = a1;
		if (i < n - 1) {
			temp.mat[i][i + 1] = a2;
			temp.mat[i + 1][i] = a2;
		}
		if (i < n - 2) {
			temp.mat[i][i + 2] = a3;
			temp.mat[i + 2][i] = a3;
		}
	}
	return temp;
}

Matrix CreateVector(int n, int f) {
	Matrix temp(n, 1);
	for (int i = 0; i < n; i++) {
		temp.mat[i][0] = sin(i * (f + 1.0));
	}
	return temp;
}

Matrix Ones(int n, int m) {
	Matrix temp(n, m);
	for (int i = 0; i < temp.n; i++) {
		for (int j = 0; j < temp.m; j++) {
			temp.mat[i][j] = 1.0;
		}
	}
	return temp;
}

Matrix Zeros(int n, int m) {
	return Matrix(n, m);
}

Matrix Identity(int n, int m) {
	Matrix temp(n, m);
	for (int i = 0; i < temp.n; i++) {
		temp.mat[i][i] = 1.0;
	}
	return temp;
}
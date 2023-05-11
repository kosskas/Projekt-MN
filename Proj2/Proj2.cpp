#include <stdio.h>
#include "Vector.h"
#include "Matrix.h"
#include "timer.h"

#define n 976

void excB1(); //Jakobi
void excB2(); //GaussSeidl
void excC1();
void excC2();
void excD();
void excE1();
void excE2();
void excE3();
int main() {
    excB1();
    excB2();
    //excC1();
    //excC2();
    //excD();
    //excE1();
    //excE2();
    //excE3();
    return 0;
}
void excB1() {
    ///Jakobi
    Vector<double> normy;
    Matrix A = CreateMatrix(n, 11, -1, -1);
    Matrix b = CreateVector(n, 8);
    Matrix r = Ones(n, 1);
    int iters = 0;
    double norm = (A * r - b).norm();
    Matrix D_inv = A.D().Inv();
    Matrix mD_inv = -D_inv;
    //Matrix DLU = (-D_inv) * (A.L() + A.U());
    Matrix LU = (A.L() + A.U());
    double jakobi_start = dtime();
    while (norm > 1.0e-9) {
        Matrix LUr = LU * r;
        r = mD_inv*LUr + (D_inv * b);
        //r = DLU * r + (D_inv * b);
        norm = (A * r - b).norm();
        iters++;
        normy.pushBack(norm);
    }
    double jakobi_stop = dtime();
    double jakobi_time = jakobi_stop - jakobi_start;
    printf("\n\tJakobi\niters =%d, secs =%.3lf\n", iters, jakobi_time);
    for (int i = 0; i < normy.GetSize(); i++) {
        printf("%10e,\n", normy[i]);
    }
   // printf("\n");
   // r.Print();
}
void excB2() {
    ///Gauss–Seidl
    Vector<double> normy;
    Matrix A = CreateMatrix(n, 11, -1, -1);
    Matrix b = CreateVector(n, 8);
    Matrix r = Ones(n, 1);
    int iters = 0;
    double norm = (A * r - b).norm();
    Matrix U = A.U();
    Matrix DL = A.D() + A.L();
    Matrix mDL = -DL;
    double gs_start = dtime();
    while (norm > 1.0e-9) {
        r = (mDL >> (U * r)) + (DL >> b);
        norm = (A * r - b).norm();
        iters++;
        normy.pushBack(norm);
        printf("\n");
    }
    double gs_stop = dtime();
    double gs_time = gs_stop - gs_start;
    printf("\n\tGauss_Seidl\niters =%d, secs =%.3lf\n", iters, gs_time);
    for (int i = 0; i < normy.GetSize(); i++) {
        printf("%10e,\n", normy[i]);
    }
}
void excC1() {
    Vector<double> normy;
    Matrix A = CreateMatrix(n, 3, -1, -1);
    Matrix b = CreateVector(n, 8); //8
    ///Jakobi
    Matrix r = Ones(n, 1);
    int iters = 0;
    double norm = (A * r - b).norm();
    Matrix D_inv = A.D().Inv();
    Matrix DLU = (-D_inv) * (A.L() + A.U());
    while (norm > 1.0e-9 ) {
        if (iters > 1000 || *(long long*)&norm == 0x7FF0000000000000)
            break;
        r = DLU * r + (D_inv * b);
        norm = (A * r - b).norm();
        iters++;
        normy.pushBack(norm);
    }
    for (int i = 0; i < normy.GetSize(); i++) {
        printf("%10e,\n", normy[i]);
    }  
}
void excC2() {
    ///Gauss–Seidl
    Vector<double> normy;
    Matrix A = CreateMatrix(n, 3, -1, -1);
    Matrix b = CreateVector(n, 8); //8
    Matrix r = Ones(n, 1);
    double iters = 0;
    double norm = (A * r - b).norm();
    Matrix U = A.U();
    Matrix DL = A.D() + A.L();
    Matrix mDL = -DL;    
    while (norm > 1.0e-9) {
        if (iters > 1000 || *(long long*)&norm == 0x7FF0000000000000)
            break;
        r = (mDL >> (U * r)) + (DL >> b);
        norm = (A * r - b).norm();
        iters++;
        normy.pushBack(norm);
    }
    
    for (int i = 0; i < normy.GetSize(); i++) {
        printf("%10e,\n", normy[i]);
    }
}
void excD() {
    Matrix A = CreateMatrix(n, 3, -1, -1);
    Matrix b = CreateVector(n, 8);
    Matrix L, U;
    A.factorizeLU(&L, &U); //A = LU
    Matrix z = L >> b;
    Matrix x = U << z;
    double res = (A * x - b).norm();
    printf("\t%10e\n", res);
    //x.Print();
}
void excE1() {
    int N[10] = { 100, 200, 300, 500, 700, 1000, 2000, 3000, 4000, 5000 };
    for (int i = 0; i < 10; i++) {
        Matrix A = CreateMatrix(N[i], 11, -1, -1);
        Matrix b = CreateVector(N[i], 8);
        Matrix r = Ones(N[i], 1);
        int iters = 0;
        double norm = (A * r - b).norm();
        Matrix D_inv = A.D().Inv();
        Matrix mD_inv = -D_inv;
        //Matrix DLU = (-D_inv) * (A.L() + A.U());
        Matrix LU = (A.L() + A.U());
        double jakobi_start = dtime();
        while (norm > 1.0e-9) {
            Matrix LUr =  LU*r;
            r = mD_inv * LUr + (D_inv * b);
            norm = (A * r - b).norm();
            iters++;
        }
        double jakobi_stop = dtime();
        double jakobi_time = jakobi_stop - jakobi_start;
        printf("%.4lf\n", jakobi_time);
    }
}
void excE2() {
    int N[10] = { 100, 200, 300, 500, 700, 1000, 2000, 3000, 4000, 5000 };
    for (int i = 0; i < 10; i++) {
        Matrix A = CreateMatrix(N[i], 11, -1, -1);
        Matrix b = CreateVector(N[i], 8);
        Matrix r = Ones(N[i], 1);
        int iters = 0;
        double norm = (A * r - b).norm();
        Matrix U = A.U();
        Matrix DL = A.D() + A.L();
        Matrix mDL = -DL;
        double gs_start = dtime();
        while (norm > 1.0e-9) {
            r = (mDL >> (U * r)) + (DL >> b);
            norm = (A * r - b).norm();
            iters++;
        }
        double gs_stop = dtime();
        double gs_time = gs_stop - gs_start;
        printf("%.4lf\n", gs_time);
    }
}
void excE3() {
    int N[10] = { 100, 200, 300, 500, 700, 1000, 2000, 3000, 4000, 5000 };
    for (int i = 0; i < 10; i++) {
        Matrix A = CreateMatrix(N[i], 11, -1, -1);
        Matrix b = CreateVector(N[i], 8);
        Matrix L, U;
        double gs_start = dtime();
        A.factorizeLU(&L, &U); //A = LU
        Matrix z = L >> b;
        Matrix x = U << z;   
        double gs_stop = dtime();
        double gs_time = gs_stop - gs_start;
        printf("%.4lf,\n", gs_time);
    }
}
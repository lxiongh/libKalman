#include <iostream>
#include "Matrix.h"
#include "stdlib.h"
#include "GpsPath.h"
#include "Kalman2D.h"

using namespace std;

int main()
{
    double data_ptr[] = {2,1,4,3};
    double data_ptr1[] = {1,1,2,2};
    double data_ptr2[] = {1,2,3,4,5,6};

    Matrix A = Matrix(2,2,data_ptr);
    Matrix B = Matrix(2,2,data_ptr1);
    Matrix D = Matrix(2,3,data_ptr2);

    Matrix C = Matrix(2,2);
    C = A*B;
    cout << "A:\n";
    A.DisplayMtr();
    cout<<"\nB:\n";
    B.DisplayMtr();
    cout<<"\nA*B: \n";
    C.DisplayMtr();
    cout<<"\nA-B: \n";
    C = A-B;
    C.DisplayMtr();
    cout<<"\nA+B: \n";
    C = A+B;
    C.DisplayMtr();

    cout<<"\nTranspose:\n";
    D.TransposeMtr();
    D.DisplayMtr();

    double data_ptr3[] = {1,2,3,4};
    Matrix E = Matrix(2,2,data_ptr3);

    cout<<"\nInverse of E\n";
    D = E.InverseMtr();
    D.DisplayMtr();

    cout <<"\nE:\n";
    E.DisplayMtr();
    //Matrix F = Matrix(2,2);
    Matrix F;

    cout <<"\ninv(E)*E\n";
    F = D*E;
    F.DisplayMtr();

    cout << "-------------\n";
    //---------------------

    GpsPath gp = GpsPath();
    Kalman2D kal = Kalman2D();
    double t1;
    kal.Reset(0.1,0.1,0.2,0.1,1);
    //for(int i=0; i<2; i++){
    //    t1 = kal.Update(0.1,0.2,0.1);
    //}
    t1 = kal.Update(0.1,0.2,0.1);
    t1 = kal.Update(0.1,0.2,0.1);
    //t1 = kal.Update(0.1,0.2,0.1);
	for(int i=0; i<3000; i++){
        t1 = kal.Update(0.1,0.2,0.1);
    }
    //t1 = kal.Update(0.1,0.2,0.1);
    cout << t1;
    //system("pause");

    return 0;
}

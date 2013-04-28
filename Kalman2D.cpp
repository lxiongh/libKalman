#include "Kalman2D.h"

Kalman2D::Kalman2D()
{
    //ctor
    m_x = Matrix(2,1);
    m_p = Matrix(2,2);
    m_q = Matrix(2,2);

}

Kalman2D::~Kalman2D()
{
    //dtor
}

void Kalman2D::Reset(double qx, double qv, double r, double pd, double ix){
    m_q.Set(1,1,qx*qx);
    m_q.Set(1,2,qx*qv);
    m_q.Set(2,1,qx*qv);
    m_q.Set(2,2,qv*qv);

    m_r = r;

    m_p.Set(1,1,pd);
    m_p.Set(2,2,pd);
    m_p.Set(1,2,0);
    m_p.Set(2,1,0);

    m_x.Set(1,1,ix);
    m_x.Set(2,1,0);
}

double Kalman2D::Update(double mx,double mv, double dt){
    double data_f[] = {1,0,dt,1};
    double data_h[] = {1,0,0,1};
    double data_ht[] = {1,0,0,1};
    double data_id[] = {1,0,0,1};
    double data_z[] = {mx,mv};
    Matrix f = Matrix(2,2,data_f);
    Matrix h = Matrix(2,2,data_h);
    Matrix ht = Matrix(2,2,data_ht);
    Matrix id = Matrix(2,2,data_id);
    Matrix z = Matrix(2,1,data_z);

    //预测系统状态,其中U={0,0}
    // X = F*X + H*U
    m_x = f * m_x;
	//f.DisplayMtr();
	//m_x.DisplayMtr();
    //预测系统方差
    // P = F*P*F^T + Q
    m_p = f * m_p * (f.TransposeMtr()) + m_q;
    //修正卡尔曼混合系数
	Matrix tmp = h * m_p * ht + m_r;
    Matrix sinv = tmp.InverseMtr();
    Matrix k = m_p * ht * sinv;
    //修正系统状态
    m_x = (id - k) * m_x + k * z;
    //修正系统均方差
    m_p = (id - k*h) * m_p;

    return m_x.Get(1,1);
}



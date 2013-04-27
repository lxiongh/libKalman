#ifndef KALMAN2D_H
#define KALMAN2D_H
#include "Matrix.h"

class Kalman2D
{
    public:
        Kalman2D();
        virtual ~Kalman2D();

        void Reset(double qx, double qv, double r, double pd, double ix);
        double Update(double mx, double mv, double dt);
    protected:
    private:
        Matrix m_x;
        Matrix m_p;
        Matrix m_q;
        double m_r;
};

#endif // KALMAN2D_H

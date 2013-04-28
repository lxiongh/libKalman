#ifndef GPSPATH_H
#define GPSPATH_H

#include <vector>
#include <math.h>
#include "Random.h"

using namespace std;

class GpsPath
{
    public:
        GpsPath();
        GpsPath(int seed);
        virtual ~GpsPath();
        void CreatePath();
        void SimulateGPS();
        int GetPosOverSampling(){return positionOverSampling;}

        vector<double> Ideal_Time;
        vector<double> Ideal_X;
        vector<double> Ideal_Y;
        vector<double> idealVX;
        vector<double> idealVY;

        vector<double> Meas_Time;
        vector<double> Meas_X;
        vector<double> Meas_Y;
        vector<double> Meas_VX;
        vector<double> Meas_VY;
        vector<double> Meas_AX;
        vector<double> Meas_AY;
    protected:
    private:

        double GPS_Position1Sigma;
        double GPS_Velocity1Sigma;
        double Accelerometer_1Sigma;
        double GPS_MeasurementHz;

        double AmountInFront;
        int RandomSeed;
        int positionOverSampling;
        };

#endif // GPSPATH_H

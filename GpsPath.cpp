#include "GpsPath.h"

GpsPath::GpsPath(){
    //ctor
    GPS_Position1Sigma = 2.5;
    GPS_Velocity1Sigma = 0.1;
    Accelerometer_1Sigma = 0.000145;
    GPS_MeasurementHz = 10;

    AmountInFront = 1.5;
    RandomSeed = 0;
    positionOverSampling = 20;

    CreatePath();
    SimulateGPS();
}

GpsPath::GpsPath(int seed){
    GPS_Position1Sigma = 2.5;
    GPS_Velocity1Sigma = 0.1;
    Accelerometer_1Sigma = 0.000145;
    GPS_MeasurementHz = 10;

    AmountInFront = 1.5;
    RandomSeed = seed;
    positionOverSampling = 20;

    CreatePath();
    SimulateGPS();
}

GpsPath::~GpsPath()
{
    //dtor
}

void GpsPath::CreatePath()
{
    double dt = 1 / (GPS_MeasurementHz * positionOverSampling);

    Ideal_Time.clear();
    Ideal_X.clear();
    Ideal_Y.clear();

    double x = 0;
    double y = 0;
    double vx = 0;
    double vy = 0;

    double secondsToReAcquire = 20;

    //double velocity = 2; // 2 m/s.
    //double amount2ndIsInFront = AmountInFront; // meters.
    //double vt = amount2ndIsInFront / velocity;  // 1.5 meter in front, at a velocity of 2 m/s = 0.75 seconds.
    double initialAngle = 10;
    double secondsToRunStrait = 100;
    double accelSeconds = 4;
    //double x2s = amount2ndIsInFront * sin(initialAngle * 3.14 / 180);
    //double y2s = amount2ndIsInFront * cos(initialAngle * 3.14 / 180);


    // Accelerate over accelSeconds to velocity.
    double ax = sin(initialAngle * 3.14 / 180);
    double ay = cos(initialAngle * 3.14 / 180);
    vector<double> xacc;
    vector<double> yacc;
    double accelMax = 0.25; // 0.25 m/s^2.

    // Hold still for secondsToReAcquire
    for (double t = 0; t < secondsToReAcquire; t += dt)
    {
        xacc.push_back(0);
        yacc.push_back(0);
    }
    // accel for accelSeconds.
    for (double t = 0; t < accelSeconds; t += dt)
    {
        xacc.push_back(accelMax * ax);
        yacc.push_back(accelMax * ay);
    }
    // Cruise for secondsToRunStrait.
    for (double t = 0; t < secondsToRunStrait; t += dt)
    {
        xacc.push_back(0);
        yacc.push_back(0);
    }
    // de-accel for accelSeconds.
    for (double t = 0; t < accelSeconds; t += dt)
    {
        xacc.push_back(-accelMax * ax);
        yacc.push_back(-accelMax * ay);
    }
    // Hold still for secondsToReAcquire
    for (double t = 0; t < secondsToReAcquire; t += dt)
    {
        xacc.push_back(0);
        yacc.push_back(0);
    }

    // accel, rotate.
    double angular = 10;
    double angle = initialAngle;
    double time = 0;
    int k = xacc.size();
    while (angle < initialAngle + 180)
    {
        angle += angular * dt;
        double acc = time < accelSeconds ?  accelMax * time / accelSeconds : accelMax;
        xacc.push_back(acc * sin(angle * 3.14 / 180));
        yacc.push_back(acc * cos(angle * 3.14 / 180));
        vx += xacc[k] * dt;
        vy += yacc[k] * dt;
        x += vx * dt;
        y += vy * dt;
        time += dt;
        k++;
    }


    // de-accel for accelSeconds.
    initialAngle = angle;
    for (double t = 0; t < accelSeconds; t += dt)
    {
        xacc.push_back(-vx / accelSeconds);
        yacc.push_back(-vy / accelSeconds);
    }

    // Hold still for secondsToReAcquire
    for (double t = 0; t < secondsToReAcquire; t += dt)
    {
        xacc.push_back(0);
        yacc.push_back(0);
    }
    // accel for accelSeconds.
    ax = sin(initialAngle * 3.14 / 180);
    ay = cos(initialAngle * 3.14 / 180);
    for (double t = 0; t < accelSeconds; t += dt)
    {
        xacc.push_back(accelMax * ax);
        yacc.push_back(accelMax * ay);
    }
    // Cruise for secondsToRunStrait.
    for (double t = 0; t < secondsToRunStrait; t += dt)
    {
        xacc.push_back(0);
        yacc.push_back(0);
    }
    // de-accel for accelSeconds.
    for (double t = 0; t < accelSeconds; t += dt)
    {
        xacc.push_back(-accelMax * ax);
        yacc.push_back(-accelMax * ay);
    }
    // Hold still for secondsToReAcquire
    for (double t = 0; t < secondsToReAcquire; t += dt)
    {
        xacc.push_back(0);
        yacc.push_back(0);
    }


    time = 0;
    vx = 0;
    vy = 0;
    x = 0;
    y = 0;
    for (unsigned int i = 0; i < xacc.size(); i++)
    {
        Ideal_Time.push_back(time);
        Ideal_X.push_back(x);
        Ideal_Y.push_back(y);

        vx += xacc[i] * dt;
        vy += yacc[i] * dt;
        x += vx * dt;
        y += vy * dt;
        time += dt;
    }
}

void GpsPath::SimulateGPS()
{
    Random r;
    r.SetSeed(RandomSeed);
    Meas_Time.clear();
    Meas_X.clear();
    Meas_Y.clear();
    Meas_VX.clear();
    Meas_VY.clear();

    vector<double> idealX;
    vector<double> idealY;

    idealVX.clear();
    idealVY.clear();

    for (unsigned int i = 0; i < Ideal_Time.size(); i += positionOverSampling)
    {
        Meas_Time.push_back(Ideal_Time[i]);
        idealX.push_back(Ideal_X[i]);
        idealY.push_back(Ideal_Y[i]);
        Meas_X.push_back(Ideal_X[i] + r.NextGuass_Double(0, GPS_Position1Sigma));
        Meas_Y.push_back(Ideal_Y[i] + r.NextGuass_Double(0, GPS_Position1Sigma));
    }
    double dt = Meas_Time[1] - Meas_Time[0];
    for (unsigned int i = 0; i < Meas_X.size() - 1; i++)
    {
        double vx = (idealX[i + 1] - idealX[i]) / dt;
        double vy = (idealY[i + 1] - idealY[i]) / dt;

        idealVX.push_back(vx);
        idealVY.push_back(vy);

        vx += r.NextGuass_Double(0, GPS_Velocity1Sigma);
        vy += r.NextGuass_Double(0, GPS_Velocity1Sigma);

        Meas_VX.push_back(vx);
        Meas_VY.push_back(vy);
    }
    idealVX.push_back(0);
    idealVY.push_back(0);
    Meas_VX.push_back(0);
    Meas_VY.push_back(0);

    for (unsigned int i = 0; i < Meas_VX.size() - 1; i++)
    {
        double ax = (idealVX[i + 1] - idealVX[i]) / dt;
        double ay = (idealVY[i + 1] - idealVY[i]) / dt;

        ax += r.NextGuass_Double(0, Accelerometer_1Sigma);
        ay += r.NextGuass_Double(0, Accelerometer_1Sigma);

        Meas_AX.push_back(ax);
        Meas_AY.push_back(ay);
    }
    Meas_AX.push_back(0);
    Meas_AY.push_back(0);
}

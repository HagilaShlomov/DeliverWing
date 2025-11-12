#include "Accelerometer.h"
#include <cmath>

Accelerometer::Accelerometer() : ax(0), ay(0), az(0) {}

void Accelerometer::update(double x, double y, double z) {
    ax = x;
    ay = y;
    az = z;
}

double Accelerometer::getAx() const {
    return ax;
}

double Accelerometer::getAy() const {
    return ay;
}

double Accelerometer::getAz() const {
    return az;
}
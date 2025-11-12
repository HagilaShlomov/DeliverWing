#pragma once
#include "Sensors.h"
class Accelerometer :
    private Sensors
{
private:
    double ax, ay, az; //תאוצה של הרחפן

public:
    Accelerometer();

    // עדכון ערכים - לדוגמה מתוך חומרה או סימולציה
    void update(double x, double y, double z);

    // מקבלים את התאוצה בצירים השונים
    double getAx() const;
    double getAy() const;
    double getAz() const;
};


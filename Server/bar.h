#pragma once
#include "Barometer.h"
#include <cmath>
#include <random>

class Barometer
{
    double pressure;      // לחץ בפסקל
    double temperature;   // טמפרטורה אם קיימת
    
	// ברירת מחדל 101325.0 = 1 atm = 1013.25 hPa = 1013.25 mbar
    //גובה פני הים התחלתי

public:    
	void setPressure(double p) { pressure = p; }
	void setTemperature(double t) { temperature = t; }


	Barometer(double p, double t): pressure(p), temperature(t) {}
	Barometer() : pressure(101325.0), temperature(15.0) {}

	double getPressure() const {
		return pressure; // מחזיר לחץ בפסקל
	}

	double getTemperature() const {
		return temperature; // מחזיר טמפרטורה אקראית בטווח 5–40 מעלות צלזיוס
	}


};
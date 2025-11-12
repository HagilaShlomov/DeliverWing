#pragma once

enum class FlightMode {
	Hover,         // טיסה יציבה
	Acceleration,  // האצה
	Deceleration,  // האטה
	Climb,         // עלייה
	Turn           // פנייה
};

double getConsumptionForMode(FlightMode mode);

class Battery
{
private:
	double capacity_mAh; //זרם במדידת מיליאמפר שעה
	double voltage_V; //מתח בולט
	double remaining_mAh;//זרם שנותר במיליאמפר שעה


public:
	Battery()
		: capacity_mAh(20000), voltage_V(22.2), remaining_mAh(20000) { //נתונים קלאסיים של רחפני משלוחים
	}
	double energy() const;//אנרגיה זמינה
	double getCapacity() const;
	double getVoltage() const;
	double getRemaining() const;
	void setRemaining(double remaining);

	double energyNeededForNewNode(double newVoltage, double newRemaining_mAh) const; //חישוב למדידת אנרגיה נדרשת צומת חדשה

	void charge();
	void discharge(double amount);
};


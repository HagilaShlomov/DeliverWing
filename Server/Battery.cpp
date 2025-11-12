#include "Battery.h"

double getConsumptionForMode(FlightMode mode) { //חישוב צריכת אנרגיה לפי מצב טיסה
	switch (mode) {
	case FlightMode::Hover: return 500;         // לדוגמה: 500 mAh לשעה
	case FlightMode::Acceleration: return 800;
	case FlightMode::Deceleration: return 400;
	case FlightMode::Climb: return 900;
	case FlightMode::Turn: return 600;
	default: return 0;
	}
}

double Battery::getCapacity() const { return capacity_mAh; }
double Battery::getVoltage() const { return voltage_V; }
double Battery::getRemaining() const { return remaining_mAh; }
void Battery::setRemaining(double remaining) {
	if (remaining >= 0 && remaining <= capacity_mAh) {
		remaining_mAh = remaining;
	}
}

double Battery::energy() const {//כמה אנרגיה זמינה עכשיו
	return voltage_V * remaining_mAh / 1000.0; // המרת מיליאמפר שעה לזרם אמיתי
}

double Battery::energyNeededForNewNode(double newVoltage, double newRemaining_mAh) const {
	double currentEnergy = Battery::energy(); // אנרגיה נוכחית
	double newEnergy = newVoltage * newRemaining_mAh / 1000.0; // אנרגיה חדשה
	double energyNeeded = newEnergy - currentEnergy;
	if (energyNeeded < 0) energyNeeded = 0; // אם האנרגיה כבר גבוהה, לא צריך להוסיף
	return energyNeeded;
}

void Battery::charge() {
	remaining_mAh = capacity_mAh; //מטעין את הסוללה למקסימום
}

void Battery::discharge(double amount) {
	if (amount > 0 && remaining_mAh - amount >= 0) {
		remaining_mAh -= amount;
	}
}
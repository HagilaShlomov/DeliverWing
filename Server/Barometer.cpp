//#include "Barometer.h"
//#include <cmath>
//#include <random>
//
//Barometer::Barometer() : pressure(101325.0), temperature(15.0) {}
//
//double Barometer::getPressure() const {
//	static std::default_random_engine generator;
//	static std::uniform_real_distribution<double> distribution(99000.0, 102000.0);
//	return distribution(generator); // מחזיר לחץ אקראי בטווח 99000–102000 פסקל
//}
//
//double Barometer::getTemperature() const {
//	static std::default_random_engine generator;
//	static std::uniform_real_distribution<double> distribution(5.0, 40.0);
//	return distribution(generator); // מחזיר טמפרטורה אקראית בטווח 5–40 מעלות צלזיוס
//}




//bool Barometer::checkSensorValidity()
//{//לחץ בין בערך 99000–102000 פסקל (בהתאם לגובה).
////טמפרטורה בין בערך 5–40 מעלות צלזיוס.
//	if (pressure < 99000 || pressure > 102000)
//		return false; // לחץ לא תקין
//	if (temperature < 5 || temperature > 40)
//		return false; // טמפרטורה לא תקינה
//	return false;
//}
//
//bool Barometer::checkAltitudeValidity() {
//	//אני רוצה לאתחל את הלחץ והטמפרטורה בטווח של 40-70 מטר על פני הים ולכן לפי חישובים הטווח הוא 
//	//בגובה 40 מטר: לחץ האוויר הוא כ־100,845 פסקל, שזה בערך 14.74° C.
//	//בגובה 70 מטר: לחץ האוויר הוא כ־100,487 פסקל, שזה בערך 14.55° C.
//	// בדיקה אם גובה תקין
//	if (pressure < 100, 845 || pressure > 100, 487)
//		return false; // גובה לא תקין
//	if (temperature < 14.55 || temperature > 14.74)
//		return false; // גובה לא תקין
//	
//	return true; 
//}
//
//
//double Barometer::calculateAltitude(double pressure){
//	//חישוב לפי טמפרטורה ולחץ
//	double tempKelvin = getTemperature() + 273.15;
//	double altitude = (tempKelvin / 0.0065) * (1.0 - pow(pressure / 101325.0, 0.1903));
//	//קלווין = צלזיוס + 273.15 תמיד
//	//קצב שינוי טמפרטורה בגובה (0.0065 מעלות למטר
//	return altitude;
//
//	// נוסחת חישוב גובה על פי לחץ
//	//double altitude = 44330.0 * (1.0 - pow(pressure / 101325.0, 0.1903));
//	// altitude = 44330.0 * (1.0 - pow(pressure / 101325.0, 0.1903));
//	// 101325 = 1 atm = 1013.25 hPa = 1013.25 mbar
//	// 44330 = גובה הים ב-Pa
//}

//#include <stdexcept> // Add this include for std::invalid_argument
//
#include "LiDar.h"
#include <cmath>
//#include "CostCalculate.h"
//#include "initAreaCoordinate.h"
//using namespace std;



//Coordinate LiDar::getPoint(Coordinate current) const
//{
//	//random point with 600 meters range from current
//	double angle = static_cast<double>(rand()) / RAND_MAX * 2 * M_PI; // Random angle in radians
//	double distance = static_cast<double>(rand()) / RAND_MAX * maxRange; // Random distance in kilometers
//	double lat = current.y + (distance * cos(angle)) / 111.32; // 1 degree latitude ~ 111.32 km
//	double lon = current.x + (distance * sin(angle)) / (111.32 * cos(degToRad(current.y))); // 1 degree longitude ~ 111.32 km at the equator
//	// Return the new point as a Coordinate object
//	if (distance < maxRange) {
//		return Coordinate(lon, lat, current.z);
//	}
//	
//	return Coordinate();
//}
//
//bool pointInPath(Coordinate next, Coordinate rand) {
//	
//	double rangeInKm = 0.5; // 500 מטר = 0.5 ק"מ
//
//	double latRange = rangeInKm / KM_IN_DEG_LAT; // קבוע: מעלה בקו רוחב ? 111.32 ק"מ
//
//	// המרחק במעלות קו אורך (תלוי בקו הרוחב שלך = x)
//	double lonRange = rangeInKm * KM_IN_DEG_LON / cos(rand.x * degToRad(rand.y));
//
//	// טווח סביב נקודת rand
//	double minLat = rand.x - latRange;
//	double maxLat = rand.x + latRange;
//	double minLon = rand.y - lonRange;
//	double maxLon = rand.y + lonRange;
//
//	cout << "random point created" << endl;
//
//
//	// בדיקה אם נקודת next נמצאת בטווח
//	return (next.x >= minLat && next.x <= maxLat &&
//		next.y >= minLon && next.y <= maxLon);
//
//}

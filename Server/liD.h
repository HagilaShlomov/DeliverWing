#pragma once
#include "Coordinate.h"
#include "LiDar.h"
#include <cmath>
#include "CostCalculate.h"
#include "initAreaCoordinate.h"

class LiDar 
{
private:
    double maxRange = 1; // טווח מדידה נמדד בקילומטר

public:
    LiDar() {}

    Coordinate getPoint(Coordinate current, Coordinate next) const {
        double distance = static_cast<double>(rand()) / RAND_MAX * maxRange; // טווח עד 1000 מטר
        double angle;

        // נגריל האם העצם יהיה בכיוון המסלול (20%) או סתם רנדומלי (80%)
        if ((rand() % 100) < 20) {
            // בכיוון המסלול – חוסם פוטנציאלי
            double dx = next.x - current.x;
            double dy = next.y - current.y;
            angle = atan2(dy, dx); // כיוון מ-A ל-B
            angle += ((rand() / (double)RAND_MAX) - 0.5) * (M_PI / 6); // רעש קטן (±15°)
        }
        else {
            // כיוון רנדומלי – לא מפריע
            angle = static_cast<double>(rand()) / RAND_MAX * 2 * M_PI;
        }

        double lat = current.y + (distance * cos(angle)) / 111.32;
        double lon = current.x + (distance * sin(angle)) / (111.32 * cos(degToRad(current.y)));

        return Coordinate(lon, lat, current.z);
    }
    
    

};

bool pointInPath(Coordinate next, Coordinate rand) {

    double rangeInKm = 0.5; // 500 מטר = 0.5 ק"מ

    double latRange = rangeInKm / 110.574; // קבוע: מעלה בקו רוחב ? 111.32 ק"מ

    // המרחק במעלות קו אורך (תלוי בקו הרוחב שלך = x)
    double lonRange = rangeInKm * 111.320 / cos(rand.x * degToRad(rand.y));

    // טווח סביב נקודת rand
    double minLat = rand.x - latRange;
    double maxLat = rand.x + latRange;
    double minLon = rand.y - lonRange;
    double maxLon = rand.y + lonRange;

    cout << "random point created" << endl;

    // בדיקה אם נקודת next נמצאת בטווח
    return (next.x >= minLat && next.x <= maxLat &&
        next.y >= minLon && next.y <= maxLon);

}
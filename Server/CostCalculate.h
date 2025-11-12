#pragma once
#include <iostream>
#include <vector>
#include "DataStructures.h"
#include "Server.h"
#include "Weather.h"
#include <unordered_set>
#include "Map.h"
#include <fstream>
#include <cmath>  
#include <winnt.h>
#include <ctype.h>
#include "Coordinate.h"
#include "Node.h"
#include <unordered_map>
#include <queue>
#include <limits>
#include <tuple>

#include "AlgorithmAstar_Functions.h"
#include "json.hpp"
#include <nlohmann/json.hpp>
//using json = nlohmann::json;
//using namespace std;

#define EARTH_RADIUS 6371000.0
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))


//#define DEG_TO_RAD(angle) ((angle) * M_PI / 180.0)
#define ALTITUDE 50.0 
#define M_PI 3.14159265358979323846 // Define M_PI if not already defined  

#define Min_X 32.269460
#define Max_X 32.344876
#define Min_Y 34.833734
#define Max_Y 34.870286


enum Direction {
    NORTH = 0,         // צפון
    EAST = 90,         // מזרח
    SOUTH = 180,       // דרום
    WEST = 270,        // מערב
};

Direction directions[] = {
    NORTH, EAST,
    SOUTH, WEST
};



double degToRad(double deg) { return deg * M_PI / 180.0; }
double radToDeg(double rad) { return rad * 180.0 / M_PI; }



Coordinate vectorToDeg(const Coordinate& c) {
    double R = sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
    double lat = asin(c.z / R);
    double lon = atan2(c.y, c.x);
    return Coordinate(radToDeg(lat), radToDeg(lon), R - EARTH_RADIUS);
}


Coordinate degToVector(double lat_deg, double lng_deg, double alt) {
    double lat = degToRad(lat_deg);
    double lng = degToRad(lng_deg);
    double radius = EARTH_RADIUS + alt; // הוספת גובה מעל פני הים

    double x = radius * cos(lat) * cos(lng);
    double y = radius * cos(lat) * sin(lng);
    double z = radius * sin(lat);
    Coordinate c(x, y, z);

    //cout << c.x << " " << c.y << " " << c.z << endl;

    return c;
}

bool isDestination(Coordinate& from, Coordinate& to) {
    double lat1 = degToRad(from.y);
    double lon1 = degToRad(from.x);
    double lat2 = degToRad(to.y);
    double lon2 = degToRad(to.x);

    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    double a = sin(dLat / 2) * sin(dLat / 2) +
        cos(lat1) * cos(lat2) *
        sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    double distance = EARTH_RADIUS * c;

    return distance < 50.0;
}


double calculateWindSpeed(Node* from, Node* to) {
    // חישוב מהירות הרוח על פי נתוני מזג האוויר של node
    double num = 0.0;
    string windSpeed = from->weatherData["windSpeed"];
    //cout << "Wind Speed: " << windSpeed << " km/h" << std::endl;
    try {
        num = std::stod(windSpeed);
        //std::cout << "המספר הוא: " << num << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "שגיאה: המחרוזת לא מכילה מספר חוקי" << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "שגיאה: המספר מחוץ לתחום האפשרי" << std::endl;
    }

    return num;
}


double calculateWindDirection(Node* from, Node* to) {
    double lat1 = degToRad(from->c.y);
    double lat2 = degToRad(to->c.y);
    double dLon = degToRad(to->c.x - from->c.x);

    double y = sin(dLon) * cos(lat2);
    double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);
    double bearing = atan2(y, x) * 180.0 / M_PI;


	if (bearing < 0) {
		bearing += 360; 
	}

    std::ifstream inFile("wind_directions.json");
    if (!inFile) {
        std::cerr << "Error opening file " << "wind_directions.json" << std::endl;
        return 0.0; 
    }

    json j;
    inFile >> j;

    string windDir = from->weatherData["windDirection"];
    //cout << "Wind direction: " << windDir << std::endl;
    
    double windAngle = getWindAngle(windDir);

    if (windAngle < 0) {
        std::cerr << "Unknown wind direction abbreviation: " << windDir << std::endl;
        return 0.0; 
    }

	windAngle = fmod(windAngle + 180.0, 360.0); // הפיכת הזווית כדי להתאים לכיוון הרוח הנכון

    double angleDiff = std::fabs(bearing - windAngle);
    if (angleDiff > 180) angleDiff = 360 - angleDiff;

    double effect = std::cos(angleDiff * M_PI / 180.0) + calculateWindSpeed(from, to);

    return effect;
}




double calculateWeatherCost(Node* node) {
    double edgeCost = 0.0; 
    double temp = 0;
    if (node->weatherData.contains("temperature")) {
        try {
            temp = std::stod(node->weatherData["temperature"].get<std::string>());
            if (temp > 30) {
                edgeCost = 10.0;
            }
            else if (temp < 10) {
                edgeCost = 5.0;
            }
            else {
                edgeCost = 7.0;
            }

            return edgeCost;
        }
        catch (...) {
            std::cerr << "unable to convert from double to string" << std::endl;
        }
    }

    return edgeCost;

}

double calculateEdgeCost(Node* source,Node* destiny) {
    double temperature = calculateWeatherCost(source); // קריאה לפונקציה לקבלת עלות מזג האוויר
    double wind = calculateWindDirection(source, destiny); // קריאה לפונקציה לקבלת מהירות הרוח

    return temperature+wind;
}

double distance(const Coordinate& a, const Coordinate& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

Node* findClosestNode(const Coordinate& c, std::vector<Node>& nodes_list) {
    Node* closestNode = nullptr;
    double minDist = 100000;

    for (auto& node : nodes_list) {
        if (node.c == c) continue; // דלג על הנקודה עצמה
        double dist = distance(c, node.c);
        if (dist < minDist) {
            minDist = dist;
            closestNode = &node;
        }
    }
    return closestNode;
}

Coordinate nextPoint(Coordinate a, double distance, double bearingDeg) {
    const double R = EARTH_RADIUS; // רדיוס כדור הארץ במטרים
    double lat1 = degToRad(a.y);   // קו רוחב במעלות → רדיאנים
    double lon1 = degToRad(a.x);   // קו אורך במעלות → רדיאנים
    double bearing = degToRad(bearingDeg);
    double angularDistance = distance / R;

    double lat2 = asin(sin(lat1) * cos(angularDistance) +
        cos(lat1) * sin(angularDistance) * cos(bearing));

    double lon2 = lon1 + atan2(sin(bearing) * sin(angularDistance) * cos(lat1),
        cos(angularDistance) - sin(lat1) * sin(lat2));

    // ממירות חזרה למעלות
    double newLat = radToDeg(lat2);
    double newLon = radToDeg(lon2);

    return Coordinate(newLon, newLat, a.z); // חשוב! x=longitude, y=latitude
}






//#pragma once
//
//#include <iostream>
//#include <vector>
//#include "DataStructures.h"
//#include "Server.h"
//#include "Weather.h"
//#include <unordered_set>
//#include "Map.h"
//#include <fstream>
//#include <cmath>  
//#include <winnt.h>
//#include <ctype.h>
//#include "Coordinate.h"
//#include "Node.h"
//#include <unordered_map>
//#include <queue>
//#include <limits>
//#include <tuple>
//#include <algorithm>
//#include "CostCalculate.h"
//
//#include "AlgorithmAstar_Functions.h"
//#include <msxml.h>
//#include "json.hpp"
//#include <nlohmann/json.hpp>
//using json = nlohmann::json;
//using namespace std;
//
//
//
//class Graph2 {
//private:
//    // מיפוי של קואורדינטה לאינדקס
//    unordered_map<Coordinate, int, CoordinateHasher> coord_to_index;
//    unordered_map<int, Coordinate> index_to_coord;
//    vector<vector<double>> adj_matrix;
//    int next_index = 0;
//
//public:
//    // הוספת צומת למיפוי (אם לא קיים)
//    void ensureNode(const Coordinate& c) {
//        if (coord_to_index.find(c) == coord_to_index.end()) {
//            coord_to_index[c] = next_index;
//            index_to_coord[next_index] = c;
//            next_index++;
//
//            // הגדלת המטריצה עבור הצומת החדש
//            for (auto& row : adj_matrix) {
//                row.push_back(0);
//            }
//            adj_matrix.push_back(vector<double>(next_index, 0));
//        }
//    }
//
//    // הוספת קשת בין שני צמתים
//    void addEdge(const Coordinate& from, const Coordinate& to, double cost) {
//        ensureNode(from);
//        ensureNode(to);
//
//        int i = coord_to_index[from];
//        int j = coord_to_index[to];
//        adj_matrix[i][j] = cost;
//        adj_matrix[j][i] = cost; // אם הגרף לא מכוון
//    }
//
//    // הדפסת מטריצת שכנות
//    void printMatrix() {
//        cout << "Adjacency Matrix:" << endl;
//        for (int i = 0; i < adj_matrix.size(); i++) {
//            for (int j = 0; j < adj_matrix[i].size(); j++) {
//                cout << adj_matrix[i][j] << "\t";
//            }
//            cout << endl;
//        }
//
//        cout << "\nIndex to Coordinate Mapping:\n";
//        for (const auto& pair : index_to_coord) {
//            cout << pair.first << ": (" << pair.second.x << ", " << pair.second.y << ", " << pair.second.z << ")\n";
//        }
//    }
//};

//double calculateWindDirectionCoord(Node& from, Node& to) {
//    // חישוב כיוון הרוח על פי נתוני מזג האוויר של node
//    double lat1 = degToRad(from.c.y);
//    double lat2 = degToRad(to.c.y);
//    double dLon = degToRad(to.c.x - from.c.x);
//
//    double y = sin(dLon) * cos(lat2);
//    double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);
//    double bearing = atan2(y, x) * 180.0 / M_PI;
//
//
//    //double bearing = atan2(to.c.y - from.c.y, to.c.x - from.c.x) * 180.0 / M_PI; // חישוב הכיוון בין שני הקודקודים בתצוגה קרטזית
//    if (bearing < 0) {
//        bearing += 360; // הבטחת כיוון בין 0 ל-360 מעלות
//    }
//
//    std::ifstream inFile("wind_directions.json");
//    if (!inFile) {
//        std::cerr << "Error opening file " << "wind_directions.json" << std::endl;
//        return 0.0; // או טיפול שגיאה אחר
//    }
//
//    json j;
//    inFile >> j;
//
//    string windDir = from.weatherData["windDirection"];
//    //cout << "Wind direction: " << windDir << std::endl;
//    cout << endl;
//
//    double windAngle = getWindAngle(windDir);
//
//    if (windAngle < 0) {
//        std::cerr << "Unknown wind direction abbreviation: " << windDir << std::endl;
//        return 0.0; // או טיפול שגיאה
//    }
//
//    windAngle = fmod(windAngle + 180.0, 360.0); // הפיכת הזווית כדי להתאים לכיוון הרוח הנכון
//
//    double angleDiff = std::fabs(bearing - windAngle);
//    if (angleDiff > 180) angleDiff = 360 - angleDiff;
//
//    double effect = std::cos(angleDiff * M_PI / 180.0) + calculateWindSpeed(from, to);
//
//    return effect;
//}
//
//
//
//
//double calculateWeatherCostCoord(Node& node, int i) {
//    
//
//        json weatherData = getWeatherInfo(node.c.x, node.c.y); // קריאה לפונקציה לקבלת מידע על מזג האוויר        
//
//        // כתיבה לקובץ
//        std::ofstream file("weather_data.json" + std::to_string(i), std::ios::app);
//        if (file.is_open()) {
//            file << weatherData.dump(4) << std::endl;
//            file.close();
//        }
//        else {
//            std::cerr << "Error opening file for writing." << std::endl;
//        }
//        //cout <<"weather for node: "<< node.weatherData << endl;
//    
//    //גישה לנתוני מזג אוויר של node
//    double edgeCost = 0.0; // חישוב עלות הקשת, לדוגמה, על פי מזג האוויר או מרחק
//    double temp = 0;
//        try {
//            temp = std::stod(node.weatherData["temperature"].get<std::string>());
//            if (temp > 30) {
//                edgeCost = 10.0;
//            }
//            else if (temp < 10) {
//                edgeCost = 5.0;
//            }
//            else {
//                edgeCost = 7.0;
//            }
//
//            return edgeCost;
//        }
//        catch (...) {
//            std::cerr << "unable to convert from double to string" << std::endl;
//        }
//
//    return edgeCost;
//
//}
//
//double calculateEdgeCostCoord(Coordinate source, Coordinate destiny, int i) {
//	Node sourceNode(source);
//	Node destinyNode(destiny);
//    //double temperature = calculateWeatherCostCoord(sourceNode, i); // קריאה לפונקציה לקבלת עלות מזג האוויר
//    //double wind = calculateWindDirectionCoord(sourceNode, destinyNode); // קריאה לפונקציה לקבלת מהירות הרוח
//    return 10;
//
//}
//void matRec(Graph2* g, Coordinate node, double x_min, double x_max, double y_min, double y_max, int i, Coordinate des) {
//    for (Direction dir : directions) {
//        Coordinate neighborCoord = nextPoint(node, 100.0, dir); // צעד 100 מטר
//
//		cout << "Checking neighbor: " << neighborCoord.x << ", " << neighborCoord.y << std::endl;
//        if (!isValidCoordinate(neighborCoord.x) || !isValidCoordinate(neighborCoord.y)) {
//            continue;
//        }
//
//        if (!(neighborCoord.x >= x_min && neighborCoord.x <= x_max &&
//            neighborCoord.y >= y_min && neighborCoord.y <= y_max)) {
//            continue;
//        }
//
//        double cost = calculateEdgeCostCoord(node, neighborCoord, i); // נדרש לגרסה מבוססת Coordinate בלבד
//
//        g->addEdge(node, neighborCoord, cost);
//
//        if (isDestination(neighborCoord, des)) {
//            std::cout << "Destination found: " << neighborCoord.x << ", " << neighborCoord.y << ", " << neighborCoord.z << std::endl;
//            return;
//        }
//
//        matRec(g, neighborCoord, x_min, x_max, y_min, y_max, i + 1, des);
//    }
//}
//
//
//
//
//Graph2 initGraph3(Coordinate sor, Coordinate des) {
//    double x_min = MIN(sor.x, des.x);
//    double x_max = MAX(sor.x, des.x);
//    double y_min = MIN(sor.y, des.y);
//    double y_max = MAX(sor.y, des.y);
//
//    double lat_margin = (x_max - x_min) * 0.05;
//    double lng_margin = (y_max - y_min) * 0.05;
//
//    double x_min_mar = x_min - lat_margin;
//    double x_max_mar = x_max + lat_margin;
//    double y_min_mar = y_min - lng_margin;
//    double y_max_mar = y_max + lng_margin;
//
//    Graph2 g; // במקום new
//
//    int i = 0;
//
//    matRec(&g, sor, x_min_mar, x_max_mar, y_min_mar, y_max_mar, i, des); // שולח Coordinate ישירות
//
//    g.printMatrix();
//    return g;
//}

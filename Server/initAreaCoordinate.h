#pragma once
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
#include <algorithm>
#include "CostCalculate.h"
#include "InitGraph.h"

#include "AlgorithmAstar_Functions.h"
#include <msxml.h>
#include "json.hpp"
#include <nlohmann/json.hpp>
//using json = nlohmann::json;
//using namespace std;

constexpr double KM_IN_DEG_LAT = 1.0 / 110.574; // קו רוחב
constexpr double KM_IN_DEG_LON = 1.0 / 111.320;


//לשנות ל nextpoint
void initAreaCoordinates() {
    double min_lat = 32.269460, max_lat = 32.344876;
    double min_lon = 34.833734, max_lon = 34.870286;
    double step_km = 0.5;
    double step_lat = step_km * KM_IN_DEG_LAT;
    double step_lon = step_km * KM_IN_DEG_LON;

    json allNodesJson = json::array();

    
    for (double lat = min_lat; lat <= max_lat; lat += step_lat) {
        for (double lon = min_lon; lon <= max_lon; lon += step_lon) {
            
            json nodeJson = {
                {"latitude", lat},
                {"longitude", lon},
                {"weather", getWeatherInfo(lat, lon)}
            };
            allNodesJson.push_back(nodeJson);
        }
    }
    std::ofstream outFile("C:/Users/חגילה/Desktop/לימודים/פרויקט גמר/drone project/Server/Server/Server/weatherjson.json");
    outFile << std::setw(2) << allNodesJson << std::endl;

}

json findWeatherForCoord(const json& data, double lat, double lon, double epsilon = 0.0001) {
    for (const auto& item : data) {
        double itemLat = item["latitude"];
        double itemLon = item["longitude"];

        if (std::abs(itemLat - lat) < epsilon && std::abs(itemLon - lon) < epsilon) {
            if (item.contains("weather")) {
                return item["weather"];
            }
        }
    }
    return {}; // מחזיר אובייקט ריק אם לא נמצא
}

Graph* initGraph() {
    double min_lat = 32.269460, max_lat = 32.344876;
    double min_lon = 34.833734, max_lon = 34.870286;
    double step_km = 0.5;
    double step_lat = step_km * KM_IN_DEG_LAT;
    double step_lon = step_km * KM_IN_DEG_LON;

    std::vector<std::pair<double, double>> directions = {
       {step_lat, 0},     // צפון
       {-step_lat, 0},    // דרום
       {0, step_lon},     // מזרח
       {0, -step_lon}     // מערב
    };

    Graph* g=new Graph();


    std::ifstream inFile("C:/Users/חגילה/Desktop/לימודים/פרויקט גמר/drone project/Server/Server/Server/weatherjson.json");

    if (!inFile) {
        std::cerr << "לא ניתן לפתוח את הקובץ לקריאה.\n";
        return nullptr; 
    }

    json data;
    inFile >> data;

    for (double lat = min_lat; lat <= max_lat; lat += step_lat) {
        for (double lon = min_lon; lon <= max_lon; lon += step_lon) {



            Coordinate c(lat, lon, ALTITUDE);
            auto it = std::find_if(g->adjacency_list.begin(), g->adjacency_list.end(),
                [&](const Node& n) { return n.c == c; });
            if (it == g->adjacency_list.end()) {
                Node node(c);
                node.weatherData = findWeatherForCoord(data, lat, lon);
                g->adjacency_list.push_back(node);
            }


        }
    }
    for (auto& node : g->adjacency_list) {
        for (const auto& dir : directions) {
            Coordinate neighborCoord(
                node.c.x + dir.first,
                node.c.y + dir.second,
                ALTITUDE
            );

            auto it = std::find_if(
                g->adjacency_list.begin(),
                g->adjacency_list.end(),
                [&](const Node& n) {
                    return n.c == neighborCoord;
                }
            );

            if (it != g->adjacency_list.end()) {
                double cost = calculateEdgeCost(&node, &(*it));
                node.neighbors.push_back({ cost, &(*it) });
            }
        }
    }

    return g;
}




//Graph* initGraph() {
//    double min_lat = 32.269460, max_lat = 32.344876;
//    double min_lon = 34.833734, max_lon = 34.870286;
//    double step_km = 0.5;
//    double step_lat = step_km * KM_IN_DEG_LAT;
//    double step_lon = step_km * KM_IN_DEG_LON;
//
//    std::vector<std::pair<double, double>> directions = {
//       {step_lat, 0},     // צפון
//       {-step_lat, 0},    // דרום
//       {0, step_lon},     // מזרח
//       {0, -step_lon}     // מערב
//    };
//
//    Graph* g = new Graph();
//
//
//    std::ifstream inFile("C:/Users/חגילה/Desktop/לימודים/פרויקט גמר/drone project/Server/Server/Server/weatherjson.json");
//
//    if (!inFile) {
//        std::cerr << "לא ניתן לפתוח את הקובץ לקריאה.\n";
//    }
//
//    json data;
//    inFile >> data;
//
//    for (const auto& node : data) {
//
//        double lat = node["latitude"];
//        double lon = node["longitude"];
//        json weather = node["weather"];
//
//        Coordinate c(lat, lon, ALTITUDE);
//        Node node(c);
//        node.weatherData = weather;
//        g->addNodeToMap(node);
//    }
//
//    for (auto& pair : g->nodes_map) {
//        Node* currentNode = &(pair.second);
//        double lat = currentNode->c.y;
//        double lon = currentNode->c.x;
//
//        for (const auto& dir : directions) {
//            double neighborLat = lat + dir.first;
//            double neighborLon = lon + dir.second;
//            Coordinate neighborCoord(neighborLat, neighborLon, ALTITUDE);
//            //Node n = g->nodes_map[neighborCoord];
//            //// בדיקה אם קיימת צומת עם הקואורדינטות האלה
//            //if (g->nodes_map.find(neighborCoord) == g->nodes_map.end()) {
//            //    double cost = calculateEdgeCost(currentNode, &(g->nodes_map[neighborCoord]));
//            //    Node* neighborNode = &(g->nodes_map[neighborCoord]);
//            //    currentNode->neighbors.push_back(std::make_pair(cost, neighborNode));
//            //}
//
//            auto it = g->nodes_map.find(neighborCoord);
//            if (it != g->nodes_map.end()) {
//                Node* neighborNode = &(it->second);
//                // רק אם זה לא הצומת עצמו
//                if (currentNode != neighborNode) {
//                    double cost = calculateEdgeCost(currentNode, neighborNode);
//                    currentNode->neighbors.push_back(std::make_pair(cost, neighborNode));
//                }
//            }
//        }
//
//    }
//
//    return g;
//}
//
//

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

#include "AlgorithmAstar_Functions.h"
#include "json.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;
 

class Graph {
public:

	vector<Node> adjacency_list;
    unordered_map<Coordinate, Node, CoordinateHasher> nodes_map;


    void insertStartAndEnd(Coordinate start, Coordinate end) {

        auto nodeExists = [&](const Coordinate& c) -> bool {
            return std::any_of(adjacency_list.begin(), adjacency_list.end(), [&](const Node& n) {
                return n.c == c;
                });
            };

        if (nodeExists(start)) {
            std::cout << "Start coordinate already exists in the graph.\n";
        }
        else {
            json startweather = getWeatherInfo(start.y, start.x);
            Node startNode(start);
            startNode.weatherData = startweather;
            adjacency_list.push_back(startNode); // הכנסה לפני השימוש במצביע
            Node* startNodePtr = &adjacency_list.back();

            Node* closestToStart = findClosestNode(start, adjacency_list);
            if (closestToStart != nullptr && closestToStart->c != start) {
                double cost = calculateEdgeCost(startNodePtr, closestToStart);
                startNodePtr->neighbors.push_back({ cost, closestToStart });
                cout << "start coordinate added to graph" << endl;
            }
        }

        if (nodeExists(end)) {
            std::cout << "End coordinate already exists in the graph.\n";
        }
        else {
            json endweather = getWeatherInfo(end.y, end.x);
            Node endNode(end);
            endNode.weatherData = endweather;
            adjacency_list.push_back(endNode); // גם כאן הכנסה לפני שימוש
            Node* endNodePtr = &adjacency_list.back();

            Node* closestToEnd = findClosestNode(end, adjacency_list);
            if (closestToEnd != nullptr && closestToEnd->c != end) {
                double cost = calculateEdgeCost(closestToEnd, endNodePtr);
                closestToEnd->neighbors.push_back({ cost, endNodePtr });
                cout << "end coordinate added to graph" << endl;
            }
        }
    }

    void removeNode(Coordinate current, Coordinate& coordToRemove) {
        // שלב 1: הסרת הקשתות שמפנות לצומת שצריך למחוק
        for (auto& node : adjacency_list) {
            if (node.c == current) {
               //node.isAvailable = false;
                for (auto& neighbor : node.neighbors) {
                    if (neighbor.second->c == coordToRemove) {
                        neighbor.first = 1000;
                    }
               }
                
            }

            //// שלב 2: הסרת הצומת עצמו מהרשימה
            //node.neighbors.erase(
            //    std::remove_if(
            //        node.neighbors.begin(),
            //        node.neighbors.end(),
            //        [&](const std::pair<double, Node*>& neighbor) {
            //            return neighbor.second->c == coordToRemove;
            //        }
            //    ),
            //    node.neighbors.end()
            //);
        }
    }

    void addEdge(Node* from,  Node* to, double cost) {
        
        auto it = std::find_if(adjacency_list.begin(), adjacency_list.end(), [&](const Node& node) {
            return node.c == from->c;
            });
        if (it != adjacency_list.end()) {
            it->neighbors.push_back(std::make_pair(cost, to));
        }
        else {
            from->neighbors.push_back(std::make_pair(cost, to));
            //adjacency_list.push_back(from);
        }
    }

    void addNodeToMap(Node& from) {
            //תצביע על מה שנימצע ואם הצומת כבר קיים במפה
            //auto it = nodes_map.find(to.c);
			auto it2 = nodes_map.find(from.c);
            if (it2 == nodes_map.end()) {
                //nodes_map[to.c] = to;
                nodes_map[from.c] = from; // הוספת השכן לצומת החדש
            }
           
    }

    void addNeighbor(Node* from, Node* to, double cost) {
        from->neighbors.push_back(std::make_pair(cost, to)); // הוספת השכן לצומת הקיים
        to->neighbors.push_back(std::make_pair(cost, from)); // הוספת השכן לצומת החדש
    }
    
	void printGraph() {
		for (const Node& node : adjacency_list) {
			/*const Coordinate& node = pair.first;
			const vector<std::pair<Coordinate, double>>& neighbors = pair.second;*/
			//cout << "Node: (" << node.weatherData << endl;
			std::cout << "Node: (" << node.c.x << ", " << node.c.y << ", " << node.c.z << ") -> "<<endl;
			for (const auto& neighbor : node.neighbors) {
				std::cout << "(" << neighbor.second->c.x << ", " << neighbor.second->c.y << ", " << neighbor.second->c.z << ") "<<endl;
				cout << "Cost: " << neighbor.first <<endl;
				std::cout << " ----------- " << endl;
			}
			std::cout << std::endl;
		}
	}

    void printMapGraph() {
		for (const auto& pair : nodes_map) {
			const Coordinate& node = pair.first;
			const Node& nodeData = pair.second;
			cout << "Node: (" << nodeData.weatherData << ") -> "<< endl;
			std::cout << "Node: (" << node.x << ", " << node.y << ", " << node.z << ") -> ";
			for (const auto& neighbor : nodeData.neighbors) {
				std::cout << "(" << neighbor.second->c.x << ", " << neighbor.second->c.y << ", " << neighbor.second->c.z << ") ";
				std::cout << "Cost: " << neighbor.first << " | ";
			}
			std::cout << std::endl;
		}
    }

};









//void func(Graph* g, Node* node, double x_min, double x_max, double y_min, double y_max, int i, Coordinate des) {
//    bool isDes = false;
//
//    for (Direction dir : directions) {
//        Coordinate neighborCoord = nextPoint(node->c, 1000.0, dir);
//        if (!isValidCoordinate(neighborCoord.x) || !isValidCoordinate(neighborCoord.y)) {
//            continue;
//        }
//
//        if (!(neighborCoord.x > x_min && neighborCoord.x <x_max &&
//            neighborCoord.y >y_min && neighborCoord.y < y_max)) {
//            continue;
//        }
//
//        Node neighbor(neighborCoord);
//        double cost;
//        if (isDestination(neighborCoord, des)) {
//            cout << "Destination found: " << neighborCoord.x << neighborCoord.y << neighborCoord.z << endl;
//            neighbor = g->nodes_map[des];
//            //cost = calculateEdgeCost(*node, neighbor, i);
//            isDes = true;
//        }
//        else {
//            //cost = calculateEdgeCost(*node, neighbor, i);
//        }
//        cout << "neighbor"<<endl;
//        if (g->nodes_map.find(neighborCoord) == g->nodes_map.end()) {
//            //g->addNodeToMap(*node, neighbor, cost);
//        }
//        //g->addNeighbor(*node, neighbor, cost);
//
//        cout << "i: " << i << endl;
//        if (isDes) {
//            return;
//        }
//
//        func(g, &neighbor, x_min, x_max, y_min, y_max, i + 1, des);
//
//    }
//}

//Graph initGraph2(Coordinate sor, Coordinate des) {
//    double x_min_mar = Min_X;
//    double x_max_mar = Max_X;
//    double y_min_mar = Min_Y;
//    double y_max_mar = Max_Y;
//
//    Graph g; // במקום new
//    Node start(sor), end(des);
//    int i = 0;
//
//    func(&g, &start, x_min_mar, x_max_mar, y_min_mar, y_max_mar, i + 1, des);
//
//    return g;
//}
//
//Graph initGraph1(Coordinate sor, Coordinate des) {
//    double x_min = MIN(sor.x, des.x); //הגדרת גבולות
//    double x_max = MAX(sor.x, des.x);
//    double y_min = MIN(sor.y, des.y);
//    double y_max = MAX(sor.y, des.y);
//
//    double lat_margin = (x_max - x_min) * 0.05; //הגדלת טווח הגבול ב-5 אחוז
//    double lng_margin = (y_max - y_min) * 0.05;
//
//    double x_min_mar = x_min - lat_margin; //מימוש גבולות
//    double x_max_mar = x_max + lat_margin;
//    double y_min_mar = y_min - lng_margin;
//    double y_max_mar = y_max + lng_margin;
//
//    double meters_per_deg_lat = 111000.0; // 1 מעלות = 111 קילומטרים
//    double meters_per_deg_lng = 111000.0 * cos(degToRad(sor.x)); // source.x = latitude
//
//    double lat_step = 100.0 / meters_per_deg_lat;// הגדרת מרחק של 100 מטר בין כל נקודה
//    double lng_step = 100.0 / meters_per_deg_lng;
//
//    Graph g;
//    int count = 0;  
//
//    Coordinate current = degToVector(sor.x, sor.y, sor.z); // מיקום התחלתי
//    Coordinate direction = Coordinate(0.5, 1.2, -0.4); // דוגמה לוקטור כיוון
//    Coordinate destVec = degToVector(des.x, des.y, des.z);
//    Coordinate dir = destVec - current; //וקטור כיוון
//    direction.normalize();
//    Coordinate next = current + (direction * 100);
//
//    int i = 0;//להגבלת הקריאות של API לכל צומת
//
//    Coordinate newLatLon = vectorToDeg(next);
//    cout << "New position: " << newLatLon.x << ", " << newLatLon.y << ", " << newLatLon.z << endl;
//
//
//    for (double lat = x_min_mar; lat <= x_max_mar; lat += lat_step) {
//        for (double lng = y_min_mar; lng <= y_max_mar; lng += lng_step) {
//            Coordinate c(lat, lng, ALTITUDE);
//            i++;
//            Node node(c);
//            node.weatherData = getWeatherInfo(node.c.x, node.c.y);
//            g.adjacency_list.push_back(node);
//            count++;
//
//            for (Direction dir : directions) {
//                Coordinate neighborCoord = nextPoint(c, 100.0, dir); // מרחק 100 מטר בכיוון נתון
//
//                // בדיקת גבולות
//                if (neighborCoord.x >= x_min_mar && neighborCoord.x <= x_max_mar &&
//                    neighborCoord.y >= y_min_mar && neighborCoord.y <= y_max_mar) {
//
//
//                    Coordinate n(neighborCoord.x, neighborCoord.y, sor.z + ALTITUDE);
//                    Node neighbor(n);
//                    cout << "i: " << i << endl;
//                    //double cost = calculateEdgeCost(node, neighbor, i); // חישוב עלות הקשת
//                    //g.addEdge(node, neighbor, cost); // הוספת קשת מהנוכחי לשכן
//
//                }
//            }
//        }
//    }
//
//    //g.printGraph();
//    cout << "Total nodes in graph: " << count << endl;
//    cout << "Graph initialized with " << g.adjacency_list.size() << " nodes." << endl;
//    cout << "i: " << i << endl;
//    return g;
//
//}


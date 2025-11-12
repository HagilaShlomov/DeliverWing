#pragma once
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <random>
#include "AStar.h"
#include "Graph.h"
#include "bar.h"
#include "liD.h"
#include "sensorsAPI.h"
using namespace std;

void runDrone(vector<Coordinate> path, Coordinate destination, Graph* g) {
	Barometer b;
	LiDar l;
    size_t index = 0;
	vector<Coordinate> newPath=path;
    vector<Coordinate> buildings = getBuildingsFromServer(path[0]);

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(14785);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    sendBuildings(buildings, 1, sock, addr);


    while (true) {
        // אם נגמר המסלול – או שהגענו ליעד
        //if (index + 1 >= path.size()) break;

        
		if (newPath[index] == destination) {
			std::cout << "Reached destination" << endl;
			break;
		}

		if (index + 1 >= newPath.size()) {
			std::cout << "No more points in the path." << endl;
			break;
		}
        cout << "Current Position: " << index << endl;

        Coordinate current = newPath[index];
        Coordinate next = newPath[index + 1];
        sendSingleCoordinate(current, 1, 1, sock, addr);

		Barometer b2 = requestBarometer(); // קריאה לשרת לקבלת נתוני ברומטר
        b.setPressure(b2.getPressure()); 
        b.setTemperature(b2.getTemperature()); 
        std::cout << "Barometer: " << b.getPressure() << " Pa, "
            << b.getTemperature() << " C\n";

        Coordinate obstaclePoint = sendLidarCoordinates(current, next);
        bool inRange = pointInPath(next, obstaclePoint);
        bool buildInRange = false;
        std::cout << "LiDAR Point: " << obstaclePoint.toString()
            << " -> " << (inRange ? "IN RANGE" : "OUT OF RANGE") << "\n";

        double droneHeight = current.z; // הגובה הנוכחי של הרחפן
        const double safetyMargin = 5.0; // מטרים מעל הבניין

        double requiredHeight = droneHeight;

        for (const Coordinate& b : buildings) {
            if (pointInPath(next, b)) {
                buildInRange=true;
            }
            if (buildInRange) {
                std::cout << "Building detected in path range.\n";
                if (droneHeight <= b.z && b.z < 60) {
                    // הבניין גבוה מהרחפן - צריך לעלות
                    double newHeight = b.z + safetyMargin;
                    requiredHeight = newHeight;
                    if (index < newPath.size() && index + 1 < newPath.size()) {
                        newPath[index].z = requiredHeight;
                        newPath[index + 1].z = requiredHeight;
                    }
                    //newPath[index + 1].z = requiredHeight;
                    //current.z = requiredHeight; // עדכון הגובה הנוכחי של הרחפן
                    //next.z = requiredHeight; // עדכון הגובה של הנקודה הבאה
                    std::cout << "Raising drone height to: " << requiredHeight << " meters\n";

                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    sendSingleCoordinate(current, 1, 1, sock, addr);
                    buildInRange = false;
                    continue;
                }
            }
        }

        
        if (inRange) {        
            sendSingleCoordinate(obstaclePoint, 1, 2, sock, addr);
            cout << "Obstacle detected at next point: " << next.toString() << endl;
            g->removeNode(current, next);
            newPath = aStarSearch(g, current, destination); // מחשבים מסלול חדש
            index = 0;
            sendPathOverSocket(newPath, 1);
            continue;
        }

        index++;
        sendSingleCoordinate(obstaclePoint, 1, -2, sock, addr);

        std::this_thread::sleep_for(std::chrono::seconds(1));

    }
    clearBuildings(1, sock, addr);

    std::cout << "Arrived at destination!"<< endl;
    sendSingleCoordinate(destination, 1, 1, sock, addr);

    closesocket(sock);
    WSACleanup();

}
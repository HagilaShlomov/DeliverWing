// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_set>
#include "Weather.h" 
#include "httplib.h"
#include "json.hpp"
#include "Server.h"
#include "DataStructures.h"
#include "InitGraph.h"
#include "Order.h"
#include "Coordinate.h"
#include "CostCalculate.h"
#include "User.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <string>

int lastUserId = 0;
extern std::unordered_set<Order> addressSet;
extern std::map<int, User> usersMap;


using namespace httplib;

 
#include <mysql/jdbc.h> // Replace "cppconn/driver.h" with this header
#include "DB_connection.h"
#include "Graph.h"
#include "initAreaCoordinate.h"
#include "Dijkstra.h"
#include "AStar.h"
#include "Simulation.h"
#include "HandleDinamic.h"
#include "sensorsAPI.h"
#include "clientAPI.h"

extern Coordinate source(32.32928, 34.85561, ALTITUDE);
Graph* g = nullptr;

int main() {
    
    bool destinationSet = false;
    g = initGraph();

    Server svr;
    handleOptions(svr);
    handleLogin(svr);
    handleAddOrder(svr, destinationSet);
    
    std::cout << "Server is running on http://localhost:8080" << std::endl;
       
    svr.listen("0.0.0.0", 8080);


	return 0;
}



//Coordinate destiny(32.32387, 34.86207, 50);


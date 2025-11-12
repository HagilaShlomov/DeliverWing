#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_set>
#include "Weather.h" // Include the header file for WeatherAPI
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
#include <string>
#include "AStar.h"
#include "Simulation.h"
#include "HandleDinamic.h"
using json = nlohmann::json;

void run(Graph* g, Coordinate source, Coordinate destination) {
    g->insertStartAndEnd(source, destination);

    std::cout << "Graph size: " << g->adjacency_list.size() << std::endl;

    std::vector<Coordinate> path = aStarSearch(g, source, destination);

    if (!path.empty()) {

        std::cout << "Path found: " << endl;
        for (const auto& coord : path) {
            std::cout << coord.toString() << endl;
        }
        sendPathOverSocket(path, 1, 0);
        runDrone(path, destination, g); // הפעלת סימולציה של מסלול עם רחפן
    }
    else {
        std::cout << "No path found from start to end." << endl;
    }
}
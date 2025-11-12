#pragma once
#ifndef Server_H   /* Include guard */
#define Server_H
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_set>

#include "Weather.h" // Include the header file for WeatherAPI
#include "httplib.h"
#include "json.hpp"
#include "DataStructures.h"
#include "InitGraph.h"
#include "Order.h"
#include "Coordinate.h"
#include "Node.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

extern Graph* g;


void getFromFile();
json getWeatherInfo(double lat, double lon);



#endif 
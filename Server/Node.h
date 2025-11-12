#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include "Coordinate.h"
#include "DataStructures.h"
#include "Order.h"
#include <cmath>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <utility>

class Node{
public:
   Coordinate c;
   //vector<pair<double, Node>> neighbors;
   vector<pair<double, Node*>> neighbors;
   bool isAvailable;
   json weatherData; // To store weather data for this node

   double g;
   double h;
   double f;

   Node(const Coordinate& c) : c(c), isAvailable(true) {}
   Node() : c(Coordinate()), isAvailable(true) {}

   bool operator==(const Node& other) const;
   bool operator!=(const Node& other) const;
   bool operator<(const Node& other) const;


   double getG() const { return g; }
   void setG(double value) { g = value; }

   double getH() const { return h; }
   void setH(double value) { h = value; }

   double getF() const { return g + h; }
   void setF() { f = g + h;}


};

class NodeHasher {
public:
	size_t operator()(const Node& node) const {
		return std::hash<double>()(node.c.x) ^ std::hash<double>()(node.c.y);
	}
};


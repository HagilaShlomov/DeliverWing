#include "Coordinate.h"
#include "DataStructures.h"
#include <unordered_set>
#include <queue> 
#include <vector>
#include <functional>
#include <string>
#include "Order.h"
#include "Node.h"



bool Node::operator==(const Node& other) const {
	return c == other.c;
}
bool Node::operator!=(const Node& other) const {
	return !(*this == other);
}
bool Node::operator<(const Node& other) const {  
   if (neighbors.empty() || other.neighbors.empty()) {  
       return false; // Handle empty neighbors gracefully  
   }  
   return neighbors.front().first < other.neighbors.front().first;  
}


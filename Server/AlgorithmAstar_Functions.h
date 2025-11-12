//#pragma once
//#ifndef AlgorithmAstar_Functions_H
//#define ALGORITHMASTARֹ_FUNCTIONS_H
//#include "DataStructures.h"
//#include <unordered_set>
//#include <queue> // Include this header for std::priority_queue
//#include <vector>
//#include "Coordinate.h"
//#include "InitGraph.h"
//
//using namespace std;
//
//double heuristic(Coordinate& a, Coordinate& b) {
//    double dx = a.x - b.x;
//    double dy = a.y - b.y;
//    double dz = a.z - b.z;
//    return sqrt(dx * dx + dy * dy + dz * dz);
//}
//
//
//std::vector<Node*> reconstruct_path(std::map<Node*, Node*>& came_from, Node* current) {
//    std::vector<Node*> path;
//
//    while (came_from.find(current) != came_from.end()) {
//        path.push_back(current);
//        current = came_from[current];
//    }
//
//    path.push_back(current); // הוספת הצומת ההתחלתי
//
//    std::reverse(path.begin(), path.end());
//    return path;
//}
//
//struct CompareNodes {
//    bool operator()(Node* a, Node* b) const {
//        return a->getF() > b->getF(); // F קטן יותר = עדיפות גבוהה יותר
//    }
//};
//
//std::vector<Coordinate> aStarSearch(Graph* graph, Coordinate& startCoord, Coordinate& endCoord) {  
//   if (!graph) {  
//       std::cerr << "Graph is null.\n";  
//       return {};  
//   }  
//
//   // Find actual start and end Node pointers in the graph's adjacency list  
//   Node* startNode = nullptr;  
//   Node* endNode = nullptr;  
//
//   for (auto& node : graph->adjacency_list) {  
//       if (node.c == startCoord) {  
//           startNode = &node;  
//       }  
//       if (node.c == endCoord) {  
//           endNode = &node;  
//       }  
//   }  
//
//   if (!startNode || !endNode) {  
//       std::cerr << "Start or End node not found in the graph.\n";  
//       return {};  
//   }  
//
//   //// Priority queue for nodes to evaluate, sorted by f_cost  
//   std::priority_queue<Node*, std::vector<Node*>, CompareNodes> openSet;  
//   std::unordered_set<Node*> closedSet;
//
//   //// Map for storing the cheapest cost from start to a node  
//   std::map<Node*, double> gCosts;  
//   std::map<Node*, double> fCost;  
//   //// Map for reconstructing the path  
//   std::map<Node*, Node*> cameFrom;  
//
//   //// Initialize gCosts with infinity for all nodes and 0 for the start node  
//   for (auto& node : graph->adjacency_list) {  
//       gCosts[&node] = std::numeric_limits<double>::infinity(); 
//       fCost[&node] = std::numeric_limits<double>::infinity();
//   }  
//   gCosts[startNode] = 0;  
//
//   //// Add start node to open set  
//   openSet.push(startNode);  
//   fCost[startNode] = gCosts[startNode] + heuristic(startNode->c, endNode->c);  
//
//   while (!openSet.empty()) {  
//       Node* current = openSet.top();  
//       if (current->c == endNode->c) {  
//           std::vector<Node*> pathNodes = reconstruct_path(cameFrom, endNode);  
//           std::vector<Coordinate> pathCoordinates;  
//           for (Node* node : pathNodes) {  
//               pathCoordinates.push_back(node->c);  
//           }  
//           return pathCoordinates;  
//       }  
//       openSet.pop();
//       closedSet.insert(current);  
//
//       for (const auto& edge : current->neighbors) {  //מעבר על כל שכן של הצומת הנוכחי
//           Node* neighbor = edge.second;  //מצביע לשכן
//           double edgeCost = edge.first;  //עלות הקשת
//
//           double tentative_gCost = gCosts[current] + edgeCost;  //מחשב את המרחק הכולל מהתחלה ועד השכן של הנוכחי
//
//           if (tentative_gCost < gCosts[neighbor]) {  //אם המרחק החדש טוב יותר ממה שהיה קודם - נעדכן
//               gCosts[neighbor] = tentative_gCost;  //מעדכנים את המרחק החדש הכי קצר
//               cameFrom[neighbor] = current;  //שמירת הנוכחי בשביל שחזור המסלול
//               double h_cost = heuristic(neighbor->c, endNode->c);  
//               fCost[neighbor] = gCosts[neighbor] + h_cost;  //חישוב היוריסטקה
//
//               if (closedSet.find(neighbor) == closedSet.end()) {  //בדיקה אם קיים ברשימה סגורה כדי למשנוע כפילויות ברשימה הפתוחה
//                   openSet.push(neighbor);  
//               }  
//           }  
//       }  
//   }  
//
//   //// If no path is found  
//   return {};  
//}
//
//
//
//#endif 

#pragma once

#include <vector>
#include "DataStructures.h"
#include "InitGraph.h"
#include "Coordinate.h"
#include "Floyd_Warshal.h"
#include "Node.h"
#include "Weather.h"
#include "Server.h"
#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
using namespace std;

vector<Node*> Dijkstra(Graph* graph, Node s, Node d) {

    Node* source=&s;
    Node* destiny = &d;

    vector<Node*> shortestPath;
    unordered_map<Node*, double> distances;//מרחק מנקודת ההתחלה לכל צומת
    unordered_map<Node*, Node*> previous;//מאיפה הגענו לכל צומת
    priority_queue<pair<double, Node*>, vector<pair<double, Node*>>, greater<pair<double, Node*>>> pq;

    for (auto& node : graph->adjacency_list) {//אתחול
        distances[&node] = numeric_limits<double>::infinity();
        previous[&node] = nullptr;
    }

    distances[source] = 0.0;//אתחול
    pq.push(make_pair(0.0, source));

    while (!pq.empty()) {//לוקחים את הצומת עם המרחק הכי נמוך בתור 
        Node* current = pq.top().second;
        pq.pop();

        if (current == destiny) {
            break;
        }

        for (const auto& neighborPair : current->neighbors) {
            Node* neighbor = neighborPair.second; //שכן הצומת הנוכחי
            double edgeCost = neighborPair.first; //מחשבים מרחק חדש : המרחק עד עכשיו + משקל הקשת
            double newDist = distances[current] + edgeCost;

            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                previous[neighbor] = current;
                pq.push(make_pair(newDist, neighbor));//אם המרחק החדש טוב יותר – מעדכנים את המרחק, רושמים מאיפה באנו, ומכניסים לתור
            }
        }
    }

    // שחזור הנתיב
    for (Node* at = destiny; at != nullptr; at = previous[at]) {//בונים את המסלול מהסוף להתחלה דרך  previous
        shortestPath.push_back(at);
    }
    reverse(shortestPath.begin(), shortestPath.end());//הפיכת  הסדר בחזרה

    return shortestPath;
}

void printDijkstraPath(const std::vector<Node*>& path) {
    if (path.empty()) {
        std::cout << "No path found." << std::endl;
        return;
    }
    std::cout << "Shortest path:" << std::endl;
    for (const auto& node : path) {
        std::cout << "(" << node->c.x << ", " << node->c.y << ", " << node->c.z << ")" << std::endl;
    }
}
// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <vector>


struct Point {
    float x;
    float y;
};

std::vector<Point> calculatePath(std::string city, std::string street, std::string houseNumber) {
    std::vector<Point> path = {
        {0, 0}, {50, 20}, {100, 50}, {150, 80}, {200, 100}
    };
    return path;
}

using namespace httplib;
using json = nlohmann::json;

int main() {
    Server svr;

    // טיפול בבקשות OPTIONS כדי לאפשר CORS
    svr.Options(R"(/.*)", [](const Request& req, Response& res) {
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        res.status = 200;
        });
    svr.set_default_headers({
    {"Access-Control-Allow-Origin", "*"},
    {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
    {"Access-Control-Allow-Headers", "Content-Type, Authorization"}
        });
    
    

    svr.Get("/", [](const Request& req, Response& res) {
        try {
            std::cout << "Received request: " << req.body << std::endl;
            res.set_content("{\"message\": \"HELLO WORLD!\"}", "application/json");

        }
        catch (const std::exception& e) {
            res.status = 500;
            std::cout << "Error: " << e.what() << std::endl;  // הדפסת השגיאה למסוף
            res.set_content("{\"error\": \"Internal Server Error\", \"details\": \"" + std::string(e.what()) + "\"}", "application/json");
        }
        });

    // מסלול POST עבור התחברות
    svr.Post("/login", [](const httplib::Request& req, httplib::Response& res) {
        try {
            std::cout << "Received request: " << req.body << std::endl;

            auto json = json::parse(req.body);  // ניסיון לפענח JSON
            std::string username = json["username"];
            std::string password = json["password"];

            if (username == "admin" && password == "good123") {
                res.set_content("{\"message\": \"Login successful\"}", "application/json");
            }
            else {
                res.status = 401;
                res.set_content("{\"error\": \"Invalid credentials\"}", "application/json");
            }
        }
        catch (const std::exception& e) {
            res.status = 500;
            std::cout << "Error: " << e.what() << std::endl;  // הדפסת השגיאה למסוף
            res.set_content("{\"error\": \"Internal Server Error\", \"details\": \"" + std::string(e.what()) + "\"}", "application/json");
        }
        });

    svr.Post("/AddOrder", [](const Request& req, Response& res) {
        try {
            std::cout << "Received request: " << req.body << std::endl;

            auto json = json::parse(req.body);  // ניסיון לפענח JSON
            std::cout << "Parsed JSON: " << json.dump() << std::endl;

            std::string city = json["city"];
            std::string street = json["street"];
            std::string houseNumber = json["houseNumber"];
            std::string itemCount = json["itemCount"];
            std::string itemWeight = json["itemWeight"];

            std::vector<Point> path = calculatePath(city, street, houseNumber);

            nlohmann::json response;
            response["path"] = json::array();
            for (const auto& p : path) {
                response["path"].push_back({ {"x", p.x}, {"y", p.y} });
            }

            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            res.status = 500;
            res.set_content("{\"error\": \"Internal Server Error\"}", "application/json");
        }
        

    });

    std::cout << "Server is running on http://localhost:8080" << std::endl;

    svr.listen("0.0.0.0", 8080);
}





// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

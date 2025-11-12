#pragma once
#include <iostream>
#include <string>
#include "Coordinate.h"
#include "Order.h"
#include <vector>
#include <fstream>
#include <unordered_set>
#include "Weather.h" // Include the header file for WeatherAPI
#include "httplib.h"
#include "json.hpp"
#include "Server.h"
#include "DataStructures.h"
#include "InitGraph.h"
#include "CostCalculate.h"
#include "User.h"
#include <nlohmann/json.hpp>
#include "runDrone.h"
using json = nlohmann::json;

void handleOptions(Server& svr) {
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
}

void handleGet(Server& svr) {
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
}

void handleLogin(Server& svr) {
    svr.Post("/login", [](const httplib::Request& req, httplib::Response& res) {
        try {
            std::cout << "Received request: " << req.body << std::endl;

            auto json = json::parse(req.body);  // ניסיון לפענח JSON
            std::string username = json["username"];
            std::string password = json["password"];

            User user(username, password);

            User newUser("testuser", "securepass");
            registerUserInDB(newUser);


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
}


void handleAddOrder(Server& svr, bool destinationSet) {
    svr.Post("/AddOrder", [](const Request& req, Response& res) {
        try {
            std::cout << "Received request: " << req.body << std::endl;

            auto json = json::parse(req.body);  // ניסיון לפענח JSON
            cout << "Parsed JSON: " << json.dump() << std::endl;
            cout << " " << std::endl;

            // בדיקת שדות חובה
            if (!json.contains("source") || !json.contains("destination")) {
                res.status = 400;
                res.set_content("{\"error\": \"Missing source or destination coordinates\"}", "application/json");
                return;
            }

            // חילוץ קואורדינטות מקור ויעד
            auto sourceArr = json["source"];
            auto destArr = json["destination"];
            if (!sourceArr.is_array() || sourceArr.size() != 2 || !destArr.is_array() || destArr.size() != 2) {
                res.status = 400;
                res.set_content("{\"error\": \"Coordinates must be arrays of [lat, lon]\"}", "application/json");
                return;
            }
            double sourceLat = sourceArr[0];
            double sourceLon = sourceArr[1];
            double destLat = destArr[0];
            double destLon = destArr[1];

            Coordinate source(sourceLat, sourceLon, ALTITUDE);
            Coordinate destination(destLat, destLon, ALTITUDE);
            bool destinationSet = true;

            cout << "source: " << source.x << source.y << source.z << endl;
            cout << "destination: " << destination.x << destination.y << destination.z << endl;

            string city = json["city"];
            string street = json["street"];
            string houseNumber = json["houseNumber"];
            string itemCount = json["itemCount"];
            string itemWeight = json["itemWeight"];

            std::cout << "Order received: " << city << ", " << street << ", " << houseNumber
                << " | Source: " << sourceLat << "," << sourceLon
                << " | Destination: " << destLat << "," << destLon << std::endl;


            Order orders(city, street, houseNumber, itemCount, itemWeight);
            addressSet.insert(orders);

            std::cout << "\n--- Sending an order ---\n";
            sendOrderToDB(city, street, houseNumber, stod(itemCount), stod(itemWeight));


            nlohmann::json response;

            
            res.set_content(response.dump(), "application/json");

			run(g, source, destination); 
        }
        catch (const std::exception& e) {
            res.status = 500;
            res.set_content("{\"error\": \"Internal Server Error\"}", "application/json");
        }

        });
}


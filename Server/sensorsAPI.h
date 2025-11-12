#pragma once
#include <iostream>
#include "httplib.h"  
#include <nlohmann/json.hpp>
#include "Coordinate.h"
#include "bar.h"
using json = nlohmann::json;

const std::string SERVER_HOST = "localhost";
const int SERVER_PORT = 5000;

Barometer requestBarometer() {
    httplib::Client cli(SERVER_HOST, SERVER_PORT);
    auto res = cli.Get("/barometer");

    if (res && res->status == 200) {
        nlohmann::json jsonRes = nlohmann::json::parse(res->body);

        // לפי השרת, הטמפרטורה והלחץ הן מחרוזות, נהפוך ל-double
        double temperature = std::stod(jsonRes["temperature"].get<std::string>());
        double pressure = std::stod(jsonRes["pressure"].get<std::string>());

        return Barometer(pressure, temperature);
    }
    else {
        std::cerr << "Failed to get barometer data\n";
        return Barometer();  // ערכים אפסיים במקרה של כישלון
    }
}


Coordinate sendLidarCoordinates(const Coordinate& current, const Coordinate& next) {
    httplib::Client cli(SERVER_HOST, SERVER_PORT);

    nlohmann::json payload;
    payload["current"] = {
        {"lon", current.x},
        {"lat", current.y},
        {"alt", current.z}
    };
    payload["next"] = {
        {"lon", next.x},
        {"lat", next.y},
        {"alt", next.z}
    };

    auto res = cli.Post("/lidar", payload.dump(), "application/json");

    if (res && res->status == 200) {
        nlohmann::json result = nlohmann::json::parse(res->body);
        double lon = result["lon"];
        double lat = result["lat"];
        double alt = result["alt"];
        return Coordinate(lon, lat, alt);
    }
    else {
        std::cerr << "Failed to send lidar coordinates\n";
        return Coordinate();
    }
}

std::vector<Coordinate> getBuildingsFromServer(Coordinate& current) {
    std::vector<Coordinate> buildings;
    httplib::Client cli("http://localhost:5000"); // או כתובת השרת שלך

    json payload = {
        {"current", {{"lon", current.x}, {"lat", current.y}, {"alt", current.z}}}
    };

    auto res = cli.Post("/build", payload.dump(), "application/json");
    if (res && res->status == 200) {
        auto data = json::parse(res->body);
        for (const auto& item : data) {
            buildings.emplace_back(item["lon"], item["lat"], item["alt"]);
        }
    }
    else {
        std::cerr << "Error: failed to get response from server" << std::endl;
    }

    return buildings;
}
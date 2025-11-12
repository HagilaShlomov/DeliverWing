#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <ws2tcpip.h>
using json = nlohmann::json;


void sendPathOverSocket(const std::vector<Coordinate>& path, int id, int type = 0) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(14785);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    std::string resetMsg = "{ \"id\": " + std::to_string(id) + ", \"type\": -1 }";
    sendto(sock, resetMsg.c_str(), resetMsg.size(), 0, (sockaddr*)&addr, sizeof(addr));
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // זמן מינימלי לעיבוד

    for (const auto& coord : path) {
        std::ostringstream oss;
        double timestamp = std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();

        oss << "{"
            << "\"id\":" << id << ","
            << "\"x\":" << coord.x << ","
            << "\"y\":" << coord.y << ","
            << "\"z\":" << coord.z << ","
            << "\"type\":" << type << ","
            << "\"timestamp\":" << std::fixed << std::setprecision(6) << timestamp
            << "}";

        std::string msg = oss.str();
        sendto(sock, msg.c_str(), msg.size(), 0, (sockaddr*)&addr, sizeof(addr));
        std::cout << "send to py; " << msg << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(300)); // שיהיה רציף
    }

    closesocket(sock);
    WSACleanup();
}


void sendSingleCoordinate(Coordinate coord, int id, int type, SOCKET sock, sockaddr_in& addr) {
    std::ostringstream oss;
    double timestamp = std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    oss << "{"
        << "\"id\":" << id << ","
        << "\"x\":" << coord.x << ","
        << "\"y\":" << coord.y << ","
        << "\"z\":" << coord.z << ","
        << "\"type\":" << type << ","
        << "\"timestamp\":" << std::fixed << std::setprecision(6) << timestamp
        << "}";

    std::string msg = oss.str();
    sendto(sock, msg.c_str(), msg.size(), 0, (sockaddr*)&addr, sizeof(addr));
}

void sendBuildings(const std::vector<Coordinate>& buildings, int id, SOCKET sock, sockaddr_in& addr) {
    // שליחת פקודת reset (רשות – אם רוצים למחוק את הקודמים)
    std::string resetMsg = "{ \"id\": " + std::to_string(id) + ", \"type\": -2 }";
    sendto(sock, resetMsg.c_str(), resetMsg.size(), 0, (sockaddr*)&addr, sizeof(addr));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // שליחת כל בניין עם type=3
    for (const auto& coord : buildings) {
        std::ostringstream oss;
        double timestamp = std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();

        oss << "{"
            << "\"id\":" << id << ","
            << "\"x\":" << coord.x << ","
            << "\"y\":" << coord.y << ","
            << "\"z\":" << coord.z << ","
            << "\"type\":3,"  // סוג 3 = בניינים
            << "\"timestamp\":" << std::fixed << std::setprecision(6) << timestamp
            << "}";

        std::string msg = oss.str();
        sendto(sock, msg.c_str(), msg.size(), 0, (sockaddr*)&addr, sizeof(addr));
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

void clearBuildings(int id, SOCKET sock, sockaddr_in& addr) {
    std::string resetBuildingsMsg = "{ \"id\": " + std::to_string(id) + ", \"type\": -3 }";
    sendto(sock, resetBuildingsMsg.c_str(), resetBuildingsMsg.size(), 0, (sockaddr*)&addr, sizeof(addr));
}





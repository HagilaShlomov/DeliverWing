#include <iostream>
#include <mysql/jdbc.h> // Include the MySQL JDBC header
#include "InitGraph.h"
#include "User.h"
using namespace std;
using namespace sql::mysql;

void registerUserInDB(User user) {
    Client cli("http://localhost:8081");

    json toSend = {
        {"username", user.getUsername()},
        {"password", user.getPassword()},

    };

    if (auto res = cli.Post("/register", toSend.dump(), "application/json")) { // Changed endpoint to /register
        if (res->status == 200) {
            try {
                json response = json::parse(res->body);
                std::cout << "User registration response: " << response.dump(4) << std::endl;
                if (response.contains("status") && response["status"] == "success") {
                    std::cout << "User '" << user.getUsername() << "' registered successfully." << std::endl;
                }
                else {
                    std::cerr << "User registration failed: " << response.value("message", "Unknown error") << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Failed to parse server response: " << e.what() << std::endl;
            }
        }
        else {
            std::cerr << "Server returned status: " << res->status << " Body: " << (!res->body.empty() ? res->body : "No body") << std::endl;
        }
    }
    else {
        std::cerr << "Failed to connect to DAL server for user registration." << std::endl;
        auto err = res.error();
        std::cerr << "Connection error: " << httplib::to_string(err) << std::endl;
    }
}

// Function to send an order to the DB via the DAL server
void sendOrderToDB(const std::string& city, const std::string& street,
    const std::string& houseNumber, int itemCount, // Changed itemCount and itemWeight to int/double
    double itemWeight) {
    Client cli("http://localhost:8081");

    json j;
    j["city"] = city;
    j["street"] = street;
    j["houseNumber"] = houseNumber;
    j["itemCount"] = itemCount;
    j["itemWeight"] = itemWeight;

    auto res = cli.Post("/orders", j.dump(), "application/json");
    if (res && res->status == 200) {
        std::cout << "Order sent successfully to DAL server\n";
    }
    else {
        std::cerr << "Failed to send order to DAL server. Status: " << (res ? std::to_string(res->status) : "No response") << std::endl;
        if (res && !res->body.empty()) {
            std::cerr << "Server response body: " << res->body << std::endl;
        }
        auto err = res.error();
        std::cerr << "Connection error: " << httplib::to_string(err) << std::endl;
    }
}


//void connection() {
//    httplib::Client cli("http://localhost:4000");
//
//    nlohmann::json login_data = {
//    {"username", "hagila"},
//    {"password", "123456"}
//    };
//
//    auto res = cli.Post("/user/login", login_data.dump(), "application/json");
//    if (res && res->status == 200) {
//        std::cout << "המשתמש קיים במערכת\n";
//    }
//    else {
//        std::cout << "המשתמש לא נמצא\n";
//    }
//
//}



//
//class DatabaseManager {
//public:
//	MySQL_Driver* driver;
//	unique_ptr<sql::Connection> con;
//
//	int connect_to_DB();
//
//	void createTable();
//	void insertWeatherData(Graph g);
//	void insertUserData(User user);
//	void updateData(int id, const std::string& data);
//
//};
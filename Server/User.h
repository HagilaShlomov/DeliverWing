#pragma once
#include <string>

class User {
private:
    std::string username;
    std::string password;
    

public:
    // Default constructor
    User() : username(""), password("") {}

    User(const std::string& user, const std::string& pass) : username(user), password(pass){}

    // Constructor with all details
    User(const std::string& user, const std::string& pass, const std::string& c, const std::string& s, const std::string& hn)
        : username(user), password(pass) {
    }


    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }

    void setUsername(const std::string& u) { username = u; }
    void setPassword(const std::string& p) { password = p; }

};
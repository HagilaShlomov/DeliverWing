cmake_minimum_required(VERSION 3.10)

project(WeatherApp)

set(CMAKE_CXX_STANDARD 11)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

add_executable(WeatherApp Server/Weather.cpp)

target_link_libraries(WeatherApp ${CURL_LIBRARIES})
#include "Weather.h"
#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include <json/json.h>
//#include <nlohmann/json.hpp>

// Ensure that the CURL library is linked correctly
#ifdef _WIN32
#pragma comment(lib, "libcurl.lib")
#endif

WeatherAPI::WeatherAPI(const std::string& apiKey) : apiKey(apiKey) {}

bool WeatherAPI::fetchWeatherData(const std::string& location) {
    std::string url = "http://api.weatherapi.com/v1/current.json?key=" + apiKey + "&q=" + location;

    CURL* curl;
    CURLcode res;
    std::ostringstream responseStream;
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void* contents, size_t size, size_t nmemb, void* userp) -> size_t {
            ((std::ostringstream*)userp)->write((char*)contents, size * nmemb);
            return size * nmemb;
            });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStream);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return false;
        }
    }

    return parseWeatherData(responseStream.str());
}

bool WeatherAPI::parseWeatherData(const std::string& jsonData) {
    Json::Value root;
    Json::CharReaderBuilder readerBuilder;
    std::string errs;

    std::istringstream s(jsonData);
    if (!Json::parseFromStream(readerBuilder, s, &root, &errs)) {
        std::cerr << "Failed to parse JSON: " << errs << std::endl;
        return false;
    }

    temperature = root["current"]["temp_c"].asString();
    humidity = root["current"]["humidity"].asString();
    weatherDescription = root["current"]["condition"]["text"].asString();
    windSpeed = root["current"]["wind_kph"].asString();
    windDirection = root["current"]["wind_dir"].asString();
    pressure = root["current"]["pressure_mb"].asString();

    return true;
}

std::string WeatherAPI::getTemperature() const {
    return temperature;
}

std::string WeatherAPI::getHumidity() const {
    return humidity;
}

std::string WeatherAPI::getWeatherDescription() const {
    return weatherDescription;
}

std::string WeatherAPI::getWindSpeed() const {
    return windSpeed;
}

std::string WeatherAPI::getWindDirection() const {
    return windDirection;
}

std::string WeatherAPI::getPressure() const {
    return pressure;
}

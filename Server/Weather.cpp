#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include "json.hpp"
#include "Weather.h"

using namespace std;
using json = nlohmann::json;


WeatherAPI::WeatherAPI(const std::string& apiKey) : apiKey(apiKey) {}

bool WeatherAPI::fetchWeatherData(const std::string& location, double altitude) {

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return false;
    }

    std::ostringstream responseStream;


    //// בניית URL עם קואורדינטות
    //std::string url = "https://api.weatherapi.com/v1/current.json?key=" + apiKey +
    //    "&q=" + std::to_string(latitude) + "," + std::to_string(longitude);
    //if (altitude > 0) {
    //    url += "&altitude=" + std::to_string(altitude);
    //}

    std::string url = "https://api.weatherapi.com/v1/current.json?key=" + apiKey + "&q=" + location;
    if (altitude > 0) {
        url += "&altitude=" + std::to_string(altitude);
    }

    // הגדרת כתובת ה-URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // פונקציית כתיבה
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
        +[](void* contents, size_t size, size_t nmemb, void* userp) -> size_t {
            if (!contents || !userp) return 0;
            std::ostringstream* stream = static_cast<std::ostringstream*>(userp);
            stream->write(static_cast<char*>(contents), size * nmemb);
            return size * nmemb;
        });

    // הגדרת יעד הכתיבה
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStream);

    std::cout << "Requesting URL: " << url << std::endl;
	// הגדרת אפשרויות SSL
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);


    // הפעלת הבקשה
    CURLcode res = curl_easy_perform(curl);

    // ניקוי משאבים
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }
    
	 //הדפסת התגובה הגולמית (לצורכי דיבוג)
    //std::cout << "Raw response: " << responseStream.str() << std::endl;

    //אם אתה רוצה לשמור את התגובה לקובץ JSON, תוכל להפעיל את הקוד הבא
    std::ofstream file("weather_data.json");
    if (file.is_open()) {
       try {
           // Parse the response stream into a JSON object
           nlohmann::json jsonResponse = nlohmann::json::parse(responseStream.str());
           // Write the formatted JSON to the file
           file << jsonResponse.dump(4); // dump(4) = formatted with 4 spaces indentation
           file.close();
           std::cout << "Data successfully written to the file." << std::endl;
       } catch (const nlohmann::json::parse_error& e) {
           std::cerr << "JSON parse error: " << e.what() << std::endl;
       }
    } else {
       std::cerr << "Failed to open the file for writing." << std::endl;
    }
    file.close();
    std::cout << "הנתונים נכתבו בהצלחה לקובץ." << std::endl;

    return parseWeatherData(responseStream.str());
}




bool WeatherAPI::parseWeatherData(const std::string& jsonData) {
    json root;

    try {
        root = json::parse(jsonData);

        temperature = std::to_string(root["current"]["temp_c"].get<double>());
        humidity = std::to_string(root["current"]["humidity"].get<int>());
        weatherDescription = root["current"]["condition"]["text"].get<std::string>();
        windSpeed = std::to_string(root["current"]["wind_kph"].get<double>());
        windDirection = root["current"]["wind_dir"].get<std::string>();
        pressure = std::to_string(root["current"]["pressure_mb"].get<double>());
        windGust = std::to_string(root["current"]["gust_kph"].get<double>());


        return true;
    }
    catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }
    catch (const json::type_error& e) {
        std::cerr << "JSON type error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }

    return false;
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

std::string WeatherAPI::getWindGust() const {
    return windGust;
}

double WeatherAPI::getAltitude() const {
	return altitude;
}

void WeatherAPI::toString() const
{
	std::cout << "Temperature: " << temperature << " °C" << std::endl;
	std::cout << "Humidity: " << humidity << " %" << std::endl;
	std::cout << "Weather Description: " << weatherDescription << std::endl;
	std::cout << "Wind Speed: " << windSpeed << " kph" << std::endl;
	std::cout << "Wind Direction: " << windDirection << std::endl;
	std::cout << "Pressure: " << pressure << " mb" << std::endl;
	std::cout << "Wind Gust: " << windGust << " kph" << std::endl;
}


json getWeatherInfo(double lat, double lon) {
    string apikey = "d3f4c60705924bd2b7f83839251305";
    string Latitude = to_string(lat);
    string Longitude = to_string(lon);

    string location = Latitude + "," + Longitude;

    WeatherAPI weatherAPI(apikey);
    double altitude = weatherAPI.getAltitude();

    // Fixing the missing closing parenthesis in the if condition  
    if (weatherAPI.fetchWeatherData(location, altitude)) {
        /*std::cout << "Weather data fetched successfully." << std::endl;
        std::cout << "-------------Weather Details------------" << std::endl;
        std::cout << "Temperature: " << weatherAPI.getTemperature() << "°C" << std::endl;
        std::cout << "Humidity: " << weatherAPI.getHumidity() << "%" << std::endl;
        std::cout << "Description: " << weatherAPI.getWeatherDescription() << std::endl;
        std::cout << "Wind Speed: " << weatherAPI.getWindSpeed() << " km/h" << std::endl;
        std::cout << "Wind Direction: " << weatherAPI.getWindDirection() << std::endl;
        std::cout << "Pressure: " << weatherAPI.getPressure() << " hPa" << std::endl;
        std::cout << "Wind Gust: " << weatherAPI.getWindGust() << " km/h" << std::endl;*/
        /*weatherAPI.toString(); */
        json weatherData = {
            {"latitude", lat},
            {"longitude", lon},
            {"altitude", altitude},
            {"temperature", weatherAPI.getTemperature()},
            {"humidity", weatherAPI.getHumidity()},
            {"weatherDescription", weatherAPI.getWeatherDescription()},
            {"windSpeed", weatherAPI.getWindSpeed()},
            {"windDirection", weatherAPI.getWindDirection()},
            {"pressure", weatherAPI.getPressure()},
            {"windGust", weatherAPI.getWindGust()}
        };
        // Save to file
        std::ofstream file("weather_data.json");
        if (file.is_open()) {
            file << weatherData.dump(4); // Write JSON with 4-space indentation
            file.close();
        }
        else {
            std::cerr << "Error opening file for writing." << std::endl;
        }
        return weatherData; // Return the JSON object

    }
    else {
        std::cerr << "Failed to fetch weather data." << std::endl;
    }
    return json::object(); // Return an empty JSON object if fetching fails
}



static std::map<std::string, double> windDirectionsMap;
static bool loaded = false;

bool loadWindDirections(const std::string& filename) {
    if (loaded) return true;

    std::ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error opening file " << filename << endl;
        return false;
    }

    json j;
    inFile >> j;

    for (const auto& item : j) {
        std::string abbreviation = item["Abbreviation"];
        double azimuth = item["Azimuth Degrees"];
        windDirectionsMap[abbreviation] = azimuth;
    }

    loaded = true;
    return true;
}


double getWindAngle(const std::string& windDir) {
    if (!loaded) {
        if (!loadWindDirections()) {
            cerr << "Failed to load wind directions data." << endl;
            return -1;
        }
    }

    auto it = windDirectionsMap.find(windDir);
    if (it == windDirectionsMap.end()) {
        cerr << "Unknown wind direction abbreviation: " << windDir << endl;
        return -1;
    }
    return it->second;
}


void getFromFile() {

    std::ifstream file("weather_data.json");
    if (file) {
        json data;
        file >> data;

        // חילוץ מידע
        double latitude = data["location"]["lat"];
        double longitude = data["location"]["lon"];
        std::string temperature = std::to_string(data["current"]["temp_c"].get<double>());
        std::string humidity = std::to_string(data["current"]["humidity"].get<double>());
        std::string weatherDescription = data["current"]["condition"]["text"];
        std::string windSpeed = std::to_string(data["current"]["wind_kph"].get<double>());
        std::string windDirection = data["current"]["wind_dir"];
        std::string pressure = std::to_string(data["current"]["pressure_mb"].get<double>());
        std::string windGust = std::to_string(data["current"]["gust_kph"].get<double>());

        //הדפסה
        std::cout << "-------------Weather Details------------" << std::endl;
        std::cout << "Latitude: " << latitude << std::endl;
        std::cout << "Longitude: " << longitude << std::endl;
        std::cout << "Temperature: " << temperature << "°C" << std::endl;
        std::cout << "Humidity: " << humidity << "%" << std::endl;
        std::cout << "Wind Speed: " << windSpeed << " km/h" << std::endl;
        std::cout << "Wind Direction: " << windDirection << std::endl;
        std::cout << "Pressure: " << pressure << " hPa" << std::endl;
        std::cout << "Wind Gust: " << windGust << " km/h" << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        file.close();
    }
    else {
        std::cerr << "לא ניתן לפתוח את הקובץ." << std::endl;
    }
}
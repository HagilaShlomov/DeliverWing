#ifndef Weather_h
#define Weather_h

//#pragma comment(lib, "BulletCollision.lib")
//#pragma comment(lib, "BulletDynamics.lib")
//#pragma comment(lib, "LinearMath.lib")
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp> 

class WeatherAPI {
public:
    WeatherAPI(const std::string& apiKey);
    bool fetchWeatherData(const std::string& location, double altitude);
    std::string getTemperature() const;
    std::string getHumidity() const;
    std::string getWeatherDescription() const;
    std::string getWindSpeed() const;
    std::string getWindDirection() const;
    std::string getPressure() const;
    std::string getWindGust() const; // פונקציה חדשה לקבלת משבי רוח
	double getAltitude() const; // פונקציה חדשה לקבלת גובה
	void toString() const;

private:
    std::string apiKey;
    std::string temperature;
    std::string humidity;
    std::string weatherDescription;
    std::string windSpeed;
    std::string windDirection;
    std::string pressure;
    std::string windGust; // משתנה חדש עבור משבי רוח
	double altitude;

    bool parseWeatherData(const std::string& jsonData);
public:

    WeatherAPI() = default;
};


using json = nlohmann::json;
json getWeatherInfo(double latitude, double longitude);
double getWindAngle(const std::string& windDir);
bool loadWindDirections(const std::string& filename = "wind_directions.json");
void getFromFile();
#endif 
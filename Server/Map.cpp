//#include "Map.h"  
//#include <iostream>  
//#include <string>  
//#include <curl/curl.h>  
//#include <nlohmann/json.hpp>  
//#include <Eigen/Dense>  
//#include <cmath>
//
//
//// Constructor definition corrected to match the class name and syntax  
//Map::Map(const std::string& apiKey) : apiKey(apiKey) {}  
//
//// Callback function for handling CURL response  
//size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {  
//  size_t totalSize = size * nmemb;  
//  output->append((char*)contents, totalSize);  
//  return totalSize;  
//}  
//
//// Function to fetch geographic information using the Map API  
//void Map::getGeoInfo(double latitude, double longitude) {  
//   std::string url = "https://api.opentopography.org/pointclouds?lat=" + std::to_string(latitude) +  
//	"&lon=" + std::to_string(longitude) + "&key=" + apiKey;  
//   CURL* curl;  
//   CURLcode res;  
//   std::string readBuffer;  
//
//   curl_global_init(CURL_GLOBAL_DEFAULT);  
//   curl = curl_easy_init();  
//
//   if (curl) {  
//       curl_easy_setopt(curl, CURLOPT_URL, url.c_str());  
//       curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);  
//       curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);  
//
//       res = curl_easy_perform(curl);  
//
//       if (res != CURLE_OK) {  
//           std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;  
//       }  
//       else {  
//           // עיבוד התשובה (הדגמה פשוטה להוצאת elevation)  
//           // תצטרך לעבד את התשובה בהתאם לפורמט שמתקבל מה-API  
//           data.latitude = latitude;  
//           data.longitude = longitude;  
//           data.altitude = 89.5;  // לדוגמה, תוכל לפרסר את התשובה כדי למלא ערכים אלה  
//           std::cout << "Elevation: " << data.altitude << " meters" << std::endl;  
//       }  
//
//       curl_easy_cleanup(curl);  
//   }  
//
//   curl_global_cleanup();  
//}  
//
//Map::Data Map::getData() const {  
//return data;  
//}

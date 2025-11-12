#pragma once  
#include <iostream>  
#include <cmath>  
#include <unordered_map>  
#include <unordered_set>  
#include <vector>  
#include <string>  
#include <functional>  
#include <algorithm>  

class Coordinate {  
public:  
   double x;  
   double y;  
   double z;  

   Coordinate(double x, double y, double z) : x(x), y(y), z(z) {}  
   Coordinate() : x(0), y(0), z(0) {}  

   Coordinate operator-(const Coordinate& other) const;  
   bool operator==(const Coordinate& other) const;  

   bool operator!=(const Coordinate& other) const;  

   double dot(const Coordinate& other) const;  

   bool operator<(const Coordinate& other) const;  

   Coordinate operator*(double scalar) const;  

   Coordinate operator+(const Coordinate& other) const;  

   double magnitude() const;  

   Coordinate normalize() const;  

   Coordinate normalize(const Coordinate& vec) const;  

   std::string toString() const {
       return std::to_string(x) + "," +
           std::to_string(y) + "," +
           std::to_string(z);
   }

   double roundCoord(double coord) {  
       return std::round(coord * 1e6) / 1e6;  
   }  
};  

struct CoordinateHasher {  
   std::size_t operator()(const Coordinate& p) const {  
       return std::hash<double>()(p.x) ^ (std::hash<double>()(p.y) << 1) ^ (std::hash<double>()(p.z) << 2);  
   }  
};  

namespace std {  
   template <>  
   struct hash<Coordinate> {  
       size_t operator()(const Coordinate& c) const {  
           return ((std::hash<double>()(c.x)  
               ^ (std::hash<double>()(c.y) << 1)) >> 1)  
               ^ (std::hash<double>()(c.z) << 1);  
       }  
   };  
}  

bool isValidCoordinate(double value);
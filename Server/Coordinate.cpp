#include "Coordinate.h"
#include "DataStructures.h"
#include <unordered_set>
#include <queue> 
#include <vector>
#include <functional>
#include <string>
#include "Order.h"
#include <iomanip>
#include <sstream>
#include <regex>

Coordinate Coordinate::operator-(const Coordinate& other) const {
	return Coordinate(x - other.x, y - other.y, z - other.z);
}

bool Coordinate::operator==(const Coordinate& other) const {
	return (x == other.x && y == other.y);
}

bool Coordinate::operator!=(const Coordinate& other) const
{
	return !(*this == other);
}

double Coordinate::dot(const Coordinate& other) const { //ביצוע מכפלה סקלרית
	return x * other.x + y * other.y + z * other.z;
}

bool Coordinate::operator<(const Coordinate& other) const {
	if (x != other.x)
		return x < other.x;
	if (y != other.y)
		return y < other.y;
	return z < other.z;
}

Coordinate Coordinate::operator*(double scalar) const {
	return Coordinate(x * scalar, y * scalar, z * scalar);
}

Coordinate Coordinate::operator+(const Coordinate& other) const {
	return Coordinate(x + other.x, y + other.y, z + other.z);
}

double Coordinate::magnitude() const {
	return sqrt(x * x + y * y + z * z);
}

Coordinate Coordinate::normalize() const {
	double mag = magnitude();
	return Coordinate(x / mag, y / mag, z / mag);
}

Coordinate Coordinate::normalize(const Coordinate& vec) const {
	double mag = vec.magnitude();
	return Coordinate(vec.x / mag, vec.y / mag, vec.z / mag);
}

bool isValidCoordinate(double value) {
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(6) << value;
	std::string strValue = oss.str();

	std::regex pattern(R"(^-?\d{2}\.\d{6}$)");
	return std::regex_match(strValue, pattern);
}

#pragma once
#include <unordered_set>
#include <vector>
#include <functional>
#include <string>

using namespace std ;

class Order {
private:
	std::string city;
	std::string street;
	std::string houseNumber;
	std::string itemCount;
	std::string itemWeight;

public:
	Order(std::string city, std::string street, std::string houseNumber, std::string itemCount, std::string itemWeight)
		: city(city), street(street), houseNumber(houseNumber), itemCount(itemCount), itemWeight(itemWeight) {
	}

	Order()
		: city(""), street(""), houseNumber(""), itemCount(""), itemWeight("") {
	}

	bool operator==(const Order& other)const;

	std::string getCity() const;
	std::string getStreet() const;
	std::string getHouseNumber() const;
	std::string getItemCount() const;
	std::string getItemWeight() const;
	std::string toString() const;

};

namespace std {
	template <>
	struct hash<Order> {
		size_t operator()(const Order& a) const {
			return hash<std::string>()(a.getCity()) ^
				(hash<std::string>()(a.getStreet()) << 1) ^
				(hash<std::string>()(a.getHouseNumber()) << 2) ^
				(hash<std::string>()(a.getItemCount()) << 3) ^
				(hash<std::string>()(a.getItemWeight()) << 4);
		}
	};
}


#include "Order.h"
#include <iostream>
#include <string>
using namespace std;

bool Order::operator==(const Order& other) const{
	return city == other.city &&
		street == other.street &&
		houseNumber == other.houseNumber &&
		itemCount == other.itemCount &&
		itemWeight == other.itemWeight;
}




std::string Order::getCity() const {
	return city;
}
std::string Order::getStreet() const {
	return street;
}
std::string Order::getHouseNumber() const {
	return houseNumber;
}
std::string Order::getItemCount() const {
	return itemCount;
}
std::string Order::getItemWeight() const {
	return itemWeight;
}
std::string Order::toString() const {
	return "City: " + city + ", Street: " + street + ", House Number: " + houseNumber +
		", Item Count: " + itemCount + ", Item Weight: " + itemWeight;
}
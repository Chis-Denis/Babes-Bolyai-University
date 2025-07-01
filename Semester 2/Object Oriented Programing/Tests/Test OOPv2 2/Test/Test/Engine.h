#pragma once
#include <string>
class Engine
{
protected:
	double basePrice;
public:
	std::string engineType;
	Engine(std::string engineType, double price = 3000) : engineType(engineType), basePrice(price) {}
	virtual double getPrice() = 0;
	virtual std::string toString() = 0;
};
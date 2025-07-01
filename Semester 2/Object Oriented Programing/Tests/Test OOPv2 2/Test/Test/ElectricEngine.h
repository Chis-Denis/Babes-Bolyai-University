#pragma once
#include "Engine.h"
class ElectricEngine : public Engine
{
private:
	int autonomy;

public:
	ElectricEngine(std::string engineType, double base_price, int autonomy) : Engine(engineType, base_price), autonomy(autonomy) {}
	double getPrice() override;
	std::string toString() override;


};




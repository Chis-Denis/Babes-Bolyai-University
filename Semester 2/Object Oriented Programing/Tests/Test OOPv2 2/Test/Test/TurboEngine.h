#pragma once
#include "Engine.h"
class TurboEngine : public Engine
{
public:
	TurboEngine(std::string engineType, double basePrice = 3000) :Engine(engineType, basePrice) {};
	double getPrice() override;
	std::string toString() override;

};

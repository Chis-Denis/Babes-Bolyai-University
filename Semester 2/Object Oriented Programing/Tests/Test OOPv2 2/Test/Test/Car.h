#pragma once
#include "TurboEngine.h"
#include "ElectricEngine.h"

class Car
{
private:
	Engine* engine;
	int numberOfDoors;
public:
	Car(Engine* engine, int numberOfDoors) : engine(engine), numberOfDoors(numberOfDoors) {}
	double computePrice();
	std::string toString();
	int getNumberOfDoors() const { return numberOfDoors; }
};


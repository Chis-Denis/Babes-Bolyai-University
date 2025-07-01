#include "Service.h"
#include <sstream>
#include <fstream>
#include <iostream>


Engine* Service::createEngine(std::string engineType, int autonomy = 0)
{
	if (engineType == "petrol")
	{
		return new TurboEngine(engineType);
	}
	else if (engineType == "electric")
	{
		return new ElectricEngine(engineType, 3000, autonomy);
	}
}

//add a car to the list using the service and the given parts :numberOfDoors, engineType, autonomy
void Service::addCar(int numberOfDoors, std::string engineType, int autonomy)
{
	Engine* engine = createEngine(engineType, autonomy);
	Car* car = new Car(engine, numberOfDoors);
	cars.push_back(car);

	std::stringstream ss;
	ss << "Price: " << car->computePrice() << '\n';
	std::cout << ss.str();
}


std::vector<Car*> Service::getCars()
{
	return this->cars;
}

std::vector<Car*> Service::getCarsWithMaxPrice(double maxPrice)
{
	std::vector<Car*> carsWithMaxPrice;
	for (auto car : cars)
	{
		if (car->computePrice() <= maxPrice)
			carsWithMaxPrice.push_back(car);
	}
	return carsWithMaxPrice;
}

void Service::writeToFile(std::string filename, std::vector<Car*> carsToDisplay)
{
	std::ofstream file(filename);
	for (auto car : carsToDisplay)
	{
		if (car->getNumberOfDoors() == 2)
			file << car->toString() << '\n';
	}
	for (auto car : carsToDisplay)
	{
		if (car->getNumberOfDoors() == 4)
			file << car->toString() << '\n';
	}
	file.close();
}



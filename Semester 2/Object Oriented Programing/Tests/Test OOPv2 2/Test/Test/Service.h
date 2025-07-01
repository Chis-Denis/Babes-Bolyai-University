#pragma once
#include "Car.h"
#include <vector>
#include <string>
class Service
{
private:
	std::vector<Car*> cars;
public:
	Engine* createEngine(std::string engineType, int autonomy);
	Service(std::vector<Car*> initialcars = {}) : cars{ initialcars } 
	{
		Engine* e1 = new TurboEngine("petrol", 3000);
		Engine* e2 = new TurboEngine("petrol", 3000);
		Engine* e3 = new ElectricEngine("electric", 3000, 141241);
		Engine* e4 = new ElectricEngine("electric", 3000, 51251);

		Car* c1 = new Car(e1, 4);
		Car* c2 = new Car(e2, 2);
		Car* c3 = new Car(e3, 4);
		Car* c4 = new Car(e4, 2);

		this->cars.push_back(c1);
		this->cars.push_back(c3);
		this->cars.push_back(c2);
		this->cars.push_back(c4);
	}
	void addCar(int numberOfDoors, std::string engineType, int autonomy);
	std::vector<Car*> getCars();
	std::vector<Car*> getCarsWithMaxPrice(double maxPrice);
	void writeToFile(std::string filename, std::vector<Car*> carsToDisplay);
};


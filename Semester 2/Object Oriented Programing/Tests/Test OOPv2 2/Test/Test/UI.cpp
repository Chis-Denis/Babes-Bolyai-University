#include "UI.h"

void UI::addCar()
{
	std::string engineType;
	int autonomy, numberOfDoors;
	std::cout << "numberOfDoors: ";
	std::cin >> numberOfDoors;
	std::cout << "Type: ";
	std::cin.ignore();
	std::getline(std::cin, engineType);
	if (engineType == "petrol") {
		this->service.addCar(numberOfDoors, engineType, 0);
	}
	else if (engineType == "electric") {
		std::cout << "Autonomy: ";
		std::cin >> autonomy;
		this->service.addCar(numberOfDoors, engineType, autonomy);
	}
	// Print the description of the car after adding it
	std::cout << "Car added: " << this->service.getCars().back()->toString() << std::endl;
}

void UI::showAll()
{
	std::vector<Car*> cars = this->service.getCars();
	for (auto car : cars) {
		std::cout << car->toString() << "\n";

	}


}

void UI::saveToFile()
{
	std::string filename;
	std::cout << "Filename: ";
	std::cin.ignore();
	std::cin >> filename;

	std::cout << "Enter the max price: ";
	int maxPrice;
	std::cin >> maxPrice;

	std::vector<Car*> cars = this->service.getCarsWithMaxPrice(maxPrice);
	this->service.writeToFile(filename, cars);

}

void UI::run()
{
	bool ok = true;
	while (ok) {
		std::cout << "1. Add car\n";
		std::cout << "2. Show all\n";
		std::cout << "3. Save to file\n";
		std::cout << "0. Exit\n";
		int cmd;
		std::cout << "Command: ";
		std::cin >> cmd;
		switch (cmd) {
		case 1:
			this->addCar();
			break;
		case 2:
			this->showAll();
			break;
		case 3:
			this->saveToFile();
			break;
		case 0:
			ok = false;
			break;
		default:
			std::cout << "Invalid command\n";
			break;
		}
	}
}

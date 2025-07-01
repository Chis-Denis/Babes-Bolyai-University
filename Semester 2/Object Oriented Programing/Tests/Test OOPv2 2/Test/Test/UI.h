#pragma once
#include <iostream>
#include "Service.h"
class UI
{
private:
	Service service;
public:
	UI(Service service) : service(service) {}
	void addCar();
	void showAll();
	void saveToFile();
	void run();

};


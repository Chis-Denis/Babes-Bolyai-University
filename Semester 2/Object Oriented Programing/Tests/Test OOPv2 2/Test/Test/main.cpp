#include "UI.h"

int main() {

	Service service;
	UI ui(service);
	ui.run();

}
//the clients can choose the number of doors for the car, as well as the type of the engine:
//the available options for number of doors are 2 and 4. the base price for 2 doors is 7000, while for 4 doors is 8500.
//the available options for the engine are electric and petrol. the base price any engine is 3000.
//for an electric engine the price is comouted as base price + autonomy of the engine * 0.01
//for a petrol engine the price is computed as base price + 100.
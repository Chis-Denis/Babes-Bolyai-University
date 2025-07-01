#include "Car.h"
#include <sstream>

double Car::computePrice()
{
    if (this->numberOfDoors == 2)
        return 7000 + this->engine->getPrice();
    else if (this->numberOfDoors == 4)
        return 8500 + this->engine->getPrice();

}

std::string Car::toString()
{
    std::string base = this->engine->toString();
    std::stringstream ss;
    ss << base << " Number Of Doors: " << this->numberOfDoors << ", Total Price: " << this->computePrice();
    return ss.str();
}

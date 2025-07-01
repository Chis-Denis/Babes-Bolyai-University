#include "ElectricEngine.h"
#include <sstream>
double ElectricEngine::getPrice()
{
    return this->basePrice + 0.01 * this->autonomy;
}

std::string ElectricEngine::toString()
{
    std::stringstream ss;
    ss << "Type: Electric Engine" << ", " << "Autonomy: " << this->autonomy << ", " << "Base Price: " << this->basePrice << ", " << "Engine price: " << this->getPrice();
    return ss.str();
}

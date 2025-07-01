#include "TurboEngine.h"
#include <sstream>
double TurboEngine::getPrice()
{
    return this->basePrice + 100;
}

std::string TurboEngine::toString()
{
    std::stringstream ss;
    ss << "Type: Turbo Engine, Engine Type " << this->engineType << ", Base Price: " << this->basePrice << ", Engine price: " << this->getPrice();
    return ss.str();
}

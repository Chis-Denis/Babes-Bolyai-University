#pragma once
#include <string>
#include "../Exception/ValidationException.h"

// Forward declaration
class TrenchCoat;

class TrenchCoatValidator {
public:
    static void validate(const TrenchCoat& coat);

private:
    static void validateSize(const std::string& size);
    static void validateColour(const std::string& colour);
    static void validatePrice(double price);
    static void validateQuantity(int quantity);
    static void validatePhotoLink(const std::string& photoLink);
}; 
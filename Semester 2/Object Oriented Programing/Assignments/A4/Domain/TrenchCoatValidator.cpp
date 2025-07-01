#include "TrenchCoatValidator.h"
#include "trench.h"
#include "../Exception/ValidationException.h"

void TrenchCoatValidator::validate(const TrenchCoat& coat) {
    validateSize(coat.getSize());
    validateColour(coat.getColour());
    validatePrice(coat.getPrice());
    validateQuantity(coat.getQuantity());
    validatePhotoLink(coat.getPhotoLink());
}

void TrenchCoatValidator::validateSize(const std::string& size) {
    if (size.empty()) {
        throw ValidationException("Size cannot be empty!");
    }
    // Add more size validation if needed
}

void TrenchCoatValidator::validateColour(const std::string& colour) {
    if (colour.empty()) {
        throw ValidationException("Colour cannot be empty!");
    }
    // Add more colour validation if needed
}

void TrenchCoatValidator::validatePrice(double price) {
    if (price <= 0) {
        throw ValidationException("Price must be positive!");
    }
}

void TrenchCoatValidator::validateQuantity(int quantity) {
    if (quantity < 0) {
        throw ValidationException("Quantity cannot be negative!");
    }
}

void TrenchCoatValidator::validatePhotoLink(const std::string& photoLink) {
    if (photoLink.empty()) {
        throw ValidationException("Photo link cannot be empty!");
    }
    // Add more photo link validation if needed
} 
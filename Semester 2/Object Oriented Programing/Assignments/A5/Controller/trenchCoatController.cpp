#include "trenchCoatController.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>

TrenchCoatController::TrenchCoatController(TrenchCoatRepository& repository) : repository(repository) {}

void TrenchCoatController::addInitialData() {
    // Add initial data with example web links
    try {
        this->addTrenchCoat("M", "Beige", 199.99, 10, "http://example.com/images/beige_m.jpg");
        this->addTrenchCoat("L", "Black", 249.99, 5, "https://example.com/images/black_l.png");
        this->addTrenchCoat("S", "Navy", 179.50, 8, "http://example.com/images/navy_s.jpg");
        this->addTrenchCoat("M", "Grey", 210.00, 12, "http://images.example.co/grey_m"); // Different domain example
        this->addTrenchCoat("XL", "Beige", 220.00, 3, "https://example.com/images/beige_xl.jpg");
        this->addTrenchCoat("L", "Olive", 235.50, 7, "http://example.com/images/olive_l.jpeg");
        this->addTrenchCoat("S", "Black", 189.99, 9, "https://example.com/images/black_s.gif");
        this->addTrenchCoat("M", "Navy", 199.99, 6, "http://example.com/images/navy_m.jpg");
        this->addTrenchCoat("XL", "Grey", 259.99, 4, "http://example.com/images/grey_xl");
        this->addTrenchCoat("L", "Beige", 215.00, 11, "https://example.com/images/beige_l.jpg");
        this->addTrenchCoat("XS", "Red", 150.00, 2, "http://example.com/images/red_xs.png");
    }
    catch (const std::exception& e) {
        std::cerr << "Error adding initial data in controller: " << e.what() << std::endl;
    }
}

void TrenchCoatController::validateTrenchCoatData(const std::string& size, const std::string& colour,
                                                double price, int quantity, const std::string& photoLink) {
    if (size.empty()) {
        throw std::invalid_argument("Size cannot be empty!");
    }
    if (colour.empty()) {
        throw std::invalid_argument("Colour cannot be empty!");
    }
    if (price <= 0) {
        throw std::invalid_argument("Price must be positive!");
    }
    if (quantity < 0) {
        throw std::invalid_argument("Quantity cannot be negative!");
    }
    if (photoLink.empty()) {
        throw std::invalid_argument("Photo link cannot be empty!");
    }
    // Basic check for web link format
    if (photoLink.rfind("http://", 0) != 0 && photoLink.rfind("https://", 0) != 0) {
        // rfind(prefix, 0) == 0 checks if the string starts with the prefix
        throw std::invalid_argument("Photo link must start with http:// or https://");
    }
}

void TrenchCoatController::addTrenchCoat(const std::string& size, const std::string& colour,
                                       double price, int quantity, const std::string& photoLink) {
    validateTrenchCoatData(size, colour, price, quantity, photoLink);
    
    // Check for duplicates (size and colour define uniqueness)
    if (repository.findTrenchCoat(size, colour) != -1) {
        throw std::invalid_argument("Trench coat with the same size and colour already exists.");
    }
    
    TrenchCoat newCoat(size, colour, price, quantity, photoLink);
    repository.addTrenchCoat(newCoat);
}

void TrenchCoatController::removeTrenchCoat(int index) {
    repository.removeTrenchCoat(index);
}

void TrenchCoatController::updateTrenchCoat(int index, const std::string& size, const std::string& colour,
                                          double price, int quantity, const std::string& photoLink) {
    validateTrenchCoatData(size, colour, price, quantity, photoLink);
    TrenchCoat updatedCoat(size, colour, price, quantity, photoLink);
    repository.updateTrenchCoat(index, updatedCoat);
}

DynamicVector<TrenchCoat> TrenchCoatController::getAllTrenchCoats() const {
    // Return a copy of the repository's vector to prevent external modification
    // Or return a const reference if external modification is not a concern and performance is critical
    return repository.getAllTrenchCoats();
}

DynamicVector<TrenchCoat> TrenchCoatController::getTrenchCoatsBySize(const std::string& size) const {
    // DynamicVector<TrenchCoat> result; // Old type
    DynamicVector<TrenchCoat> result; // New type
    const DynamicVector<TrenchCoat>& coats = repository.getAllTrenchCoats();
    
    for (int i = 0; i < coats.getSize(); i++) {
        if (size.empty() || coats[i].getSize() == size) {
            result.add(coats[i]); // Use DynamicVector::add
        }
    }
    return result;
}

DynamicVector<TrenchCoat> TrenchCoatController::getTrenchCoatsCheaperThan(double maxPrice) const {
    // std::vector<TrenchCoat> result; // Old type
    DynamicVector<TrenchCoat> result; // New type
    const DynamicVector<TrenchCoat>& coats = repository.getAllTrenchCoats();
    
    for (int i = 0; i < coats.getSize(); i++) {
        if (coats[i].getPrice() < maxPrice) {
            result.add(coats[i]); // Use DynamicVector::add
        }
    }
    return result;
}

TrenchCoat TrenchCoatController::getTrenchCoat(int index) const {
    return repository.getTrenchCoat(index);
}

int TrenchCoatController::getSize() const {
    return repository.getSize();
}

int TrenchCoatController::findTrenchCoat(const std::string& size, const std::string& colour) const {
    return repository.findTrenchCoat(size, colour);
} 
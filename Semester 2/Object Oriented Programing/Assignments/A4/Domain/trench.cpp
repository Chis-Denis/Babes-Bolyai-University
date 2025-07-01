#include "trench.h"
#include <iostream>
#include <sstream>
#include "../Exception/ValidationException.h"

// Parameterized constructor
TrenchCoat::TrenchCoat(const std::string& size, const std::string& colour, double price, 
                       int quantity, const std::string& photoLink) {
    this->size = size;
    this->colour = colour;
    this->price = price;
    this->quantity = quantity;
    this->photoLink = photoLink;
    TrenchCoatValidator::validate(*this);
}

// Getters
std::string TrenchCoat::getSize() const { return size; }
std::string TrenchCoat::getColour() const { return colour; }
double TrenchCoat::getPrice() const { return price; }
int TrenchCoat::getQuantity() const { return quantity; }
std::string TrenchCoat::getPhotoLink() const { return photoLink; }

// Setters
void TrenchCoat::setSize(const std::string& size) { 
    this->size = size;
    TrenchCoatValidator::validate(*this);
}

void TrenchCoat::setColour(const std::string& colour) { 
    this->colour = colour;
    TrenchCoatValidator::validate(*this);
}

void TrenchCoat::setPrice(double price) { 
    this->price = price;
    TrenchCoatValidator::validate(*this);
}

void TrenchCoat::setQuantity(int quantity) { 
    this->quantity = quantity;
    TrenchCoatValidator::validate(*this);
}

void TrenchCoat::setPhotoLink(const std::string& photoLink) { 
    this->photoLink = photoLink;
    TrenchCoatValidator::validate(*this);
}

// Method to display trench coat information
void TrenchCoat::display() const {
    std::cout << "Trench Coat Details:" << std::endl;
    std::cout << "Size: " << size << std::endl;
    std::cout << "Colour: " << colour << std::endl;
    std::cout << "Price: $" << price << std::endl;
    std::cout << "Quantity: " << quantity << std::endl;
    std::cout << "Photo Link: " << photoLink << std::endl;
}

std::ostream& operator<<(std::ostream& os, const TrenchCoat& coat) {
    os << coat.size << '\n';
    os << coat.colour << '\n';
    os << coat.price << '\n';
    os << coat.quantity << '\n';
    os << coat.photoLink << '\n';
    return os;
}

std::istream& operator>>(std::istream& is, TrenchCoat& coat) {
    std::string line;
    
    // Read size
    if (!std::getline(is, line)) return is;
    coat.size = line;
    
    // Read colour
    if (!std::getline(is, line)) return is;
    coat.colour = line;
    
    // Read price
    if (!std::getline(is, line)) return is;
    std::stringstream ss(line);
    ss >> coat.price;
    
    // Read quantity
    if (!std::getline(is, line)) return is;
    ss.clear();
    ss.str(line);
    ss >> coat.quantity;
    
    // Read photo link
    if (!std::getline(is, line)) return is;
    coat.photoLink = line;
    
    // Validate the read data
    try {
        TrenchCoatValidator::validate(coat);
    } catch (const ValidationException& e) {
        is.setstate(std::ios::failbit);
    }
    
    return is;
} 
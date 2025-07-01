#include "trench.h"
#include <iostream>

// Default constructor
TrenchCoat::TrenchCoat() : size(""), colour(""), price(0.0), quantity(0), photoLink("") {}

// Parameterized constructor
TrenchCoat::TrenchCoat(const std::string& size, const std::string& colour, double price, 
                       int quantity, const std::string& photoLink) {
    this->size = size;
    this->colour = colour;
    this->price = price;
    this->quantity = quantity;
    this->photoLink = photoLink;
}

// Getters
std::string TrenchCoat::getSize() const { return size; }
std::string TrenchCoat::getColour() const { return colour; }
double TrenchCoat::getPrice() const { return price; }
int TrenchCoat::getQuantity() const { return quantity; }
std::string TrenchCoat::getPhotoLink() const { return photoLink; }

// Setters
void TrenchCoat::setSize(const std::string& size) { this->size = size; }
void TrenchCoat::setColour(const std::string& colour) { this->colour = colour; }
void TrenchCoat::setPrice(double price) { this->price = price; }
void TrenchCoat::setQuantity(int quantity) { this->quantity = quantity; }
void TrenchCoat::setPhotoLink(const std::string& photoLink) { this->photoLink = photoLink; }

// Method to display trench coat information
void TrenchCoat::display() const {
    std::cout << "Trench Coat Details:" << std::endl;
    std::cout << "Size: " << size << std::endl;
    std::cout << "Colour: " << colour << std::endl;
    std::cout << "Price: $" << price << std::endl;
    std::cout << "Quantity: " << quantity << std::endl;
    std::cout << "Photo Link: " << photoLink << std::endl;
} 
#pragma once

#include <string>
#include <iostream>
#include "TrenchCoatValidator.h"

class TrenchCoat {
private:
    std::string size;
    std::string colour;
    double price;
    int quantity;
    std::string photoLink;

public:
    // Default constructor
    TrenchCoat() = default;

    // Constructor with parameters
    TrenchCoat(const std::string& size, const std::string& colour, double price, int quantity, const std::string& photoLink);

    // Getters
    std::string getSize() const;
    std::string getColour() const;
    double getPrice() const;
    int getQuantity() const;
    std::string getPhotoLink() const;

    // Setters
    void setSize(const std::string& size);
    void setColour(const std::string& colour);
    void setPrice(double price);
    void setQuantity(int quantity);
    void setPhotoLink(const std::string& photoLink);

    // Method to display trench coat information
    void display() const;

    // Overloaded operators for file I/O
    friend std::ostream& operator<<(std::ostream& os, const TrenchCoat& coat);
    friend std::istream& operator>>(std::istream& is, TrenchCoat& coat);
};
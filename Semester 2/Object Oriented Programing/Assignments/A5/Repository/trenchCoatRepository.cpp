#include "trenchCoatRepository.h"
#include <stdexcept>

// Copy constructor
TrenchCoatRepository::TrenchCoatRepository(const TrenchCoatRepository& other) : trenchCoats(other.trenchCoats) {
}

// Assignment operator
TrenchCoatRepository& TrenchCoatRepository::operator=(const TrenchCoatRepository& other) {
    if (this != &other) {
        trenchCoats = other.trenchCoats;
    }
    return *this;
}

void TrenchCoatRepository::addTrenchCoat(const TrenchCoat& trenchCoat) {
    // Check if a trench coat with same size and colour already exists
    if (findTrenchCoat(trenchCoat.getSize(), trenchCoat.getColour()) != -1) {
        throw std::runtime_error("A trench coat with the same size and colour already exists!");
    }
    trenchCoats.add(trenchCoat);
}

void TrenchCoatRepository::removeTrenchCoat(int index) {
    if (index < 0 || index >= trenchCoats.getSize()) {
        throw std::out_of_range("Invalid trench coat index!");
    }
    trenchCoats.remove(index);
}

void TrenchCoatRepository::updateTrenchCoat(int index, const TrenchCoat& newTrenchCoat) {
    if (index < 0 || index >= trenchCoats.getSize()) {
        throw std::out_of_range("Invalid trench coat index!");
    }
    
    // Check if update would create a duplicate (same size and colour as another coat)
    int existingIndex = findTrenchCoat(newTrenchCoat.getSize(), newTrenchCoat.getColour());
    if (existingIndex != -1 && existingIndex != index) {
        throw std::runtime_error("A trench coat with the same size and colour already exists!");
    }
    
    trenchCoats[index] = newTrenchCoat;
}

const DynamicVector<TrenchCoat>& TrenchCoatRepository::getAllTrenchCoats() const {
    return trenchCoats;
}

const TrenchCoat& TrenchCoatRepository::getTrenchCoat(int index) const {
    if (index < 0 || index >= trenchCoats.getSize()) {
        throw std::out_of_range("Invalid trench coat index!");
    }
    return trenchCoats[index];
}

int TrenchCoatRepository::getSize() const {
    return trenchCoats.getSize();
}

int TrenchCoatRepository::findTrenchCoat(const std::string& size, const std::string& colour) const {
    for (int i = 0; i < trenchCoats.getSize(); i++) {
        if (trenchCoats[i].getSize() == size && trenchCoats[i].getColour() == colour) {
            return i;
        }
    }
    return -1;
} 
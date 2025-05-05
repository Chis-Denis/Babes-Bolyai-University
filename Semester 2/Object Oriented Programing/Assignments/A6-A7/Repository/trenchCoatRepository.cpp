#include "trenchCoatRepository.h"
#include <stdexcept>
#include <algorithm>
#include <fstream>
#include "../Exception/ValidationException.h"

TrenchCoatRepository::TrenchCoatRepository(const std::string& filename) : filename(filename) {
    loadFromFile();
}

void TrenchCoatRepository::loadFromFile() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // If file doesn't exist, create it
        std::ofstream createFile(filename);
        createFile.close();
        return;
    }

    trenchCoats.clear();
    TrenchCoat coat;
    while (file >> coat) {
        try {
            TrenchCoatValidator::validate(coat);
            trenchCoats.push_back(coat);
        } catch (const ValidationException& e) {
            // Skip invalid entries
            continue;
        }
    }
    file.close();
}

void TrenchCoatRepository::saveToFile() const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw FileException("Could not open file for writing: " + filename);
    }

    for (const auto& coat : trenchCoats) {
        file << coat;
    }
    file.close();
}

void TrenchCoatRepository::addTrenchCoat(const TrenchCoat& trenchCoat) {
    // Validate the trench coat
    TrenchCoatValidator::validate(trenchCoat);

    // Check if a trench coat with same size and colour already exists
    if (findTrenchCoat(trenchCoat.getSize(), trenchCoat.getColour()) != -1) {
        throw RepositoryException("A trench coat with the same size and colour already exists!");
    }
    trenchCoats.push_back(trenchCoat);
    saveToFile();
}

void TrenchCoatRepository::removeTrenchCoat(int index) {
    if (index < 0 || index >= static_cast<int>(trenchCoats.size())) {
        throw RepositoryException("Invalid trench coat index!");
    }
    trenchCoats.erase(trenchCoats.begin() + index);
    saveToFile();
}

void TrenchCoatRepository::updateTrenchCoat(int index, const TrenchCoat& newTrenchCoat) {
    if (index < 0 || index >= static_cast<int>(trenchCoats.size())) {
        throw RepositoryException("Invalid trench coat index!");
    }
    
    // Validate the new trench coat
    TrenchCoatValidator::validate(newTrenchCoat);
    
    // Check if update would create a duplicate (same size and colour as another coat)
    int existingIndex = findTrenchCoat(newTrenchCoat.getSize(), newTrenchCoat.getColour());
    if (existingIndex != -1 && existingIndex != index) {
        throw RepositoryException("A trench coat with the same size and colour already exists!");
    }
    
    trenchCoats[index] = newTrenchCoat;
    saveToFile();
}

const std::vector<TrenchCoat>& TrenchCoatRepository::getAllTrenchCoats() const {
    return trenchCoats;
}

const TrenchCoat& TrenchCoatRepository::getTrenchCoat(int index) const {
    if (index < 0 || index >= static_cast<int>(trenchCoats.size())) {
        throw RepositoryException("Invalid trench coat index!");
    }
    return trenchCoats[index];
}

int TrenchCoatRepository::getSize() const {
    return static_cast<int>(trenchCoats.size());
}

int TrenchCoatRepository::findTrenchCoat(const std::string& size, const std::string& colour) const {
    auto it = std::find_if(trenchCoats.begin(), trenchCoats.end(),
        [&](const TrenchCoat& coat) {
            return coat.getSize() == size && coat.getColour() == colour;
        });
    
    if (it != trenchCoats.end()) {
        return static_cast<int>(std::distance(trenchCoats.begin(), it));
    }
    return -1;
} 
#include "CSVRepository.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "../Exception/RepositoryException.h"
#include "../Exception/ValidationException.h"
#include "../Utils/Utils.h"

CSVRepository::CSVRepository(const std::string& filename) : FileRepository(filename) {
    loadFromFile();
}

void CSVRepository::loadFromFile() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // If file doesn't exist, create it
        std::ofstream createFile(filename);
        createFile.close();
        return;
    }

    trenchCoats.clear();
    std::string line;
    while (std::getline(file, line)) {
        line = Utils::trim(line);
        if (line.empty()) continue;

        auto tokens = Utils::tokenize(line, ',');
        if (tokens.size() != 5) continue; // Skip invalid lines

        try {
            std::string size = tokens[0];
            std::string colour = tokens[1];
            double price = std::stod(tokens[2]);
            int quantity = std::stoi(tokens[3]);
            std::string photoLink = tokens[4];

            TrenchCoat coat(size, colour, price, quantity, photoLink);
            TrenchCoatValidator::validate(coat);
            trenchCoats.push_back(coat);
        } catch (const std::exception& e) {
            // Skip invalid entries
            continue;
        }
    }
    file.close();
}

void CSVRepository::saveToFile() const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw FileException("Could not open file for writing: " + filename);
    }

    for (const auto& coat : trenchCoats) {
        file << coat.getSize() << ','
             << coat.getColour() << ','
             << std::fixed << std::setprecision(2) << coat.getPrice() << ','
             << coat.getQuantity() << ','
             << coat.getPhotoLink() << '\n';
    }
    file.close();
}

void CSVRepository::addTrenchCoat(const TrenchCoat& trenchCoat) {
    TrenchCoatValidator::validate(trenchCoat);
    if (findTrenchCoat(trenchCoat.getSize(), trenchCoat.getColour()) != -1) {
        throw RepositoryException("A trench coat with the same size and colour already exists!");
    }
    trenchCoats.push_back(trenchCoat);
    saveToFile();
}

void CSVRepository::removeTrenchCoat(int index) {
    if (index < 0 || index >= static_cast<int>(trenchCoats.size())) {
        throw RepositoryException("Invalid trench coat index!");
    }
    trenchCoats.erase(trenchCoats.begin() + index);
    saveToFile();
}

void CSVRepository::updateTrenchCoat(int index, const TrenchCoat& newTrenchCoat) {
    if (index < 0 || index >= static_cast<int>(trenchCoats.size())) {
        throw RepositoryException("Invalid trench coat index!");
    }
    
    TrenchCoatValidator::validate(newTrenchCoat);
    int existingIndex = findTrenchCoat(newTrenchCoat.getSize(), newTrenchCoat.getColour());
    if (existingIndex != -1 && existingIndex != index) {
        throw RepositoryException("A trench coat with the same size and colour already exists!");
    }
    
    trenchCoats[index] = newTrenchCoat;
    saveToFile();
}

const std::vector<TrenchCoat>& CSVRepository::getAllTrenchCoats() const {
    return trenchCoats;
}

const TrenchCoat& CSVRepository::getTrenchCoat(int index) const {
    if (index < 0 || index >= static_cast<int>(trenchCoats.size())) {
        throw RepositoryException("Invalid trench coat index!");
    }
    return trenchCoats[index];
}

int CSVRepository::getSize() const {
    return static_cast<int>(trenchCoats.size());
}

int CSVRepository::findTrenchCoat(const std::string& size, const std::string& colour) const {
    auto it = std::find_if(trenchCoats.begin(), trenchCoats.end(),
        [&](const TrenchCoat& coat) {
            return coat.getSize() == size && coat.getColour() == colour;
        });
    
    if (it != trenchCoats.end()) {
        return static_cast<int>(std::distance(trenchCoats.begin(), it));
    }
    return -1;
} 
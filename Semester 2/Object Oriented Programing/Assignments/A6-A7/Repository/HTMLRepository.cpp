#include "HTMLRepository.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "../Exception/RepositoryException.h"
#include "../Utils/Utils.h"

HTMLRepository::HTMLRepository(const std::string& filename) : FileRepository(filename) {
    loadFromFile();
}

void HTMLRepository::loadFromFile() {
    // HTML files are for display only, not for reading data
    trenchCoats.clear();
}

void HTMLRepository::saveToFile() const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw FileException("Could not open file for writing: " + filename);
    }

    // Write HTML header
    file << "<!DOCTYPE html>\n"
         << "<html>\n"
         << "<head>\n"
         << "    <title>Trench Coats</title>\n"
         << "    <style>\n"
         << "        table { border-collapse: collapse; width: 100%; }\n"
         << "        th, td { border: 1px solid black; padding: 8px; text-align: left; }\n"
         << "        th { background-color: #f2f2f2; }\n"
         << "        .total { font-weight: bold; text-align: right; padding: 8px; }\n"
         << "    </style>\n"
         << "</head>\n"
         << "<body>\n"
         << "    <h1>Trench Coats</h1>\n"
         << "    <table>\n"
         << "        <tr>\n"
         << "            <th>Size</th>\n"
         << "            <th>Colour</th>\n"
         << "            <th>Price</th>\n"
         << "            <th>Quantity</th>\n"
         << "            <th>Photo Link</th>\n"
         << "        </tr>\n";

    // Write table rows
    for (const auto& coat : trenchCoats) {
        file << "        <tr>\n"
             << "            <td>" << Utils::trim(coat.getSize()) << "</td>\n"
             << "            <td>" << Utils::trim(coat.getColour()) << "</td>\n"
             << "            <td>$" << std::fixed << std::setprecision(2) << coat.getPrice() << "</td>\n"
             << "            <td>" << coat.getQuantity() << "</td>\n"
             << "            <td><a href=\"" << Utils::trim(coat.getPhotoLink()) << "\">View Photo</a></td>\n"
             << "        </tr>\n";
    }

    // Write total and close table
    file << "    </table>\n"
         << "</body>\n"
         << "</html>\n";

    file.close();
}

void HTMLRepository::addTrenchCoat(const TrenchCoat& trenchCoat) {
    TrenchCoatValidator::validate(trenchCoat);
    if (findTrenchCoat(trenchCoat.getSize(), trenchCoat.getColour()) != -1) {
        throw RepositoryException("A trench coat with the same size and colour already exists!");
    }
    trenchCoats.push_back(trenchCoat);
    saveToFile();
}

void HTMLRepository::removeTrenchCoat(int index) {
    if (index < 0 || index >= static_cast<int>(trenchCoats.size())) {
        throw RepositoryException("Invalid trench coat index!");
    }
    trenchCoats.erase(trenchCoats.begin() + index);
    saveToFile();
}

void HTMLRepository::updateTrenchCoat(int index, const TrenchCoat& newTrenchCoat) {
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

const std::vector<TrenchCoat>& HTMLRepository::getAllTrenchCoats() const {
    return trenchCoats;
}

const TrenchCoat& HTMLRepository::getTrenchCoat(int index) const {
    if (index < 0 || index >= static_cast<int>(trenchCoats.size())) {
        throw RepositoryException("Invalid trench coat index!");
    }
    return trenchCoats[index];
}

int HTMLRepository::getSize() const {
    return static_cast<int>(trenchCoats.size());
}

int HTMLRepository::findTrenchCoat(const std::string& size, const std::string& colour) const {
    auto it = std::find_if(trenchCoats.begin(), trenchCoats.end(),
        [&](const TrenchCoat& coat) {
            return Utils::trim(coat.getSize()) == Utils::trim(size) && 
                   Utils::trim(coat.getColour()) == Utils::trim(colour);
        });
    
    if (it != trenchCoats.end()) {
        return static_cast<int>(std::distance(trenchCoats.begin(), it));
    }
    return -1;
} 
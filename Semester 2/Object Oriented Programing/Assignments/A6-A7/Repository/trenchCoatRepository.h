#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "../Domain/trench.h"
#include "../Exception/RepositoryException.h"
#include "../Exception/FileException.h"
#include "../Domain/TrenchCoatValidator.h"

class TrenchCoatRepository {
private:
    std::vector<TrenchCoat> trenchCoats;
    std::string filename;

    // Load trench coats from file
    void loadFromFile();

    // Save trench coats to file
    void saveToFile() const;

public:
    // Constructor with filename
    explicit TrenchCoatRepository(const std::string& filename = "trench_coats.txt");

    // Copy constructor
    TrenchCoatRepository(const TrenchCoatRepository& other) = default;

    // Destructor
    ~TrenchCoatRepository() = default;

    // Assignment operator
    TrenchCoatRepository& operator=(const TrenchCoatRepository& other) = default;

    // Add a new trench coat to the repository
    void addTrenchCoat(const TrenchCoat& trenchCoat);

    // Remove a trench coat by index
    void removeTrenchCoat(int index);

    // Update a trench coat at the specified index
    void updateTrenchCoat(int index, const TrenchCoat& newTrenchCoat);

    // Get all trench coats
    const std::vector<TrenchCoat>& getAllTrenchCoats() const;

    // Get trench coat by index
    const TrenchCoat& getTrenchCoat(int index) const;

    // Get the number of trench coats
    int getSize() const;

    // Find index of a trench coat by its attributes
    int findTrenchCoat(const std::string& size, const std::string& colour) const;
}; 
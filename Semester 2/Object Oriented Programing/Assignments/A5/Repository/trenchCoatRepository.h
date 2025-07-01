#pragma once

#include "../Domain/dynamicVector.h"
#include "../Domain/trench.h"

class TrenchCoatRepository {
private:
    DynamicVector<TrenchCoat> trenchCoats;

public:
    // Default constructor
    TrenchCoatRepository() = default;

    // Copy constructor
    TrenchCoatRepository(const TrenchCoatRepository& other);

    // Destructor
    ~TrenchCoatRepository() = default;

    // Assignment operator
    TrenchCoatRepository& operator=(const TrenchCoatRepository& other);

    // Add a new trench coat to the repository
    void addTrenchCoat(const TrenchCoat& trenchCoat);

    // Remove a trench coat by index
    void removeTrenchCoat(int index);

    // Update a trench coat at the specified index
    void updateTrenchCoat(int index, const TrenchCoat& newTrenchCoat);

    // Get all trench coats
    const DynamicVector<TrenchCoat>& getAllTrenchCoats() const;

    // Get trench coat by index
    const TrenchCoat& getTrenchCoat(int index) const;

    // Get the number of trench coats
    int getSize() const;

    // Find index of a trench coat by its attributes
    int findTrenchCoat(const std::string& size, const std::string& colour) const;
}; 
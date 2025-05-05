#pragma once

#include "../Repository/trenchCoatRepository.h"
#include "../Domain/trench.h"
#include <string>
#include <vector>

class TrenchCoatController {
private:
    TrenchCoatRepository& repository;

public:
    // Constructor
    explicit TrenchCoatController(TrenchCoatRepository& repository);

    // Copy constructor - deleted because we don't want to copy the controller
    TrenchCoatController(const TrenchCoatController& other) = delete;

    // Assignment operator - deleted because we don't want to assign controllers
    TrenchCoatController& operator=(const TrenchCoatController& other) = delete;

    // Destructor
    ~TrenchCoatController() = default;

    // Initialize repository with some data
    void addInitialData();

    // Add a new trench coat
    void addTrenchCoat(const std::string& size, const std::string& colour, 
                      double price, int quantity, const std::string& photoLink);

    // Remove a trench coat
    void removeTrenchCoat(int index);

    // Update a trench coat
    void updateTrenchCoat(int index, const std::string& size, const std::string& colour,
                         double price, int quantity, const std::string& photoLink);

    // Get all trench coats
    std::vector<TrenchCoat> getAllTrenchCoats() const;

    // Get trench coats filtered by size
    std::vector<TrenchCoat> getTrenchCoatsBySize(const std::string& size) const;

    // Get trench coats with price less than given value
    std::vector<TrenchCoat> getTrenchCoatsCheaperThan(double maxPrice) const;

    // Get trench coat by index
    TrenchCoat getTrenchCoat(int index) const;

    // Get number of trench coats
    int getSize() const;

    // Find index of a trench coat by size and colour
    int findTrenchCoat(const std::string& size, const std::string& colour) const;

    // Validate trench coat data
    static void validateTrenchCoatData(const std::string& size, const std::string& colour,
                                     double price, int quantity, const std::string& photoLink);
};
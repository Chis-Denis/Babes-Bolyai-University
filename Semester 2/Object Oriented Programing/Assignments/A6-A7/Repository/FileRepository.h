#pragma once
#include <string>
#include <vector>
#include "../Domain/trench.h"

class FileRepository {
protected:
    std::string filename;
    std::vector<TrenchCoat> trenchCoats;

    virtual void loadFromFile() = 0;
    virtual void saveToFile() const = 0;

public:
    explicit FileRepository(const std::string& filename) : filename(filename) {}
    virtual ~FileRepository() = default;

    virtual void addTrenchCoat(const TrenchCoat& trenchCoat) = 0;
    virtual void removeTrenchCoat(int index) = 0;
    virtual void updateTrenchCoat(int index, const TrenchCoat& newTrenchCoat) = 0;
    virtual const std::vector<TrenchCoat>& getAllTrenchCoats() const = 0;
    virtual const TrenchCoat& getTrenchCoat(int index) const = 0;
    virtual int getSize() const = 0;
    virtual int findTrenchCoat(const std::string& size, const std::string& colour) const = 0;
}; 
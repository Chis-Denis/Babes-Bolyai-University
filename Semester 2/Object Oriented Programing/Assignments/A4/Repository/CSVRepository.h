#pragma once
#include "FileRepository.h"
#include "../Exception/FileException.h"
#include "../Domain/TrenchCoatValidator.h"

class CSVRepository : public FileRepository {
private:
    void loadFromFile() override;
    void saveToFile() const override;

public:
    explicit CSVRepository(const std::string& filename);

    void addTrenchCoat(const TrenchCoat& trenchCoat) override;
    void removeTrenchCoat(int index) override;
    void updateTrenchCoat(int index, const TrenchCoat& newTrenchCoat) override;
    const std::vector<TrenchCoat>& getAllTrenchCoats() const override;
    const TrenchCoat& getTrenchCoat(int index) const override;
    int getSize() const override;
    int findTrenchCoat(const std::string& size, const std::string& colour) const override;
}; 
#pragma once

#include "UI.h"

class AdminUI : public UI {
private:
    // Admin specific operations
    void addTrenchCoat();
    void removeTrenchCoat();
    void updateTrenchCoat();
    void viewAllTrenchCoats();
    static void printAdminMenu();

public:
    explicit AdminUI(TrenchCoatController& controller);
    void run() override;
};

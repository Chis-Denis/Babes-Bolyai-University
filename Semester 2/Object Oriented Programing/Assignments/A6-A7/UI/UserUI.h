#pragma once

#include "UI.h"
#include <vector>

class UserUI : public UI {
private:
    std::vector<TrenchCoat> shoppingCart;
    double totalAmount;

    // User specific operations
    void viewTrenchCoatsBySize();
    void viewShoppingCart();
    void checkout();
    void exportToCSV() const;
    void exportToHTML() const;
    static void printUserMenu();

public:
    explicit UserUI(TrenchCoatController& controller);
    void run() override;
};
#pragma once

#include "UI.h"
#include "../Domain/dynamicVector.h"

class UserUI : public UI {
private:
    DynamicVector<TrenchCoat> shoppingCart;
    double totalAmount;

    // User specific operations
    void viewTrenchCoatsBySize();
    void viewShoppingCart();
    void checkout();
    static void printUserMenu();

public:
    explicit UserUI(TrenchCoatController& controller);
    void run() override;
};
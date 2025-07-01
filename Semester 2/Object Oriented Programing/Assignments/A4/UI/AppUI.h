#pragma once

#include "../Controller/trenchCoatController.h"
#include <limits> // For numeric_limits
#include <iostream> // For cout, cin

class AppUI {
private:
    TrenchCoatController& controller;

    void printWelcomeMessage();
    int readChoice();
    void clearScreen();

public:
    explicit AppUI(TrenchCoatController& controller);
    void run();
}; 
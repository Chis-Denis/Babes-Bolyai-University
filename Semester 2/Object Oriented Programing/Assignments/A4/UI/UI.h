#pragma once

#include "../Controller/trenchCoatController.h"
#include <string>

class UI {
protected:
    TrenchCoatController& controller;

    // Helper functions
    static void printMenu();
    static void printTrenchCoat(const TrenchCoat& coat, int index);
    static void clearScreen();
    static std::string readString(const std::string& prompt);
    static double readDouble(const std::string& prompt);
    static int readInteger(const std::string& prompt);

    // Helper functions for input
    int readInt(const std::string& prompt = "");

public:
    explicit UI(TrenchCoatController& controller);
    virtual ~UI() = default;

    // Pure virtual function to run the UI
    virtual void run() = 0;
};
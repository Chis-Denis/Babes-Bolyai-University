#include "UI.h"
#include <iostream>
#include <limits>
#include <cstdlib>

UI::UI(TrenchCoatController& controller) : controller(controller) {}

void UI::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void UI::printTrenchCoat(const TrenchCoat& coat, int index) {
    std::cout << "Trench Coat #" << index << std::endl;
    std::cout << "Size: " << coat.getSize() << std::endl;
    std::cout << "Colour: " << coat.getColour() << std::endl;
    std::cout << "Price: $" << coat.getPrice() << std::endl;
    std::cout << "Quantity: " << coat.getQuantity() << std::endl;
    std::cout << "Photo: " << coat.getPhotoLink() << std::endl;
    std::cout << "------------------------" << std::endl;
}

std::string UI::readString(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

int UI::readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a valid integer: ";
    }
}

double UI::readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a valid number: ";
    }
} 
#include "AppUI.h"
#include "AdminUI.h"
#include "UserUI.h"
#include <cstdlib> // For system()

AppUI::AppUI(TrenchCoatController& controller) : controller(controller) {}

void AppUI::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void AppUI::printWelcomeMessage() {
    std::cout << "=====================================" << std::endl;
    std::cout << "Welcome to Proper Trench Coats Store!" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "\n1. Admin Mode" << std::endl;
    std::cout << "2. User Mode" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "\nChoose mode: ";
}

int AppUI::readChoice() {
    int choice;
    while (!(std::cin >> choice)) {
        std::cout << "Invalid input. Please enter a number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    // Consume the newline character left by >>
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    return choice;
}

void AppUI::run() {
    while (true) {
        clearScreen();
        printWelcomeMessage();
        
        int mode = readChoice();
        
        try {
            switch (mode) {
                case 1: {
                    AdminUI adminUI(controller);
                    adminUI.run();
                    break;
                }
                case 2: {
                    UserUI userUI(controller);
                    userUI.run();
                    break;
                }
                case 0: {
                    std::cout << "\nThank you for using our application!" << std::endl;
                    return; // Exit the run loop and thus the application
                }
                default: {
                    std::cout << "\nInvalid mode! Press Enter to try again...";
                    std::cin.get(); // Wait for user to press Enter
                    break;
                }
            }
        }
        catch (const std::exception& e) {
            // Catch exceptions that might bubble up from AdminUI/UserUI run methods
            std::cout << "\nAn unexpected error occurred: " << e.what() << std::endl;
            std::cout << "Press Enter to continue...";
            std::cin.get(); // Wait for user acknowledgement
        }
    }
} 
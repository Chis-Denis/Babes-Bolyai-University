#include "AdminUI.h"
#include <iostream>

AdminUI::AdminUI(TrenchCoatController& controller) : UI(controller) {}

void AdminUI::printAdminMenu() {
    std::cout << "\nAdmin Menu:" << std::endl;
    std::cout << "1. Add new trench coat" << std::endl;
    std::cout << "2. Remove trench coat" << std::endl;
    std::cout << "3. Update trench coat" << std::endl;
    std::cout << "4. View all trench coats" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choose an option: ";
}

void AdminUI::addTrenchCoat() {
    std::cout << "\nAdd New Trench Coat" << std::endl;
    std::cout << "-------------------" << std::endl;
    
    std::string size = readString("Enter size: ");
    std::string colour = readString("Enter colour: ");
    double price = readDouble("Enter price: ");
    int quantity = readInteger("Enter quantity: ");
    std::string photoLink = readString("Enter photo link: ");

    try {
        controller.addTrenchCoat(size, colour, price, quantity, photoLink);
        std::cout << "Trench coat added successfully!" << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Error adding trench coat: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "An unexpected error occurred: " << e.what() << std::endl;
    }
}

void AdminUI::removeTrenchCoat() {
    std::cout << "\nRemove Trench Coat" << std::endl;
    std::cout << "------------------" << std::endl;
    
    viewAllTrenchCoats();
    if (controller.getSize() == 0) return;

    int index = readInteger("Enter the index of the trench coat to remove: ");
    
    try {
        controller.removeTrenchCoat(index);
        std::cout << "Trench coat removed successfully!" << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cout << "Error removing trench coat: Invalid index provided." << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "An unexpected error occurred: " << e.what() << std::endl;
    }
}

void AdminUI::updateTrenchCoat() {
    std::cout << "\nUpdate Trench Coat" << std::endl;
    std::cout << "------------------" << std::endl;
    
    viewAllTrenchCoats();
    if (controller.getSize() == 0) return;

    int index = readInteger("Enter the index of the trench coat to update: ");
    
    try {
        if (index < 0 || index >= controller.getSize()) {
             throw std::out_of_range("Invalid index provided.");
        }
        TrenchCoat currentCoat = controller.getTrenchCoat(index);
        std::cout << "\nCurrent values:" << std::endl;
        printTrenchCoat(currentCoat, index);
        
        std::cout << "\nEnter new values (press Enter to keep current value):" << std::endl;
        
        std::string size = readString("Enter new size: ");
        if (size.empty()) size = currentCoat.getSize();
        
        std::string colour = readString("Enter new colour: ");
        if (colour.empty()) colour = currentCoat.getColour();
        
        std::string priceStr = readString("Enter new price: ");
        double price = priceStr.empty() ? currentCoat.getPrice() : std::stod(priceStr);
        
        std::string quantityStr = readString("Enter new quantity: ");
        int quantity = quantityStr.empty() ? currentCoat.getQuantity() : std::stoi(quantityStr);
        
        std::string photoLink = readString("Enter new photo link: ");
        if (photoLink.empty()) photoLink = currentCoat.getPhotoLink();

        int existingIndex = controller.findTrenchCoat(size, colour);
        if (existingIndex != -1 && existingIndex != index) {
            throw std::invalid_argument("Another trench coat with the same size and colour already exists.");
        }

        controller.updateTrenchCoat(index, size, colour, price, quantity, photoLink);
        std::cout << "Trench coat updated successfully!" << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Error updating trench coat: " << e.what() << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cout << "Error updating trench coat: Invalid index provided." << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "An unexpected error occurred during update: " << e.what() << std::endl;
    }
}

void AdminUI::viewAllTrenchCoats() {
    std::cout << "\nAll Trench Coats" << std::endl;
    std::cout << "----------------" << std::endl;
    
    DynamicVector<TrenchCoat> coats = controller.getAllTrenchCoats();
    if (coats.getSize() == 0) {
        std::cout << "No trench coats available." << std::endl;
        return;
    }

    for (int i = 0; i < coats.getSize(); i++) {
        printTrenchCoat(coats[i], i);
    }
}

void AdminUI::run() {
    while (true) {
        clearScreen();
        printAdminMenu();
        
        int choice = readInteger("");
        
        switch (choice) {
            case 1:
                addTrenchCoat();
                break;
            case 2:
                removeTrenchCoat();
                break;
            case 3:
                updateTrenchCoat();
                break;
            case 4:
                viewAllTrenchCoats();
                break;
            case 0:
                std::cout << "Exiting admin mode..." << std::endl;
                return;
            default:
                std::cout << "Invalid option!" << std::endl;
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
} 
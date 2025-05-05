#include "AdminUI.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

AdminUI::AdminUI(TrenchCoatController& controller) : UI(controller) {}

void AdminUI::printAdminMenu() {
    std::cout << "\nAdmin Menu:" << std::endl;
    std::cout << "1. View all trench coats" << std::endl;
    std::cout << "2. Add trench coat" << std::endl;
    std::cout << "3. Remove trench coat" << std::endl;
    std::cout << "4. Update trench coat" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choose an option: ";
}

void AdminUI::viewAllTrenchCoats() {
    std::vector<TrenchCoat> coats = controller.getAllTrenchCoats();
    
    if (coats.empty()) {
        std::cout << "No trench coats in the store." << std::endl;
        return;
    }

    std::cout << "\nAll Trench Coats" << std::endl;
    std::cout << "----------------" << std::endl;
    
    for (size_t i = 0; i < coats.size(); ++i) {
        std::cout << "Index: " << i << std::endl;
        std::cout << "Size: " << coats[i].getSize() << std::endl;
        std::cout << "Colour: " << coats[i].getColour() << std::endl;
        std::cout << "Price: $" << std::fixed << std::setprecision(2) << coats[i].getPrice() << std::endl;
        std::cout << "Quantity: " << coats[i].getQuantity() << std::endl;
        std::cout << "Photo: " << coats[i].getPhotoLink() << std::endl;
        std::cout << "-------------------" << std::endl;
    }
}

void AdminUI::addTrenchCoat() {
    std::cout << "\nAdd New Trench Coat" << std::endl;
    std::cout << "------------------" << std::endl;
    
    std::string size = readString("Enter size: ");
    std::string colour = readString("Enter colour: ");
    double price = readDouble("Enter price: ");
    int quantity = readInt("Enter quantity: ");
    std::string photoLink = readString("Enter photo link: ");
    
    try {
        controller.addTrenchCoat(size, colour, price, quantity, photoLink);
        std::cout << "Trench coat added successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error adding trench coat: " << e.what() << std::endl;
    }
}

void AdminUI::removeTrenchCoat() {
    std::cout << "\nRemove Trench Coat" << std::endl;
    std::cout << "-----------------" << std::endl;
    
    // Show all trench coats first
    viewAllTrenchCoats();
    if (controller.getSize() == 0) {
        return;
    }
    
    int index = readInt("Enter index of trench coat to remove: ");
    
    try {
        controller.removeTrenchCoat(index);
        std::cout << "Trench coat removed successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error removing trench coat: " << e.what() << std::endl;
    }
}

void AdminUI::updateTrenchCoat() {
    std::cout << "\nUpdate Trench Coat" << std::endl;
    std::cout << "-----------------" << std::endl;
    
    // Show all trench coats first
    viewAllTrenchCoats();
    if (controller.getSize() == 0) {
        return;
    }
    
    int index = readInt("Enter index of trench coat to update: ");
    
    try {
        // Get current coat details
        TrenchCoat currentCoat = controller.getTrenchCoat(index);
        
        std::cout << "\nCurrent values:" << std::endl;
        std::cout << "Size: " << currentCoat.getSize() << std::endl;
        std::cout << "Colour: " << currentCoat.getColour() << std::endl;
        std::cout << "Price: $" << std::fixed << std::setprecision(2) << currentCoat.getPrice() << std::endl;
        std::cout << "Quantity: " << currentCoat.getQuantity() << std::endl;
        std::cout << "Photo: " << currentCoat.getPhotoLink() << std::endl;
        
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
        
        controller.updateTrenchCoat(index, size, colour, price, quantity, photoLink);
        std::cout << "Trench coat updated successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error updating trench coat: " << e.what() << std::endl;
    }
}

void AdminUI::run() {
    bool running = true;
    while (running) {
        printAdminMenu();
        int option = readInt();
        switch (option) {
            case 1:
                viewAllTrenchCoats();
                break;
            case 2:
                addTrenchCoat();
                break;
            case 3:
                removeTrenchCoat();
                break;
            case 4:
                updateTrenchCoat();
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "Invalid option!" << std::endl;
        }
    }
} 
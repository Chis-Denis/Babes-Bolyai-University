#include "UserUI.h"
#include <iostream>
#include <iomanip>

UserUI::UserUI(TrenchCoatController& controller) : UI(controller), totalAmount(0.0) {}

void UserUI::printUserMenu() {
    std::cout << "\nUser Menu:" << std::endl;
    std::cout << "1. Browse trench coats" << std::endl;
    std::cout << "2. View shopping cart" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choose an option: ";
}

void UserUI::viewTrenchCoatsBySize() {
    std::cout << "\nBrowse Trench Coats" << std::endl;
    std::cout << "------------------" << std::endl;

    std::string size = readString("Enter size to filter by (leave empty to see all): ");
    DynamicVector<TrenchCoat> coats;
    try {
        coats = controller.getTrenchCoatsBySize(size);
    }
    catch (const std::exception& e) {
        std::cerr << "Error retrieving coats: " << e.what() << std::endl;
        return;
    }

    if (coats.getSize() == 0) {
        std::cout << "No trench coats found matching the criteria." << std::endl;
        return;
    }

    int current_index = 0;
    bool browsing = true;
    while (browsing) {
        clearScreen();
        const TrenchCoat& current_coat = coats[current_index];
        std::cout << "Trench Coat (" << (current_index + 1) << "/" << coats.getSize() << "):\n";
        printTrenchCoat(current_coat, -1);

        std::cout << "Photograph: " << current_coat.getPhotoLink() << std::endl;

        std::cout << "\nOptions:\n";
        std::cout << "1. Add to basket\n";
        std::cout << "2. Next\n";
        std::cout << "0. Exit browsing\n";
        std::cout << "Choose an option: ";

        int choice = readInteger("");

        switch (choice) {
            case 1: {
                if (current_coat.getQuantity() > 0) {
                    shoppingCart.add(current_coat);
                    totalAmount += current_coat.getPrice();
                    std::cout << "Added to basket! Current total: $" << std::fixed << std::setprecision(2) << totalAmount << std::endl;
                } else {
                    std::cout << "Sorry, this trench coat is out of stock." << std::endl;
                }
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                [[fallthrough]];
            }
            case 2: {
                current_index = (current_index + 1) % coats.getSize();
                break;
            }
            case 0:
                browsing = false;
                break;
            default:
                std::cout << "Invalid option!" << std::endl;
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
        }
    }
}

void UserUI::viewShoppingCart() {
    std::cout << "\nShopping Cart" << std::endl;
    std::cout << "-------------" << std::endl;
    
    if (shoppingCart.getSize() == 0) {
        std::cout << "Your shopping cart is empty." << std::endl;
        return;
    }

    for (int i = 0; i < shoppingCart.getSize(); i++) {
        printTrenchCoat(shoppingCart[i], i);
    }
    
    std::cout << "\nTotal amount: $" << std::fixed << std::setprecision(2) << totalAmount << std::endl;
}

void UserUI::checkout() {
    std::cout << "\nCheckout" << std::endl;
    std::cout << "--------" << std::endl;
    
    if (shoppingCart.getSize() == 0) {
        std::cout << "Your shopping cart is empty." << std::endl;
        return;
    }

    viewShoppingCart();
    std::cout << "\nConfirm purchase? (y/n): ";
    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm == "y" || confirm == "Y") {
        for (int i = 0; i < shoppingCart.getSize(); ++i) {
            const TrenchCoat& coat = shoppingCart[i];
            try {
                int index = controller.findTrenchCoat(coat.getSize(), coat.getColour());
                if (index != -1) {
                    TrenchCoat storeCoat = controller.getTrenchCoat(index);
                    if (storeCoat.getQuantity() > 0) {
                        int newQuantity = storeCoat.getQuantity() - 1;
                        if (newQuantity == 0) {
                            controller.removeTrenchCoat(index);
                            std::cout << "Note: " << coat.getColour() << " " << coat.getSize() << " trench coat removed from stock." << std::endl;
                        } else {
                            controller.updateTrenchCoat(index, storeCoat.getSize(), storeCoat.getColour(),
                                                     storeCoat.getPrice(), newQuantity, storeCoat.getPhotoLink());
                        }
                    } else {
                        std::cout << "Warning: " << coat.getColour() << " " << coat.getSize() << " trench coat was already out of stock. Purchase not completed for this item." << std::endl;
                    }
                }
            }
            catch (const std::exception& e) {
                std::cout << "Error updating inventory for " << coat.getColour() << " " << coat.getSize() << ": " << e.what() << std::endl;
            }
        }

        if (shoppingCart.getSize() > 0) {
            shoppingCart = DynamicVector<TrenchCoat>();
        }
        totalAmount = 0.0;
        std::cout << "Checkout complete! Thank you for your purchase!" << std::endl;
    } else {
        std::cout << "Purchase cancelled." << std::endl;
    }
}

void UserUI::run() {
    while (true) {
        clearScreen();
        printUserMenu();
        
        int choice = readInteger("");
        
        switch (choice) {
            case 1:
                viewTrenchCoatsBySize();
                break;
            case 2:
                viewShoppingCart();
                break;
            case 3:
                checkout();
                break;
            case 0:
                std::cout << "Exiting user mode..." << std::endl;
                return;
            default:
                std::cout << "Invalid option!" << std::endl;
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }
} 
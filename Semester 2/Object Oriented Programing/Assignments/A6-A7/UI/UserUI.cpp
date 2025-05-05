#include "UserUI.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <cstdlib>

UserUI::UserUI(TrenchCoatController& controller) : UI(controller), totalAmount(0.0) {}

void UserUI::printUserMenu() {
    std::cout << "\nUser Menu:" << std::endl;
    std::cout << "1. Browse trench coats" << std::endl;
    std::cout << "2. View shopping cart" << std::endl;
    std::cout << "3. Export cart to CSV" << std::endl;
    std::cout << "4. Export cart to HTML" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choose an option: ";
}

void UserUI::viewTrenchCoatsBySize() {
    std::cout << "\nBrowse Trench Coats" << std::endl;
    std::cout << "------------------" << std::endl;

    std::string size = readString("Enter size to filter by (leave empty to see all): ");
    std::vector<TrenchCoat> coats;
    try {
        coats = controller.getTrenchCoatsBySize(size);
    }
    catch (const std::exception& e) {
        std::cerr << "Error retrieving coats: " << e.what() << std::endl;
        return;
    }

    if (coats.empty()) {
        std::cout << "No trench coats found matching the criteria." << std::endl;
        return;
    }

    int current_index = 0;
    bool browsing = true;
    while (browsing && current_index < static_cast<int>(coats.size())) {
        const auto& coat = coats[current_index];
        std::cout << "\nCurrent coat:" << std::endl;
        std::cout << "Size: " << coat.getSize() << std::endl;
        std::cout << "Colour: " << coat.getColour() << std::endl;
        std::cout << "Price: $" << std::fixed << std::setprecision(2) << coat.getPrice() << std::endl;
        std::cout << "Quantity: " << coat.getQuantity() << std::endl;
        std::cout << "Photo: " << coat.getPhotoLink() << std::endl;

        std::cout << "\nOptions:" << std::endl;
        std::cout << "1. Add to cart" << std::endl;
        std::cout << "2. Next coat" << std::endl;
        std::cout << "0. Back to menu" << std::endl;
        std::cout << "Choose an option: ";

        int option = readInt();
        switch (option) {
            case 1:
                if (coat.getQuantity() > 0) {
                    shoppingCart.push_back(coat);
                    totalAmount += coat.getPrice();
                    std::cout << "Added to cart!" << std::endl;
                    current_index++;  // Automatically move to next coat
                } else {
                    std::cout << "This coat is out of stock!" << std::endl;
                }
                break;
            case 2:
                current_index++;
                break;
            case 0:
                browsing = false;
                break;
            default:
                std::cout << "Invalid option!" << std::endl;
        }

        // Check if we've reached the end of the list
        if (current_index >= static_cast<int>(coats.size())) {
            std::cout << "\nYou've reached the end of the trench coat list." << std::endl;
            browsing = false;
        }
    }
}

void UserUI::viewShoppingCart() {
    if (shoppingCart.empty()) {
        std::cout << "Your shopping cart is empty!" << std::endl;
        return;
    }

    std::cout << "\nShopping Cart" << std::endl;
    std::cout << "-------------" << std::endl;
    
    for (const auto& coat : shoppingCart) {
        std::cout << "Size: " << coat.getSize() << std::endl;
        std::cout << "Colour: " << coat.getColour() << std::endl;
        std::cout << "Price: $" << std::fixed << std::setprecision(2) << coat.getPrice() << std::endl;
        std::cout << "-------------------" << std::endl;
    }
    
    std::cout << "Total amount: $" << std::fixed << std::setprecision(2) << totalAmount << std::endl;
}

void UserUI::checkout() {
    if (shoppingCart.empty()) {
        std::cout << "Your shopping cart is empty!" << std::endl;
        return;
    }

    std::cout << "\nCheckout" << std::endl;
    std::cout << "--------" << std::endl;
    std::cout << "Total amount: $" << std::fixed << std::setprecision(2) << totalAmount << std::endl;
    std::cout << "Thank you for your purchase!" << std::endl;
    
    // Clear the shopping cart
    shoppingCart.clear();
    totalAmount = 0.0;
}

void UserUI::exportToCSV() const {
    if (shoppingCart.empty()) {
        std::cout << "Your shopping cart is empty!" << std::endl;
        return;
    }

    std::ofstream file("shopping_cart.csv");
    if (!file.is_open()) {
        std::cout << "Error: Could not create CSV file!" << std::endl;
        return;
    }

    // Write header
    file << "Size,Colour,Price,Quantity,PhotoLink\n";

    // Write data
    for (const auto& coat : shoppingCart) {
        file << coat.getSize() << ","
             << coat.getColour() << ","
             << std::fixed << std::setprecision(2) << coat.getPrice() << ","
             << "1" << "," // Quantity is 1 for each item in cart
             << coat.getPhotoLink() << "\n";
    }

    file.close();
    std::cout << "Shopping cart exported to shopping_cart.csv" << std::endl;
    
    // Open the file with the default application
    system("start shopping_cart.csv");
}

void UserUI::exportToHTML() const {
    if (shoppingCart.empty()) {
        std::cout << "Your shopping cart is empty!" << std::endl;
        return;
    }

    std::ofstream file("shopping_cart.html");
    if (!file.is_open()) {
        std::cout << "Error: Could not create HTML file!" << std::endl;
        return;
    }

    // Write HTML header
    file << "<!DOCTYPE html>\n"
         << "<html>\n"
         << "<head>\n"
         << "    <title>Shopping Cart</title>\n"
         << "    <style>\n"
         << "        table { border-collapse: collapse; width: 100%; }\n"
         << "        th, td { border: 1px solid black; padding: 8px; text-align: left; }\n"
         << "        th { background-color: #f2f2f2; }\n"
         << "        .total { font-weight: bold; text-align: right; padding: 8px; }\n"
         << "    </style>\n"
         << "</head>\n"
         << "<body>\n"
         << "    <h1>Shopping Cart</h1>\n"
         << "    <table>\n"
         << "        <tr>\n"
         << "            <th>Size</th>\n"
         << "            <th>Colour</th>\n"
         << "            <th>Price</th>\n"
         << "            <th>Photo Link</th>\n"
         << "        </tr>\n";

    // Write table rows
    for (const auto& coat : shoppingCart) {
        file << "        <tr>\n"
             << "            <td>" << coat.getSize() << "</td>\n"
             << "            <td>" << coat.getColour() << "</td>\n"
             << "            <td>$" << std::fixed << std::setprecision(2) << coat.getPrice() << "</td>\n"
             << "            <td><a href=\"" << coat.getPhotoLink() << "\">View Photo</a></td>\n"
             << "        </tr>\n";
    }

    // Write total and close table
    file << "    </table>\n"
         << "    <div class=\"total\">Total Amount: $" 
         << std::fixed << std::setprecision(2) << totalAmount << "</div>\n"
         << "</body>\n"
         << "</html>\n";

    file.close();
    std::cout << "Shopping cart exported to shopping_cart.html" << std::endl;
    
    // Open the file with the default browser
    system("start shopping_cart.html");
}

void UserUI::run() {
    bool running = true;
    while (running) {
        printUserMenu();
        int option = readInt();
        switch (option) {
            case 1:
                viewTrenchCoatsBySize();
                break;
            case 2:
                viewShoppingCart();
                break;
            case 3:
                exportToCSV();
                break;
            case 4:
                exportToHTML();
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "Invalid option!" << std::endl;
        }
    }
} 
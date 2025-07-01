#include "user_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

UserInterface* createUserInterface(Service* service) {
    UserInterface* ui = (UserInterface*)malloc(sizeof(UserInterface));
    if (ui == NULL) return NULL;
    ui->service = service;
    return ui;
}

void destroyUserInterface(UserInterface* ui) {
    if (ui == NULL) return;
    free(ui);
}

void displayMenu() {
    printf("\nSmart Refrigerator Management System\n");
    printf("1. Add product\n");
    printf("2. Delete product\n");
    printf("3. Update product\n");
    printf("4. Display products (filtered by name)\n");
    printf("5. Display expiring products\n");
    printf("6. Undo\n");
    printf("7. Redo\n");
    printf("0. Exit\n");
    printf("Choose an option: ");
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char* readString(const char* prompt) {
    char buffer[100];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        return _strdup(buffer);
    }
    return NULL;
}

void handleAddProduct(UserInterface* ui) {
    char* name = readString("Enter product name: ");
    if (name == NULL) return;

    printf("Enter product category:\n");
    printf("1. Dairy\n2. Sweets\n3. Meat\n4. Fruit\n");
    int categoryChoice;
    scanf("%d", &categoryChoice);
    clearInputBuffer();

    char* category;
    switch(categoryChoice) {
        case 1: category = _strdup(CATEGORY_DAIRY); break;
        case 2: category = _strdup(CATEGORY_SWEETS); break;
        case 3: category = _strdup(CATEGORY_MEAT); break;
        case 4: category = _strdup(CATEGORY_FRUIT); break;
        default:
            printf("Invalid category!\n");
            free(name);
            return;
    }

    printf("Enter quantity: ");
    int quantity;
    scanf("%d", &quantity);
    clearInputBuffer();

    char* date = readString("Enter expiration date (YYYY-MM-DD): ");
    if (date == NULL) {
        free(name);
        free(category);
        return;
    }

    Product* product = createProduct(name, category, quantity, date);
    if (product == NULL) {
        printf("Error: Invalid product data!\n");
    } else {
        if (addProductService(ui->service, product)) {
            printf("Product added successfully!\n");
        } else {
            printf("Error adding product!\n");
        }
        destroyProduct(product);
    }

    free(name);
    free(category);
    free(date);
}

void handleDeleteProduct(UserInterface* ui) {
    char* name = readString("Enter product name: ");
    if (name == NULL) return;

    printf("Enter product category:\n");
    printf("1. Dairy\n2. Sweets\n3. Meat\n4. Fruit\n");
    int categoryChoice;
    scanf("%d", &categoryChoice);
    clearInputBuffer();

    char* category;
    switch(categoryChoice) {
        case 1: category = _strdup(CATEGORY_DAIRY); break;
        case 2: category = _strdup(CATEGORY_SWEETS); break;
        case 3: category = _strdup(CATEGORY_MEAT); break;
        case 4: category = _strdup(CATEGORY_FRUIT); break;
        default:
            printf("Invalid category!\n");
            free(name);
            return;
    }

    Product* product = createProduct(name, category, 0, "2024-01-01"); // Dummy date and quantity for search
    if (product == NULL) {
        printf("Error creating product for deletion!\n");
    } else {
        if (deleteProductService(ui->service, product)) {
            printf("Product deleted successfully!\n");
        } else {
            printf("Product not found!\n");
        }
        destroyProduct(product);
    }

    free(name);
    free(category);
}

void handleUpdateProduct(UserInterface* ui) {
    char* name = readString("Enter product name to update: ");
    if (name == NULL) return;

    printf("Enter product category:\n");
    printf("1. Dairy\n2. Sweets\n3. Meat\n4. Fruit\n");
    int categoryChoice;
    scanf("%d", &categoryChoice);
    clearInputBuffer();

    char* category;
    switch(categoryChoice) {
        case 1: category = _strdup(CATEGORY_DAIRY); break;
        case 2: category = _strdup(CATEGORY_SWEETS); break;
        case 3: category = _strdup(CATEGORY_MEAT); break;
        case 4: category = _strdup(CATEGORY_FRUIT); break;
        default:
            printf("Invalid category!\n");
            free(name);
            return;
    }

    printf("Enter new quantity: ");
    int quantity;
    scanf("%d", &quantity);
    clearInputBuffer();

    char* date = readString("Enter new expiration date (YYYY-MM-DD): ");
    if (date == NULL) {
        free(name);
        free(category);
        return;
    }

    Product* oldProduct = createProduct(name, category, 0, "2024-01-01"); // Dummy values for search
    Product* newProduct = createProduct(name, category, quantity, date);

    if (oldProduct == NULL || newProduct == NULL) {
        printf("Error creating products for update!\n");
    } else {
        if (updateProductService(ui->service, oldProduct, newProduct)) {
            printf("Product updated successfully!\n");
        } else {
            printf("Product not found!\n");
        }
    }

    if (oldProduct) destroyProduct(oldProduct);
    if (newProduct) destroyProduct(newProduct);
    free(name);
    free(category);
    free(date);
}

void handleDisplayFiltered(UserInterface* ui) {
    char* searchString = readString("Enter search string (or press Enter for all products): ");
    if (searchString == NULL) return;

    getProductsBasedOnFilteringService(ui->service, searchString, productContainsString);
    free(searchString);
}

void handleDisplayExpiring(UserInterface* ui) {
    printf("Enter number of days: ");
    int days;
    scanf("%d", &days);
    clearInputBuffer();

    if (days < 0) {
        printf("Number of days must be positive!\n");
        return;
    }

    getProductsExpiringInXDaysService(ui->service, days);
}

void handleUndo(UserInterface* ui) {
    if (undoService(ui->service)) {
        printf("Undo successful!\n");
    } else {
        printf("No operation to undo!\n");
    }
}

void handleRedo(UserInterface* ui) {
    if (redoService(ui->service)) {
        printf("Redo successful!\n");
    } else {
        printf("No operation to redo!\n");
    }
}

void startUserInterface(UserInterface* ui) {
    while (1) {
        displayMenu();
        int choice;
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 0:
                printf("Goodbye!\n");
                return;
            case 1:
                handleAddProduct(ui);
                break;
            case 2:
                handleDeleteProduct(ui);
                break;
            case 3:
                handleUpdateProduct(ui);
                break;
            case 4:
                handleDisplayFiltered(ui);
                break;
            case 5:
                handleDisplayExpiring(ui);
                break;
            case 6:
                handleUndo(ui);
                break;
            case 7:
                handleRedo(ui);
                break;
            default:
                printf("Invalid option! Please try again.\n");
        }
    }
}

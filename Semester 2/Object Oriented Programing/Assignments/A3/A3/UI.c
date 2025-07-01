#define _CRT_SECURE_NO_WARNINGS

#include "controller.h"
#include <stdio.h>
#include <string.h>
#include "UI.h"
#include <ctype.h>


void addPredefinedProductsUI(Controller* controller) {
    addPredefinedProducts(controller);
    printf("10 predefined products have been added.\n");
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getValidIntegerInput(const char* prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            clearInputBuffer();
            return value;
        }
        printf("Invalid input. Please enter a valid integer.\n");
        clearInputBuffer();
    }
}

void getValidStringInput(const char* prompt, char* buffer, int size) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;
            if (strlen(buffer) > 0) return;
        }
        printf("Invalid input. Please enter a valid text.\n");
    }
}

void runUI() {
    Repository* repo = createRepository();
    Controller* controller = createController(repo);
    addPredefinedProducts(controller);

    int choice;
    char name[50], category[50], expiration[20];
    int quantity;

    do {
        printf("\n1. Add Product\n2. Remove Product\n3. Update Product\n4. List Products\n");
        printf("5. Show Expiring Products\n6. Undo\n7. Redo\n8. Exit\n");
        choice = getValidIntegerInput("Choose an option: ");

        switch (choice) {
        case 1:
            getValidStringInput("Enter name: ", name, 50);
            getValidStringInput("Enter category: ", category, 50);
            quantity = getValidIntegerInput("Enter quantity: ");
            getValidStringInput("Enter expiration date (YYYY-MM-DD): ", expiration, 20);
            addProductController(controller, name, category, quantity, expiration);
            break;
        case 2:
            getValidStringInput("Enter name: ", name, 50);
            getValidStringInput("Enter category: ", category, 50);
            removeProductController(controller, name, category);
            break;
        case 3:
            getValidStringInput("Enter name: ", name, 50);
            getValidStringInput("Enter category: ", category, 50);
            quantity = getValidIntegerInput("Enter new quantity: ");
            getValidStringInput("Enter new expiration date: ", expiration, 20);
            updateProductController(controller, name, category, quantity, expiration);
            break;
        case 4:
			Product
        case 5: {
            getValidStringInput("Enter category (or leave empty for all): ", category, 50);
            int days = getValidIntegerInput("Enter number of days until expiration: ");
            int count;
            Product** results = getExpiringProductsController(controller, strlen(category) > 0 ? category : NULL, days, &count);
            printf("Products expiring within %d days:\n", days);
			for (int i = 0; i < count; i++)
				printf("%s %s %d %s\n", results[i]->name, results[i]->category, results[i]->quantity, results[i]->expiration_date);
			break;
        }
        case 6:
            undo(controller);
            printf("Undo performed.\n");
            break;
        case 7:
            redo(controller);
            printf("Redo performed.\n");
            break;
        case 8:
            destroyController(controller);
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid option. Try again.\n");
        }
    } while (choice != 8);
}

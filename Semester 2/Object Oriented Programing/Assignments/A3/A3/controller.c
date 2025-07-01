#include "controller.h"
#include <stdlib.h>

History* createHistory() {
    History* history = (History*)malloc(sizeof(History));
    history->undoIndex = -1;
    history->redoIndex = -1;
    return history;
}

Controller* createController(Repository* repo) {
    Controller* controller = (Controller*)malloc(sizeof(Controller));
    controller->repo = repo;
    controller->history = createHistory();  // Initialize history
    return controller;
}

void destroyController(Controller* controller) {
    destroyRepository(controller->repo);
    free(controller->history);  // Free history
    free(controller);
}

void addProductController(Controller* controller, const char* name, const char* category, int quantity, const char* expiration_date) {
    saveState(controller);  // Save state before modification
    addProduct(controller->repo, createProduct(name, category, quantity, expiration_date));
}


void removeProductController(Controller* controller, const char* name, const char* category) {
    saveState(controller);  // Save state before modification
    removeProduct(controller->repo, name, category);
}

void updateProductController(Controller* controller, const char* name, const char* category, int quantity, const char* expiration_date) {
    saveState(controller);  // Save state before modification
    updateProduct(controller->repo, name, category, quantity, expiration_date);
}

int getAllProductsController(Controller* controller) {
	return getAllProducts(controller->repo);
}

void addPredefinedProducts(Controller* controller) {
    addProductController(controller, "Milk", "Dairy", 10, "2025-05-10");
    addProductController(controller, "Cheese", "Dairy", 5, "2025-06-15");
    addProductController(controller, "Yogurt", "Dairy", 8, "2025-04-20");
    addProductController(controller, "Chocolate", "Sweets", 12, "2025-12-01");
    addProductController(controller, "Cake", "Sweets", 6, "2025-07-30");
    addProductController(controller, "Chicken", "Meat", 7, "2025-03-25");
    addProductController(controller, "Beef", "Meat", 4, "2025-08-14");
    addProductController(controller, "Apple", "Fruit", 20, "2025-09-10");
    addProductController(controller, "Banana", "Fruit", 15, "2025-04-18");
    addProductController(controller, "Orange", "Fruit", 18, "2025-05-25");
}

void saveState(Controller* controller) {
    if (controller->history->undoIndex < 99) {  // Prevent overflow
        controller->history->undoIndex++;
        controller->history->pastStates[controller->history->undoIndex] = copyDynamicArrayOfProducts(controller->repo->products);
        controller->history->redoIndex = controller->history->undoIndex;  // Reset redo index
    }
}


void undo(Controller* controller) {
    if (controller->history->undoIndex <= 0) {
        printf("Nothing to undo.\n");
        return;
    }

    destroyDynamicArray(controller->repo->products);
    controller->repo->products = copyDynamicArrayOfProducts(controller->history->pastStates[--controller->history->undoIndex]);
}


void redo(Controller* controller) {
    if (controller->history->undoIndex >= controller->history->redoIndex) {
        printf("Nothing to redo.\n");
        return;
    }

    destroyDynamicArray(controller->repo->products);
    controller->repo->products = copyDynamicArrayOfProducts(controller->history->pastStates[++controller->history->undoIndex]);
}


Product** getExpiringProductsController(Controller* controller, const char* category, int days, int* resultCount) {
    return getProductsExpiringSoon(controller->repo, category, days, resultCount);
}

#pragma once

#include "repository.h"

typedef struct {
    DynamicArray* pastStates[100];
    int undoIndex;
    int redoIndex;
} History;

typedef struct {
    Repository* repo;
    History* history;
} Controller;

Controller* createController(Repository* repo);
void destroyController(Controller* controller);
void addProductController(Controller* controller, const char* name, const char* category, int quantity, const char* expiration_date);
void removeProductController(Controller* controller, const char* name, const char* category);
void updateProductController(Controller* controller, const char* name, const char* category, int quantity, const char* expiration_date);
int getAllProductsController(Controller* controller);
void addPredefinedProducts(Controller* controller);
Product** getExpiringProductsController(Controller* controller, const char* category, int days, int* resultCount);
void undo(Controller* controller);
void redo(Controller* controller);
void saveState(Controller* controller);
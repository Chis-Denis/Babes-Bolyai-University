#pragma once
#include "services.h"

typedef struct {
    Service* service;
} UserInterface;

UserInterface* createUserInterface(Service*);
void destroyUserInterface(UserInterface*);
void startUserInterface(UserInterface*);

// Menu functions
void displayMenu();
void handleAddProduct(UserInterface*);
void handleDeleteProduct(UserInterface*);
void handleUpdateProduct(UserInterface*);
void handleDisplayFiltered(UserInterface*);
void handleDisplayExpiring(UserInterface*);
void handleUndo(UserInterface*);
void handleRedo(UserInterface*);

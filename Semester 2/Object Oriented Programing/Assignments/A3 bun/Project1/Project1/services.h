#pragma once
#include "repository.h"
#include "undo_redo_repository.h"

typedef struct {
    Repository* repository;
    UndoRedoRepository* undoRedoRepository;
} Service;

Service* createService(Repository* repository, UndoRedoRepository* undoRedoRepository);
void destroyService(Service* service);

int addProductService(Service* service, Product* product);
int deleteProductService(Service* service, Product* product);
int updateProductService(Service* service, Product* oldProduct, Product* newProduct);

// Filter functions
DynamicArray* filterProductsByCategory(Service* service, const char* category);
DynamicArray* filterProductsByQuantity(Service* service, int quantity);
DynamicArray* filterExpiredProducts(Service* service, const char* currentDate);

// Undo/Redo operations
int undoService(Service* service);
int redoService(Service* service);

// Test data generation
void generateRandomEntries(Service* service, int count);

#pragma once
#include "dynamic_array.h"
#include "operation.h"
#include "repository.h"

typedef struct
{
    DynamicArray* undoOperations;
    DynamicArray* redoOperations;
} UndoRedoRepository;

// Function prototypes
UndoRedoRepository* createUndoRedoRepository(int typeOfUndoRedo);
void destroyUndoRedoRepository(UndoRedoRepository*);
void resetRedoOperations(UndoRedoRepository*);
void resetRedoDynamicArray(UndoRedoRepository*);
void addUndoOperation(UndoRedoRepository*, Operation*);
void addRedoOperation(UndoRedoRepository*, Operation*);
void addUndoDynamicArray(UndoRedoRepository*, DynamicArray*);
void addRedoDynamicArray(UndoRedoRepository*, DynamicArray*);
Operation* getUndoOperation(UndoRedoRepository*);
Operation* getRedoOperation(UndoRedoRepository*);
DynamicArray* getUndoDynamicArray(UndoRedoRepository*);
DynamicArray* getRedoDynamicArray(UndoRedoRepository*);
void removeUndoOperation(UndoRedoRepository*);
void removeRedoOperation(UndoRedoRepository*);
DynamicArray* getUndoOperations(UndoRedoRepository*);
DynamicArray* getRedoOperations(UndoRedoRepository*);
void addUndoRedoOperation(UndoRedoRepository*, Repository*, Repository*);
int undoOperation(UndoRedoRepository*, Repository*);
int redoOperation(UndoRedoRepository*, Repository*);

#include "undo_redo_repository.h"
#include "repository.h"
#include <stdlib.h>
#include <string.h>

UndoRedoRepository* createUndoRedoRepository(int typeOfUndoRedo)
{
    UndoRedoRepository* repo = (UndoRedoRepository*)malloc(sizeof(UndoRedoRepository));
    if (repo == NULL) return NULL;

    repo->undoOperations = createDynamicArray(10, (DestroyElementFunctionPointer)destroyRepository);
    repo->redoOperations = createDynamicArray(10, (DestroyElementFunctionPointer)destroyRepository);

    if (repo->undoOperations == NULL || repo->redoOperations == NULL) {
        if (repo->undoOperations) destroyDynamicArray(repo->undoOperations);
        if (repo->redoOperations) destroyDynamicArray(repo->redoOperations);
        free(repo);
        return NULL;
    }

    return repo;
}

void destroyUndoRedoRepository(UndoRedoRepository* repo)
{
    if (repo == NULL) return;
    if (repo->undoOperations) destroyDynamicArray(repo->undoOperations);
    if (repo->redoOperations) destroyDynamicArray(repo->redoOperations);
    free(repo);
}

void resetRedoOperations(UndoRedoRepository* undoRedoRepository)
{
    destroyDynamicArray(undoRedoRepository->redoOperations);
    undoRedoRepository->redoOperations = createDynamicArray(1000, (DestroyElementFunctionPointer)destroyOperation);
}

void resetRedoDynamicArray(UndoRedoRepository* undoRedoRepository)
{
    destroyDynamicArray(undoRedoRepository->redoOperations);
    undoRedoRepository->redoOperations = createDynamicArray(1000, (DestroyElementFunctionPointer)destroyDynamicArray);
}

void addUndoOperation(UndoRedoRepository* undoRedoRepository, Operation* operation)
{
    addElementToDynamicArray(undoRedoRepository->undoOperations, operation);
}

void addRedoOperation(UndoRedoRepository* undoRedoRepository, Operation* operation)
{
    addElementToDynamicArray(undoRedoRepository->redoOperations, operation);
}

void addUndoDynamicArray(UndoRedoRepository* undoRedoRepository, DynamicArray* dynamicArray)
{
    addElementToDynamicArray(undoRedoRepository->undoOperations, dynamicArray);
}

void addRedoDynamicArray(UndoRedoRepository* undoRedoRepository, DynamicArray* dynamicArray)
{
    addElementToDynamicArray(undoRedoRepository->redoOperations, dynamicArray);
}

Operation* getUndoOperation(UndoRedoRepository* undoRedoRepository)
{
    if (getLengthOfDynamicArray(undoRedoRepository->undoOperations) == 0)
        return NULL;

    return (Operation*)getElementOnPosition(undoRedoRepository->undoOperations, getLengthOfDynamicArray(undoRedoRepository->undoOperations) - 1);
}

Operation* getRedoOperation(UndoRedoRepository* undoRedoRepository)
{
    if (getLengthOfDynamicArray(undoRedoRepository->redoOperations) == 0)
        return NULL;

    return (Operation*)getElementOnPosition(undoRedoRepository->redoOperations, getLengthOfDynamicArray(undoRedoRepository->redoOperations) - 1);
}

DynamicArray* getUndoDynamicArray(UndoRedoRepository* undoRedoRepository)
{
    if (getLengthOfDynamicArray(undoRedoRepository->undoOperations) == 0)
        return NULL;

    return (DynamicArray*)getElementOnPosition(undoRedoRepository->undoOperations, getLengthOfDynamicArray(undoRedoRepository->undoOperations) - 1);
}

DynamicArray* getRedoDynamicArray(UndoRedoRepository* undoRedoRepository)
{
    if (getLengthOfDynamicArray(undoRedoRepository->redoOperations) == 0)
        return NULL;

    return (DynamicArray*)getElementOnPosition(undoRedoRepository->redoOperations, getLengthOfDynamicArray(undoRedoRepository->redoOperations) - 1);
}

void removeUndoOperation(UndoRedoRepository* undoRedoRepository)
{
    if (getLengthOfDynamicArray(undoRedoRepository->undoOperations) == 0)
        return;

    deleteElementFromDynamicArray(undoRedoRepository->undoOperations, getLengthOfDynamicArray(undoRedoRepository->undoOperations) - 1);
}

void removeRedoOperation(UndoRedoRepository* undoRedoRepository)
{
    if (getLengthOfDynamicArray(undoRedoRepository->redoOperations) == 0)
        return;

    deleteElementFromDynamicArray(undoRedoRepository->redoOperations, getLengthOfDynamicArray(undoRedoRepository->redoOperations) - 1);
}

DynamicArray* getUndoOperations(UndoRedoRepository* undoRedoRepository)
{
    return undoRedoRepository->undoOperations;
}

DynamicArray* getRedoOperations(UndoRedoRepository* undoRedoRepository)
{
    return undoRedoRepository->redoOperations;
}

void addUndoRedoOperation(UndoRedoRepository* repo, Repository* oldState, Repository* newState) {
    if (repo == NULL || oldState == NULL) return;
    
    // Clear redo stack when a new operation is performed
    if (getLengthOfDynamicArray(repo->redoOperations) > 0) {
        destroyDynamicArray(repo->redoOperations);
        repo->redoOperations = createDynamicArray(10, (DestroyElementFunctionPointer)destroyRepository);
    }
    
    // Add the old state to undo stack
    addElementToDynamicArray(repo->undoOperations, oldState);
}

int undoOperation(UndoRedoRepository* repo, Repository* currentRepo) {
    if (repo == NULL || currentRepo == NULL || 
        getLengthOfDynamicArray(repo->undoOperations) == 0) return 0;

    // Save current state for redo
    Repository* currentState = copyRepository(currentRepo);
    if (currentState == NULL) return 0;
    
    // Get the last undo state
    int lastIndex = getLengthOfDynamicArray(repo->undoOperations) - 1;
    Repository* lastState = (Repository*)getElementOnPosition(repo->undoOperations, lastIndex);
    if (lastState == NULL) {
        destroyRepository(currentState);
        return 0;
    }

    // Add current state to redo stack
    addElementToDynamicArray(repo->redoOperations, currentState);
    
    // Restore the state
    Repository* lastStateCopy = copyRepository(lastState);
    if (lastStateCopy == NULL) {
        return 0;
    }
    
    // Update the current repository with the undo state
    DynamicArray* temp = currentRepo->dynamicArray;
    currentRepo->dynamicArray = lastStateCopy->dynamicArray;
    lastStateCopy->dynamicArray = NULL; // Prevent double free
    destroyRepository(lastStateCopy);
    
    // Remove the used undo state
    deleteElementFromDynamicArray(repo->undoOperations, lastIndex);
    
    return 1;
}

int redoOperation(UndoRedoRepository* repo, Repository* currentRepo) {
    if (repo == NULL || currentRepo == NULL || 
        getLengthOfDynamicArray(repo->redoOperations) == 0) return 0;

    // Save current state for undo
    Repository* currentState = copyRepository(currentRepo);
    if (currentState == NULL) return 0;
    
    // Get the last redo state
    int lastIndex = getLengthOfDynamicArray(repo->redoOperations) - 1;
    Repository* lastState = (Repository*)getElementOnPosition(repo->redoOperations, lastIndex);
    if (lastState == NULL) {
        destroyRepository(currentState);
        return 0;
    }

    // Add current state to undo stack
    addElementToDynamicArray(repo->undoOperations, currentState);
    
    // Restore the state
    Repository* lastStateCopy = copyRepository(lastState);
    if (lastStateCopy == NULL) {
        return 0;
    }
    
    // Update the current repository with the redo state
    DynamicArray* temp = currentRepo->dynamicArray;
    currentRepo->dynamicArray = lastStateCopy->dynamicArray;
    lastStateCopy->dynamicArray = NULL; // Prevent double free
    destroyRepository(lastStateCopy);
    
    // Remove the used redo state
    deleteElementFromDynamicArray(repo->redoOperations, lastIndex);
    
    return 1;
}

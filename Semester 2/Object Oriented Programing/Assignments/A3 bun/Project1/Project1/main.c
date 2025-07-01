#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "user_interface.h"
#include "undo_redo_repository.h"
#include "AllTests.h"

int main()
{
    // Run all tests first
    testAll();
    printf("All tests passed successfully!\n\n");

    // Create the application components
    Repository* repository = createRepository();
    UndoRedoRepository* undoRedoRepository = createUndoRedoRepository(1);  // 1 for operation-based undo/redo
    Service* service = createService(repository, undoRedoRepository, 1);
    
    // Generate initial test data
    generateRandomEntries(service, 10);
    
    // Create and start the UI
    UserInterface* ui = createUserInterface(service);
    if (ui == NULL) {
        printf("Error: Failed to create user interface!\n");
        destroyService(service);
        return 1;
    }
    
    startUserInterface(ui);
    
    // Cleanup
    destroyUserInterface(ui);
    destroyService(service);
    
    // Check for memory leaks
    _CrtDumpMemoryLeaks();
    
    return 0;
}

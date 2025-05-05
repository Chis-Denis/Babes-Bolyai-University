#include "UI/AppUI.h"
#include "Repository/trenchCoatRepository.h"
#include "Controller/trenchCoatController.h"

int main() {
    // Initialize the repository and controller
    TrenchCoatRepository repository;
    TrenchCoatController controller(repository);

    // Call the controller method to add initial data
    controller.addInitialData();

    // Create the main application UI and run it
    AppUI appUI(controller);
    appUI.run();
    
    return 0;
} 
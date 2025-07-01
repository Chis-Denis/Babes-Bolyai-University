#include "gtest/gtest.h" // Use relative path from include dir
#include "../Controller/trenchCoatController.h" // Include controller header (adjust path if needed)
#include "../Repository/trenchCoatRepository.h" // Include repository header
#include <stdexcept>

// Simple test for the controller
TEST(TrenchCoatControllerSimpleTest, AddValid) {
    TrenchCoatRepository repo;
    TrenchCoatController controller(repo);
    ASSERT_EQ(controller.getSize(), 0);
    controller.addTrenchCoat("M", "Beige", 199.99, 10, "http://example.com/1");
    ASSERT_EQ(controller.getSize(), 1);
}

TEST(TrenchCoatControllerSimpleTest, AddDuplicate) {
    TrenchCoatRepository repo;
    TrenchCoatController controller(repo);
    controller.addTrenchCoat("L", "Black", 250.00, 5, "http://example.com/2");
    ASSERT_EQ(controller.getSize(), 1);
    // Expect adding the same size/colour combination to throw
    EXPECT_THROW(controller.addTrenchCoat("L", "Black", 150.00, 1, "http://example.com/3"), std::invalid_argument);
    ASSERT_EQ(controller.getSize(), 1); // Size should remain 1
}

// Add more simple tests here for other controller/repo/domain functions... 
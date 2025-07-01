#include <gtest/gtest.h>
#include "Controller/trenchCoatController.h"
#include "Repository/trenchCoatRepository.h"
#include <stdexcept>

// Test fixture for controller tests
class TrenchCoatControllerTest : public ::testing::Test {
protected:
    TrenchCoatRepository repo;
    TrenchCoatController controller{repo};

    // Pre-populate repo for some tests
    void SetUp() override {
        // Controller uses the same repo instance, start fresh
        repo = TrenchCoatRepository(); // Reset repo
        controller.addTrenchCoat("M", "Beige", 199.99, 10, "http://l1");
        controller.addTrenchCoat("L", "Black", 249.99, 5, "http://l2");
        controller.addTrenchCoat("S", "Navy", 179.50, 8, "http://l3");
        controller.addTrenchCoat("M", "Navy", 189.99, 12, "http://l4");
    }
};

// Test adding a valid trench coat
TEST_F(TrenchCoatControllerTest, AddValidTrenchCoat) {
    ASSERT_EQ(controller.getSize(), 4);
    controller.addTrenchCoat("XL", "Green", 299.0, 2, "http://l5");
    ASSERT_EQ(controller.getSize(), 5);
    EXPECT_EQ(controller.findTrenchCoat("XL", "Green"), 4); // Assuming it adds at the end
}

// Test adding a duplicate trench coat
TEST_F(TrenchCoatControllerTest, AddDuplicateTrenchCoat) {
    ASSERT_EQ(controller.getSize(), 4);
    EXPECT_THROW(controller.addTrenchCoat("M", "Beige", 150.0, 1, "http://lx"), std::invalid_argument);
    ASSERT_EQ(controller.getSize(), 4); // Size should not change
}

// Test adding with invalid data
TEST_F(TrenchCoatControllerTest, AddInvalidData) {
    ASSERT_EQ(controller.getSize(), 4);
    EXPECT_THROW(controller.addTrenchCoat("", "Red", 100, 1, "http://l"), std::invalid_argument); // Empty size
    EXPECT_THROW(controller.addTrenchCoat("S", "", 100, 1, "http://l"), std::invalid_argument); // Empty colour
    EXPECT_THROW(controller.addTrenchCoat("S", "Red", 0, 1, "http://l"), std::invalid_argument); // Zero price
    EXPECT_THROW(controller.addTrenchCoat("S", "Red", -10, 1, "http://l"), std::invalid_argument); // Negative price
    EXPECT_THROW(controller.addTrenchCoat("S", "Red", 100, -1, "http://l"), std::invalid_argument); // Negative quantity
    EXPECT_THROW(controller.addTrenchCoat("S", "Red", 100, 1, ""), std::invalid_argument); // Empty link
    EXPECT_THROW(controller.addTrenchCoat("S", "Red", 100, 1, "photo.jpg"), std::invalid_argument); // Invalid link format
    ASSERT_EQ(controller.getSize(), 4); // Size should not change
}

// Test removing a trench coat
TEST_F(TrenchCoatControllerTest, RemoveTrenchCoat) {
    ASSERT_EQ(controller.getSize(), 4);
    controller.removeTrenchCoat(1); // Remove L Black
    ASSERT_EQ(controller.getSize(), 3);
    EXPECT_EQ(controller.findTrenchCoat("L", "Black"), -1);
    EXPECT_EQ(controller.findTrenchCoat("M", "Beige"), 0); // Check remaining indices
    EXPECT_EQ(controller.findTrenchCoat("S", "Navy"), 1);
    EXPECT_EQ(controller.findTrenchCoat("M", "Navy"), 2);
}

// Test removing trench coat with invalid index
TEST_F(TrenchCoatControllerTest, RemoveInvalidIndex) {
    ASSERT_EQ(controller.getSize(), 4);
    EXPECT_THROW(controller.removeTrenchCoat(4), std::out_of_range);
    EXPECT_THROW(controller.removeTrenchCoat(-1), std::out_of_range);
    ASSERT_EQ(controller.getSize(), 4);
}

// Test updating a trench coat
TEST_F(TrenchCoatControllerTest, UpdateValidTrenchCoat) {
    ASSERT_EQ(controller.getSize(), 4);
    controller.updateTrenchCoat(1, "L", "Black", 299.99, 3, "http://new_link");
    ASSERT_EQ(controller.getSize(), 4);
    TrenchCoat updatedCoat = controller.getTrenchCoat(1);
    EXPECT_EQ(updatedCoat.getPrice(), 299.99);
    EXPECT_EQ(updatedCoat.getQuantity(), 3);
    EXPECT_EQ(updatedCoat.getPhotoLink(), "http://new_link");
}

// Test updating with invalid data
TEST_F(TrenchCoatControllerTest, UpdateInvalidData) {
    ASSERT_EQ(controller.getSize(), 4);
    EXPECT_THROW(controller.updateTrenchCoat(0, "", "Beige", 200, 10, "http://l1"), std::invalid_argument);
    EXPECT_THROW(controller.updateTrenchCoat(0, "M", "", 200, 10, "http://l1"), std::invalid_argument);
    EXPECT_THROW(controller.updateTrenchCoat(0, "M", "Beige", -5, 10, "http://l1"), std::invalid_argument);
    EXPECT_THROW(controller.updateTrenchCoat(0, "M", "Beige", 200, -1, "http://l1"), std::invalid_argument);
    EXPECT_THROW(controller.updateTrenchCoat(0, "M", "Beige", 200, 10, "link"), std::invalid_argument);
    ASSERT_EQ(controller.getTrenchCoat(0).getPrice(), 199.99); // Verify no change
}

// Test updating with invalid index
TEST_F(TrenchCoatControllerTest, UpdateInvalidIndex) {
    ASSERT_EQ(controller.getSize(), 4);
    EXPECT_THROW(controller.updateTrenchCoat(4, "L", "Black", 300, 1, "http://l2"), std::out_of_range);
    EXPECT_THROW(controller.updateTrenchCoat(-1, "L", "Black", 300, 1, "http://l2"), std::out_of_range);
    ASSERT_EQ(controller.getSize(), 4);
}

// Test Get All Trench Coats
TEST_F(TrenchCoatControllerTest, GetAllTrenchCoats) {
    DynamicVector<TrenchCoat> coats = controller.getAllTrenchCoats();
    ASSERT_EQ(coats.getSize(), 4);
    EXPECT_EQ(coats[0].getSize(), "M");
    EXPECT_EQ(coats[1].getSize(), "L");
    // Add more checks if needed
}

// Test Get Trench Coats By Size
TEST_F(TrenchCoatControllerTest, GetTrenchCoatsBySize) {
    // Specific size 'M'
    DynamicVector<TrenchCoat> m_coats = controller.getTrenchCoatsBySize("M");
    ASSERT_EQ(m_coats.getSize(), 2);
    EXPECT_EQ(m_coats[0].getColour(), "Beige");
    EXPECT_EQ(m_coats[1].getColour(), "Navy");

    // Specific size 'L'
    DynamicVector<TrenchCoat> l_coats = controller.getTrenchCoatsBySize("L");
    ASSERT_EQ(l_coats.getSize(), 1);
    EXPECT_EQ(l_coats[0].getColour(), "Black");

    // Specific size 'XL' (none exist)
    DynamicVector<TrenchCoat> xl_coats = controller.getTrenchCoatsBySize("XL");
    ASSERT_EQ(xl_coats.getSize(), 0);

    // Empty size (should return all)
    DynamicVector<TrenchCoat> all_coats = controller.getTrenchCoatsBySize("");
    ASSERT_EQ(all_coats.getSize(), 4);
}

// Test Get Trench Coats Cheaper Than
TEST_F(TrenchCoatControllerTest, GetTrenchCoatsCheaperThan) {
    DynamicVector<TrenchCoat> cheaper = controller.getTrenchCoatsCheaperThan(200.0);
    ASSERT_EQ(cheaper.getSize(), 3);
    // Check colours or sizes to be sure
    EXPECT_EQ(cheaper[0].getColour(), "Beige"); // Price 199.99
    EXPECT_EQ(cheaper[1].getColour(), "Navy"); // Price 179.50
    EXPECT_EQ(cheaper[2].getColour(), "Navy"); // Price 189.99

    DynamicVector<TrenchCoat> cheaper_none = controller.getTrenchCoatsCheaperThan(100.0);
    ASSERT_EQ(cheaper_none.getSize(), 0);

    DynamicVector<TrenchCoat> cheaper_all = controller.getTrenchCoatsCheaperThan(300.0);
    ASSERT_EQ(cheaper_all.getSize(), 4);
}

// Test Get Trench Coat by Index
TEST_F(TrenchCoatControllerTest, GetTrenchCoat) {
    TrenchCoat coat = controller.getTrenchCoat(1);
    EXPECT_EQ(coat.getSize(), "L");
    EXPECT_EQ(coat.getColour(), "Black");

    EXPECT_THROW(controller.getTrenchCoat(4), std::out_of_range);
    EXPECT_THROW(controller.getTrenchCoat(-1), std::out_of_range);
}

// Test Get Size
TEST_F(TrenchCoatControllerTest, GetSize) {
    EXPECT_EQ(controller.getSize(), 4);
    controller.addTrenchCoat("XL", "Red", 100, 1, "http://l5");
    EXPECT_EQ(controller.getSize(), 5);
    controller.removeTrenchCoat(0);
    EXPECT_EQ(controller.getSize(), 4);
}

// Test Find Trench Coat
TEST_F(TrenchCoatControllerTest, FindTrenchCoat) {
    EXPECT_EQ(controller.findTrenchCoat("M", "Beige"), 0);
    EXPECT_EQ(controller.findTrenchCoat("S", "Navy"), 2);
    EXPECT_EQ(controller.findTrenchCoat("XL", "Green"), -1);
}

// Test Add Initial Data (check size mainly)
TEST(TrenchCoatControllerInitTest, AddInitialData) {
    TrenchCoatRepository repo; // Separate repo for this test
    TrenchCoatController controller(repo);
    EXPECT_EQ(controller.getSize(), 0);
    controller.addInitialData();
    EXPECT_GE(controller.getSize(), 10); // Check if at least 10 were added
    // Add more specific checks if the exact initial items are critical
} 
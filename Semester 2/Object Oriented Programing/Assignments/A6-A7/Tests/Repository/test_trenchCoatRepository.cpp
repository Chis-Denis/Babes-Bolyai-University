#include <gtest/gtest.h>
#include "Repository/trenchCoatRepository.h"
#include "Domain/trench.h"
#include <stdexcept>

// Test fixture to reset repository before each test
class TrenchCoatRepositoryTest : public ::testing::Test {
protected:
    TrenchCoatRepository repo;
    TrenchCoat coat1 = TrenchCoat("M", "Beige", 199.99, 10, "link1");
    TrenchCoat coat2 = TrenchCoat("L", "Black", 249.99, 5, "link2");
    TrenchCoat coat3 = TrenchCoat("M", "Navy", 179.50, 8, "link3");

    void SetUp() override {
        // Repo is already default constructed (empty)
        // Or clear it if necessary: repo = TrenchCoatRepository();
    }
};

// Test adding coats
TEST_F(TrenchCoatRepositoryTest, AddTrenchCoat) {
    EXPECT_EQ(repo.getSize(), 0);
    repo.addTrenchCoat(coat1);
    EXPECT_EQ(repo.getSize(), 1);
    repo.addTrenchCoat(coat2);
    EXPECT_EQ(repo.getSize(), 2);

    EXPECT_EQ(repo.getTrenchCoat(0).getColour(), "Beige");
    EXPECT_EQ(repo.getTrenchCoat(1).getColour(), "Black");
}

// Test finding coats
TEST_F(TrenchCoatRepositoryTest, FindTrenchCoat) {
    repo.addTrenchCoat(coat1); // M, Beige
    repo.addTrenchCoat(coat2); // L, Black
    repo.addTrenchCoat(coat3); // M, Navy

    EXPECT_EQ(repo.findTrenchCoat("M", "Beige"), 0);
    EXPECT_EQ(repo.findTrenchCoat("L", "Black"), 1);
    EXPECT_EQ(repo.findTrenchCoat("M", "Navy"), 2);
    EXPECT_EQ(repo.findTrenchCoat("S", "Green"), -1); // Not found
    EXPECT_EQ(repo.findTrenchCoat("M", "Black"), -1); // Not found
}

// Test removing coats
TEST_F(TrenchCoatRepositoryTest, RemoveTrenchCoat) {
    repo.addTrenchCoat(coat1); // M, Beige [0]
    repo.addTrenchCoat(coat2); // L, Black [1]
    repo.addTrenchCoat(coat3); // M, Navy  [2]
    ASSERT_EQ(repo.getSize(), 3);

    // Remove middle
    repo.removeTrenchCoat(1);
    ASSERT_EQ(repo.getSize(), 2);
    EXPECT_EQ(repo.getTrenchCoat(0).getColour(), "Beige");
    EXPECT_EQ(repo.getTrenchCoat(1).getColour(), "Navy");
    EXPECT_EQ(repo.findTrenchCoat("L", "Black"), -1); // Verify removed
    EXPECT_EQ(repo.findTrenchCoat("M", "Navy"), 1); // Verify index shifted

    // Remove first
    repo.removeTrenchCoat(0);
    ASSERT_EQ(repo.getSize(), 1);
    EXPECT_EQ(repo.getTrenchCoat(0).getColour(), "Navy");
    EXPECT_EQ(repo.findTrenchCoat("M", "Beige"), -1);

    // Remove last
    repo.removeTrenchCoat(0);
    ASSERT_EQ(repo.getSize(), 0);
    EXPECT_EQ(repo.findTrenchCoat("M", "Navy"), -1);

    // Remove out of bounds
    ASSERT_THROW(repo.removeTrenchCoat(0), std::out_of_range);
    repo.addTrenchCoat(coat1);
    ASSERT_THROW(repo.removeTrenchCoat(1), std::out_of_range);
    ASSERT_THROW(repo.removeTrenchCoat(-1), std::out_of_range);
}

// Test updating coats
TEST_F(TrenchCoatRepositoryTest, UpdateTrenchCoat) {
    repo.addTrenchCoat(coat1); // M, Beige [0]
    repo.addTrenchCoat(coat2); // L, Black [1]
    ASSERT_EQ(repo.getSize(), 2);

    TrenchCoat updatedCoat = TrenchCoat("M", "Beige", 210.0, 15, "new_link");
    repo.updateTrenchCoat(0, updatedCoat);
    ASSERT_EQ(repo.getSize(), 2);
    TrenchCoat retrieved = repo.getTrenchCoat(0);
    EXPECT_EQ(retrieved.getPrice(), 210.0);
    EXPECT_EQ(retrieved.getQuantity(), 15);
    EXPECT_EQ(retrieved.getPhotoLink(), "new_link");
    EXPECT_EQ(retrieved.getSize(), "M"); // Ensure other fields didn't change
    EXPECT_EQ(retrieved.getColour(), "Beige");

    // Ensure second coat wasn't affected
    EXPECT_EQ(repo.getTrenchCoat(1).getColour(), "Black");

    // Update out of bounds
    TrenchCoat invalidUpdate("S", "Red", 1, 1, "link");
    ASSERT_THROW(repo.updateTrenchCoat(2, invalidUpdate), std::out_of_range);
    ASSERT_THROW(repo.updateTrenchCoat(-1, invalidUpdate), std::out_of_range);
}

// Test getting all coats
TEST_F(TrenchCoatRepositoryTest, GetAllTrenchCoats) {
    DynamicVector<TrenchCoat> coats = repo.getAllTrenchCoats();
    EXPECT_EQ(coats.getSize(), 0);

    repo.addTrenchCoat(coat1);
    repo.addTrenchCoat(coat2);
    coats = repo.getAllTrenchCoats();
    ASSERT_EQ(coats.getSize(), 2);
    EXPECT_EQ(coats[0].getColour(), "Beige");
    EXPECT_EQ(coats[1].getColour(), "Black");
}

// Test getting coat by index
TEST_F(TrenchCoatRepositoryTest, GetTrenchCoat) {
    repo.addTrenchCoat(coat1);
    repo.addTrenchCoat(coat2);

    EXPECT_EQ(repo.getTrenchCoat(0).getColour(), "Beige");
    EXPECT_EQ(repo.getTrenchCoat(1).getColour(), "Black");

    // Get out of bounds
    ASSERT_THROW(repo.getTrenchCoat(2), std::out_of_range);
    ASSERT_THROW(repo.getTrenchCoat(-1), std::out_of_range);

    // Test const version
    const TrenchCoatRepository& const_repo = repo;
    EXPECT_EQ(const_repo.getTrenchCoat(0).getColour(), "Beige");
    ASSERT_THROW(const_repo.getTrenchCoat(2), std::out_of_range);
}

// Test get size
TEST_F(TrenchCoatRepositoryTest, GetSize) {
    EXPECT_EQ(repo.getSize(), 0);
    repo.addTrenchCoat(coat1);
    EXPECT_EQ(repo.getSize(), 1);
    repo.addTrenchCoat(coat2);
    EXPECT_EQ(repo.getSize(), 2);
    repo.removeTrenchCoat(0);
    EXPECT_EQ(repo.getSize(), 1);
} 
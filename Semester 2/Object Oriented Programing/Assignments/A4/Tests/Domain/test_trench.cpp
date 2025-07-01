#include <gtest/gtest.h>
#include "Domain/trench.h"

// Test constructor and getters
TEST(TrenchCoatTest, ConstructorAndGetters) {
    TrenchCoat coat("M", "Beige", 199.99, 10, "http://example.com/photo.jpg");

    EXPECT_EQ(coat.getSize(), "M");
    EXPECT_EQ(coat.getColour(), "Beige");
    EXPECT_DOUBLE_EQ(coat.getPrice(), 199.99);
    EXPECT_EQ(coat.getQuantity(), 10);
    EXPECT_EQ(coat.getPhotoLink(), "http://example.com/photo.jpg");
}

// Test setters
TEST(TrenchCoatTest, Setters) {
    TrenchCoat coat("S", "Black", 100.0, 5, "link1");

    coat.setSize("L");
    EXPECT_EQ(coat.getSize(), "L");

    coat.setColour("Navy");
    EXPECT_EQ(coat.getColour(), "Navy");

    coat.setPrice(250.50);
    EXPECT_DOUBLE_EQ(coat.getPrice(), 250.50);

    coat.setQuantity(15);
    EXPECT_EQ(coat.getQuantity(), 15);

    coat.setPhotoLink("link2");
    EXPECT_EQ(coat.getPhotoLink(), "link2");
}

// Test equality operator if implemented
TEST(TrenchCoatTest, EqualityOperator) {
    TrenchCoat coat1("M", "Beige", 199.99, 10, "link");
    TrenchCoat coat2("M", "Beige", 199.99, 10, "link");
    TrenchCoat coat3("L", "Beige", 199.99, 10, "link");
    TrenchCoat coat4("M", "Black", 199.99, 10, "link");

    EXPECT_TRUE(coat1 == coat2); // Assuming operator== is defined
    EXPECT_FALSE(coat1 == coat3);
    EXPECT_FALSE(coat1 == coat4);
}

// Add tests for stream operator if implemented 
#include <gtest/gtest.h>
#include "Domain/dynamicVector.h"
#include <string>
#include <stdexcept> // For std::out_of_range

// Test fixture for setup if needed, otherwise just use TEST

// Test default constructor
TEST(DynamicVectorTest, DefaultConstructor) {
    DynamicVector<int> vec;
    EXPECT_EQ(vec.getSize(), 0);
    // Assuming a getCapacity method exists
    // EXPECT_EQ(vec.getCapacity(), initial_capacity); // Replace initial_capacity
    EXPECT_TRUE(vec.isEmpty());
}

// Test constructor with capacity
TEST(DynamicVectorTest, ConstructorWithCapacity) {
    DynamicVector<double> vec(20);
    EXPECT_EQ(vec.getSize(), 0);
    EXPECT_GE(vec.getCapacity(), 20); // Capacity might be rounded up
    EXPECT_TRUE(vec.isEmpty());
}

// Test copy constructor
TEST(DynamicVectorTest, CopyConstructor) {
    DynamicVector<std::string> vec1;
    vec1.add("hello");
    vec1.add("world");

    DynamicVector<std::string> vec2 = vec1; // Use copy constructor
    EXPECT_EQ(vec2.getSize(), 2);
    ASSERT_EQ(vec1.getSize(), 2);
    EXPECT_EQ(vec2[0], "hello");
    EXPECT_EQ(vec2[1], "world");
}

// Test assignment operator
TEST(DynamicVectorTest, AssignmentOperator) {
    DynamicVector<int> vec1;
    vec1.add(1); vec1.add(2);
    DynamicVector<int> vec2;
    vec2.add(10); vec2.add(20); vec2.add(30);

    vec2 = vec1; // Use assignment operator
    EXPECT_EQ(vec2.getSize(), 2);
    ASSERT_EQ(vec1.getSize(), 2);
    EXPECT_EQ(vec2[0], 1);
    EXPECT_EQ(vec2[1], 2);

    // Test self-assignment
    vec1 = vec1;
    EXPECT_EQ(vec1.getSize(), 2);
    EXPECT_EQ(vec1[0], 1);
    EXPECT_EQ(vec1[1], 2);
}

// Test add and resize
TEST(DynamicVectorTest, AddAndResize) {
    DynamicVector<int> vec(2); // Start with small capacity
    EXPECT_EQ(vec.getCapacity(), 2);
    vec.add(10);
    vec.add(20);
    EXPECT_EQ(vec.getSize(), 2);
    EXPECT_EQ(vec.getCapacity(), 2);

    vec.add(30); // Should trigger resize
    EXPECT_EQ(vec.getSize(), 3);
    EXPECT_GT(vec.getCapacity(), 2); // Capacity should have increased
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);

    // Add more elements
    for(int i = 0; i < 10; ++i) {
        vec.add(100 + i);
    }
    EXPECT_EQ(vec.getSize(), 13);
    EXPECT_EQ(vec[12], 109);
}

// Test remove
TEST(DynamicVectorTest, Remove) {
    DynamicVector<std::string> vec;
    vec.add("A"); vec.add("B"); vec.add("C"); vec.add("D");
    ASSERT_EQ(vec.getSize(), 4);

    vec.remove(1); // Remove "B"
    EXPECT_EQ(vec.getSize(), 3);
    EXPECT_EQ(vec[0], "A");
    EXPECT_EQ(vec[1], "C");
    EXPECT_EQ(vec[2], "D");

    vec.remove(2); // Remove "D"
    EXPECT_EQ(vec.getSize(), 2);
    EXPECT_EQ(vec[0], "A");
    EXPECT_EQ(vec[1], "C");

    vec.remove(0); // Remove "A"
    EXPECT_EQ(vec.getSize(), 1);
    EXPECT_EQ(vec[0], "C");

    vec.remove(0); // Remove "C"
    EXPECT_EQ(vec.getSize(), 0);
    EXPECT_TRUE(vec.isEmpty());

    // Test remove out of bounds
    ASSERT_THROW(vec.remove(0), std::out_of_range);
    vec.add("E");
    ASSERT_THROW(vec.remove(1), std::out_of_range);
    ASSERT_THROW(vec.remove(-1), std::out_of_range);
}

// Test operator[] and get
TEST(DynamicVectorTest, AccessOperators) {
    DynamicVector<char> vec;
    vec.add('x'); vec.add('y'); vec.add('z');

    // Test non-const access
    EXPECT_EQ(vec[0], 'x');
    EXPECT_EQ(vec[1], 'y');
    EXPECT_EQ(vec[2], 'z');
    EXPECT_EQ(vec.get(1), 'y');

    vec[1] = 'Y'; // Modify using operator[]
    EXPECT_EQ(vec[1], 'Y');
    EXPECT_EQ(vec.get(1), 'Y');

    // Test const access
    const DynamicVector<char>& const_vec = vec;
    EXPECT_EQ(const_vec[0], 'x');
    EXPECT_EQ(const_vec.get(1), 'Y');
    // const_vec[0] = 'X'; // This should not compile

    // Test out of bounds access
    EXPECT_THROW(vec[3], std::out_of_range);
    EXPECT_THROW(vec[-1], std::out_of_range);
    EXPECT_THROW(vec.get(3), std::out_of_range);
    EXPECT_THROW(vec.get(-1), std::out_of_range);
    EXPECT_THROW(const_vec[3], std::out_of_range);
    EXPECT_THROW(const_vec.get(3), std::out_of_range);

    DynamicVector<int> empty_vec;
    EXPECT_THROW(empty_vec[0], std::out_of_range);
    EXPECT_THROW(empty_vec.get(0), std::out_of_range);
}

// Test clear if implemented
TEST(DynamicVectorTest, Clear) {
    DynamicVector<int> vec;
    vec.add(1); vec.add(2);
    ASSERT_EQ(vec.getSize(), 2);
    vec.clear(); // Assuming clear() exists
    EXPECT_EQ(vec.getSize(), 0);
    EXPECT_TRUE(vec.isEmpty());
    // Optionally check capacity remains or resets
}

// Add more tests for other methods like isEmpty, getCapacity etc. 
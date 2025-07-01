cmake_minimum_required(VERSION 3.14)
project(ProperTrenchCoatsTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Fetch Google Test ---
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- Include Source Directories ---
# Allows tests to find headers like "Domain/trench.h"
include_directories(${CMAKE_SOURCE_DIR}/..)

# --- Compiler Flags for Coverage (GCC/Clang) ---
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Add coverage flags, typically for Debug builds
    target_compile_options(ProperTrenchCoatsTests INTERFACE
        $<$<CONFIG:Debug>:--coverage>
    )
    target_link_options(ProperTrenchCoatsTests INTERFACE
        $<$<CONFIG:Debug>:--coverage>
    )
    message(STATUS "Coverage flags enabled for GCC/Clang Debug build.")
endif()

# --- Define Source Files ---
# Add ALL .cpp files from Domain, Repository, Controller needed for linking
set(LIB_SOURCES
    ../Domain/trench.cpp
    ../Repository/trenchCoatRepository.cpp
    ../Controller/trenchCoatController.cpp
    # Add dynamicVector.cpp if you separate its implementation
)

# --- Define Test Files ---
# Add all your test_*.cpp files here
set(TEST_SOURCES
    test_main.cpp
    Domain/test_dynamicVector.cpp
    # Domain/test_trench.cpp # Assuming you might have this
    Repository/test_trenchCoatRepository.cpp # Assuming you might have this
    Controller/test_trenchCoatController.cpp
)

# --- Create Test Executable ---
add_executable(runTests ${TEST_SOURCES} ${LIB_SOURCES})

# --- Link Google Test ---
# Link against the targets created by FetchContent_MakeAvailable
target_link_libraries(runTests PRIVATE GTest::gtest GTest::gtest_main)

# --- Enable Test Discovery with CTest ---
include(GoogleTest)
gtest_discover_tests(runTests) 
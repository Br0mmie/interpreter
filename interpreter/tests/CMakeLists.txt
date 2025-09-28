cmake_minimum_required(VERSION 3.10)

# Find Google Test
find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

# Include directories
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})

# Add executable for tests
add_executable(ExecutorTest ExecutorTest.cpp ../src/Executor.cpp)

# Link Google Test and Google Mock
target_link_libraries(ExecutorTest ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES} pthread)

# Enable testing
enable_testing()
add_test(NAME ExecutorTests COMMAND ExecutorTest)
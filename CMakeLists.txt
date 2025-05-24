cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(delegate)

enable_testing()

# Find GoogleTest
find_package(GTest REQUIRED)

# Add your test executable
add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main)

add_test(NAME RunTests COMMAND tests)
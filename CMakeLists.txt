cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(delegate)

add_library(${PROJECT_NAME} INTERFACE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

# Export include directories
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

enable_testing()

# Find GoogleTest
find_package(GTest REQUIRED)

# Add your test executable
add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main)

add_test(NAME RunTests COMMAND tests)

cmake_minimum_required(VERSION 3.14)

project(delegate)

# Create the INTERFACE library for the header-only project
add_library(${PROJECT_NAME} INTERFACE)

# Set C++20 as the required standard
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

# Expose include directories with BUILD_INTERFACE and INSTALL_INTERFACE
target_include_directories(${PROJECT_NAME} INTERFACE
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)

# Install the header file
install(FILES ${PROJECT_SOURCE_DIR}/delegate.hpp DESTINATION include)

# Install the library and export configuration
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets)
install(EXPORT ${PROJECT_NAME}Targets
	FILE ${PROJECT_NAME}Config.cmake
	DESTINATION lib/cmake/${PROJECT_NAME}
)

enable_testing()

# Find Google Test
find_package(GTest REQUIRED)

# Create test executable and link to the main library and GTest
add_executable(${PROJECT_NAME}-tests tests/tests.cpp)
target_link_libraries(${PROJECT_NAME}-tests PRIVATE
	GTest::GTest
	GTest::Main
	${PROJECT_NAME}
)

# Register the test with CTest
add_test(NAME RunTests COMMAND $<TARGET_FILE:${PROJECT_NAME}-tests>)
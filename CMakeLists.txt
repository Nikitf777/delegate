cmake_minimum_required(VERSION 3.14)

project(delegate)

add_library(${PROJECT_NAME} INTERFACE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

target_include_directories(${PROJECT_NAME} INTERFACE
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)

install(FILES ${PROJECT_SOURCE_DIR}/delegate.hpp DESTINATION include)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets)

install(EXPORT ${PROJECT_NAME}Targets
	FILE ${PROJECT_NAME}Config.cmake
	DESTINATION lib/cmake/${PROJECT_NAME}
)

enable_testing()

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}-tests tests.cpp)
target_link_libraries(${PROJECT_NAME}-tests PRIVATE GTest::GTest GTest::Main)

add_test(NAME RunTests COMMAND tests)

cmake_minimum_required(VERSION 3.14)

project(delegate)

add_library(${PROJECT_NAME} INTERFACE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

target_include_directories(delegate INTERFACE
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)

install(FILES ${PROJECT_SOURCE_DIR}/delegate.hpp DESTINATION include)

install(TARGETS delegate EXPORT delegateTargets)

install(EXPORT delegateTargets
	FILE delegateConfig.cmake
	DESTINATION lib/cmake/delegate
)

enable_testing()

find_package(GTest REQUIRED)

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main)

add_test(NAME RunTests COMMAND tests)

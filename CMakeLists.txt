# Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.20)

# Set the project name and version
project(rangex_test VERSION 0.3.1 LANGUAGES CXX)

# Specify which compiler will be Use
#set(CMAKE_C_COMPILER "/usr/bin/gcc")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++")
#set(CMAKE_C_COMPILER "/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

set(USE_CPP_STD 23)
message(STATUS "Using C++${USE_CPP_STD}")
# Specify the C++ standard
set(CMAKE_CXX_STANDARD ${USE_CPP_STD})
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS Off)  # Disable compiler-specific extensions

#option(USE_DOC_TEST "Use DocTest framework" ON)
option(USE_GOOGLE_TEST "Use Google Test framework" ON)

if (USE_GOOGLE_TEST)

message(STATUS "Use Google Test framework")
add_compile_definitions(USE_GOOGLE_TEST=1)

# Add GoogleTest
set(GOOGLETEST_PATH ${CMAKE_SOURCE_DIR}/external/googletest)
#add_subdirectory(${GOOGLETEST_PATH})
add_subdirectory(${CMAKE_SOURCE_DIR}/external/googletest ${CMAKE_BINARY_DIR}/googletest)

# Enable CTest for running tests
enable_testing()
add_test(NAME rangex_test COMMAND rangex_test)

endif()

if (USE_DOC_TEST)

message(STATUS "Use DocTest framework")
add_compile_definitions(USE_DOC_TEST=1)
add_compile_definitions(DOCTEST_USE_STD_HEADERS=1)

set(DOCTEST_USE_STD_HEADERS 1)
# Add DocTest
set(DOCTEST_PATH ${CMAKE_SOURCE_DIR}/external/doctest)
add_subdirectory(${DOCTEST_PATH})

find_package(DocTest REQUIRED 2.4.11)

endif()

####################################

# Specify the source files
set(SOURCES
    src/main.cpp
    src/main.compatible.cpp
)
# Create the executable target
add_executable(rangex_test ${SOURCES})
target_include_directories(rangex_test PUBLIC
    ${PROJECT_SOURCE_DIR}/external/googletest/googletest/include
    ${PROJECT_SOURCE_DIR}/external/doctest/doctest
    ${PROJECT_SOURCE_DIR}/src/lib/include
    ${PROJECT_SOURCE_DIR}
)

# Optionally, you can add any required libraries
if (USE_DOC_TEST)
target_link_libraries(rangex_test PRIVATE doctest)
endif()

if (USE_GOOGLE_TEST)
set(GTest_LINK_ENTRIES
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)
target_link_libraries(rangex_test PRIVATE ${GTest_LINK_ENTRIES})
endif()

set(EXAMPLE_SOURCES
    examples/rangex_demo.cpp
)
add_executable(rangex_demo ${EXAMPLE_SOURCES})
target_include_directories(rangex_demo PUBLIC
    ${PROJECT_SOURCE_DIR}/src/lib/include
    ${PROJECT_SOURCE_DIR}
)
target_link_libraries(rangex_demo PRIVATE)

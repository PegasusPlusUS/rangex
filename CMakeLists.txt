#C++11
#C++14
#C++17
#C++20
#C++23
#
# Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.30)

# Set the project name and version
project(rangex_demo VERSION 0.3 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS Off)  # Disable compiler-specific extensions

add_subdirectory(external/doctest)

set(DocTest_DIR
    "external/doctest/"
)
##find_package(DocTest REQUIRED 2.4.11)

####################################

set(RANGEX_LIB_SOURCES
    src/lib/rangex_lib.cpp
)
add_library(rangex_lib STATIC ${RANGEX_LIB_SOURCES})
# Optionally add include directories for the library
target_include_directories(rangex_lib PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/lib/include
)

# Specify the source files
set(SOURCES
    src/main.cpp
)

# Create the executable target
add_executable(rangex_demo ${SOURCES})
# Optionally, you can add any required libraries
target_link_libraries(rangex_demo PRIVATE rangex_lib doctest)

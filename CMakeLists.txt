cmake_minimum_required(VERSION 3.22)
project(pythia VERSION 0.0.1 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files excluding main.cpp
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Create an object library from the source files
add_library(pythiaObj OBJECT ${SOURCES})

# Find and link nlohmann_json library
find_package(nlohmann_json 3.11.3 REQUIRED)
target_link_libraries(pythiaObj PRIVATE nlohmann_json::nlohmann_json)

# Create the main executable with main.cpp
add_executable(pythia src/main.cpp)
target_link_libraries(pythia PRIVATE pythiaObj)

# Install a pkg-config file.
# CONFIGURE_FILE(
#     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/pkg-config.pc.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
# )

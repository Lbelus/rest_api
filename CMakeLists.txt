cmake_minimum_required(VERSION 3.22)
project(rest_api VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(/usr/include/mysql)
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")

find_package(Threads REQUIRED)
find_package(nlohmann_json 3.11.3 REQUIRED)
find_library(MYSQLPP_LIB mysqlpp REQUIRED)

add_library(rest_apiObj OBJECT ${SOURCES})
target_link_libraries(rest_apiObj PRIVATE nlohmann_json::nlohmann_json)

add_executable(rest_api src/main.cpp)

target_link_libraries(rest_api
    PRIVATE
    rest_apiObj
    ${MYSQLPP_LIB}
    Threads::Threads
)

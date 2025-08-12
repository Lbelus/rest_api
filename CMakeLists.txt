cmake_minimum_required(VERSION 3.22)
project(rest_api VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE SRC_DIRS LIST_DIRECTORIES true "${SRC_DIR}/*")
list(FILTER SRC_DIRS EXCLUDE REGEX "\\.[^.]+$")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    /usr/include/mysql
    ${SRC_DIRS}
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_library(rest_apiObj OBJECT ${SOURCES})

find_package(Threads REQUIRED)

find_package(nlohmann_json 3.11.3 REQUIRED)
target_link_libraries(rest_apiObj PRIVATE nlohmann_json::nlohmann_json)

find_library(MYSQLPP_LIB mysqlpp REQUIRED)
find_library(MYSQLCLIENT_LIB mysqlclient REQUIRED)

# Find and link hiredis library
find_path(HIREDIS_HEADER hiredis)
target_include_directories(rest_apiObj PUBLIC ${HIREDIS_HEADER})
find_library(HIREDIS_LIB hiredis)
target_link_libraries(rest_apiObj PRIVATE ${HIREDIS_LIB})

# Find and link redis-plus-plus library
find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories(rest_apiObj PUBLIC ${REDIS_PLUS_PLUS_HEADER})
find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries(rest_apiObj PRIVATE ${REDIS_PLUS_PLUS_LIB})

add_executable(rest_api src/main.cpp)
target_link_libraries(rest_api
    PRIVATE
    rest_apiObj
    ${MYSQLPP_LIB}
    ${MYSQLCLIENT_LIB}
    Threads::Threads
)

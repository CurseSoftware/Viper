cmake_minimum_required(VERSION 3.10)

project(testing VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources for the base tool-chain
file(GLOB_RECURSE TESTING_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cc"
)

file(GLOB_RECURSE TESTING_HEADERS 
    "${PROJECT_SOURCE_DIR}/include/*.h"
)

set(SOURCE_INCLUDE "${PROJECT_SOURCE_DIR}/include")

add_library("${PROJECT_NAME}" STATIC "${TESTING_SOURCES}")

target_include_directories("${PROJECT_NAME}" PRIVATE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}>
)

# Public header export
target_include_directories("${PROJECT_NAME}" PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries("${PROJECT_NAME}" PUBLIC common)
target_link_libraries("${PROJECT_NAME}" PUBLIC diagnostics)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

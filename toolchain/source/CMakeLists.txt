cmake_minimum_required(VERSION 3.10)

project(source VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources for the base tool-chain
file(GLOB_RECURSE SOURCE_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cc"
)

file(GLOB_RECURSE SOURCE_HEADERS 
    "${PROJECT_SOURCE_DIR}/include/*.h"
)

set(SOURCE_INCLUDE "${PROJECT_SOURCE_DIR}/include")

message("Source Sources: ${SOURCE_SOURCES}")
message("Source Headers: ${SOURCE_HEADERS}")
message("Source Include: ${SOURCE_INCLUDE}")

add_library("${PROJECT_NAME}" STATIC "${SOURCE_SOURCES}")

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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

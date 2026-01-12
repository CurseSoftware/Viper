cmake_minimum_required(VERSION 3.10)

project(lex VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources for the base tool-chain
file(GLOB_RECURSE LEX_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cc"
    "${PROJECT_SOURCE_DIR}/src/tests/*.cc"
)

file(GLOB_RECURSE LEX_HEADERS 
    "${PROJECT_SOURCE_DIR}/include/*.h"
)

set(LEX_INCLUDE "${PROJECT_SOURCE_DIR}/include")

message("Lex Sources: ${LEX_SOURCES}")
message("Lex Headers: ${LEX_HEADERS}")
message("Lex Include: ${LEX_INCLUDE}")

add_library("${PROJECT_NAME}" STATIC "${LEX_SOURCES}")

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
target_link_libraries("${PROJECT_NAME}" PUBLIC source)
target_link_libraries("${PROJECT_NAME}" PUBLIC base)
target_link_libraries("${PROJECT_NAME}" PUBLIC diagnostics)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

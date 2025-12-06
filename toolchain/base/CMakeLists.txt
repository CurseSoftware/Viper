cmake_minimum_required(VERSION 3.10)

project(base VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources for the base tool-chain
file(GLOB_RECURSE LEX_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cc"
)

file(GLOB_RECURSE LEX_HEADERS 
    "${PROJECT_SOURCE_DIR}/include/*.h"
)

set(LEX_INCLUDE "${PROJECT_SOURCE_DIR}/include")


message("Lex Sources: ${LEX_SOURCES}")
message("Lex Headers: ${LEX_HEADERS}")
message("Lex Include: ${LEX_INCLUDE}")

add_library("${PROJECT_NAME}" STATIC "${TOOLCHAIN_SOURCES}")

target_include_directories("${PROJECT_NAME}" PUBLIC 
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    # "${LEX_INCLUDE}"
    # Include any external paths
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

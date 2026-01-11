cmake_minimum_required(VERSION 3.10)

project(driver VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources for the base tool-chain
file(GLOB_RECURSE DRIVER_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cc"
)

file(GLOB_RECURSE DRIVER_HEADERS 
    "${PROJECT_SOURCE_DIR}/include/*.h"
)

set(DRIVER_INCLUDE "${PROJECT_SOURCE_DIR}/include")

message("Driver Sources: ${DRIVER_SOURCES}")
message("Driver Headers: ${DRIVER_HEADERS}")
message("Driver Include: ${DRIVER_INCLUDE}")

add_library("${PROJECT_NAME}" STATIC "${DRIVER_SOURCES}")

target_include_directories("${PROJECT_NAME}" PUBLIC 
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    
    # Include any external paths
)

target_link_libraries("${PROJECT_NAME}" PUBLIC common)
target_link_libraries("${PROJECT_NAME}" PUBLIC source)
target_link_libraries("${PROJECT_NAME}" PUBLIC base)
target_link_libraries("${PROJECT_NAME}" PUBLIC diagnostics)
target_link_libraries("${PROJECT_NAME}" PUBLIC lex)
target_link_libraries("${PROJECT_NAME}" PUBLIC testing)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

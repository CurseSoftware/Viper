cmake_minimum_required(VERSION 3.10)

project(common VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources for the base tool-chain
file(GLOB COMMON_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cc"
)

message("Common Sources: ${COMMON_SOURCES}")

add_library("${PROJECT_NAME}" STATIC "${COMMON_SOURCES}")

target_include_directories("${PROJECT_NAME}" PRIVATE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/common>
)

# Public header export
target_include_directories("${PROJECT_NAME}" PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

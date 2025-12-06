cmake_minimum_required(VERSION 3.10)

project(toolchain VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources for the base tool-chain
file(GLOB TOOLCHAIN_SOURCES
    "${PROJECT_SOURCE_DIR}/*.cc"
)

add_subdirectory(base)

message("Sources: ${TOOLCHAIN_SOURCES}")

add_executable("${PROJECT_NAME}" "${TOOLCHAIN_SOURCES}")

target_link_libraries("${PROJECT_NAME}" PRIVATE base)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

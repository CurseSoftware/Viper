cmake_minimum_required(VERSION 3.10)

project(toolchain VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Project C++ Standard: ${CMAKE_CXX_STANDARD}")

# Sources for the base tool-chain
file(GLOB TOOLCHAIN_SOURCES
    "${PROJECT_SOURCE_DIR}/*.cc"
)

add_subdirectory(driver)
add_subdirectory(source)
add_subdirectory(base)
add_subdirectory(diagnostics)
add_subdirectory(lex)

message("Sources: ${TOOLCHAIN_SOURCES}")

add_executable("${PROJECT_NAME}" "${TOOLCHAIN_SOURCES}")

target_link_libraries("${PROJECT_NAME}" PUBLIC common)
target_link_libraries("${PROJECT_NAME}" PRIVATE source)
target_link_libraries("${PROJECT_NAME}" PRIVATE driver)
target_link_libraries("${PROJECT_NAME}" PRIVATE base)
target_link_libraries("${PROJECT_NAME}" PRIVATE diagnostics)
target_link_libraries("${PROJECT_NAME}" PRIVATE lex)

target_include_directories("${PROJECT_NAME}" PRIVATE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/driver/include>
)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

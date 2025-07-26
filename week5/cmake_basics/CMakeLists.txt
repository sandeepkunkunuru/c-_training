# This project demonstrates fundamental CMake concepts and best practices

# Minimum CMake version required
cmake_minimum_required(VERSION 3.16)

# Project metadata
project(CMakeBasics 
    VERSION 1.0.0
    DESCRIPTION "CMake Basics Learning Project"
    LANGUAGES CXX
)

# Build information
message(STATUS "CMake version: ${CMAKE_VERSION}")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# Compiler-specific flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Wall /Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
endif()

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Create version header if it doesn't exist
file(WRITE "${CMAKE_SOURCE_DIR}/include/version.hpp.in"
"#pragma once

#define VERSION \"@PROJECT_VERSION@\"
#define BUILD_DATE \"@BUILD_DATE@\"
#define COMPILER_INFO \"@CMAKE_CXX_COMPILER_ID@ @CMAKE_CXX_COMPILER_VERSION@\"

namespace MathUtils {
    constexpr const char* VERSION = \"@PROJECT_VERSION@\";
    constexpr const char* BUILD_DATE = \"@BUILD_DATE@\";
    constexpr const char* COMPILER_INFO = \"@CMAKE_CXX_COMPILER_ID@ @CMAKE_CXX_COMPILER_VERSION@\";
}
")

# Set build date
string(TIMESTAMP BUILD_DATE "%Y-%m-%d %H:%M:%S")

# Configure version header
configure_file(
    "${CMAKE_SOURCE_DIR}/include/version.hpp.in"
    "${CMAKE_BINARY_DIR}/include/version.hpp"
    @ONLY
)

include_directories(${CMAKE_BINARY_DIR}/include)

# Library target
add_library(MathUtils STATIC
    src/math_utils.cpp
)

target_include_directories(MathUtils PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Set library properties
set_target_properties(MathUtils PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "include/math_utils.hpp;${CMAKE_BINARY_DIR}/include/version.hpp"
)

# Executable targets
add_executable(calculator src/main.cpp)
target_link_libraries(calculator MathUtils)

add_executable(demo src/demo.cpp)
target_link_libraries(demo MathUtils)

# Custom target for running the calculator
add_custom_target(run
    COMMAND calculator
    DEPENDS calculator
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running calculator program"
)

# Installation rules
include(GNUInstallDirs)

install(TARGETS MathUtils calculator demo
    EXPORT CMakeBasicsTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export targets
install(EXPORT CMakeBasicsTargets
    FILE CMakeBasicsTargets.cmake
    NAMESPACE CMakeBasics::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CMakeBasics
)

# Package configuration
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/CMakeBasicsConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/CMakeBasicsConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CMakeBasics
)

write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/CMakeBasicsConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_BINARY_DIR}/CMakeBasicsConfig.cmake"
    "${CMAKE_BINARY_DIR}/CMakeBasicsConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CMakeBasics
)

# Testing
enable_testing()

# Basic test
add_test(NAME calculator_test
    COMMAND calculator --test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Build summary
message(STATUS "")
message(STATUS "Build Summary:")
message(STATUS "  Source directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "  Binary directory: ${CMAKE_BINARY_DIR}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "    - calculator (executable)")
message(STATUS "    - demo (executable)")
message(STATUS "    - MathUtils (static library)")
message(STATUS "")
message(STATUS "Available targets:")
message(STATUS "  make all      - Build all targets")
message(STATUS "  make install  - Install the project")
message(STATUS "  make test     - Run tests")
message(STATUS "  make run      - Run calculator program")
message(STATUS "")

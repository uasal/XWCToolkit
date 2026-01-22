cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(XWCToolkit
    VERSION 1.0.0
    DESCRIPTION "Core C++20 library extracted from MagAO-X for building adaptive optics control applications"
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20) # mxlib is build with C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ============================================================================
# CMake Modules and Configuration
# ============================================================================

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

include(dependencies)
include(utils)

# Enable export of compile_commands.json for IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Build Options
# ============================================================================

option(BUILD_SHARED_LIBS "Build shared libraries (ON creates .so shared library)" ON)

# Enable CTest infrastructure (defines BUILD_TESTING option, default ON)
include(CTest)

# Catch2 header location
set(CATCH2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests/catch2")

if(NOT DEFINED MXLIB_MILK)
    option(MXLIB_MILK "Enable milk support for mxlib (required by mxlib)" TRUE)
endif()

# ============================================================================
# Installation Paths
# ============================================================================

set(XWCTK_PREFIX "/opt/MagAOX" CACHE PATH "Base directory for MagAOX vendor, packages, configs, drivers, apps & logs")

if(NOT EXISTS "${XWCTK_PREFIX}")
    message(WARNING "Required directory ${XWCTK_PREFIX} does not exist.")
endif()

set(PREFIX "/usr/local" CACHE PATH "Default system installation path")

# CMAKE_INSTALL_PREFIX defaults to /usr/local if not specified
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PREFIX}" CACHE PATH "Installation prefix" FORCE)
endif()

set(XWCTK_DRIVER_PATH "${XWCTK_PREFIX}/drivers" CACHE PATH "Path to the drivers directory")
set(XWCTK_FIFO_PATH "${XWCTK_PREFIX}/drivers/fifos" CACHE PATH "Path to the fifos directory")
set(XWCTK_LOG_PATH "${XWCTK_PREFIX}/logs" CACHE PATH "Path to the logs directory")
set(XWCTK_CONFIG_PATH "${XWCTK_PREFIX}/config" CACHE PATH "Path to the config directory")
set(XWCTK_BIN_PATH "${XWCTK_PREFIX}/bin" CACHE PATH "Path to the binaries directory")
set(XWCTK_APPS_PATH "${XWCTK_PREFIX}/apps" CACHE PATH "Path to apps directory")
set(XWCTK_PLUGIN_PATH "${XWCTK_PREFIX}/plugins" CACHE PATH "Path to telemetry schema plugins directory. External applications should install telemetry schema plugins to this directory.")

# Event code ranges for conflict prevention
# XWCTk reserves codes 0-20999. External packages must use 21000+
set(XWCTK_EVENT_CODE_MAX_RESERVED 20999 CACHE STRING "Maximum event code reserved for XWCTk core library")
set(XWCTK_EVENT_CODE_MIN_EXTERNAL 21000 CACHE STRING "Minimum event code for external packages (must be > MAX_RESERVED)")

set(MILK_PATH "${PREFIX}/milk-1.03.00" CACHE PATH "Path to milk directory")
set(MXLIB_PATH "${PREFIX}/lib" CACHE PATH "Path to mxlib lib directory")

# ============================================================================
# Dependencies
# ============================================================================

find_package(PkgConfig REQUIRED)

find_dependencies()

# ============================================================================
# Compiler Flags
# ============================================================================

# Detect compiler
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11)
        message(FATAL_ERROR "GCC >= 11 is required for C++17 support, found ${CMAKE_CXX_COMPILER_VERSION}")
    endif()
endif()

# Add warning flags (matching Makefile: -Wall -Wextra)
add_compile_options(
    -Wall
    -Wextra
)

# Position-independent code (required for libraries, matches Makefile -fPIC)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add optimization flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -ffast-math)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-O3 -g -ffast-math)
else()
    add_compile_options(-O3 -ffast-math)
endif()

# Coverage support
option(COVERAGE "Enable code coverage" OFF)
if(COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# ============================================================================
# RPATH Configuration for setuid binaries
# ============================================================================
# Hard-code library paths so setuid executables can find libraries
# (LD_LIBRARY_PATH is ignored for security when running setuid)
# This matches the Makefile behavior:
#   LDLIBRPATH := $(shell echo $$LD_LIBRARY_PATH | sed 's/::/:/g' | sed 's/:/ -Wl,-rpath,/g')

# Use CMAKE_INSTALL_RPATH to set runtime library paths
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Add standard library paths to RPATH
list(APPEND CMAKE_INSTALL_RPATH
    "${CMAKE_INSTALL_PREFIX}/lib"
    "${XWCTK_PREFIX}/lib"
    "${MILK_PATH}/lib"
)

# Also include paths from LD_LIBRARY_PATH if set
if(DEFINED ENV{LD_LIBRARY_PATH})
    string(REPLACE ":" ";" LD_LIB_PATHS "$ENV{LD_LIBRARY_PATH}")
    foreach(path ${LD_LIB_PATHS})
        if(path AND EXISTS "${path}")
            list(APPEND CMAKE_INSTALL_RPATH "${path}")
        endif()
    endforeach()
endif()

# Remove duplicates
list(REMOVE_DUPLICATES CMAKE_INSTALL_RPATH)

# ============================================================================
# Configuration Header
# ============================================================================

# Generate config.h with build-time settings
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

# Add generated headers to include path
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# ============================================================================
# Git Version Tracking
# ============================================================================
# Uses gengithead.sh (from mxlib) to generate git version header.

set(GIT_VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/xwctk_git_version.h")

find_program(GENGITHEAD_SH gengithead.sh REQUIRED
    HINTS /usr/local/bin ${PREFIX}/bin
)

add_custom_command(
    OUTPUT ${GIT_VERSION_HEADER}
    COMMAND ${GENGITHEAD_SH} ${CMAKE_CURRENT_SOURCE_DIR} ${GIT_VERSION_HEADER} XWCTK
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating git version header via gengithead.sh"
    VERBATIM
)

add_custom_target(git_version_header
    DEPENDS ${GIT_VERSION_HEADER}
)

# Make the generated header available to all targets
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# ============================================================================
# Source Directories
# ============================================================================

add_subdirectory(lib)
add_subdirectory(XWCTk)
add_subdirectory(utils)

# Tests - controlled by BUILD_TESTING (from CTest, default ON)
# Disable with: cmake -DBUILD_TESTING=OFF ..
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# ============================================================================
# Package Configuration
# ============================================================================

# Configure install path for bundled Catch2 headers so package config
# can advertise the installed location to consumers.
set(CATCH2_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include/XWCTk/tests/catch2")

# Generate and install Config files for find_package()
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/XWCTkConfig.cmake"
    INSTALL_DESTINATION "lib/cmake/XWCTk"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/XWCTkConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/XWCTkConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/XWCTkConfigVersion.cmake"
    DESTINATION "lib/cmake/XWCTk"
)

# Install alias config names to tolerate XWCTK vs XWCTk casing
# (install identical copies under those names)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/XWCTkConfig.cmake"
    DESTINATION "lib/cmake/XWCTk"
    RENAME "XWCTKConfig.cmake")

# Also provide aliased names for the version file
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/XWCTkConfigVersion.cmake"
    DESTINATION "lib/cmake/XWCTk"
    RENAME "XWCTKConfigVersion.cmake")

# Install CMake utility scripts for use by external packages
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake/"
    DESTINATION "lib/cmake/XWCTk"
    FILES_MATCHING PATTERN "*.cmake"
)

# Install export targets
install(
    EXPORT XWCTkTargets
    FILE XWCTkTargets.cmake
    NAMESPACE XWCTk::
    DESTINATION "lib/cmake/XWCTk"
)

# Install generated config.h and git version header
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/config.h"
        "${CMAKE_CURRENT_BINARY_DIR}/xwctk_git_version.h"
    DESTINATION include/XWCTk/
    COMPONENT headers
)

# ============================================================================
# Summary
# ============================================================================

message(STATUS "")
message(STATUS "XWCToolkit Configuration Summary:")
message(STATUS "==================================")
message(STATUS "Build type:              ${CMAKE_BUILD_TYPE}")
if(BUILD_SHARED_LIBS)
    message(STATUS "Library type:            SHARED (.so)")
else()
    message(STATUS "Library type:            STATIC (.a)")
endif()
message(STATUS "Compiler:                ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard:            ${CMAKE_CXX_STANDARD}")
message(STATUS "")
message(STATUS "Installation paths:")
message(STATUS "  System prefix:         ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  XWCTk prefix:          ${XWCTK_PREFIX}")
message(STATUS "  Apps path:             ${XWCTK_APPS_PATH}")
message(STATUS "")
message(STATUS "Features:")
message(STATUS "  Build tests:           ${BUILD_TESTING}")
message(STATUS "  Code coverage:         ${COVERAGE}")
message(STATUS "")
message(STATUS "Dependencies:")

if(IMAGESTREAMIO_FOUND)
    message(STATUS "  ImageStreamIO:         FOUND")
else()
    message(STATUS "  ImageStreamIO:         NOT FOUND")
endif()

if(MXLIB)
    message(STATUS "  mxlib:                 FOUND")
else()
    message(STATUS "  mxlib:                 NOT FOUND")
endif()

if(XRIFLIB)
    message(STATUS "  xrif:                  FOUND")
else()
    message(STATUS "  xrif:                  NOT FOUND")
endif()

if(CFITSIO_LIB)
    message(STATUS "  CFITSIO:               FOUND")
else()
    message(STATUS "  CFITSIO:               NOT FOUND")
endif()

if(Eigen3_FOUND)
    message(STATUS "  Eigen3:                FOUND")
else()
    message(STATUS "  Eigen3:                NOT FOUND")
endif()

if(FLATBUFFERS_HEADERS)
    message(STATUS "  FlatBuffers:           FOUND")
else()
    message(STATUS "  FlatBuffers:           NOT FOUND")
endif()

if(LIBUDEV_FOUND)
    message(STATUS "  libudev:               FOUND")
else()
    message(STATUS "  libudev:               NOT FOUND")
endif()

if(OpenMP_CXX_FOUND)
    message(STATUS "  OpenMP:                FOUND")
else()
    message(STATUS "  OpenMP:                NOT FOUND")
endif()

if(Threads_FOUND)
    message(STATUS "  Threads:               FOUND")
else()
    message(STATUS "  Threads:               NOT FOUND")
endif()

if(ZLIB_FOUND)
    message(STATUS "  zlib:                  FOUND")
else()
    message(STATUS "  zlib:                  NOT FOUND")
endif()

message(STATUS "")
message(STATUS "Build directory:         ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "Source directory:        ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "")
message(STATUS "To build: cmake --build . [--target <target>]")
message(STATUS "To install: cmake --install . [--component <component>]")
message(STATUS "")

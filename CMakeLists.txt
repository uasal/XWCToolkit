cmake_minimum_required(VERSION 3.16)

# set the project name and version
project(XWC VERSION 1.0 LANGUAGES CXX C)

########################################
## C & C++ versions
#######################################

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#######################################
## Set default library type
#######################################

# Set default to build shared libraries
# TODO!!!! Test what happens if this is off
set(BUILD_SHARED_LIBS ON)

#######################################
## Define common folder locations 
## and ensure they exist
#######################################
include(GNUInstallDirs) # standard module to guarantee portable project layout

# Define base variables with optional default values
set(MAGAOX_PREFIX "/opt/MagAOX" CACHE PATH "Base directory for MagAOX vendor packages, configs & logs")
set(PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE PATH "Default system installation path" FORCE) # CMAKE_INSTALL_PREFIX defaults to /usr/local

# Check if MAGAOX_PREFIX exists
if(NOT EXISTS "${MAGAOX_PREFIX}")
    message(WARNING "Required directory ${MAGAOX_PREFIX} does not exist.")
endif()

# Define paths based on the base variables
# set(BIN_PATH "${MAGAOX_PREFIX}/${CMAKE_INSTALL_BINDIR}" CACHE PATH "Path to the bin directory")
set(DRIVER_PATH "${MAGAOX_PREFIX}/drivers" CACHE PATH "Path to the drivers directory")
set(FIFO_PATH "${MAGAOX_PREFIX}/drivers/fifos" CACHE PATH "Path to the fifos directory")
set(LOG_PATH "${MAGAOX_PREFIX}/logs" CACHE PATH "Path to the logs directory")
set(CONFIG_PATH "${MAGAOX_PREFIX}/config" CACHE PATH "Path to the config directory")

set(BIN_PATH "${PREFIX}/${CMAKE_INSTALL_BINDIR}" CACHE PATH "Path to the bin directory")
set(LIB_PATH "${PREFIX}/${CMAKE_INSTALL_LIBDIR}" CACHE PATH "Path to the library directory")
set(HEADER_PATH "${PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}" CACHE PATH "Path to the include directory")

set(MILK_PATH "${PREFIX}/milk" CACHE PATH "Path to the milk directory")
set(MXLIB_PATH "${PREFIX}/${CMAKE_INSTALL_LIBDIR}" CACHE PATH "Path to the mxlib lib directory")
set(INGR_LIB_PATH "${LIB_PATH}" CACHE PATH "Path to the location of the instGraph lib")
set(INGR_HEADER_PATH "${HEADER_PATH}" CACHE PATH "Path to the location of the instGraph headers directory")

set(GIT_HEADER "xwc_git_version.h" CACHE PATH "Name of required git version tracker file.")

#######################################
## Define options
#######################################

option(MAGAOX_NOEDT "Disable EDT driver support" ON)

########################################
## Define variables
#######################################

if(NOT DEFINED MXLIB_MILK)
    set(MXLIB_MILK TRUE CACHE BOOL "Enable milk (required by mxlib)")
endif()

#######################################
## Include other cmake files
#######################################

# CMAKE_SOURCE_DIR is the project's root dir
# Set the path to custom CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Include global settings
include(common)
include(macros)

if (EXISTS "${CMAKE_SOURCE_DIR}/local/common.cmake")
    include("${CMAKE_SOURCE_DIR}/local/common.cmake")
endif()

#######################################
## Check dependencies exist
#######################################

# set(CMAKE_FIND_DEBUG_MODE True)

find_package(PkgConfig REQUIRED)

set(MILK_PC_PATH "${MILK_PATH}/${CMAKE_INSTALL_LIBDIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH ${MILK_PC_PATH})
require_isio()
require_eigen()
require_mxlib()
require_flatbuffers()
require_libudev()
require_cfitsio()
require_xrif()
require_openmp()

#######################################
## Add subfolders
#######################################

add_subdirectory(lib)
add_subdirectory(XWC)
# add_subdirectory(gui)
add_subdirectory(utils)

#######################################
## Set flags
#######################################

# Optimize
add_compile_flags_to_all_targets(-O3 -fopenmp -ffast-math)

# Warnings
add_compile_flags_to_all_targets(-Wall -Wextra)

# Position-independent code
add_compile_flags_to_all_targets(-fPIC)

#######################################
## Turn on position independent code
#######################################

set_position_independent_code_for_all_targets()

#######################################
## Install
#######################################

##### Executables #####
install(TARGETS flatlogcodes
                indiserver 
                getINDI 
                setINDI 
                evalINDI 
                xindidriver
                cursesINDI
                logdump
                logstream
                logsurgeon
                xrif2fits
                xrif2shmim
        DESTINATION ${BIN_PATH}
        )

# # Symbolic link
# # If $BIN_PATH is different to ${PREFIX}/${CMAKE_INSTALL_BINDIR}
# # Normalize the paths to absolute paths
# get_filename_component(abs_path1 "${BIN_PATH}" ABSOLUTE)
# get_filename_component(abs_path2 "${PREFIX}/${CMAKE_INSTALL_BINDIR}" ABSOLUTE)
# # Compare the normalized paths
# if(abs_path1 STREQUAL abs_path2)
#         hyperlink_targets(
#                         TARGETS indiserver getINDI setINDI evalINDI
#                         DESTINATION "${PREFIX}/${CMAKE_INSTALL_BINDIR}"
#                         )
# endif()


##### Libraries #####
install(TARGETS XWC
                flatlogslib 
                libtelnet
                libcommon
                liblilxml
        EXPORT XWCTargets
        LIBRARY DESTINATION ${LIB_PATH}
        )

##### Headers #####
# Recursively collect all header files in the project directory
file(GLOB_RECURSE PROJECT_HEADERS
    ${CMAKE_SOURCE_DIR}/**/*.h
    ${CMAKE_SOURCE_DIR}/**/*.hpp
)

# Install all collected headers, preserving the directory structure
foreach(header ${PROJECT_HEADERS})
    # Get the relative path of the header from the project directory
    file(RELATIVE_PATH rel_header ${CMAKE_SOURCE_DIR} ${header})

    # Get the directory path of the relative header
    get_filename_component(header_dir ${rel_header} DIRECTORY)

    # Install the header to the corresponding location in the install prefix
    install(FILES ${header}
            DESTINATION ${HEADER_PATH}/XWC/${header_dir})
endforeach()

# Define installation of generated headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/XWC/logger/generated
        DESTINATION ${HEADER_PATH}/XWC/XWC/logger
        FILES_MATCHING PATTERN "*.hpp" PATTERN "*.inc"
)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/XWC/logger/types/generated
        DESTINATION ${HEADER_PATH}/XWC/XWC/logger/types
        FILES_MATCHING PATTERN "*.h"
)

# Install git header
install(FILES ${CMAKE_BINARY_DIR}/${GIT_HEADER} DESTINATION ${HEADER_PATH}/XWC)


##### Export #####
install(EXPORT XWCTargets
  FILE XWCTargets.cmake
  NAMESPACE XWC::  
  DESTINATION ${LIB_PATH}/cmake/XWC
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/XWCConfigVersion.cmake"
    VERSION "${XWC_VERSION_MAJOR}.${XWC_VERSION_MINOR}"
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/XWCConfig.cmake"
  INSTALL_DESTINATION ${LIB_PATH}/cmake/XWC
  PATH_VARS MILK_PC_PATH
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/XWCConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/XWCConfig.cmake
  DESTINATION ${LIB_PATH}/cmake/XWC
  )

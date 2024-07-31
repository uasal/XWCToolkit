cmake_minimum_required(VERSION 3.16) # 3.16 has precompiled header support

# set the project name and version
project(XWC VERSION 1.0 LANGUAGES CXX C)

########################################
## C & C++ versions
#######################################

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Is there a reason for this?
# # Ensure C++ compiler and linker are used
# set(CMAKE_CXX_COMPILER "g++")

# add_compile_definitions(_XOPEN_SOURCE=700)
# add_compile_definitions(MX_OLD_GSL)

#######################################
## Set default library type
#######################################

# Set default to build shared libraries
# TODO!!!! Test what happens if this is off
set(BUILD_SHARED_LIBS ON)

#######################################
## Define options
#######################################

option(NEED_CUDA "Enable CUDA support" OFF)
option(NEED_CACAO "Enable CACAO support" OFF)
option(MAGAOX_NOEDT "Disable EDT driver support" ON)

#######################################
## Define common folder locations 
## and ensure they exist
#######################################
include(GNUInstallDirs) # standard module to guarantee portable project layout

# Define base variables with optional default values
set(MAGAOX_PREFIX "/opt/MagAOX" CACHE PATH "Base directory for MagAOX")
set(PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE PATH "Default system installation path" FORCE) # CMAKE_INSTALL_PREFIX defaults to /usr/local

# Define paths based on the base variables
# set(BIN_PATH "${MAGAOX_PREFIX}/${CMAKE_INSTALL_BINDIR}" CACHE PATH "Path to the bin directory")
set(DRIVER_PATH "${MAGAOX_PREFIX}/drivers" CACHE PATH "Path to the drivers directory")
set(FIFO_PATH "${MAGAOX_PREFIX}/drivers/fifos" CACHE PATH "Path to the fifos directory")

set(BIN_PATH "${PREFIX}/${CMAKE_INSTALL_BINDIR}" CACHE PATH "Path to the bin directory")
set(LIB_PATH "${PREFIX}/${CMAKE_INSTALL_LIBDIR}" CACHE PATH "Path to the library directory")
set(HEADER_PATH "${PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}" CACHE PATH "Path to the include directory")

set(MILK_PATH "${PREFIX}/milk" CACHE PATH "Path to the milk directory")
set(ISIO_PATH "${MILK_PATH}/${CMAKE_INSTALL_INCLUDEDIR}" CACHE PATH "Path to ImageStreamIO in the milk include directory")
set(MXLIB_PATH "${PREFIX}/${CMAKE_INSTALL_LIBDIR}" CACHE PATH "Path to the mxlib lib directory")

# Check if MAGAOX_PREFIX exists
if(NOT EXISTS "${MAGAOX_PREFIX}")
    message(WARNING "Required directory ${MAGAOX_PREFIX} does not exist.")
endif()

########################################
## Define variables
#######################################

if(NOT DEFINED MXLIB_MILK)
    set(MXLIB_MILK TRUE CACHE BOOL "Enable milk (required by mxlib)")
endif()

if(NOT DEFINED EDT)
    set(EDT FALSE CACHE BOOL "Enable EDT")
endif()
if(NOT DEFINED PYLON)
    set(PYLON FALSE CACHE BOOL "Enable Pylon")
endif()
if(NOT DEFINED PICAM)
    set(PICAM FALSE CACHE BOOL "Enable PiCam")
endif()

########################################
## Define roles
#######################################

if(MAGAOX_ROLE STREQUAL "ICC")
    set(EDT TRUE)
    set(PYLON TRUE)
    set(PICAM TRUE)
elseif(MAGAOX_ROLE STREQUAL "RTC")
    set(EDT TRUE)
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
## Add subfolders
#######################################

add_subdirectory(lib)
add_subdirectory(src)
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
install(
        TARGETS flatlogcodes
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
install(
        TARGETS flatlogslib 
                libtelnet
                libcommon
                liblilxml
                XWC
        LIBRARY DESTINATION ${LIB_PATH}
        )

##### Headers #####
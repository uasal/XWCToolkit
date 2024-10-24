cmake_minimum_required(VERSION 3.16)

# set the project name and version
project(XWCTK VERSION 1.0 LANGUAGES CXX C)

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
set(BUILD_SHARED_LIBS ON)

#######################################
## Define common folder locations 
## and ensure they exist
#######################################
include(GNUInstallDirs) # standard module to guarantee portable project layout

# Define base variables with optional default values
set(XWCTK_PREFIX "/opt/MagAOX" CACHE PATH "Base directory for MagAOX vendor, packages, configs, drivers, apps & logs")
set(PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE PATH "Default system installation path" FORCE) # CMAKE_INSTALL_PREFIX defaults to /usr/local

# Check if XWCTK_PREFIX exists
if(NOT EXISTS "${XWCTK_PREFIX}")
    message(WARNING "Required directory ${XWCTK_PREFIX} does not exist.")
endif()

# Define paths based on the base variables
# set(BIN_PATH "${XWCTK_PREFIX}/${CMAKE_INSTALL_BINDIR}" CACHE PATH "Path to the bin directory")
set(XWCTK_DRIVER_PATH "${XWCTK_PREFIX}/drivers") # "Path to the drivers directory"
set(XWCTK_FIFO_PATH "${XWCTK_PREFIX}/drivers/fifos") # "Path to the fifos directory"
set(XWCTK_LOG_PATH "${XWCTK_PREFIX}/logs") # "Path to the logs directory"
set(XWCTK_CONFIG_PATH "${XWCTK_PREFIX}/config") #"Path to the config directory"
set(XWCTK_APPS_PATH "${XWCTK_PREFIX}/apps" CACHE PATH "Path to apps directory")

set(USE_APPS_LOGSCHEMAS_DESCRIPTION "Compile log schemas in apps dirs with the library. Assumes dir structure ${XWCTK_APPS_PATH}/appdir1/logger/types/schemas/ and ${XWCTK_APPS_PATH}/appdir1/logger/logCodes.dat")
set(USE_APPS_LOGSCHEMAS TRUE CACHE BOOL "${USE_APPS_LOGSCHEMAS_DESCRIPTION}")

set(BIN_PATH "${PREFIX}/${CMAKE_INSTALL_BINDIR}") # "Path to the bin directory"
set(LIB_PATH "${PREFIX}/${CMAKE_INSTALL_LIBDIR}") # "Path to the library directory"
set(HEADER_PATH "${PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}") # "Path to the include directory"

set(MILK_PATH "${PREFIX}/milk" CACHE PATH "Path to the milk directory")
set(MXLIB_PATH "${LIB_PATH}" CACHE PATH "Path to the mxlib lib directory")
set(INGR_LIB_PATH "${LIB_PATH}" CACHE PATH "Path to the instGraph lib")
set(INGR_HEADER_PATH "${HEADER_PATH}" CACHE PATH "Path to the instGraph headers directory")

set(GIT_HEADER "xwctk_git_version.h" CACHE PATH "Name of required git version tracker file.")

#######################################
## Define options
#######################################

option(MAGAOX_NOEDT "Disable EDT driver support" ON)
option(NOINSTGRAPH "Disable instGraph support" ON)
option(XWCTK_DISABLE_USER_CHECK "Disable user check" ON)

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
# set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(PkgConfig REQUIRED)

set(MILK_PC_PATH "${MILK_PATH}/${CMAKE_INSTALL_LIBDIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH ${MILK_PC_PATH})
require_milk()
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

set(SUBDIRECTORIES
    lib
    XWCTK
    utils
)

foreach(SUBDIR ${SUBDIRECTORIES})
    add_subdirectory(${SUBDIR})
endforeach()

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

##### Executables with setuid #####
install(TARGETS xindiserver
        DESTINATION ${BIN_PATH}
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                    GROUP_READ GROUP_EXECUTE
                    WORLD_READ WORLD_EXECUTE
                    SETUID
        )

# Check instgraph flag
if(NOT NOINSTGRAPH)
    install(TARGETS instGraph
    DESTINATION ${BIN_PATH}
    )
endif()

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
set(XWCTK_LIBS XWCTK
            flatlogslib 
            telnet
            common
            lilxml
            )

install(TARGETS ${XWCTK_LIBS}
        EXPORT XWCTKTargets
        LIBRARY DESTINATION ${LIB_PATH}
        )

##### Headers #####
# Recursively collect all header files in the project directory
set(PROJECT_HEADERS)
foreach(SUBDIR ${SUBDIRECTORIES})
    file(GLOB_RECURSE HEADERS
        ${SUBDIR}/**/*.h
        ${SUBDIR}/**/*.hpp
        ${SUBDIR}/*.h
        ${SUBDIR}/*.hpp
    )
    list(APPEND PROJECT_HEADERS ${HEADERS})
endforeach()

# Install all collected headers, preserving the directory structure
foreach(header ${PROJECT_HEADERS})
    # Get the relative path of the header from the project directory
    file(RELATIVE_PATH rel_header ${CMAKE_SOURCE_DIR} ${header})

    # Get the directory path of the relative header
    get_filename_component(header_dir ${rel_header} DIRECTORY)

    # Install the header to the corresponding location in the install prefix
    install(FILES ${header}
            DESTINATION ${HEADER_PATH}/${PROJECT_NAME}/${header_dir})
endforeach()

# Also install external app headers (only external app headers are in $CMAKE_CURRENT_BINARY_DIR)
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/XWCTK/logger/types/"
        DESTINATION "${HEADER_PATH}/${PROJECT_NAME}/XWCTK/logger/types"
        FILES_MATCHING PATTERN "*.hpp")

# Define installation of generated headers & .inc files
install(DIRECTORY ${CMAKE_SOURCE_DIR}/XWCTK/logger/generated
    DESTINATION ${HEADER_PATH}/XWCTK/XWCTK/logger
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.inc"
)

# If not explicitly installed, app logger headers don't get installed
install(DIRECTORY ${CMAKE_SOURCE_DIR}/XWCTK/logger/types/generated
    DESTINATION ${HEADER_PATH}/XWCTK/XWCTK/logger/types
    FILES_MATCHING PATTERN "*.h"
)

# Install git header
install(FILES ${CMAKE_BINARY_DIR}/${GIT_HEADER} DESTINATION ${HEADER_PATH}/${PROJECT_NAME})


##### Export #####
install(EXPORT XWCTKTargets
  FILE XWCTKTargets.cmake
  NAMESPACE XWCTK::  
  DESTINATION ${LIB_PATH}/cmake/${PROJECT_NAME}
)

# Set alias for collective XWCTK::XWCTKall target as well
add_library(XWCTKall INTERFACE)
target_link_libraries(XWCTKall INTERFACE ${XWCTK_LIBS})
target_include_directories(XWCTKall INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)
install(TARGETS XWCTKall EXPORT XWCTKTargets)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION "${XWCTK_VERSION_MAJOR}.${XWCTK_VERSION_MINOR}"
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${LIB_PATH}/cmake/${PROJECT_NAME}
  PATH_VARS MILK_PC_PATH
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  DESTINATION ${LIB_PATH}/cmake/${PROJECT_NAME}
  )

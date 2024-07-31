########################################
## Libraries
#######################################

# Macro: require_mxlib
# Description: This macro finds the mxlib library and its headers.
# Output variables:
#   - MXLIB: Path to the found mxlib library.
#   - MXLIB_HEADERS: Path to the directory containing mxlib headers.
macro(require_mxlib)
    ## Require mxlib library
    find_library(MXLIB "libmxlib.so" PATHS ${MXLIB_PATH} REQUIRED)
    if (NOT MXLIB)
        message(FATAL_ERROR "libmxlib.so not found at location ${MXLIB_PATH}. Please install it here.")
    else()
        message(STATUS "Found libmxlib.so at location ${MXLIB}")
    endif()

    message(STATUS "Searching for mxlib header in ${HEADER_PATH}")
    # Check mxlib header files exist
    find_path(MXLIB_HEADERS NAMES "mx/improc/milkImage.hpp" PATHS "${HEADER_PATH}")

    # Check if the header files were found
    if(NOT MXLIB_HEADERS)
        message(FATAL_ERROR "Header files for mxlib not found in ${HEADER_PATH}.")
    else()
        message(STATUS "Header files for mxlib found at location ${MXLIB_HEADERS}.")
    endif()
endmacro()


# Macro: require_flatbuffers
# Description: This macro finds the flatbuffers library.
# Output variables:
#   - FLATBUFFERSLIB: Path to the flatbuffers library.
#   - FLATBUFFERS_HEADERS: Path to the flatbuffers headers.
macro(require_flatbuffers)
    find_library(FLATBUFFERSLIB "libflatbuffers.a" PATHS ${LIB_PATH} REQUIRED)
    if (NOT FLATBUFFERSLIB)
        message(FATAL_ERROR "libflatbuffers.a not found at location ${LIB_PATH}. Please install it here.")
    else()
        message(STATUS "Found libflatbuffers.a at location ${FLATBUFFERSLIB}")
    endif()

    message(STATUS "Searching for flatbuffers headers in ${HEADER_PATH}")
    # Check flatbuffers header files exist
    find_path(FLATBUFFERS_HEADERS NAMES "flatbuffers/flatbuffers.h" PATHS "${HEADER_PATH}")

    # Check if the header files were found
    if(NOT FLATBUFFERS_HEADERS)
        message(FATAL_ERROR "Header files for flatbuffers not found in ${HEADER_PATH}.")
    else()
        message(STATUS "Header files for flatbuffers found at location ${FLATBUFFERS_HEADERS}")
    endif()
endmacro()


# Macro: require_xrif
# Description: This macro finds the xrif library.
# Output variables:
#   - XRIFLIB: Path to the xrif library.
#   - XRIF_HEADERS: Path to the xrif headers.
macro(require_xrif)
    find_library(XRIFLIB "libxrif.a" PATHS ${LIB_PATH} REQUIRED)
    if (NOT XRIFLIB)
        message(FATAL_ERROR "libxrif.a not found at location ${LIB_PATH}. Please install it here.")
    else()
        message(STATUS "Found libxrif.a at location ${XRIFLIB}")
    endif()

    message(STATUS "Searching for xrif headers in ${HEADER_PATH}")
    # Check xrfi header files exist
    find_path(XRIF_HEADERS NAMES "xrif/xrif.h" PATHS "${HEADER_PATH}")

    # Check if the header files were found
    if(NOT XRIF_HEADERS)
        message(FATAL_ERROR "Header files for xrif not found in ${HEADER_PATH}.")
    else()
        message(STATUS "Header files for xrif found at location ${XRIF_HEADERS}")
    endif()
endmacro()

# Macro: require_cfitsio
# Description: This macro finds the cfitsio library.
# Output variables:
#   - CFITSIO_LIB: Path to the found cfitsio library.
macro(require_cfitsio)
    find_library(CFITSIO_LIB cfitsio REQUIRED)
    if (NOT CFITSIO_LIB)
        message(FATAL_ERROR "CFITSIO library not found. Please install it.")
    else()
        message(STATUS "CFITSIO found at location ${CFITSIO_LIB}")            
    endif()
endmacro()

# Macro: require_isio
# Description: This macro finds the isio library.
macro(require_isio)
    if(EXISTS ${ISIO_PATH})
        message(STATUS "Milk path to ${ISIO_PATH} exists.")
    else()
        message(FATAL_ERROR "Please set the variable ${ISIO_PATH} to the milk path to the ImageStreamIO folder.")
    endif()
endmacro()

# Macro: require_eigen
# Description: This macro finds the eigen header.
# Output variables:
#   - EIGEN3_INCLUDE_DIR: Path to the found eigen header.
macro(require_eigen)
    # Find the Eigen library & include directories
    find_package(Eigen3 REQUIRED)
    if(Eigen3_FOUND)
        message(STATUS "Eigen3 headers found: ${EIGEN3_INCLUDE_DIR}")
    else()
        message(FATAL_ERROR "Package Eigen3 not found. Please install it.")
    endif()
endmacro()

# Macro: require_libudev
# Description: This macro finds the libudev header.
# Output variables:
#   - LIBUDEV_INCLUDE_DIRS: Path to the found libudev header.
#   - LIBUDEV_LIBRARIES: Path to the libudev libraries
macro(require_libudev)
    ## Require libudev-dev (needed by tty)
    pkg_check_modules(LIBUDEV REQUIRED libudev)
    if(LIBUDEV_FOUND)
        message(STATUS "libudev found ${LIBUDEV_INCLUDE_DIRS}")
    else()
        message(FATAL_ERROR "libudev not found. Please install it.")
    endif()
endmacro()

# Macro: require_milk
# Description: This macro finds the milk libraries.
# Output variables:
#   - MILK_LIB_FILES: list of milk libraries.
macro(require_milk)
    if(EXISTS ${MILK_PATH}/${CMAKE_INSTALL_LIBDIR})
        message(STATUS "Milk lib path ${MILK_PATH}/${CMAKE_INSTALL_LIBDIR} exists.")
        # Collect all library files in the directory
        file(GLOB MILK_LIB_FILES "${MILK_PATH}/${CMAKE_INSTALL_LIBDIR}/lib*.so")
    else()
        message(FATAL_ERROR "Milk libraries not found at ${MILK_PATH}/${CMAKE_INSTALL_LIBDIR}. Please se ${MILK_PATH}.")
    endif()
endmacro()


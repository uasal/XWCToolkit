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

# Macro: require_eigen
# Description: This macro finds the eigen header.
# Output variables:
#   - EIGEN3_INCLUDE_DIR: Path to the found eigen header.
macro(require_eigen)
    # Find the Eigen library & include directories
    find_package(Eigen3 REQUIRED NO_MODULE)
    if(Eigen3_FOUND)
        message(STATUS "Eigen3 found")
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
#   - milk:: - milk imported targets
macro(require_milk)  
    find_package(milk REQUIRED NO_MODULE PATHS ${MILK_PC_PATH})
    if(milk_FOUND)
        message(STATUS "milk library found")
    else()
        message(FATAL_ERROR "Package milk not found at ${MILK_PC_PATH}. Please install it here or set the MILK_PATH variable to the current location.")
    endif()    
endmacro()

# Macro: require_isio
# Description: This macro finds the ImageStreamIO library.
# Output variables:
#   - ImageStreamIO:: - isio imported targets
#   - ImageStreamIO:: - isio imported targets
# macro(require_isio)  
#     find_package(ImageStreamIO REQUIRED NO_MODULE PATHS ${MILK_PC_PATH})
#     if(ImageStreamIO_FOUND)
#         message(STATUS "ImageStreamIO library found")
#         message(STATUS "ImageStreamIO target: ${ImageStreamIO_LIBRARIES}")
#     else()
#         message(FATAL_ERROR "Package ImageStreamIO not found at ${MILK_PC_PATH}. Please install it here or set the MILK_PATH variable to the current location.")
#     endif()    
# endmacro()

## Macro: Require OpenMP
# Description: This macro finds the OpenMP library.
# Output variables:
#   - OpenMP::OpenMP_CXX - OpenMP imported target
macro(require_openmp)
    find_package(OpenMP REQUIRED)
    if(NOT OpenMP_CXX_FOUND)
        message(FATAL_ERROR "OpenMP package not found.Please install it.")
    endif()
endmacro()

########################################
## App dirs
#######################################

# Macro: process_appdirs
# Description: This macro finds app dirs in ${XWCTK_APPS_PATH} and returns the paths 
# to the corresponding dat files and schema dirs.
# Output variables:
#   - schemadirs - list of schema dirs
#   - telemfiles - list of telem.cpp files
#   - datfiles - list of datfiles
macro(process_appdirs)
    if(${USE_APPS_LOGSCHEMAS})
        message(STATUS "Find paths to app log schemas, dat files and telem.cpp.")
        
        # Find all directories in ${XWCTK_APPS_PATH}
        file(GLOB APPDIRS "${XWCTK_APPS_PATH}/*")
        foreach(dirpath ${APPDIRS})

            # Ignore anything starting with '.'
            get_filename_component(dirname ${dirpath} NAME)
            string(REGEX MATCH "^\\." is_hidden "${dirname}")
            if(is_hidden)
                message(STATUS "Skipping hidden directory: ${dirname}")
                continue()
            endif()

            message(STATUS "Checking appdir ${dirpath}")
            if(IS_DIRECTORY "${dirpath}/logger/types/schemas")
                if(EXISTS "${dirpath}/logger/logCodes.dat")
                    message(STATUS "Found a logger/types/schemas dir and a logCodes.dat in ${dirpath}.")
                    list(APPEND schemadirs "${dirpath}/logger/types/schemas")
                    list(APPEND telemfiles "${dirpath}/logger/types/telem.cpp")
                    list(APPEND datfiles "${dirpath}/logger/logCodes.dat")
                endif()
            else()
                continue()
            endif()

        endforeach()
    endif()
endmacro()

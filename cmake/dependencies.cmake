########################################
## Libraries
#######################################

## Find all required dependencies for XWCToolkit
function(find_dependencies)
    message(STATUS "Searching for XWCToolkit dependencies...")

    # ImageStreamIO (from milk)
    #   - Creates imported target: PkgConfig::IMAGESTREAMIO
    set(ENV{PKG_CONFIG_PATH} "${MILK_PATH}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
    message(STATUS "Searching for ImageStreamIO in ${MILK_PATH}")
    pkg_check_modules(IMAGESTREAMIO REQUIRED IMPORTED_TARGET ImageStreamIO)
    if(IMAGESTREAMIO_FOUND)
        message(STATUS "  Found ImageStreamIO include dirs: ${IMAGESTREAMIO_INCLUDE_DIRS}")
        message(STATUS "  Found ImageStreamIO libraries: ${IMAGESTREAMIO_LIBRARIES}")
    else()
        message(FATAL_ERROR "  ImageStreamIO not found. Searched in ${MILK_PATH}/lib/pkgconfig. Please install milk or set MILK_PATH.")
    endif()


    # mxlib
    #   - MXLIB: Path to the found mxlib library.
    #   - MXLIB_HEADERS: Path to the directory containing mxlib headers.
    message(STATUS "Searching for mxlib in ${MXLIB_PATH}")
    find_library(MXLIB "libmxlib.so" PATHS ${MXLIB_PATH} REQUIRED)
    if (MXLIB)
        message(STATUS "  Found libmxlib.so at location ${MXLIB}")
    else()
        message(FATAL_ERROR "  libmxlib.so not found at location ${MXLIB_PATH}. Please install it here.")
    endif()


    # xrif
    #   - XRIFLIB: Path to the xrif library.
    #   - XRIF_HEADERS: Path to the xrif headers.
    #   - xrif::xrif: Imported target with OpenMP dependency.
    message(STATUS "Searching for xrif")
    find_library(XRIFLIB "libxrif.a")
    if (XRIFLIB)
        message(STATUS "  Found libxrif.a at location ${XRIFLIB}")
    else()
        message(FATAL_ERROR "  libxrif.a not found. Please install it.")
    endif()

    message(STATUS "Searching for xrif headers")
    # Check xrfi header files exist
    find_path(XRIF_HEADERS NAMES "xrif/xrif.h")

    # Check if the header files were found
    if(XRIF_HEADERS)
        message(STATUS "  Header files for xrif found at location ${XRIF_HEADERS}")
    else()
        message(FATAL_ERROR "  Header files for xrif not found.")
    endif()

    # Create imported target for xrif with OpenMP dependency
    # (otherwise OpenMP needs to be linked separately everywhere xrif is used)
    if(NOT TARGET xrif::xrif)
        add_library(xrif::xrif STATIC IMPORTED GLOBAL)
        set_target_properties(xrif::xrif PROPERTIES
            IMPORTED_LOCATION "${XRIFLIB}"
            INTERFACE_INCLUDE_DIRECTORIES "${XRIF_HEADERS}"
            INTERFACE_LINK_LIBRARIES "OpenMP::OpenMP_C"
        )
    endif()


    # CFITSIO (needed by xrif2fits and xrif2shmim)
    #   - CFITSIO_LIB: Path to the found cfitsio library.
    #   - CFITSIO_INCLUDE_DIR: Path to the cfitsio headers.
    message(STATUS "Searching for cfitsio library")
    find_library(CFITSIO_LIB cfitsio REQUIRED)
    if (CFITSIO_LIB)
        message(STATUS "  CFITSIO found at location ${CFITSIO_LIB}")            
    else()
        message(FATAL_ERROR "  CFITSIO library not found. Please install it.")
    endif()

    message(STATUS "Searching for cfitsio headers")
    find_path(CFITSIO_INCLUDE_DIR NAMES "fitsio.h")
    if(CFITSIO_INCLUDE_DIR)
        message(STATUS "  CFITSIO headers found at location ${CFITSIO_INCLUDE_DIR}")
    else()
        message(FATAL_ERROR "  CFITSIO headers not found. Please install cfitsio development package.")
    endif()


    # Eigen3
    #   - EIGEN3_INCLUDE_DIR: Path to the found eigen header.
    message(STATUS "Searching for Eigen3")
    find_package(Eigen3 REQUIRED)
    if(Eigen3_FOUND)
        message(STATUS "  Found Eigen3 ${Eigen3_VERSION} at location ${EIGEN3_INCLUDE_DIR}")
    else()
        message(FATAL_ERROR "  Package Eigen3 not found. Please install it.")
    endif()


    # FlatBuffers
    #   - FLATBUFFERSLIB: Path to the flatbuffers library.
    #   - FLATBUFFERS_HEADERS: Path to the flatbuffers headers.
    message(STATUS "Searching for FlatBuffers")
    find_library(FLATBUFFERSLIB "libflatbuffers.a")
    if (FLATBUFFERSLIB)
        message(STATUS "  Found libflatbuffers.a at location ${FLATBUFFERSLIB}")
    else()
        message(FATAL_ERROR "  libflatbuffers.a not found. Please install it.")
    endif()

    message(STATUS "Searching for flatbuffers headers")
    # Check flatbuffers header files exist
    find_path(FLATBUFFERS_HEADERS NAMES "flatbuffers/flatbuffers.h")

    # Check if the header files were found
    if(FLATBUFFERS_HEADERS)
        message(STATUS "  Header files for flatbuffers found at location ${FLATBUFFERS_HEADERS}")
    else()
        message(FATAL_ERROR "  Header files for flatbuffers not found.")
    endif()


    # libudev
    #   - Creates imported target: PkgConfig::LIBUDEV
    message(STATUS "Searching for libudev")
    pkg_check_modules(LIBUDEV REQUIRED IMPORTED_TARGET libudev)
    if(LIBUDEV_FOUND)
        message(STATUS "  Found libudev ${LIBUDEV_VERSION}")
        message(STATUS "  Found libudev libraries: ${LIBUDEV_LIBRARIES}")
    else()
        message(FATAL_ERROR "  libudev not found. Please install it.")
    endif() 


    # OpenMP
    #   - OpenMP::OpenMP_CXX - OpenMP imported target
    message(STATUS "Searching for OpenMP")
    find_package(OpenMP REQUIRED)
    if(OpenMP_CXX_FOUND)
        message(STATUS "  Found OpenMP ${OpenMP_CXX_VERSION}")
    else()
        message(FATAL_ERROR "  OpenMP package not found. Please install it.")
    endif()


    # Threads
    message(STATUS "Searching for Threads")
    find_package(Threads REQUIRED)
    if(Threads_FOUND)
        message(STATUS "  Found Threads")
    else()
        message(FATAL_ERROR "  pthread not found. Please install libpthread-stubs0-dev before proceeding.")
    endif()


    # ZLIB
    message(STATUS "Searching for ZLIB")
    find_package(ZLIB REQUIRED)
    if(ZLIB_FOUND)
        message(STATUS "  Found ZLIB ${ZLIB_VERSION} at location ${ZLIB_INCLUDE_DIRS}")
    else()
        message(FATAL_ERROR "  zlib not found. Please install zlib1g-dev before proceeding.")
    endif()


    # Curses (needed for cursesINDI)
    message(STATUS "Searching for Curses")
    find_package(Curses REQUIRED)
    if(CURSES_FOUND)
        message(STATUS "  Found Curses at location ${CURSES_INCLUDE_DIRS}")
    else()
        message(STATUS "  Curses not found. Please install Curses before proceeding.")
    endif()


    message(STATUS "Dependency search complete")

    # Propagate variables to parent scope
    # Note: IMPORTED_TARGETs (PkgConfig::*) are automatically global
    set(IMAGESTREAMIO_FOUND ${IMAGESTREAMIO_FOUND} PARENT_SCOPE)
    set(LIBUDEV_FOUND ${LIBUDEV_FOUND} PARENT_SCOPE)
    set(MXLIB ${MXLIB} PARENT_SCOPE)
    set(XRIFLIB ${XRIFLIB} PARENT_SCOPE)
    set(XRIF_HEADERS ${XRIF_HEADERS} PARENT_SCOPE)
    set(CFITSIO_LIB ${CFITSIO_LIB} PARENT_SCOPE)
    set(CFITSIO_INCLUDE_DIR ${CFITSIO_INCLUDE_DIR} PARENT_SCOPE)
    set(Eigen3_FOUND ${Eigen3_FOUND} PARENT_SCOPE)
    set(EIGEN3_INCLUDE_DIR ${EIGEN3_INCLUDE_DIR} PARENT_SCOPE)
    set(FLATBUFFERSLIB ${FLATBUFFERSLIB} PARENT_SCOPE)
    set(FLATBUFFERS_HEADERS ${FLATBUFFERS_HEADERS} PARENT_SCOPE)
    set(OpenMP_CXX_FOUND ${OpenMP_CXX_FOUND} PARENT_SCOPE)
    set(Threads_FOUND ${Threads_FOUND} PARENT_SCOPE)
    set(ZLIB_FOUND ${ZLIB_FOUND} PARENT_SCOPE)
    set(CURSES_FOUND ${CURSES_FOUND} PARENT_SCOPE)
    set(CURSES_LIBRARIES ${CURSES_LIBRARIES} PARENT_SCOPE)
endfunction()
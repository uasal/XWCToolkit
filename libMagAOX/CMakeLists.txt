## Require milk ImageStreamIO headers
if(EXISTS ${ISIO_PATH})
    message(STATUS "Milk path to ${ISIO_PATH} exists.")
else()
    message(FATAL_ERROR "Please set the variable ${ISIO_PATH} to the milk path to the ImageStreamIO folder.")
endif()

## Require milk libraries
if(EXISTS ${MILK_PATH}/${CMAKE_INSTALL_LIBDIR})
    message(STATUS "Milk lib path ${MILK_PATH}/${CMAKE_INSTALL_LIBDIR} exists.")
    # Collect all library files in the directory
    file(GLOB MILK_LIB_FILES "${MILK_PATH}/${CMAKE_INSTALL_LIBDIR}/lib*.so")
else()
    message(FATAL_ERROR "Milk libraries not found at ${MILK_PATH}/${CMAKE_INSTALL_LIBDIR}. Please se ${MILK_PATH}.")
endif()


## Require mxlib library
find_library(MXLIB "libmxlib.so" PATHS ${MXLIB_PATH} REQUIRED)
if (NOT MXLIB)
    message(FATAL_ERROR "libmxlib.so not found at location ${MXLIB_PATH}. Please install it here.")
else()
    message(STATUS "Found libmxlib.so at location ${MXLIB}")
endif()


message(STATUS "Searching for mxlib header in ${MXLIB_HEADER_PATH}")
# Check mxlib header files exist
find_path(MXLIB_HEADERS NAMES "mx/improc/milkImage.hpp" PATHS "${MXLIB_HEADER_PATH}")

# Check if the header files were found
if(NOT MXLIB_HEADERS)
    message(FATAL_ERROR "Header files for mxlib not found in ${MXLIB_HEADER_PATH}.")
else()
    message(STATUS "Header files for mxlib found at location ${MXLIB_HEADERS}")
endif()

## Require Eigen (needed by mxlib)
# Find the Eigen library & include directories
find_package(Eigen3 REQUIRED)
if(Eigen3_FOUND)
    message(STATUS "Eigen3 headers found: ${EIGEN3_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Package Eigen3 not found. Please install it.")
endif()


## Require libudev-dev (needed by tty)
pkg_check_modules(LIBUDEV REQUIRED libudev)
if(LIBUDEV_FOUND)
    message(STATUS "libudev found ${LIBUDEV_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "libudev not found. Please install it.")
endif()


## Require flatbuffers
# Find flatbuffers executable 'flatc'
set(EXECUTABLE_NAME "flatc")
# Use GLOB to check flatc exists in ${PREFIX}/${CMAKE_INSTALL_BINDIR}
file(GLOB DEFAULT_FLATBUFFERS_PATH "${PREFIX}/${CMAKE_INSTALL_BINDIR}/${EXECUTABLE_NAME}")
# Check if a match was found
if (DEFAULT_FLATBUFFERS_PATH)
    list(GET DEFAULT_FLATBUFFERS_PATH 0 FOUND_FLATBUFFERS)
    message(STATUS "Found ${EXECUTABLE_NAME}: ${DEFAULT_FLATBUFFERS_PATH}")
else()
    message(FATAL_ERROR "${EXECUTABLE_NAME} not found at location ${PREFIX}/${CMAKE_INSTALL_BINDIR}. Please install it here.")
endif()

# Source files
set(SOURCES
    app/MagAOXApp.cpp
    app/stateCodes.cpp
    app/dev/ioDevice.cpp
    app/dev/outletController.cpp
    app/dev/stdCamera.cpp
    logger/types/telem.cpp
    logger/logFileName.cpp
    logger/logFileRaw.cpp
    logger/logMap.cpp
    logger/logMeta.cpp
    logger/logBinarySchemata.cpp
    modbus/modbus.cpp
    sys/runCommand.cpp
    sys/thSetuid.cpp
    tty/netSerial.cpp
    tty/telnetConn.cpp
    tty/ttyIOUtils.cpp
    tty/ttyErrors.cpp
    tty/ttyUSB.cpp
    tty/usbDevice.cpp
)


# Find all .fbs files required by logTypes.hpp
file(GLOB FBS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/logger/types/schemas/*.fbs")

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/logTypes.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/binarySchemata.inc
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/logger/types/generated    
    COMMAND ${CMAKE_COMMAND} -E echo "Executing flatlogcodes"    
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR}/logger bash -c "${CMAKE_SOURCE_DIR}/flatlogs/bin/flatlogcodes"
    COMMAND ${CMAKE_COMMAND} -E echo "Generating header files"    
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR}/logger/types/generated bash -c "for bfb in *.bfbs; do xxd -i \$bfb > \$(basename \$bfb).h; done"
    COMMAND ${CMAKE_COMMAND} -E echo "Generatinc .inc file"
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR}/logger/types/generated bash -c "find . -name '*.bfbs.h' -print0 | xargs -0 cat > ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/binarySchemata.inc"
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/logger/logCodes.dat ${FBS_FILES}
    COMMENT "Generating logger/generated/logTypes.hpp"
    VERBATIM
)

# Create a custom target for generating log types
add_custom_target(generated_logTypes DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/logTypes.hpp)

# Create a precompiled header target
add_library(XWC_precompiled_header INTERFACE)
target_precompile_headers(XWC_precompiled_header INTERFACE libMagAOX.hpp)
target_include_directories(XWC_precompiled_header INTERFACE
                            ${EIGEN3_INCLUDE_DIR}                            
                            ${ISIO_PATH}                            
                            ${MXLIB_HEADERS}
                            ${CMAKE_CURRENT_SOURCE_DIR}
                            ${CMAKE_CURRENT_SOURCE_DIR}/app
                            ${CMAKE_CURRENT_SOURCE_DIR}/app/dev
                            ${CMAKE_CURRENT_SOURCE_DIR}/common
                            ${CMAKE_CURRENT_SOURCE_DIR}/ImageStreamIO
                            ${CMAKE_CURRENT_SOURCE_DIR}/logger
                            ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated
                            ${CMAKE_CURRENT_SOURCE_DIR}/logger/types
                            ${CMAKE_CURRENT_SOURCE_DIR}/modbus
                            ${CMAKE_CURRENT_SOURCE_DIR}/sys
                            ${CMAKE_CURRENT_SOURCE_DIR}/tty
                            ${CMAKE_CURRENT_SOURCE_DIR}/utils
                            ${CMAKE_SOURCE_DIR}/flatlogs/include                            
                            )

add_dependencies(XWC_precompiled_header generated_logTypes)
target_compile_definitions(XWC_precompiled_header INTERFACE MXLIB_MILK)

# Add the MagAOX library
add_library(MagAOX ${SOURCES})
message(STATUS "Including header dirs: ${ISIO_PATH} ${EIGEN3_INCLUDE_DIR} ${MXLIB_HEADERS}")
message(STATUS "Including libraries ${MILK_LIB_FILES} and ${MXLIB}")
target_include_directories(MagAOX PRIVATE
                            ${EIGEN3_INCLUDE_DIR}                                                 
                            ${MXLIB_HEADERS}
                            ${CMAKE_CURRENT_SOURCE_DIR}
                            ${CMAKE_CURRENT_SOURCE_DIR}/app
                            ${CMAKE_CURRENT_SOURCE_DIR}/app/dev
                            ${CMAKE_CURRENT_SOURCE_DIR}/common
                            ${CMAKE_CURRENT_SOURCE_DIR}/ImageStreamIO
                            ${CMAKE_CURRENT_SOURCE_DIR}/logger
                            ${CMAKE_CURRENT_SOURCE_DIR}/logger/types
                            ${CMAKE_CURRENT_SOURCE_DIR}/modbus
                            ${CMAKE_CURRENT_SOURCE_DIR}/sys
                            ${CMAKE_CURRENT_SOURCE_DIR}/tty
                            ${CMAKE_CURRENT_SOURCE_DIR}/utils
                            ${CMAKE_SOURCE_DIR}/flatlogs/include                            
                            )
target_link_libraries(MagAOX PRIVATE XWC_precompiled_header 
                                        "${MILK_LIB_FILES}"
                                        "${MXLIB}"
                                        )
add_dependencies(MagAOX generated_logTypes)


# Install rules
install(TARGETS MagAOX
    LIBRARY DESTINATION ${LIB_PATH}
)
## Require milk ImageStreamIO headers
require_isio()

## Require milk library
require_milk()

## Require mxlib library & headers
require_mxlib()

## Require Eigen library & include directories
require_eigen()

## Require flatbuffers
require_flatbuffers()

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
    COMMAND $<TARGET_FILE:flatlogcodes>
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/logger   
    COMMAND ${CMAKE_COMMAND} -E echo "Generating header files"    
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR}/logger/types/generated bash -c "for bfb in *.bfbs; do xxd -i \$bfb > \$(basename \$bfb).h; done"
    COMMAND ${CMAKE_COMMAND} -E echo "Generatinc .inc file"
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR}/logger/types/generated bash -c "find . -name '*.bfbs.h' -print0 | xargs -0 cat > ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/binarySchemata.inc"
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/logger/logCodes.dat ${FBS_FILES} flatlogcodes
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
                            ${FLATBUFFERS_HEADERS}
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
                            ${CMAKE_SOURCE_DIR}/lib/flatlogs/include                            
                            )

add_dependencies(XWC_precompiled_header generated_logTypes)
target_compile_definitions(XWC_precompiled_header INTERFACE MXLIB_MILK)

# Add the XWC library
add_library(XWC ${SOURCES})
target_include_directories(XWC PRIVATE
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
                            ${CMAKE_SOURCE_DIR}/lib/flatlogs/include                            
                            )
target_link_libraries(XWC PRIVATE XWC_precompiled_header 
                                        ${MILK_LIB_FILES}
                                        ${MXLIB}
                                        )
add_dependencies(XWC generated_logTypes)
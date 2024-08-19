message(STATUS "Adding subdirectory XWC")

set(XWC_INCLUDE_DIR
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app/dev>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ImageStreamIO>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/logger>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/logger/generated>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/logger/types>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/modbus>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sys>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tty>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/utils>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/flatlogs/include>
    )

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


## Execute flatlogcodes and generate log types
###########################################################

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


## Execute gengithead.sh and generate magaox_git_version.h
###########################################################

# Try to find gengithead.sh
find_program(GENGITHEAD "gengithead.sh"
    HINTS
        /usr/local/bin
    NO_DEFAULT_PATH
)

if(NOT GENGITHEAD)
    message(FATAL_ERROR "The required script 'gengithead.sh' was not found on the system.")
else()
    message(STATUS "Found script: ${GENGITHEAD}")
endif()

# Define parameters for the scripts
message(STATUS "Generating ${GIT_HEADER}")

add_custom_command(
    OUTPUT "${CMAKE_SOURCE_DIR}/${GIT_HEADER}"
    COMMAND ${CMAKE_COMMAND} -E echo "Generating ${GIT_HEADER}"  
    COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_SOURCE_DIR}" bash -c "${GENGITHEAD} ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/${GIT_HEADER} XWC"
    COMMENT "Generated ${GIT_HEADER} header" 
    DEPENDS ${GENGITHEAD}
    VERBATIM
)

# Create a custom target for generating git header
add_custom_target(githeader DEPENDS "${CMAKE_SOURCE_DIR}/${GIT_HEADER}")


## Add the XWC library
###########################################################
add_library(XWC ${SOURCES})
target_include_directories(XWC PUBLIC
                            ${MXLIB_HEADERS}
                            ${XWC_INCLUDE_DIR}
                            ${ImageStreamIO_INCLUDE_DIR}
                            )
target_link_libraries(XWC PUBLIC 
                                ImageStreamIO::ImageStreamIO
                                Eigen3::Eigen
                                ${MXLIB}
                                )
add_dependencies(XWC generated_logTypes githeader)
target_compile_definitions(XWC PUBLIC MXLIB_MILK)
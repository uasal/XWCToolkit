message(STATUS "Adding subdirectory XWCTK")

set(XWCTK_INCLUDE_DIR
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app/dev>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ImageStreamIO>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/logger>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/logger/generated>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/logger/generated>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/logger/types>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/modbus>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sys>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tty>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/utils>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/flatlogs/include>
    $<INSTALL_INTERFACE:${HEADER_PATH}/${PROJECT_NAME}/${PROJECT_NAME}>
    $<INSTALL_INTERFACE:${HEADER_PATH}/${PROJECT_NAME}/${PROJECT_NAME}/app>
    $<INSTALL_INTERFACE:${HEADER_PATH}/${PROJECT_NAME}/${PROJECT_NAME}/app/dev>
    $<INSTALL_INTERFACE:${HEADER_PATH}/${PROJECT_NAME}/${PROJECT_NAME}/common>
    $<INSTALL_INTERFACE:${HEADER_PATH}/${PROJECT_NAME}/${PROJECT_NAME}/ImageStreamIO>
    $<INSTALL_INTERFACE:${HEADER_PATH}/${PROJECT_NAME}/${PROJECT_NAME}/logger>
    $<INSTALL_INTERFACE:${HEADER_PATH}/${PROJECT_NAME}/${PROJECT_NAME}/logger/generated>
    $<INSTALL_INTERFACE:${HEADER_PATH}/${PROJECT_NAME}/${PROJECT_NAME}/logger/types>
    $<INSTALL_INTERFACE:${HEADER_PATH}/${PROJECT_NAME}/${PROJECT_NAME}/modbus>
    $<INSTALL_INTERFACE:${HEADER_PATH}/${PROJECT_NAME}/${PROJECT_NAME}/sys>
    $<INSTALL_INTERFACE:${HEADER_PATH}/${PROJECT_NAME}/${PROJECT_NAME}/tty>
    $<INSTALL_INTERFACE:${HEADER_PATH}/${PROJECT_NAME}/${PROJECT_NAME}/utils>
    $<INSTALL_INTERFACE:${HEADER_PATH}/${PROJECT_NAME}/lib/flatlogs/include>
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

# Header files
set(HEADER_FILES
    libMagAOX.hpp
    app/MagAOXApp.hpp
    app/indiDriver.hpp
    app/indiMacros.hpp
    app/indiUtils.hpp
    app/semUtils.hpp
    app/stateCodes.hpp
    app/dev/semUtilsDerived.hpp
    app/dev/outletController.hpp
    app/dev/ioDevice.hpp
    app/dev/stdMotionStage.hpp
    app/dev/frameGrabber.hpp
    app/dev/stdCamera.hpp
    app/dev/dssShutter.hpp
    app/dev/shmimMonitor.hpp
    app/dev/dm.hpp
    app/dev/telemeter.hpp
    app/dev/dmPokeWFS.hpp
    sys/runCommand.hpp
    common/config.hpp
    common/defaults.hpp
    common/environment.hpp
    ImageStreamIO/ImageStruct.hpp
    ImageStreamIO/pixaccess.hpp
    logger/logFileRaw.hpp
    logger/logManager.hpp
    logger/logFileName.hpp
    logger/logMap.hpp
    logger/logMeta.hpp
    logger/logBinarySchemata.hpp
    tty/ttyErrors.hpp
    tty/ttyIOUtils.hpp
    tty/ttyUSB.hpp
    tty/usbDevice.hpp
    tty/telnetConn.hpp
    tty/netSerial.hpp
    modbus/modbus.hpp
    modbus/modbus_exception.hpp
)


## Execute flatlogcodes and generate log types
###########################################################

# Find all .fbs files required by logTypes.hpp
file(GLOB FBS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/logger/types/schemas/*.fbs")

# If app directories exist at XWCTK_APPS_PATH, get the paths to their corresponding dat files and schema dirs
process_appdirs()

# If schema dirs founds, copy headers to equivalent path in build dir
if(DEFINED schemadirs)
    set(I 0)
    foreach(schemadir ${schemadirs})
        # schemadir points to logger/types/schema, but header files shoud be in logger/types
        get_filename_component(typesdir "${schemadir}" DIRECTORY)

        # Find all .hpp files in the types dir
        file(GLOB hpp_files "${typesdir}/*.hpp")

        set(DEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/logger/types/")
        add_custom_command(
            OUTPUT "${DEST_DIR}"
            COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy "${hpp_files}" "${DEST_DIR}"
            DEPENDS "${typesdir}"
            COMMENT "Copying ${typesdir} headers to ${DEST_DIR}"
        )        
        
        # Add telem.cpp file to sources
        if(EXISTS "${typesdir}/telem.cpp")
            list(APPEND SOURCES  "${typesdir}/telem.cpp")
        else()
            message(FATAL_ERROR "Telemetry schemas found in ${typesdir}/schemas, but no ${typesdir}/telem.cpp. File required.")
        endif()

        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/logger/generated)

        MATH(EXPR I "${I}+1")
    endforeach()
endif()
add_custom_target(copy_app_headers ALL DEPENDS "${DEST_DIR}")

# Build inputfiles arg - looks at the library's logCode.dat and any app ones
list(APPEND datfiles "logCodes.dat")

# Build schemadirs arg- looks at the library's types/schemas and any app ones
list(APPEND schemadirs "types/schemas")

# Make logger/types/generated dir
# In logger dir execute flatlogcodes - this reads the codes in logCodes.dat and generates
# the .inc & .hpp files for those codes in logger/generated
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/logTypes.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/binarySchemata.inc
            ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/logCodes.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/logStdFormat.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/logVerify.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/logCodeValid.hpp
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/logger/types/generated    
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/logger
    COMMAND ${CMAKE_COMMAND} -E echo "Executing flatlogcodes with inputfiles '${datfiles}' and schemadirs '${schemadirs}'"    
    COMMAND $<TARGET_FILE:flatlogcodes_build> --inputfiles ${datfiles} --schemadirs ${schemadirs}
    COMMAND ${CMAKE_COMMAND} -E echo "Generating header files"    
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR}/logger/types/generated bash -c "for bfb in *.bfbs; do xxd -i \$bfb > \$(basename \$bfb).h; done"
    COMMAND ${CMAKE_COMMAND} -E echo "Generatinc .inc file"
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR}/logger/types/generated bash -c "find . -name '*.bfbs.h' -print0 | xargs -0 cat > ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/binarySchemata.inc"
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/logger/logCodes.dat ${FBS_FILES} flatlogcodes_build
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
    OUTPUT "${CMAKE_BINARY_DIR}/${GIT_HEADER}"
    COMMAND ${CMAKE_COMMAND} -E echo "Generating ${GIT_HEADER}"  
    COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_SOURCE_DIR}" bash -c "${GENGITHEAD} ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}/${GIT_HEADER} XWCTK"
    COMMENT "Generated ${GIT_HEADER} header" 
    DEPENDS ${GENGITHEAD}
    VERBATIM
)

# Create a custom target for generating git header
add_custom_target(githeader DEPENDS "${CMAKE_BINARY_DIR}/${GIT_HEADER}")


## Add the XWCTK library
###########################################################
add_library(XWCTK ${SOURCES} 
                    ${HEADER_FILES}
                    ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/logCodes.hpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/logStdFormat.hpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/logVerify.hpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/logTypes.hpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/logger/generated/logCodeValid.hpp
                    )
target_include_directories(XWCTK PUBLIC
                            ${XWCTK_INCLUDE_DIR}
                            ${LIBUDEV_INCLUDE_DIRS}
                            )
target_link_libraries(XWCTK PUBLIC -Wl,--no-as-needed
                                Eigen3::Eigen
                                ${MXLIB}
                                ${LIBUDEV_LIBRARIES}
                                milk::ImageStreamIO
                                )

add_dependencies(XWCTK generated_logTypes githeader copy_app_headers)

target_compile_definitions(XWCTK PUBLIC MXLIB_MILK MAGAOX_NOEDT)
target_compile_definitions(XWCTK PUBLIC MAGAOX_path="${XWCTK_PREFIX}")

if (XWCTK_DISABLE_USER_CHECK)
    message(STATUS "Disabling user check")
    target_compile_definitions(XWCTK PRIVATE XWCTK_DISABLE_USER_CHECK)
endif()
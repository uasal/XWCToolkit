## Builds:
##   - logger/generated/logTypes.hpp (via flatlogcodes from logCodes.dat + schemas)
##   - logger/generated/binarySchemata.inc (via xxd + cat on .bfbs files)
##   - libMagAOX.hpp.gch (precompiled header)
##   - libMagAOX.a
##

message(STATUS "Starting XWCTk library configuration")

# ============================================================================
# Source Files
# ============================================================================

set(XWCTK_SOURCES
    app/MagAOXApp.cpp
    app/stateCodes.cpp
    app/dev/ioDevice.cpp
    app/dev/outletController.cpp
    app/dev/stdCamera.cpp
    file/stdFileName.cpp
    file/stdSubDir.cpp
    logger/logBinarySchemata.cpp
    logger/logFileRaw.cpp
    logger/logManager.cpp
    logger/logMap.cpp
    logger/logMeta.cpp
    logger/types/telem.cpp
    modbus/modbus.cpp
    sys/runCommand.cpp
    sys/thSetuid.cpp
    tty/netSerial.cpp
    tty/telnetConn.cpp
    tty/ttyErrors.cpp
    tty/ttyIOUtils.cpp
    tty/ttyUSB.cpp
    tty/usbDevice.cpp
)

# ============================================================================
# Generated Code Directories
# ============================================================================

set(LOGGER_SCHEMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/logger/types/schemas")
set(LOGGER_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/logger/generated")
set(TYPES_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/logger/types/generated")

file(MAKE_DIRECTORY ${LOGGER_GENERATED_DIR})
file(MAKE_DIRECTORY ${TYPES_GENERATED_DIR})

# ============================================================================
# FlatBuffers Schema Generation (.fbs -> _generated.h + .bfbs)
# ============================================================================

find_program(FLATC_EXECUTABLE flatc REQUIRED)

file(GLOB FBS_SCHEMAS "${LOGGER_SCHEMA_DIR}/*.fbs")

foreach(schema ${FBS_SCHEMAS})
    get_filename_component(schema_name ${schema} NAME_WE)
    set(generated_header "${LOGGER_GENERATED_DIR}/${schema_name}_generated.h")
    set(generated_bfbs "${TYPES_GENERATED_DIR}/${schema_name}.bfbs")
    
    add_custom_command(
        OUTPUT ${generated_header} ${generated_bfbs}
        COMMAND ${FLATC_EXECUTABLE} --cpp --gen-mutable -o ${LOGGER_GENERATED_DIR} ${schema}
        COMMAND ${FLATC_EXECUTABLE} --binary --schema -o ${TYPES_GENERATED_DIR} ${schema}
        DEPENDS ${schema}
        COMMENT "Generating FlatBuffers header and binary schema for ${schema_name}"
    )
    
    list(APPEND GENERATED_HEADERS ${generated_header})
    list(APPEND GENERATED_BFBS ${generated_bfbs})
endforeach()

# ============================================================================
# Binary Schemata Generation (.bfbs -> .bfbs.h -> binarySchemata.inc)
# ============================================================================

find_program(XXD_EXECUTABLE xxd REQUIRED)

set(BINARY_SCHEMATA_INC "${LOGGER_GENERATED_DIR}/binarySchemata.inc")

add_custom_command(
    OUTPUT ${BINARY_SCHEMATA_INC}
    COMMAND ${CMAKE_COMMAND} -E echo "// Auto-generated binary schemata" > ${BINARY_SCHEMATA_INC}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/generate_binary_schemata.sh
            ${TYPES_GENERATED_DIR} ${BINARY_SCHEMATA_INC} ${XXD_EXECUTABLE}
    DEPENDS ${GENERATED_BFBS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating binarySchemata.inc from .bfbs files"
)

# ============================================================================
# logTypes.hpp Generation (via flatlogcodes)
# ============================================================================

set(LOG_CODES_DAT "${CMAKE_CURRENT_SOURCE_DIR}/logger/logCodes.dat")
set(LOG_TYPES_HPP "${LOGGER_GENERATED_DIR}/logTypes.hpp")

add_custom_command(
    OUTPUT ${LOG_TYPES_HPP}
    COMMAND flatlogcodes
    DEPENDS ${LOG_CODES_DAT} ${FBS_SCHEMAS} flatlogcodes
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/logger
    COMMENT "Generating logTypes.hpp via flatlogcodes"
)

# Custom target for all generated code
add_custom_target(generate_logger_code
    DEPENDS ${GENERATED_HEADERS} ${LOG_TYPES_HPP} ${BINARY_SCHEMATA_INC}
)

# ============================================================================
# XWCTk Library Target
# ============================================================================

add_library(XWCTk ${XWCTK_SOURCES})

add_dependencies(XWCTk generate_logger_code git_version_header)

# Set library version
set_target_properties(XWCTk PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# ============================================================================
# Precompiled Header (PCH)
# ============================================================================

target_precompile_headers(XWCTk
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libMagAOX.hpp
)

# ============================================================================
# Include Directories
# ============================================================================

target_include_directories(XWCTk
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/logger>
        $<BUILD_INTERFACE:${LOGGER_GENERATED_DIR}>
        $<BUILD_INTERFACE:${TYPES_GENERATED_DIR}>
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/XWCTk>
)

# ============================================================================
# Link Libraries
# ============================================================================

target_link_libraries(XWCTk
    PUBLIC
        Eigen3::Eigen
        Threads::Threads
        OpenMP::OpenMP_CXX
        ZLIB::ZLIB
        PkgConfig::LIBUDEV
        ${MXLIB}
        flatlogs
        telnet
        indi_common
        lilxml
        xrif::xrif
        ${FLATBUFFERSLIB}
        PkgConfig::IMAGESTREAMIO
        ${CFITSIO_LIB}
)

# ============================================================================
# Compile Definitions
# ============================================================================

target_compile_definitions(XWCTk
    PUBLIC
        XWCTK_PREFIX="${XWCTK_PREFIX}"
        XWCTK_APPS_PATH="${XWCTK_APPS_PATH}"
        MXLIB_MILK  # Enable mxlib milk support (milkImage, etc.)
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

# ============================================================================
# Installation
# ============================================================================

install(TARGETS XWCTk
    EXPORT XWCTkTargets
    LIBRARY DESTINATION lib COMPONENT libraries
    ARCHIVE DESTINATION lib COMPONENT libraries
)

# Install main header
install(FILES libMagAOX.hpp
    DESTINATION include/XWCTk
    COMPONENT headers
)

# Install subdirectory headers (excluding tests)
install(DIRECTORY
    app/
    common/
    file/
    ImageStreamIO/
    logger/
    modbus/
    sys/
    tty/
    DESTINATION include/XWCTk
    COMPONENT headers
    FILES_MATCHING
        PATTERN "*.hpp"
        PATTERN "*.h"
        PATTERN "tests" EXCLUDE
        PATTERN "generated" EXCLUDE
)

# Install generated headers
install(DIRECTORY ${LOGGER_GENERATED_DIR}/
    DESTINATION include/XWCTk/logger/generated
    COMPONENT headers
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h" PATTERN "*.inc"
)

# ============================================================================
# Tests
# ============================================================================

if(BUILD_TESTING)
    # app/ tests
    add_xwctk_test(app MagAOXApp app/tests/MagAOXApp_test.cpp)
    add_xwctk_test(app MagAOXAppExecute app/tests/MagAOXAppExecute_test.cpp)
    add_xwctk_test(app indiUtils app/tests/indiUtils_test.cpp)
    add_xwctk_test(app semUtils app/tests/semUtils_test.cpp)
    add_xwctk_test(app stateCodes app/tests/stateCodes_test.cpp)

    # app/dev/ tests
    add_xwctk_test(app_dev dev app/dev/tests/dev_test.cpp)
    add_xwctk_test(app_dev dm app/dev/tests/dm_test.cpp)
    add_xwctk_test(app_dev outletController app/dev/tests/outletController_test.cpp)
    add_xwctk_test(app_dev telemeter app/dev/tests/telemeter_test.cpp)

    # common/ tests
    add_xwctk_test(common common common/tests/common_test.cpp)

    # file/ tests
    add_xwctk_test(file fileTimes file/tests/fileTimes_test.cpp)
    add_xwctk_test(file stdFileName file/tests/stdFileName_test.cpp)
    add_xwctk_test(file stdSubDir file/tests/stdSubDir_test.cpp)

    # ImageStreamIO/ tests
    add_xwctk_test(ImageStreamIO ImageStreamIO ImageStreamIO/tests/ImageStreamIO_test.cpp)

    # logger/ tests
    add_xwctk_test(logger catch2 logger/tests/catch2_tests.cpp)
    add_xwctk_test(logger logFileRaw logger/tests/logFileRaw_test.cpp)
    add_xwctk_test(logger logFileRaw_fileTime_error logger/tests/logFileRaw_fileTime_error_test.cpp)
    add_xwctk_test(logger logMap logger/tests/logMap_test.cpp)
    add_xwctk_test(logger logTypes_Accessor logger/tests/logTypes_Accessor_test.cpp)

    # modbus/ tests
    add_xwctk_test(modbus modbus modbus/tests/modbus_test.cpp)

    # sys/ tests
    add_xwctk_test(sys runCommand sys/tests/runCommand_test.cpp)
    add_xwctk_test(sys thSetuid sys/tests/thSetuid_test.cpp)

    # tty/ tests
    add_xwctk_test(tty tty tty/tests/tty_test.cpp)
    add_xwctk_test(tty ttyIOUtils tty/tests/ttyIOUtils_test.cpp)
endif()

message(STATUS "XWCTk library configuration complete")

# ============================================================================
# indiserver - INDI server
# ============================================================================

add_executable(indiserver
    indiserver.c
    strcat_varargs.c
    open_named_fifo.c
    fq.c
)

# Get git tag for version info
execute_process(
    COMMAND git describe --tags
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if(NOT GIT_TAG)
    set(GIT_TAG "unknown")
endif()

target_compile_definitions(indiserver PRIVATE
    GIT_TAG_STRING="${GIT_TAG}"
)

target_include_directories(indiserver PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../liblilxml
)
target_link_libraries(indiserver PRIVATE lilxml ZLIB::ZLIB)

# ============================================================================
# getINDI - Get INDI property values
# ============================================================================

add_executable(getINDI 
    getINDI.c
    connect_to.c
)
target_include_directories(getINDI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../liblilxml
)
target_link_libraries(getINDI PRIVATE lilxml ZLIB::ZLIB)

# ============================================================================
# setINDI - Set INDI property values
# ============================================================================

add_executable(setINDI 
    setINDI.c
    connect_to.c
)
target_include_directories(setINDI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../liblilxml
)
target_link_libraries(setINDI PRIVATE lilxml)

# ============================================================================
# evalINDI - Evaluate INDI expressions
# ============================================================================

add_executable(evalINDI 
    evalINDI.c
    connect_to.c
    compiler.c
)
target_include_directories(evalINDI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../liblilxml
)
target_link_libraries(evalINDI PRIVATE lilxml m)

install(TARGETS indiserver getINDI setINDI evalINDI
    RUNTIME DESTINATION bin COMPONENT tools
)

# ============================================================================
# INDI headers
# ============================================================================

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/XWCTk/lib/INDI
    COMPONENT libraries
    FILES_MATCHING PATTERN "*.h"
)

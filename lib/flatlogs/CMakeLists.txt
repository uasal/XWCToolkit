## Provides:
##   - flatlogs: INTERFACE (header-only) library for log serialization
##   - flatlogcodes: Executable tool for generating log type code from schemas

# ============================================================================
# Header-only library
# ============================================================================

add_library(flatlogs INTERFACE)

target_include_directories(flatlogs
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include/XWCTk/lib/flatlogs>
)

# Flatlogs requires FlatBuffers
target_link_libraries(flatlogs INTERFACE ${FLATBUFFERSLIB})
target_include_directories(flatlogs INTERFACE ${FLATBUFFERS_HEADERS})

install(TARGETS flatlogs
    EXPORT XWCTkTargets
)

install(
    DIRECTORY include/
    DESTINATION include/XWCTk/lib/flatlogs
    COMPONENT libraries
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# ============================================================================
# flatlogcodes tool
# ============================================================================
# Installs to ${CMAKE_INSTALL_PREFIX}/bin

add_executable(flatlogcodes src/flatlogcodes.cpp)
target_compile_features(flatlogcodes PRIVATE cxx_std_17)

install(TARGETS flatlogcodes
    RUNTIME DESTINATION bin
    COMPONENT tools
)

# ============================================================================
# Tests
# ============================================================================

if(BUILD_TESTING)
    add_executable(flatlogs_test tests/flatlogcodes_test.cpp)
    target_link_libraries(flatlogs_test PRIVATE flatlogs)
    target_include_directories(flatlogs_test PRIVATE ${CATCH2_INCLUDE_DIR})
    add_test(NAME lib::flatlogs COMMAND flatlogs_test)
    message(STATUS "Test registered: lib::flatlogs")
endif()
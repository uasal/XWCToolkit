# ============================================================================
# cursesINDI - Curses-based INDI control interface
# ============================================================================

add_utility(cursesINDI
    SOURCES cursesINDI.cpp
    DEPENDENCIES XWCTk ${CURSES_LIBRARIES}
    INCLUDES ${CURSES_INCLUDE_DIRS}
)

# ============================================================================
# Tests
# ============================================================================

if(BUILD_TESTING)
    add_utils_test(cursesINDI cursesINDI tests/cursesINDI_test.cpp
        DEPENDENCIES ${CURSES_LIBRARIES}
        INCLUDES ${CURSES_INCLUDE_DIRS}
    )
endif()

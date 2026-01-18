# ============================================================================
# logdump - Display binary log files
# ============================================================================

# Needs mxlib and flatbuffers which are dependencies of XWCTk
add_utility(logdump
    SOURCES logdump.cpp
    DEPENDENCIES XWCTk
)

# ============================================================================
# Tests
# ============================================================================

if(BUILD_TESTING)
    add_utils_test(logdump logdump tests/logdump_test.cpp)
endif()
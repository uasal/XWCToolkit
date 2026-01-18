# ============================================================================
# logsurgeon - Repair and analyze log files
# ============================================================================

# Needs mxlib and flatbuffers which are dependencies of XWCTk
add_utility(logsurgeon
    SOURCES logsurgeon.cpp
    DEPENDENCIES XWCTk
)

# ============================================================================
# Tests
# ============================================================================

if(BUILD_TESTING)
    add_utils_test(logsurgeon logsurgeon_config tests/logsurgeon_config_test.cpp)
endif()
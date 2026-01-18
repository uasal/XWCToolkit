# ============================================================================
# shmimLat - Shared memory image latency utility
# ============================================================================

add_utility(shmimLat
    SOURCES shmimLat.cpp
    DEPENDENCIES XWCTk
)


# ============================================================================
# Tests
# ============================================================================

if(BUILD_TESTING)
    add_utils_test(shmimLat shmimLat tests/shmimLat_test.cpp)
endif()

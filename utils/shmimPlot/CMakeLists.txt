# ============================================================================
# shmimPlot - Shared memory image plotting utility
# ============================================================================

add_utility(shmimPlot
    SOURCES shmimPlot.cpp
    DEPENDENCIES XWCTk
)

# ============================================================================
# Tests
# ============================================================================

if(BUILD_TESTING)
    add_utils_test(shmimPlot shmimPlot tests/shmimPlot_test.cpp)
endif()

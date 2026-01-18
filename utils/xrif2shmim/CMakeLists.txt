# ============================================================================
# xrif2shmim - Stream xrif images to shared memory
# ============================================================================

add_utility(xrif2shmim
    SOURCES xrif2shmim.cpp
    DEPENDENCIES XWCTk ${CFITSIO_LIB}
    INCLUDES ${CFITSIO_INCLUDE_DIR}
)

# ============================================================================
# Tests
# ============================================================================

if(BUILD_TESTING)
    add_utils_test(xrif2shmim xrif2shmim tests/xrif2shmim_test.cpp
        DEPENDENCIES ${CFITSIO_LIB}
        INCLUDES ${CFITSIO_INCLUDE_DIR}
    )
endif()

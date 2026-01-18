# ============================================================================
# xrif2fits - Convert xrif images to FITS
# ============================================================================

add_utility(xrif2fits
    SOURCES xrif2fits.cpp
    DEPENDENCIES XWCTk ${CFITSIO_LIB}
    INCLUDES ${CFITSIO_INCLUDE_DIR}
)

# ============================================================================
# Tests
# ============================================================================

if(BUILD_TESTING)
    add_utils_test(xrif2fits xrif2fits tests/xrif2fits_test.cpp
        DEPENDENCIES ${CFITSIO_LIB}
        INCLUDES ${CFITSIO_INCLUDE_DIR}
    )
endif()

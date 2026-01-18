# ============================================================================
# resurrector_indi - INDI resurrection utility
# ============================================================================

add_utility(resurrector_indi
    SOURCES
        resurrector_indi.cpp
        redirect_prototype.cpp
    DEPENDENCIES XWCTk
    INCLUDES
        ${CMAKE_SOURCE_DIR}/XWCTk/common
        ${CMAKE_SOURCE_DIR}/lib/INDI/INDI
)

# Test not registered since it is more of an integration test
# and requires a more complex setup than the current pattern.
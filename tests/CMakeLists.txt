## XWCToolkit Tests
##
## Unit and integration tests using Catch2 framework
##

# ============================================================================
# Test Configuration
# ============================================================================

# Catch2 is header-only and included in tests/catch2/
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ============================================================================
# Test Targets
# ============================================================================

# Main test executable
file(GLOB TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Create test executable if sources exist
if(TEST_SOURCES)
    add_executable(xwctk_tests ${TEST_SOURCES})
    
    target_include_directories(xwctk_tests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    target_link_libraries(xwctk_tests
        PRIVATE XWCTk
    )
    
    # Register with ctest
    add_test(
        NAME XWCTkTests
        COMMAND xwctk_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    message(STATUS "Test registered: XWCTk::tests")
else()
    message(STATUS "No test sources found - skipping test build")
endif()

# ============================================================================
# Coverage Target (Optional)
# ============================================================================

if(COVERAGE)
    # Generate coverage report
    add_custom_target(coverage
        COMMAND lcov --directory . --capture --output-file coverage.info
        COMMAND genhtml coverage.info --output-directory coverage_html
        DEPENDS xwctk_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating code coverage report"
    )
endif()

message(STATUS "XWCToolkit tests configuration complete")

# ---------------------------------------------------------------------------
# Install test support headers (Catch2 and project test helpers)
# ---------------------------------------------------------------------------
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/catch2/"
    DESTINATION "include/XWCTk/tests/catch2"
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

install(
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/testMacrosINDI.hpp"
    DESTINATION "include/XWCTk/tests"
)
add_subdirectory(liblilxml)
add_subdirectory(libcommon)
add_subdirectory(INDI)
add_subdirectory(xindidriver)

# ============================================================================
# INDI interface library
# ============================================================================

add_library(indi INTERFACE)
target_link_libraries(indi INTERFACE indi_common lilxml)
target_include_directories(indi
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/INDI>
        $<INSTALL_INTERFACE:include/XWCTk/lib/INDI>
)

# ============================================================================
# Tests
# ============================================================================

if(BUILD_TESTING)
    add_executable(indi_test tests/INDI_test.cpp)
    target_link_libraries(indi_test PRIVATE indi indi_common lilxml flatlogs)
    target_include_directories(indi_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/INDI
        ${CMAKE_CURRENT_SOURCE_DIR}/libcommon
        ${CMAKE_CURRENT_SOURCE_DIR}/liblilxml
        ${CATCH2_INCLUDE_DIR}
    )
    add_test(NAME lib::INDI COMMAND indi_test)
    message(STATUS "Test registered: lib::INDI")
endif()

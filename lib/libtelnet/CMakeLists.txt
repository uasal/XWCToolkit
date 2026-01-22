# ============================================================================
# Telnet library
# ============================================================================

add_library(telnet
    libtelnet.c
)

target_include_directories(telnet
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/XWCTk/lib/libtelnet>
)

install(TARGETS telnet
    EXPORT XWCTkTargets
    LIBRARY DESTINATION lib COMPONENT libraries
    ARCHIVE DESTINATION lib COMPONENT libraries
)

install(FILES libtelnet.h
    DESTINATION include/XWCTk/lib/libtelnet
    COMPONENT libraries
)

# ============================================================================
# Tests
# ============================================================================

if(BUILD_TESTING)
    add_executable(libtelnet_test tests/libtelnet_test.cpp)
    target_link_libraries(libtelnet_test PRIVATE telnet flatlogs)
    target_include_directories(libtelnet_test PRIVATE ${CATCH2_INCLUDE_DIR})
    add_test(NAME lib::telnet COMMAND libtelnet_test)
    message(STATUS "Test registered: lib::telnet")
endif()

add_library(lilxml
    lilxml.c
    base64.c
)

target_include_directories(lilxml
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/XWCTk/lib/INDI>
)

install(TARGETS lilxml
    EXPORT XWCTkTargets
    LIBRARY DESTINATION lib/XWCTk COMPONENT libraries
    ARCHIVE DESTINATION lib/XWCTk COMPONENT libraries
)

install(FILES lilxml.h base64.h
    DESTINATION include/XWCTk/lib/INDI
    COMPONENT libraries
)

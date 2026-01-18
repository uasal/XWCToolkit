add_library(indi_common
    Cmd.cpp
    IndiConnection.cpp
    IndiClient.cpp
    IndiDriver.cpp
    IndiElement.cpp
    IndiMessage.cpp
    IndiProperty.cpp
    IndiPropertyMap.cpp
    IndiXmlParser.cpp
    System.cpp
    SystemSocket.cpp
    Thread.cpp
    TimeStamp.cpp
)

target_include_directories(indi_common
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/XWCTk/lib/INDI>
)

target_link_libraries(indi_common PUBLIC lilxml)

install(TARGETS indi_common
    EXPORT XWCTkTargets
    ARCHIVE DESTINATION lib/XWCTk COMPONENT libraries
)

# Install libcommon headers
file(GLOB LIBCOMMON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
install(FILES ${LIBCOMMON_HEADERS}
    DESTINATION include/XWCTk/lib/INDI
    COMPONENT libraries
)

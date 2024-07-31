## Require mxlib library & headers
require_mxlib()
## Require flatbuffers library
require_flatbuffers()
## Require libudev library
require_libudev()

add_executable(logstream logstream.cpp)
target_link_libraries(logstream PRIVATE XWC
                                    XWC_precompiled_header 
                                    libcommon
                                    libtelnet
                                    liblilxml
                                    ${LIBUDEV_LIBRARIES}
                                    ${FLATBUFFERSLIB}
                                    ${MXLIB})
target_include_directories(logstream PRIVATE 
                            ${FLATBUFFERS_HEADERS}
                            ${CMAKE_CURRENT_SOURCE_DIR}
                            ${LIBUDEV_INCLUDE_DIRS}
                            )
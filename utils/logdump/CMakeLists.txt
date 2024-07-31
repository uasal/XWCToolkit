## Require mxlib library & headers
require_mxlib()
## Require flatbuffers library
require_flatbuffers()
## Require libudev library
require_libudev()

add_executable(logdump logdump.cpp)
target_link_libraries(logdump PRIVATE XWC
                                    XWC_precompiled_header 
                                    libcommon
                                    libtelnet
                                    liblilxml
                                    ${LIBUDEV_LIBRARIES}
                                    ${FLATBUFFERSLIB}
                                    ${MXLIB})
target_include_directories(logdump PRIVATE 
                            ${FLATBUFFERS_HEADERS}
                            ${CMAKE_CURRENT_SOURCE_DIR}
                            ${LIBUDEV_INCLUDE_DIRS}                            
                            )
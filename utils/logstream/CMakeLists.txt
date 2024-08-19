add_executable(logstream logstream.cpp)
target_link_libraries(logstream PRIVATE XWC
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
add_executable(logsurgeon logsurgeon.cpp)
target_link_libraries(logsurgeon PRIVATE XWCTK
                                    common
                                    telnet
                                    lilxml
                                    ${LIBUDEV_LIBRARIES}
                                    ${FLATBUFFERSLIB})
target_include_directories(logsurgeon PRIVATE 
                            ${FLATBUFFERS_HEADERS}
                            ${CMAKE_CURRENT_SOURCE_DIR}
                            ${LIBUDEV_INCLUDE_DIRS}
                            )
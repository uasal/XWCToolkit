add_executable(logdump logdump.cpp)
target_link_libraries(logdump PRIVATE XWCTK
                                    common
                                    telnet
                                    lilxml
                                    ${LIBUDEV_LIBRARIES}
                                    ${FLATBUFFERSLIB})
target_include_directories(logdump PRIVATE 
                            ${FLATBUFFERS_HEADERS}
                            ${CMAKE_CURRENT_SOURCE_DIR}
                            ${LIBUDEV_INCLUDE_DIRS}                            
                            )
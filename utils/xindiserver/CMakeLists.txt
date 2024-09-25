add_executable(xindiserver xindiserver.cpp)

target_link_libraries(xindiserver PRIVATE XWCTK
                                        common
                                        telnet
                                        lilxml)

add_dependencies(xindiserver githeader)
target_include_directories(xindiserver PRIVATE 
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                            $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                            )
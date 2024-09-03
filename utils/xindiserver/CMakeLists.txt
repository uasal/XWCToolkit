add_executable(xindiserver xindiserver.cpp)

target_link_libraries(xindiserver PRIVATE XWCTK
                                        libcommon
                                        libtelnet
                                        liblilxml)
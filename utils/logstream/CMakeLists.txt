add_executable(logstream logstream.cpp)
target_link_libraries(logstream PRIVATE XWCTK
                                    common
                                    telnet
                                    lilxml
                                    ${LIBUDEV_LIBRARIES}
                                    ${FLATBUFFERSLIB})
target_include_directories(logstream PRIVATE 
                            ${FLATBUFFERS_HEADERS}
                            ${CMAKE_CURRENT_SOURCE_DIR}
                            ${LIBUDEV_INCLUDE_DIRS}
                            )

# Define preprocessor macro with value from XWCTK_LOG_PATH
target_compile_definitions(logstream PRIVATE XWCTK_LOG_PATH="${XWCTK_LOG_PATH}")
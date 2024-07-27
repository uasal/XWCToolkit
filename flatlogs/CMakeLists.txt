# Create the library
add_library(flatlogslib src/flatlogcodes.cpp)

# Create the executable
add_executable(flatlogcodes src/flatlogcodes.cpp)

# Include directories
target_include_directories(flatlogslib PRIVATE 
                            ${CMAKE_CURRENT_SOURCE_DIR}/src
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/flatlogs)
target_include_directories(flatlogcodes PRIVATE 
                            ${CMAKE_CURRENT_SOURCE_DIR}/src
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/flatlogs)

# # Install - is the destination okay, or should it go in /usr/local or $BIN_PATH?
# # Actually, is this even needed?
# install(TARGETS flatlogcodes DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_INSTALL_BINDIR})

# Install library
install(
        TARGETS flatlogslib
        LIBRARY DESTINATION ${LIB_PATH}
        )
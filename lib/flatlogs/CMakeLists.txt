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
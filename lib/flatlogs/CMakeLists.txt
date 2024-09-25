# Create the library
add_library(flatlogslib src/flatlogcodes.cpp)

# Create one executable to use during build with the build FLATLOGS_PATH
# and another one with the install one
add_executable(flatlogcodes_build src/flatlogcodes.cpp)
add_executable(flatlogcodes src/flatlogcodes.cpp)

# Include directories
target_include_directories(flatlogslib PRIVATE 
                            ${CMAKE_CURRENT_SOURCE_DIR}/src
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/flatlogs)
target_include_directories(flatlogcodes_build PRIVATE 
                            ${CMAKE_CURRENT_SOURCE_DIR}/src
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/flatlogs)
target_include_directories(flatlogcodes PRIVATE 
                            ${CMAKE_CURRENT_SOURCE_DIR}/src
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/flatlogs)

# Keep existing name over cmake's convention
set_target_properties(flatlogslib PROPERTIES OUTPUT_NAME "flatlogslib")

target_compile_definitions(flatlogcodes_build PRIVATE FLATLOGS_PATH="../../..")
target_compile_definitions(flatlogcodes PRIVATE FLATLOGS_PATH="${PROJECT_NAME}")
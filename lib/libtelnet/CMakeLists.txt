# Create the shared library
add_library(libtelnet libtelnet.c)

target_include_directories(libtelnet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
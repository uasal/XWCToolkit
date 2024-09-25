# Create the shared library
add_library(telnet libtelnet.c)

target_include_directories(telnet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
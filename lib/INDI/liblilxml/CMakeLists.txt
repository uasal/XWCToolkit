# Create the library
add_library(lilxml lilxml.c base64.c)
target_include_directories(lilxml PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
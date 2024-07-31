# Create the library
add_library(liblilxml lilxml.c base64.c)
target_include_directories(liblilxml PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
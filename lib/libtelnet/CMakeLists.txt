# Create the shared library
add_library(libtelnet libtelnet.c)

target_include_directories(libtelnet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Define the installation rules
install(TARGETS libtelnet LIBRARY DESTINATION ${LIB_PATH})
install(TARGETS libtelnet ARCHIVE DESTINATION ${LIB_PATH})
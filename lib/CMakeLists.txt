message(STATUS "Configuring lib...")

add_subdirectory(flatlogs)
add_subdirectory(INDI)
add_subdirectory(libtelnet)

message(STATUS "lib configuration complete")
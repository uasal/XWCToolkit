## Require ncurses library
find_package(Curses REQUIRED)

add_executable(cursesINDI cursesINDI.cpp cursesTableGrid.hpp)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(cursesINDI ${CURSES_LIBRARIES} common lilxml)
target_compile_options(cursesINDI PRIVATE -ggdb) # optimize

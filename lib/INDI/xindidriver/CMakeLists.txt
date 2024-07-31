# Create the executable target
add_executable(xindidriver xindidriver.cpp)

# Link pthread
find_package(Threads REQUIRED)
target_link_libraries(xindidriver PRIVATE Threads::Threads)

# Define preprocessor macro with value from FIFO_PATH
target_compile_definitions(xindidriver PRIVATE XINDID_FIFODIR="${FIFO_PATH}")
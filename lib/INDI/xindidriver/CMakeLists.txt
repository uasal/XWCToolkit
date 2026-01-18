add_executable(xindidriver
    xindidriver.cpp
)

target_compile_definitions(xindidriver PRIVATE
    XINDID_FIFODIR="${XWCTK_FIFO_PATH}"
)

target_link_libraries(xindidriver PRIVATE Threads::Threads)

install(TARGETS xindidriver
    RUNTIME DESTINATION bin COMPONENT tools
)

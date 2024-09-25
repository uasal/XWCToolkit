# indiserver
# Define the executable
add_executable(indiserver indiserver.c fq.c)
# Include directories
target_include_directories(indiserver PRIVATE 
                            ${CMAKE_CURRENT_SOURCE_DIR}/../liblilxml)
# Link library
target_link_libraries(indiserver PRIVATE lilxml)
# Link pthread
find_package(Threads REQUIRED)
if(NOT Threads_FOUND)
    message(FATAL_ERROR "pthread not found. Please install libpthread-stubs0-dev before proceeding.")
endif()
target_link_libraries(indiserver PRIVATE Threads::Threads)

                    
# getINDI
# Define the executable
add_executable(getINDI getINDI.c connect_to.c indiapi.h)
# Include directories
target_include_directories(getINDI PRIVATE 
                            ${CMAKE_CURRENT_SOURCE_DIR}/../liblilxml)
# Link library
target_link_libraries(getINDI PRIVATE lilxml)
# Link zlib
find_package(ZLIB REQUIRED)
if(NOT ZLIB_FOUND)
    message(FATAL_ERROR "zlib not found. Please install zlib1g-dev before proceeding.")
endif()
target_link_libraries(getINDI PRIVATE ZLIB::ZLIB)


# setINDI
# Define the executable
add_executable(setINDI setINDI.c indiapi.h connect_to.c)
# Include directories
target_include_directories(setINDI PRIVATE 
                            ${CMAKE_CURRENT_SOURCE_DIR}/../liblilxml)
# Link library
target_link_libraries(setINDI PRIVATE lilxml)

                    
# evalINDI
# Define the executable
add_executable(evalINDI evalINDI.c connect_to.c compiler.c)
# Include directories
target_include_directories(evalINDI PRIVATE 
                            ${CMAKE_CURRENT_SOURCE_DIR}/../liblilxml)
# Link library
target_link_libraries(evalINDI PRIVATE lilxml)
# Link libm
target_link_libraries(evalINDI PRIVATE m)

########################################
## Libraries
#######################################

# Macro to add common libraries to a target
# This needs to called after defining the executable, e.g.:
# add_common_libraries(my_executable)
macro(add_common_libraries target)
    # Required system libraries
    target_link_libraries(${target} PRIVATE 
                            mxlib
                            udev
                            pthread
                            telnet
                            cfitsio
                            xrif
                            fftw3
                            fftw3f
                            fftw3l
                            gsl
                            Boost::system
                            Boost::filesystem
                            )

    # Required local libraries
    target_link_libraries(${target} PRIVATE 
                        ${CMAKE_SOURCE_DIR}/../INDI/libcommon/libcommon.a
                        ${CMAKE_SOURCE_DIR}/../INDI/liblilxml/liblilxml.a
                        )

    # Only enable on x86 because ARM doesn't have quad precision by default
    if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
        # Link additional libraries specific to x86_64
        target_link_libraries(${target} PRIVATE lfftw3q)
    endif()  
    
    # OpenBLAS
    # Variables were set earlier with pkg_check_modules
    target_link_libraries(${target} PRIVATE ${BLAS_LIBRARIES})
    set_target_properties(${target} PROPERTIES LINK_FLAGS "${BLAS_LDFLAGS}")
    link_directories(${BLAS_LIBRARY_DIRS})

    # Hard-code the paths to system libraries so setuid works
    # Retrieve LD_LIBRARY_PATH from environment
    set(LD_LIBRARY_PATH $ENV{LD_LIBRARY_PATH} PARENT_SCOPE)
    # Process LD_LIBRARY_PATH to create rpath entries
    if(LD_LIBRARY_PATH)
        # Replace colons with spaces for rpath options
        string(REPLACE ":" " " LD_LIBRARY_PATH_LIST ${LD_LIBRARY_PATH})
        # Create rpath options
        foreach(path IN LISTS LD_LIBRARY_PATH_LIST)
            target_link_options(${target} PRIVATE "-Wl,-rpath,${path}")
        endforeach()
    endif()
    
endmacro()


# Macro to be used to build each target
macro(build_xwc_target source)
    get_filename_component(basename ${source} NAME_WE)
    get_filename_component(pathname ${source} DIRECTORY)
    
    # Allow for building .cpp and .c files.
    # Add more extensions here if necessary
    set(extensions ".cpp" ".c")
    
    # Check each extension and add the file if it exists    
    set(sources "")
    foreach(ext IN LISTS extensions)
        file(GLOB potential_sources "${pathname}/${basename}${ext}")
        list(APPEND sources ${potential_sources})
    endforeach()

    set(targetname "${pathname}/${basename}" PARENT_SCOPE)

    add_executable(${targetname} ${sources})

    # Set precompiled libMagAOX header
    target_precompile_headers(${targetname} PRIVATE "${CMAKE_SOURCE_DIR}/../libMagAOX/libMagAOX.hpp")

    target_link_libraries(${targetname} ${CMAKE_SOURCE_DIR}/../libMagAOX/libMagAOX.a)
    add_common_libraries(${targetname})

    # Optionally, add other headers and object files if provided
    if(ARGC GREATER 1)
        foreach(arg IN ITEMS ${ARGN})
            if("${arg}" MATCHES ".*\\.hpp$")
                target_include_directories(${target_name} PRIVATE ${arg})
            elseif("${arg}" MATCHES ".*\\.o$")
                target_sources(${target_name} PRIVATE ${arg})
            endif()
        endforeach()
    endif()

    # Add custom command for generating the git version header
    add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/../magaox_git_version.h
        COMMAND ${CMAKE_SOURCE_DIR}/gengithead.sh ${CMAKE_SOURCE_DIR}/../ ${CMAKE_SOURCE_DIR}/../magaox_git_version.h MAGAOX
        DEPENDS ${CMAKE_SOURCE_DIR}/gengithead.sh
        COMMENT "Generating git version header"
    )

    # Add custom target for cleaning
    add_custom_target(clean
        COMMAND ${CMAKE_COMMAND} -E rm -f ${targetname}
        COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_SOURCE_DIR}/../magaox_git_version.h
        COMMAND ${CMAKE_COMMAND} -E rm -f *.o
        COMMAND ${CMAKE_COMMAND} -E rm -f *~
        COMMENT "Cleaning project"
    )

    # Optionally set debug flags if needed
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${target_name} PRIVATE -g -O0)
    endif()
endmacro()
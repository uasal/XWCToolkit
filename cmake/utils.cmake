## Add a utility executable with standard configuration
##
## Usage:
##   add_utility(logdump
##       SOURCES logdump/logdump.cpp
##       DEPENDENCIES XWCTk Curses
##       OPTIONAL
##   )
function(add_utility target_name)
    set(options OPTIONAL)
    set(oneValueArgs)
    set(multiValueArgs SOURCES DEPENDENCIES INCLUDES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ARG_SOURCES)
        message(FATAL_ERROR "add_utility: ${target_name} requires SOURCES")
    endif()

    # Skip if optional and dependencies not found
    if(ARG_OPTIONAL AND ARG_DEPENDENCIES)
        foreach(dep ${ARG_DEPENDENCIES})
            if(NOT TARGET ${dep} AND NOT ${dep}_FOUND)
                message(STATUS "Skipping optional utility ${target_name} (missing ${dep})")
                return()
            endif()
        endforeach()
    endif()

    # Create executable
    add_executable(${target_name} ${ARG_SOURCES})

    # Link dependencies
    if(ARG_DEPENDENCIES)
        target_link_libraries(${target_name} PRIVATE ${ARG_DEPENDENCIES})
    endif()

    # Include directories
    if(ARG_INCLUDES)
        target_include_directories(${target_name} PRIVATE ${ARG_INCLUDES})
    endif()

    # Standard C++17
    target_compile_features(${target_name} PRIVATE cxx_std_17)

    # Install
    install(TARGETS ${target_name}
        RUNTIME DESTINATION bin COMPONENT utilities
    )

    message(STATUS "Added utility: ${target_name}")
endfunction()


## Add an XWCTk test executable with standard configuration
##
## Usage:
##   add_xwctk_test(app MagAOXApp app/tests/MagAOXApp_test.cpp)
##
## Creates test with name XWCTk::<module>::<test_name>
function(add_xwctk_test module_name test_name test_source)
    set(target_name "XWCTk_${module_name}_${test_name}")
    add_executable(${target_name} ${test_source})
    target_link_libraries(${target_name} PRIVATE XWCTk)
    target_include_directories(${target_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CATCH2_INCLUDE_DIR}
    )
    # Disable precompiled headers for tests since they need to define macros
    # (like OUTLET_CTRL_TEST_NOLOG) before including headers
    set_target_properties(${target_name} PROPERTIES
        DISABLE_PRECOMPILE_HEADERS ON
    )
    add_test(NAME XWCTk::${module_name}::${test_name} COMMAND ${target_name})
    message(STATUS "Test registered: XWCTk::${module_name}::${test_name}")
endfunction()


## Add a utility test executable with standard configuration
##
## Usage:
##   add_utils_test(logdump logdump tests/logdump_test.cpp)
##   add_utils_test(cursesINDI cursesINDI tests/cursesINDI_test.cpp DEPENDENCIES ${CURSES_LIBRARIES})
##
## Creates test with name utils::<utility>::<test_name>
function(add_utils_test utility_name test_name test_source)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs DEPENDENCIES INCLUDES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    set(target_name "utils_${utility_name}_${test_name}")
    add_executable(${target_name} ${test_source})
    target_link_libraries(${target_name} PRIVATE XWCTk)
    
    # Link additional dependencies if provided
    if(ARG_DEPENDENCIES)
        target_link_libraries(${target_name} PRIVATE ${ARG_DEPENDENCIES})
    endif()
    
    target_include_directories(${target_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CATCH2_INCLUDE_DIR}
    )
    
    # Include additional directories if provided
    if(ARG_INCLUDES)
        target_include_directories(${target_name} PRIVATE ${ARG_INCLUDES})
    endif()
    
    # Disable precompiled headers for tests
    set_target_properties(${target_name} PROPERTIES
        DISABLE_PRECOMPILE_HEADERS ON
    )
    add_test(NAME utils::${utility_name}::${test_name} COMMAND ${target_name})
    message(STATUS "Test registered: utils::${utility_name}::${test_name}")
endfunction()

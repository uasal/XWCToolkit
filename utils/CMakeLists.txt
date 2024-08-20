# Find all directories
file(GLOB UTILDIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*")

# Add all directories
foreach(subdir ${UTILDIRS})
    message(STATUS "Looking at subdir ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}")
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}")
    message(STATUS "Adding subdirectory ${subdir}")
    add_subdirectory("${subdir}")    
        # if(NOT subdir STREQUAL "instGraph")
        #     message(STATUS "Adding subdirectory ${subdir}")
        #     add_subdirectory("${subdir}")    
        # endif()
    endif()
endforeach()


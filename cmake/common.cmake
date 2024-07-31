
###############################################
## Function to find & add package via PkgConfig
###############################################

find_package(PkgConfig REQUIRED)

# function(find_and_add_library)
#     set(options)
#     set(oneValueArgs TARGET LIB LINK_LIB INCLUDE_DIR)
#     set(multiValueArgs)    
#     cmake_parse_arguments(find_and_add_library
#                         "${options}"
#                         "${oneValueArgs}"    
#                         "${multiValueArgs}"
#                         ${ARGN}
#                         )

#     # Check for the library using pkg_check_modules
#     pkg_check_modules(${find_and_add_library_LIB} REQUIRED ${find_and_add_library_LIB})

#     # Check if the library was found
#     if (NOT ${find_and_add_library_LIB}_FOUND)
#         message(FATAL_ERROR "PkgConfig could not find the ${find_and_add_library_LIB} library")
#     endif()

#     # Link the library to the target
#     if (find_and_add_library_LINK_LIB)
#         target_link_libraries(${find_and_add_library_TARGET} PRIVATE ${${find_and_add_library_LIB}_LIBRARIES})
#     endif()

#     # Include the library's include directories
#     if (find_and_add_library_INCLUDE_DIR)
#         target_include_directories(${find_and_add_library_TARGET} PRIVATE ${${find_and_add_library_LIB}_INCLUDE_DIRS})
#     endif()

#     # Optionally add additional compile definitions or flags
#     if (find_and_add_library_LINK_LIB OR find_and_add_library_INCLUDE_DIR)
#         target_compile_definitions(${find_and_add_library_TARGET} PRIVATE ${${find_and_add_library_LIB}_CFLAGS_OTHER})
#     endif()
# endfunction()


# # Eigen3
# pkg_check_modules(EIGEN3 REQUIRED eigen3)
# include_directories(${EIGEN3_INCLUDE_DIRS})

# # OpenBLAS
# pkg_check_modules(BLAS REQUIRED openblas)
# include_directories(${BLAS_INCLUDE_DIRS})

########################################
## Function to add compile flags
#######################################

function(add_compile_flags_to_all_targets flags)
    get_property(all_targets GLOBAL PROPERTY TARGETS)
    foreach(target IN LISTS targets)
        if(TARGET ${target})
            target_compile_options(${target} PRIVATE ${flags})
        endif()
    endforeach()
endfunction()


################################################
## Function to turn on position independent code
################################################

function(set_position_independent_code_for_all_targets)
    get_property(all_targets GLOBAL PROPERTY TARGETS)
    foreach(target IN LISTS targets)
        if(TARGET ${target})
            set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE ON)
        endif()
    endforeach()
endfunction()


################################################
## Function to hyperlink
################################################
function(hyperlink_targets)
    set(options)
    set(oneValueArgs DESTINATION)
    set(multiValueArgs TARGETS)
    cmake_parse_arguments(hyperlink_targets
                        "${options}"
                        "${oneValueArgs}"    
                        "${multiValueArgs}"
                        ${ARGN}
                        )
                        
    foreach(target IN LISTS hyperlink_targets_TARGETS)
        if (hyperlink_targets_DESTINATION)
            install(CODE "
                    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink \$<TARGET_FILE:${target}> ${DESTINATION}/${target})
                    ")
        endif()    
    endforeach()      
endfunction()
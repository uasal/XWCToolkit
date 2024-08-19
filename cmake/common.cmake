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
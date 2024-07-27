add_subdirectory(INDI)
add_subdirectory(libcommon)
add_subdirectory(liblilxml)
add_subdirectory(xindidriver)

# INDI
install(
        TARGETS indiserver getINDI setINDI evalINDI 
        DESTINATION ${BIN_PATH}
        )
# If $BIN_PATH is different to ${PREFIX}/${CMAKE_INSTALL_BINDIR}
# Normalize the paths to absolute paths
get_filename_component(abs_path1 "${BIN_PATH}" ABSOLUTE)
get_filename_component(abs_path2 "${PREFIX}/${CMAKE_INSTALL_BINDIR}" ABSOLUTE)
# Compare the normalized paths
if(abs_path1 STREQUAL abs_path2)
        hyperlink_targets(
                        TARGETS indiserver getINDI setINDI evalINDI
                        DESTINATION "${PREFIX}/${CMAKE_INSTALL_BINDIR}"
                        )
endif()

# libcommon
install(
        TARGETS libcommon
        LIBRARY DESTINATION ${LIB_PATH}
        )

# liblilxml
install(
        TARGETS liblilxml
        LIBRARY DESTINATION ${LIB_PATH}
        )

# xindidriver
install(
        TARGETS xindidriver
        DESTINATION "${PREFIX}/${CMAKE_INSTALL_BINDIR}"
        )
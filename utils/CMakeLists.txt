message(STATUS "Configuring XWCToolkit utilities...")

add_subdirectory(cursesINDI)
add_subdirectory(logdump)
add_subdirectory(logsurgeon)
add_subdirectory(resurrector_indi)
add_subdirectory(shmimLat)
add_subdirectory(shmimPlot)
add_subdirectory(xrif2fits)
add_subdirectory(xrif2shmim)

message(STATUS "XWCToolkit utilities configuration complete")
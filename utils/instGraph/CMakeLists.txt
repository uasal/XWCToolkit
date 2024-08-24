## Require instGraph library
find_library(INGRLIB instGraph PATHS ${INGR_LIB_PATH} REQUIRED)
if (NOT INGRLIB)
    message(FATAL_ERROR "instGraph library not found. Please install it.")
else()
    message(STATUS "instGraph found at location ${INGRLIB}")            
endif()

message(STATUS "Searching for instGraph header in ${INGR_HEADER_PATH}")
# Check instGraph header files exist
find_path(INGR_HEADERS NAMES "instGraph/instGraph.hpp" PATHS "${INGR_HEADER_PATH}")

# Check if the header files were found
if(NOT INGR_HEADERS)
    message(FATAL_ERROR "Header files for instGraph not found in ${INGR_HEADER_PATH}.")
else()
    message(STATUS "Header files for instGraph found at location ${INGR_HEADERS}")
endif()

add_executable(instGraph instGraph.cpp)
target_link_libraries(instGraph PRIVATE XWCTK
                                        libcommon
                                        liblilxml
                                        libtelnet
                                        ${INGRLIB}
                                        ImageStreamIO::ImageStreamIO
                                        Eigen3::Eigen
                                        ${MXLIB})
target_include_directories(instGraph PRIVATE ${INGR_HEADERS}
                                            ${CMAKE_CURRENT_SOURCE_DIR}
                                            ${CMAKE_CURRENT_SOURCE_DIR}/xigNodes
                                            )

target_compile_definitions(instGraph PRIVATE MXLIB_MILK)
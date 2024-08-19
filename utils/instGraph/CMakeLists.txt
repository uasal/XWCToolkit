## Require instGraph library
find_library(INGR_LIB instGraph REQUIRED)
if (NOT INGR_LIB)
    message(FATAL_ERROR "instGraph library not found. Please install it.")
else()
    message(STATUS "instGraph found at location ${INGR_LIB}")            
endif()

set(INGR_HEADER_PATH "${PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")
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
target_link_libraries(instGraph PRIVATE ${INGR_LIB}
                                        ImageStreamIO::ImageStreamIO
                                        Eigen3::Eigen
                                        ${MXLIB})
target_include_directories(instGraph PRIVATE ${INGR_HEADERS}
                                            ${MXLIB_HEADERS}
                                            ${CMAKE_CURRENT_SOURCE_DIR}
                                            ${CMAKE_SOURCE_DIR}/XWC
                                            )
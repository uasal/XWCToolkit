## Require milk ImageStreamIO headers
require_isio()

## Require milk library
require_milk()

## Require mxlib library & headers
require_mxlib()

## Require Eigen library & include directories
require_eigen()

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
                                        XWC_precompiled_header 
                                        ${MILK_LIB_FILES}
                                        ${MXLIB})
target_include_directories(instGraph PRIVATE ${INGR_HEADERS}
                                            ${EIGEN3_INCLUDE_DIR}                            
                                            ${ISIO_PATH}                            
                                            ${MXLIB_HEADERS}
                                            ${CMAKE_CURRENT_SOURCE_DIR}
                                            ${CMAKE_SOURCE_DIR}/src
                                            )
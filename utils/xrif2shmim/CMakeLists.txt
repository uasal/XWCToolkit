## Require milk ImageStreamIO headers
require_isio()
## Require milk
require_milk()
## Require mxlib library & headers
require_mxlib()
# Find the CFITSIO library
require_cfitsio()
# Find the xrif library
require_xrif()
## Require Eigen library & include directories
require_eigen()
## Require libudev library
require_libudev()
## Require OpenMP
find_package(OpenMP REQUIRED)
if(NOT OpenMP_CXX_FOUND)
    message(STATUS "OpenMP package not found.Please install it.")
endif()

add_executable(xrif2shmim xrif2shmim.cpp)
target_link_libraries(xrif2shmim PRIVATE XWC
                                        XWC_precompiled_header 
                                        libcommon
                                        libtelnet
                                        liblilxml                                        
                                        ${CFITSIO_LIB}
                                        ${XRIFLIB}
                                        ${MXLIB}
                                        ${LIBUDEV_LIBRARIES}
                                        ${MILK_LIB_FILES}
                                        OpenMP::OpenMP_CXX)
target_include_directories(xrif2shmim PRIVATE 
                            ${ISIO_PATH}
                            ${MXLIB_HEADERS}                            
                            ${XRIF_HEADERS}
                            ${EIGEN3_INCLUDE_DIR} 
                            ${LIBUDEV_INCLUDE_DIRS} 
                            ${CMAKE_CURRENT_SOURCE_DIR}                 
                            )
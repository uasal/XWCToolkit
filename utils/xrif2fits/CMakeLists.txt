add_executable(xrif2fits xrif2fits.cpp)
target_link_libraries(xrif2fits PRIVATE XWC
                                        libcommon
                                        libtelnet
                                        liblilxml                                        
                                        ${CFITSIO_LIB}
                                        ${XRIFLIB}
                                        ${MXLIB}
                                        ${LIBUDEV_LIBRARIES}
                                        OpenMP::OpenMP_CXX
                                        Eigen3::Eigen)
target_include_directories(xrif2fits PRIVATE 
                            ${MXLIB_HEADERS}                            
                            ${XRIF_HEADERS}
                            ${LIBUDEV_INCLUDE_DIRS} 
                            ${CMAKE_CURRENT_SOURCE_DIR}                 
                            )
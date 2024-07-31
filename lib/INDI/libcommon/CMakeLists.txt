# Create the library
add_library(libcommon Cmd.cpp
                    IndiConnection.cpp
                    IndiClient.cpp
                    IndiDriver.cpp
                    IndiElement.cpp
                    IndiMessage.cpp
                    IndiProperty.cpp
                    IndiPropertyMap.cpp
                    IndiXmlParser.cpp
                    System.cpp
                    SystemSocket.cpp
                    Thread.cpp
                    TimeStamp.cpp)

target_include_directories(libcommon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
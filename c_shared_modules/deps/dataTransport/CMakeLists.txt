

############################################################
# Create a library
############################################################

#Generate the static library from the library sources
add_library(datatransport_lib SHARED
        dataTransport.cpp
        )

target_include_directories(datatransport_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
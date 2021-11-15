
############################################################
# Initial defs
############################################################
cmake_minimum_required(VERSION 3.16) # CMake version check
project(szs_datatransport VERSION 0.1.4) # Create project "szs_datastransport"

############################################################
# Create a library
############################################################

#Generate the static library from the library sources
add_library(szs_datatransport INTERFACE)

target_include_directories(szs_datatransport INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_sources(szs_datatransport INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dataTransport.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/udp_client.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/udp_server.cpp
        )

add_subdirectory(tests)

############################################################
# Initial defs
############################################################
cmake_minimum_required(VERSION 3.16) # CMake version check
project(szs_datastransport VERSION 0.0.1) # Create project "szs_datastransport"

############################################################
# Create a library
############################################################

#Generate the static library from the library sources
add_library(szs_datastransport SHARED
        dataTransport.cpp
        )

target_include_directories(szs_datastransport
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

add_subdirectory(tests)
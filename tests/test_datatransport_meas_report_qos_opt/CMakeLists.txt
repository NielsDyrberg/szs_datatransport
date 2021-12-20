cmake_minimum_required(VERSION 3.16) # CMake version check
set(CMAKE_CXX_STANDARD 20) # Enable c++20 standard

############################################################
# Master
############################################################

project(test_datatransport_qos_opt_master VERSION 0.0.1) # Create project
# Add main.cpp file of project root directory as source file
set(${PROJECT_NAME} master_main.cpp)
# Add executable target with source files listed in SOURCE_FILES variable
add_executable(${PROJECT_NAME} master_main.cpp)
# Link libraries to project
target_link_libraries(${PROJECT_NAME} szs_datatransport)

############################################################
# Slave 1
############################################################

project(test_datatransports_qos_opt_slave VERSION 0.0.1) # Create project
# Add main.cpp file of project root directory as source file
set(${PROJECT_NAME} slave_main.cpp)
# Add executable target with source files listed in SOURCE_FILES variable
add_executable(${PROJECT_NAME} slave_main.cpp)
# Link libraries to project
target_link_libraries(${PROJECT_NAME} szs_datatransport)



cmake_minimum_required(VERSION 3.16) # CMake version check
project(test_dt_master VERSION 0.0.1) # Create project "test_send_pack"
set(CMAKE_CXX_STANDARD 20) # Enable c++20 standard


# Add main.cpp file of project root directory as source file
set(${PROJECT_NAME} test_dt_master.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(${PROJECT_NAME} test_dt_master.cpp)

# Link libraries to project
target_link_libraries(${PROJECT_NAME} szs_datatransport)



cmake_minimum_required(VERSION 3.10)
project(OpenCV_Project)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(ascii_cam src/main.cpp)

target_link_libraries(ascii_cam ${OpenCV_LIBS})

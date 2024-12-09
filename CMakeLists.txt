cmake_minimum_required(VERSION 3.22)
project(large_scale_systems)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
find_package(GTest REQUIRED)

add_subdirectory(1)
add_subdirectory(5)
add_subdirectory(6)
add_subdirectory(7)


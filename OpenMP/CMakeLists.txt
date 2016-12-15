cmake_minimum_required(VERSION 3.4)
project(project)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -Wall -std=gnu++98 -fopenmp")

include_directories(
        ${CMAKE_SOURCE_DIR}/src
)

add_subdirectory(src)

# g++ OpenMP.cpp -fopenmp
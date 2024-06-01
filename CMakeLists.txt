cmake_minimum_required(VERSION 3.26)
project(RaytracingFromScratch)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(RaytracingFromScratch
        main.cpp
        main.h
        vec3.h
        color.h
        ray.h
)

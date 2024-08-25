cmake_minimum_required(VERSION 3.26)
project(RaytracingFromScratch)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(RaytracingFromScratch
        main.cpp
        main.h
        MathLib/vec3.h
        Materials/color.h
        RayGeneration/ray.h
        Hittables/hittable.h
        Hittables/sphere.h
        Hittables/hittable_list.h
        MathLib/mathLibrary.h
        MathLib/interval.h
        RayGeneration/camera.h
        Materials/material.h
        Hittables/mesh.h
        MathLib/transform.h
        MathLib/matrix.h
        Lights/light.h
)

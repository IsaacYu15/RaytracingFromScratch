cmake_minimum_required(VERSION 3.26)
project(RaytracingFromScratch)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        .
        imgui
        imgui/backends
)

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
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_internal.h
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        imgui/backends/imgui_impl_dx9.cpp
        imgui/backends/imgui_impl_win32.cpp
)

target_link_libraries(RaytracingFromScratch d3dx9)
target_link_libraries(RaytracingFromScratch d3d9)
target_link_libraries(RaytracingFromScratch dwmapi)
target_link_libraries(RaytracingFromScratch d3d9 dwmapi)
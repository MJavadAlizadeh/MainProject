cmake_minimum_required(VERSION 3.16)
project(ray_graphic C)

set(CMAKE_C_STANDARD 11)

# Raylib via FetchContent
include(FetchContent)
FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(raylib)

add_executable(ray_graphic main.c)

target_link_libraries(ray_graphic
        raylib
        opengl32
        gdi32
        winmm
)
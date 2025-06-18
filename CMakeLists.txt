cmake_minimum_required(VERSION 3.10)
project(PixelFinder C)
set(CMAKE_C_STANDARD 99)

add_executable(PixelFinder
        threadpool.c
        grab_pixel.c
        time_tracker.c
        keylistener.c
        keylistener.h
        main.c
)

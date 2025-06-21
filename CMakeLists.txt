cmake_minimum_required(VERSION 3.10)
project(PixelFinder C)

set(CMAKE_C_STANDARD 99)

# Find required packages
find_package(Threads REQUIRED)

# Add executable with source files only (no headers)
add_executable(PixelFinder
        threadpool.c
        grab_pixel.c
        keylistener.c
        main.c
)

# Link libraries based on platform
if(WIN32)
    # Windows libraries
    target_link_libraries(PixelFinder
            d3d11
            dxgi
            user32
            gdi32
            Threads::Threads
    )
elseif(UNIX AND NOT APPLE)
    # Linux libraries
    target_link_libraries(PixelFinder
            X11
            Xext
            rt
            m
            Threads::Threads
    )
elseif(APPLE)
    # macOS libraries
    find_library(COREGRAPHICS_LIBRARY CoreGraphics)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    target_link_libraries(PixelFinder
            ${COREGRAPHICS_LIBRARY}
            ${COREFOUNDATION_LIBRARY}
            Threads::Threads
    )
endif()

# Set compiler flags
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(PixelFinder PRIVATE
            -Wall
            -Wextra
            -O2
    )
endif()

# For debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(PixelFinder PRIVATE DEBUG=1)
endif()
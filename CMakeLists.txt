cmake_minimum_required(VERSION 3.15)

project(BITMAP_RENDER LANGUAGES CXX)

# Enforce C++20 standard strictly
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enforce static MSVC C/C++ runtime library (static linking for MSVC)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_executable(main main.cpp)

# Enable compiler warnings
if(MSVC)
    target_compile_options(main PRIVATE /W4)
else()
    target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Request static linking for GCC / Clang (e.g., MinGW on Windows)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # -static links standard libraries and dependencies statically
    target_link_options(main PRIVATE -static)
endif()

# POWERSHELL COMMAND
# cmake --build build; if($?) {./build/main.exe}
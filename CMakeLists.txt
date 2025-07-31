cmake_minimum_required(VERSION 3.14)
project(image_tool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_BENCHMARKS "Build benchmark executables" ON)
option(BUILD_APPS "Build applications" ON)


find_package(OpenCV REQUIRED)

include(FetchContent)

FetchContent_Declare(
        CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG        v2.5.0
)

FetchContent_MakeAvailable(CLI11)


add_subdirectory(src)


if(BUILD_APPS)
    add_subdirectory(apps)
endif()

if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
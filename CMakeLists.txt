cmake_minimum_required(VERSION 3.20)

project(LinuxEmbeddedAutomotivePlatform
    VERSION 0.1.0
    DESCRIPTION "Linux Embedded Automotive Platform"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(can)
add_subdirectory(ecu/engine)
add_subdirectory(ecu/battery)

cmake_minimum_required(VERSION 3.20)

project(LinuxEmbeddedAutomotivePlatform
    VERSION 0.1.0
    DESCRIPTION "Linux Embedded Automotive Platform"
    LANGUAGES C CXX
)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(common
    common/Logger.cpp
    common/Configuration.cpp
)

target_include_directories(common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/common
)

add_executable(test_logger
    tests/testLogger.cpp
    )

target_link_libraries(test_logger
    PRIVATE
        common
)

add_test(
    NAME logger_test
    COMMAND test_logger
)

add_executable(test_configuration
    tests/test_configuration.cpp
)

target_link_libraries(test_configuration
    PRIVATE
        common
)

add_test(
    NAME configuration_test
    COMMAND test_configuration
)

add_subdirectory(can)
add_subdirectory(ecu/engine)
add_subdirectory(ecu/battery)
add_subdirectory(apps/gateway)

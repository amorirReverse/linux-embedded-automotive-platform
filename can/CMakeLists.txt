# can/CMakeLists.txt

add_library(can
    CanSocket.cpp
)

target_include_directories(can
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(test_can
    test_can.cpp
)

target_link_libraries(test_can
    PRIVATE
        can
)   
# can/CMakeLists.txt

add_library(can
    CanSocket.cpp
    EngineCanMessage.cpp
    BatteryCanMessage.cpp
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

add_executable(test_engine_can_message
    ../tests/EngineCanMessageTest.cpp
)

target_link_libraries(test_engine_can_message
    PRIVATE
        can
) 

add_executable(test_battery_can_message
    ../tests/BatteryCanMessageTest.cpp
)

target_link_libraries(test_battery_can_message
    PRIVATE
        can
)
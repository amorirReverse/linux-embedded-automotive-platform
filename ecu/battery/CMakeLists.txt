# ecu/battery/CMakeLists.txt

add_executable(battery_ecu
    main.cpp
    Battery.cpp
    BatteryEcu.cpp
)

target_link_libraries(battery_ecu
    PRIVATE
        can
        common
)

add_executable(test_battery
    ../../tests/test_battery.cpp
    Battery.cpp
)

target_link_libraries(test_battery
    PRIVATE
        common
)

target_include_directories(test_battery
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_test(
    NAME battery_test
    COMMAND test_battery
)

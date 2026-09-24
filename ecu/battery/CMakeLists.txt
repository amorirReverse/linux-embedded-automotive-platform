# ecu/battery/CMakeLists.txt

add_executable(battery_ecu
    main.cpp
    Battery.cpp
    BatteryEcu.cpp
)

target_link_libraries(battery_ecu
    PRIVATE
        can
)
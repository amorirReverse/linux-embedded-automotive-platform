# ecu/engine/CMakeLists.txt

add_executable(engine_ecu
    main.cpp
    Engine.cpp
    EngineEcu.cpp
)

target_link_libraries(engine_ecu
    PRIVATE
        can
)

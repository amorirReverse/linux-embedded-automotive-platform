# ecu/engine/CMakeLists.txt

add_executable(engine_ecu
    main.cpp
    Engine.cpp
    EngineEcu.cpp
)

target_link_libraries(engine_ecu
    PRIVATE
        can
        common
)

add_executable(test_engine
    Engine.cpp
    ../../tests/test_engine.cpp
)

target_link_libraries(test_engine
    PRIVATE
        common
)

target_include_directories(test_engine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_test(
    NAME engine_test
    COMMAND test_engine
)
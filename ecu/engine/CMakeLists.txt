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

add_executable(test_engine_ecu
    Engine.cpp
    EngineEcu.cpp
    ../../tests/test_engine_ecu.cpp
)

target_link_libraries(test_engine_ecu
    PRIVATE
        can
        common
)

target_include_directories(test_engine_ecu
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_test(
    NAME engine_ecu_test
    COMMAND test_engine_ecu
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
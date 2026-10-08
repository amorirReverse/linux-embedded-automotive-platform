add_executable(gateway
    main.cpp
    Gateway.cpp
)

target_link_libraries(gateway
    PRIVATE
        can
        common
)

add_executable(test_gateway
    test_gateway.cpp
    Gateway.cpp
)

target_link_libraries(test_gateway
    PRIVATE
        can
        common
)

add_test(
    NAME gateway_test
    COMMAND test_gateway
)
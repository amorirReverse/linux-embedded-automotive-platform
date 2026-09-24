add_executable(gateway
    main.cpp
    Gateway.cpp
)

target_link_libraries(gateway
    PRIVATE
        can
)
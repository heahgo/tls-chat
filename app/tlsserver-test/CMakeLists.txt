add_executable(tlsserver-test
    tlsserver-test.cpp
)

target_link_libraries(tlsserver-test PRIVATE tls_network)

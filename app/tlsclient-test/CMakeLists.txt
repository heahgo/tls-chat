add_executable(tlsclient-test
    tlsclient-test.cpp
)

target_link_libraries(tlsclient-test PRIVATE tls_network)

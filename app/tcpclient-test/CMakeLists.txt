add_executable(tcpclient-test
    tcpclient-test.cpp
)

target_link_libraries(tcpclient-test PRIVATE tcp_network)

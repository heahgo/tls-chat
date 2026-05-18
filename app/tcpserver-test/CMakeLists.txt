add_executable(tcpserver-test
    tcpserver-test.cpp
)

target_link_libraries(tcpserver-test PRIVATE tcp_network)

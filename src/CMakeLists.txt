add_library(tcp_network STATIC
    ip.cpp
    tcpclient.cpp
    tcpserver.cpp
    tcpsession.cpp
)

target_include_directories(tcp_network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(tls_network STATIC
    tlsclient.cpp
    tlsserver.cpp
    tlssession.cpp
)

target_include_directories(tls_network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tls_network PUBLIC tcp_network OpenSSL::SSL OpenSSL::Crypto)

if(WIN32)
    target_link_libraries(tcp_network PUBLIC ws2_32)
endif()

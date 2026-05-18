cmake_minimum_required(VERSION 3.16)
project(tls-chat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(OpenSSL REQUIRED)

add_subdirectory(src)
add_subdirectory(app/chat)
add_subdirectory(app/tcpclient-test)
add_subdirectory(app/tcpserver-test)
add_subdirectory(app/tlsclient-test)
add_subdirectory(app/tlsserver-test)

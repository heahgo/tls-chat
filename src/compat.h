#pragma once

#include <string>
#include <cstdio>

#ifdef _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  pragma comment(lib, "ws2_32.lib")
#  ifdef _MSC_VER
     typedef int ssize_t;
#  endif

   inline void net_init() {
       static bool done = false;
       if (done) return;
       WSADATA wsa;
       WSAStartup(MAKEWORD(2, 2), &wsa);
       done = true;
   }
   inline int sock_close(int s)    { return closesocket((SOCKET)s); }
   inline int sock_shutdown(int s) { return shutdown((SOCKET)s, SD_BOTH); }
   inline std::string sock_error() {
       char buf[32];
       snprintf(buf, sizeof(buf), "WSA error %d", WSAGetLastError());
       return buf;
   }
   inline void platform_usleep(unsigned int us) { Sleep(us / 1000 < 1 ? 1 : us / 1000); }
#else
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <unistd.h>
#  include <cerrno>
#  include <cstring>

   inline void net_init() {}
   inline int sock_close(int s)    { return ::close(s); }
   inline int sock_shutdown(int s) { return ::shutdown(s, SHUT_RDWR); }
   inline std::string sock_error() { return strerror(errno); }
   inline void platform_usleep(unsigned int us) { usleep(us); }
#endif

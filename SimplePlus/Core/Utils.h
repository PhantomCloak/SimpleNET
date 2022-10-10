#ifndef UTILS_H
#define UTILS_H

#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>
#include <arpa/inet.h>

class Utils {
  public:
    static const char* GetSocketAddress(int sock);
    static const int GetSocketPort(int sock);
};
#endif

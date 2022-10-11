#include "Utils.h"

const char* Utils::GetSocketAddress(int sock) {
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int res = getpeername(sock, (struct sockaddr*)&addr, &addr_size);

    if (res < 0) {
        printf("an error occured during the GetSocketPort");
    }

    return inet_ntoa(addr.sin_addr);
}


const int Utils::GetSocketPort(int sock) {
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int res = getpeername(sock, (struct sockaddr*)&addr, &addr_size);

    if (res < 0) {
        printf("an error occured during the GetSocketPort");
        return 0;
    }

    char* clientip = new char[20];
    return ntohs(addr.sin_port);
}

const long Utils::GetUniqueIdentifier(int sock) {
  return sock;
}

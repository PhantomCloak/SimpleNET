#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "IO.h"

#define sock_h int

class Server {
    public:
    void StartServer(int port);
    bool PollForConnection(sock_h* newClient);
    bool CheckConnectionAlive(sock_h client) const;
    sock_h Accept();
    sock_h sockfd;

    private:
    struct pollfd fds[200];
};
#endif

#include "Server.h"
#include <unistd.h>

static char* keepAliveBuffer = new char[0];

Server::Server(int port) {
    srvPort = port;
}

int Server::StartServer() {
    int connfd, len;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        fprintf(stderr, "socket create failed: %s\n", strerror(errno));
        return -1;
    } else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(srvPort);

    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        fprintf(stderr, "socket bind failed: %s\n", strerror(errno));
        close(sockfd);
        return -1;
    } else
        printf("Socket successfully binded..\n");

    if ((listen(sockfd, 5)) != 0) {
        fprintf(stderr, "socket listen failed: %s\n", strerror(errno));
        close(sockfd);
        return -1;
    } else
        printf("Server listening..\n");

    memset(&fds, 0, sizeof(fds));
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;

    struct timeval timeout;
    timeout.tv_sec = 4;
    timeout.tv_usec = 0;

    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(struct timeval));
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));

    // low buffer size for keep alive packages
    int buffersize = 4;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &buffersize, 4);

    int nonBlockResult = fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);

    if (nonBlockResult == -1) {
        fprintf(stderr, "an error occured while trying to set socket non-blocking %s\n", strerror(errno));
    }

    return 0;
}

bool Server::PollForConnection(sock_h* newClient) {
    int nfds = 1;
    int rc = poll(fds, nfds, 16);
    int current_size = nfds;

    for (int i = 0; i < current_size; i++) {
        if (fds[i].revents == 0)
            continue;

        if (fds[i].revents != POLLIN) {
            printf("revents != POLLIN, revents = %d\n", fds[i].revents);
            close(fds[i].fd);
            fds[i].fd = -1;
            break;
        }

        if (fds[i].fd == sockfd) {
            struct sockaddr_in cli;
            socklen_t len = sizeof(cli);
            *newClient = accept(sockfd, (struct sockaddr*)&cli, &len);
            return true;
        }
    }

    return false;
}

bool Server::CheckConnectionAlive(sock_h client) const {
    return IO::SendPackage(client, keepAliveBuffer, 0);
}

void Server::Shutdown() {
    close(sockfd);
}

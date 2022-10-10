#include "Server.h"
#include <unistd.h>

Server::Server(int port)
{
  srvPort = port;
}

void Server::StartServer() {
    int connfd, len;
    struct sockaddr_in servaddr;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(srvPort);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    } else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    } else
        printf("Server listening..\n");

    memset(&fds, 0, sizeof(fds));
    fds[0].fd = sockfd;
    fds[0].events = POLLIN; // check if data to read
                            //

    struct timeval timeout;
   timeout.tv_sec = 4;
   timeout.tv_usec = 0;
 
   //apply send timeout socket options
   setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(struct timeval));
 
   //set output buffer size so that send blocks
   int buffersize = 2;
   setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &buffersize, 4);
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
            printf("Listening socket available\n");
            struct sockaddr_in cli;
            socklen_t len = sizeof(cli);
            *newClient = accept(sockfd, (struct sockaddr*)&cli, &len);
            return true;
        }
    }

    return false;
}

bool Server::CheckConnectionAlive(sock_h client) const {
    char* keepAliveBuffer = new char[0];
    return IO::SendPackage(client, keepAliveBuffer, 0);
}

void Server::Shutdown()
{
  close(sockfd);
}

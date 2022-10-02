#include "Client.h"

bool Client::Connect(const char *address, int port) {
  struct addrinfo hints = {}, *addrs;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  socketHandle = socket(AF_INET, SOCK_STREAM, 0);
  if (socketHandle <= 0) {
    return false;
  }

  portNo = port;
  server = gethostbyname(address);

  memset((char *)&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;

  memcpy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

  serv_addr.sin_port = htons(port);

  if (connect(socketHandle, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
      0) {
    return false;
  }

  return true;
}

void Client::Shutdown() { close(socketHandle); }

bool Client::CheckConnectionAlive() const {
  char *keepAliveBuffer = new char[0];
  return IO::SendPackage(socketHandle, keepAliveBuffer, 0);
}

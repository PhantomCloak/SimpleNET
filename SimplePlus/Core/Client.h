#ifndef CLIENT_H
#define CLIENT_H

#include "IO.h"
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class Client {
public:
  bool Connect(const char *address, int port);
  void Shutdown();
  bool CheckConnectionAlive() const;
  int socketHandle;

private:
  int portNo;
};
#endif // !CLIENT_H

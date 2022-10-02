#ifndef IO_H
#define IO_H

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

class IO {
public:
  static bool SendPackage(int sockHandle, char *buffer);
  static bool SendPackage(int sockHandle, char *buffer, int count);
  static int ReadNextPackage(int sockHandle, char *buffer);
};

#endif // !IO_H

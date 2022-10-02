#include "IO.h"
#include <cstdio>

static int MaxPackageSize = 4096;

bool IO::SendPackage(int socketHandle, char *buffer) {
  return SendPackage(socketHandle, buffer, strlen(buffer));
}

bool IO::SendPackage(int socketHandle, char *buffer, int count) {

  char *tmpBuff = new char[sizeof(int) + count];

  if (count > 0) {
    *((int *)&tmpBuff[0]) = count;
    memcpy(&tmpBuff[sizeof(int)], buffer, count);
    count += sizeof(int);
  }

  int bytesWritten = send(socketHandle, tmpBuff, count, MSG_NOSIGNAL);

  delete[] tmpBuff;

  // socket possibly borken
  if (bytesWritten < 0) {
    return false;
  }

  return true;
}

int IO::ReadNextPackage(int socketHandle, char *buffer) {
  char *sizeBuff = new char[sizeof(int)];
  int dataAvaible = 0;

  ioctl(socketHandle, FIONREAD, &dataAvaible);
  if (dataAvaible <= 0)
    return dataAvaible;

  recv(socketHandle, sizeBuff, sizeof(int), 0);
  int packageSize = *((int *)&sizeBuff[0]);

  // if package is empty
  if (packageSize == 0)
    return 0;

  // check for malicious package
  if (packageSize > MaxPackageSize)
    return -1;

  int bytesReaded = 0;
  while (bytesReaded != packageSize) {
    int resultRead =
        recv(socketHandle, &buffer[bytesReaded], packageSize - bytesReaded, 0);

    // on unscessfull read
    if (resultRead == -1) {
      packageSize = -1;
      break;
    }

    bytesReaded += resultRead;
  }

  delete[] sizeBuff;
  return packageSize;
}

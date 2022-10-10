#include "IO.h"
#include <cstdio>

static int MaxPackageSize = 4096;
static char * sendTmpBuffer = new char[1470];
static char * recvHeaderBuffer = new char[sizeof(int)];

bool IO::SendPackage(int socketHandle, char* buffer) {
    return SendPackage(socketHandle, buffer, strlen(buffer));
}

bool IO::SendPackage(int socketHandle, char* buffer, int count) {
    *((int*)&sendTmpBuffer[0]) = count;
    memcpy(&sendTmpBuffer[sizeof(int)], buffer, count);
    count += sizeof(int);

    int bytesWritten = send(socketHandle, sendTmpBuffer, count, MSG_NOSIGNAL);

    return bytesWritten >= 0;
}

int IO::ReadNextPackage(int socketHandle, char* buffer) {
    int dataAvaible = 0;

    ioctl(socketHandle, FIONREAD, &dataAvaible);
    if (dataAvaible <= 0)
        return dataAvaible;

    recv(socketHandle, recvHeaderBuffer, sizeof(int), 0);
    int packageSize = *((int*)&recvHeaderBuffer[0]);

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

    return packageSize;
}

#include "Client.h"
#include "IO.h"
#include "Serializer.h"
#include <iostream>
#include <stdio.h>
#include <thread>

int main() {
  Client *client = new Client();

  bool success = client->Connect("127.0.0.1", 22003);

  if (!success) {
    printf("Connection cannot established.\n");
    return -1;
  }

  char *buffer = new char[512];
  char *input = new char[512];

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    int writeOffset = 0;

    bool isAlive = client->CheckConnectionAlive();

    if (!isAlive) {
      printf("Connection lost\n");
      return -1;
    }

    printf("input your message: ");
    scanf("%s", input);

    Serializer::WriteString(input, buffer, &writeOffset);
    IO::SendPackage(client->socketHandle, buffer, writeOffset);
  }

  client->Shutdown();

  delete[] buffer;
  delete[] input;
}

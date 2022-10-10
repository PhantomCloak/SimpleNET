#include "Server.h"
#include "Utils.h"
#include "IO.h"
#include "Serializer.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include <vector>

int main() {
  Server* server = new Server(22003);
  std::vector<int> clients;
  server->StartServer();
  int newClient = 0;
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(16));

    if (server->PollForConnection(&newClient)) {
      printf("New Client Connected: %s\n", Utils::GetSocketAddress(newClient));
      clients.push_back(newClient);
    }

    clients.erase(std::remove_if(clients.begin(), clients.end(),
        [server](const int client) {
          bool status = server->CheckConnectionAlive(client);
          if (!status)
            printf("Lost connection with client port: %d\n", Utils::GetSocketPort(client));
          return !status;
        }),
      clients.end());

    for (auto sock: clients) {
      char* buffer = new char[512];
      int packageSize = IO::ReadNextPackage(sock, buffer);
      if (packageSize == 0 || packageSize == -1)
        continue;

      int readOffset = 0;
      const char* msgStr = Serializer::ReadString(buffer, &readOffset);
      printf("Client says: %s package size: %d\n", msgStr, packageSize);
      delete[] buffer;
    }
  }

  server->Shutdown();
  clients.clear();
}

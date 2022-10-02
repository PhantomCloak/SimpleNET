# SimpleNET
Simple TCP library written in both C++ and C# to stop reinventing the simple IPC or Transport, It's justly really thin wrapper between native API with some extra features

### Features
- Detecting connection drop
- Simple serializer
- Message framing

## For C#
### Server Setup
```csharp
var server = new Server(22003);
var clients = new List<Socket>();
server.StartServer();

while (true)
{
   Thread.Sleep(16);
   if (server.PollForConnection(out var newClient))
   {
      Console.WriteLine("New Client Connected: " + Utils.GetSocketAddress(newClient));
      clients.Add(newClient);
   }

   foreach (var sock in clients.ToList())
   {
      var result = server.CheckConnectionAlive(sock);
      if (!result)
      {
          Console.WriteLine("Lost connection with client port:" + Utils.GetSocketPort(sock));
          clients.Remove(sock);
      }
   }

   foreach (var sock in clients)
   {
      var buffer = new byte[512];
      int packageSize = IO.ReadNextPackage(sock, ref buffer);
      if (packageSize == 0 || packageSize == -1)
          continue;
          
      var readOffset = 0;
      var msgStr = Serializer.ReadString(buffer, ref readOffset);
      
      Console.WriteLine("Client says: " + msgStr + " size: " + packageSize);
   }
}

server.ShutdownServer();
clients.Clear();
```


### Client Setup
```csharp
var client = new Client();

client.Connect("127.0.0.1", 22003);

var buffer = new byte[512];
while (true)
{
  Thread.Sleep(16);
  int writeOffset = 0;

  var isAlive = client.CheckConnectionAlive();
  if (!isAlive)
  {
      Console.WriteLine("Connection lost");
      return;
  }

  var msg = Console.ReadLine();
  Serializer.WriteString(msg, ref buffer, ref writeOffset);
  
  IO.SendPackage(client.Sock, Encoding.ASCII.GetBytes(msg));
}

```
## For C++

### Client Setup
```cpp
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
```

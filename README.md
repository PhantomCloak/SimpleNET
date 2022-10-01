# SimpleNET
Simple TCP library to stop re-inventing it again, It's justly really thin wrapper between native API with some extra features

### Features
- Detecting connection drop
- Simple serializer
- Message framing


## Server Setup
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
      var msgStr = Encoding.ASCII.GetString(buffer);
      Console.WriteLine("Client says: " + msgStr + " size: " + packageSize);
   }
}

server.ShutdownServer();
clients.Clear();
```


## Client Setup
```csharp
var client = new Client();

client.Connect("127.0.0.1", 22003);

while (true)
{
  Thread.Sleep(16);

  var isAlive = client.CheckConnectionAlive();
  if (!isAlive)
  {
      Console.WriteLine("Connection lost");
      return;
  }

  var msg = Console.ReadLine();
  var result = IO.SendPackage(client.Sock, Encoding.ASCII.GetBytes(msg));
}

```

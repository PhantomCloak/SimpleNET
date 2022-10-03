using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;

namespace SimpleNET
{
    public class Server
    {
        private Socket listener;

        private List<Socket> clients;
        private int srvPort;
        public Server(int port)
        {
            srvPort = port;
            var ep = new IPEndPoint(IPAddress.Any, port);
            listener = new Socket(ep.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            clients = new List<Socket>();
            listener.Bind(ep);
        }

        public void StartServer()
        {
            listener.Listen(srvPort);
        }

        public void ShutdownServer()
        {
            listener.Close();
        }

        public bool PollForConnection(out Socket newConnection)
        {
            var pollResult = listener.Poll(25, SelectMode.SelectRead);
            newConnection = null;

            if (!pollResult)
                return false;

            newConnection = listener.Accept();
            clients.Add(newConnection);

            return true;
        }

        public bool CheckConnectionAlive(Socket sock)
        {
            return IO.SendPackage(sock, new byte[0]);
        }
    }
}

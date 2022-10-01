using System.Net;
using System.Net.Sockets;

namespace SimpleNET
{
    public class Client
    {
        public Socket Sock;

        public void Connect(string address, int port)
        {
            IPEndPoint ep = new(IPAddress.Parse(address), port);
            Sock = new Socket(ep.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            Sock.Connect(ep);
        }

        public void ShutdownServer()
        {
            Sock.Close();
        }

        public bool CheckConnectionAlive()
        {
            return IO.SendPackage(Sock, new byte[0]);
        }
    }
}

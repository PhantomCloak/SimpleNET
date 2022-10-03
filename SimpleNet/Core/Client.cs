using System.Net;
using System.Net.Sockets;

namespace SimpleNET
{
    public class Client
    {
        public Socket Sock;

        public void Connect(string address, int port)
        {
            var ep = new IPEndPoint(IPAddress.Parse(address), port);
            Sock = new Socket(ep.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            Sock.Connect(ep);
        }

        public void Shutdown()
        {
            Sock.Close();
        }

        public bool CheckConnectionAlive()
        {
            return IO.SendPackage(Sock, new byte[0]);
        }
    }
}


using System.Net;
using System.Net.Sockets;

namespace SimpleNET
{
    public static class Utils
    {
        public static int GetSocketPort(Socket socket)
        {
            return ((IPEndPoint)socket.RemoteEndPoint).Port;
        }

        public static string GetSocketAddress(Socket socket)
        {
            return ((IPEndPoint)socket.RemoteEndPoint).Address.ToString();
        }
    }
}

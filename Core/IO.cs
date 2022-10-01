
using System;
using System.Net.Sockets;

namespace SimpleNET
{
    public static class IO
    {
        private const int MtuSize = 1472;
        private const int MaxPackageSize = 4096;
        private const int MagicPollInterval = 6;

        public static bool SendPackage(Socket socket, byte[] buffer)
        {
            if (buffer.Length > MtuSize)
                return false;

            var tmpBuff = new byte[sizeof(int) + buffer.Length];

            var header = BitConverter.GetBytes(buffer.Length);

            Buffer.BlockCopy(header, 0, tmpBuff, 0, sizeof(int));
            Buffer.BlockCopy(buffer, 0, tmpBuff, sizeof(int), buffer.Length);

            try
            {
                socket.Send(tmpBuff, 0, sizeof(int) + buffer.Length, SocketFlags.None);
            }
            catch (Exception)
            {
                return false;
            }

            return true;
        }

        public static int ReadNextPackage(Socket socket, ref byte[] buffer)
        {
            var sizeBuf = new byte[sizeof(int)];
            var dataAvaible = socket.Poll(MagicPollInterval, SelectMode.SelectRead);

            if (!dataAvaible)
                return 0;

            try
            {
                socket.Receive(sizeBuf, 0, sizeof(int), SocketFlags.None);
                int packageSize = BitConverter.ToInt32(sizeBuf, 0);

                // if empty or keep-alive package
                if (packageSize == 0)
                    return 0;

                // if malicious package
                if (packageSize > MaxPackageSize)
                    return -1;

                int bytesReaded = 0;
                while (bytesReaded != packageSize)
                {
                    int resultRead = socket.Receive(buffer,
                                          sizeof(int) + bytesReaded, // offset
                                          packageSize - bytesReaded, // remaining
                                          SocketFlags.None);

                    bytesReaded += resultRead;
                }

                return packageSize;
            }
            catch (Exception)
            {
                return -1;
            }
        }
    }
}

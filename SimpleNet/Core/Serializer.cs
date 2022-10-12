
using System;
using System.Text;

namespace SimpleNET
{
    public class Serializer
    {
        public static int ReadInt(byte[] buffer, ref int readIndex)
        {
            var value = BitConverter.ToInt32(buffer, readIndex);
            readIndex += sizeof(int);
            return value;
        }

        public static float ReadFloat(byte[] buffer, ref int readIndex)
        {
            var value = BitConverter.ToSingle(buffer, readIndex);
            readIndex += sizeof(float);
            return value;
        }

        public static double ReadDouble(byte[] buffer, ref int readIndex)
        {
            var value = BitConverter.ToDouble(buffer, readIndex);
            readIndex += sizeof(double);
            return value;
        }

        public static string ReadString(byte[] buffer, ref int readIndex)
        {
            var len = ReadInt(buffer, ref readIndex);
            var value = Encoding.ASCII.GetString(buffer, readIndex, len);
            readIndex += value.Length;
            return value;
        }

        public static void WriteInt(int value, ref byte[] buffer, ref int writeIndex)
        {
            var valBuf = BitConverter.GetBytes(value);
            Buffer.BlockCopy(valBuf, 0, buffer, writeIndex, sizeof(int));
            writeIndex += sizeof(int);
        }

        public static void WriteFloat(float value, ref byte[] buffer, ref int writeIndex)
        {
            var valBuf = BitConverter.GetBytes(value);
            Buffer.BlockCopy(valBuf, 0, buffer, writeIndex, sizeof(float));
            writeIndex += sizeof(float);
        }

        public static void WriteDouble(double value, ref byte[] buffer, ref int writeIndex)
        {
            var valBuf = BitConverter.GetBytes(value);
            Buffer.BlockCopy(valBuf, 0, buffer, writeIndex, sizeof(float));
            writeIndex += sizeof(float);
        }

        public static void WriteString(string value, ref byte[] buffer, ref int writeIndex)
        {
            WriteInt(value.Length, ref buffer, ref writeIndex);
            var valBuf = Encoding.ASCII.GetBytes(value);
            Buffer.BlockCopy(valBuf, 0, buffer, writeIndex, valBuf.Length);
            writeIndex += valBuf.Length;
        }
    }
}

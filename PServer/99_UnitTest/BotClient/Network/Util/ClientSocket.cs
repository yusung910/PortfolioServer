using BotClient.Network.Const;
using FlatBuffers;
using K4os.Compression.LZ4;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace BotClient.Network.Util
{
    class ClientSocket : Socket
    {
        private byte[] m_buff= new byte[NetworkGlobalConst.MAX_PACKET_BINARY_SIZE];

        private IPEndPoint m_IEP;

        private int m_nHostID;
        public int HostID
        {
            get { return m_nHostID; }
            set { m_nHostID = value; }
        }

        public ClientSocket(string _ip, int _port) : base(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp)
        {
            m_IEP = new IPEndPoint(IPAddress.Parse(_ip), _port);
            base.BeginConnect(m_IEP, ConnectComplete, this);
        }

        public void Send(string _msg)
        {
            if (Connected)
            {
                byte[] data = Encoding.UTF8.GetBytes(_msg);
                base.Send(data, data.Length, SocketFlags.None);
            }
        }

        private void ConnectComplete(IAsyncResult _rslt)
        {
            base.EndConnect(_rslt);
            base.BeginReceive(m_buff, 0, m_buff.Length, SocketFlags.None, Receive, this);
        }

        private void Receive(IAsyncResult _rslt)
        {
            if (Connected)
            {
                ByteBuffer buff = new ByteBuffer(m_buff);

                //패킷 헤더
                byte[] packetHeader = buff.ToArray(0, 4);
                BitArray packetHeaderBit = new BitArray(packetHeader);

                //패킷 압죽 여부 확인 비트
                bool isCompress = packetHeaderBit.Get(31);

                //패킷 메세지 id
                byte[] msgIDByte = buff.ToArray(4, 4);

                // If the system architecture is little-endian (that is, little end first),
                // reverse the byte array.
                Array.Reverse(msgIDByte);
                EPacketProtocol msgID = (EPacketProtocol)BitConverter.ToInt32(msgIDByte, 0);

                //패킷 body 길이
                int msgBodyLen = 0;

                if (isCompress)
                {
                    packetHeaderBit.Set(31, false);
                }

                msgBodyLen = BitArrayConverter.BitArrayToInt32(packetHeaderBit);
                byte[] msgBody = buff.ToArray(8, msgBodyLen);

                if (isCompress)
                {
                    byte[] uncompressPacketData = new byte[LZ4Codec.MaximumOutputSize(msgBodyLen)];
                    LZ4Codec.Decode(msgBody, 0, msgBodyLen, uncompressPacketData, 0, uncompressPacketData.Length);
                    Array.Clear(msgBody, 0, msgBody.Length);
                    msgBody = uncompressPacketData;
                }
            }
        }
    }
}

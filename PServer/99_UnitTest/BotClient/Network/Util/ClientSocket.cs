using BotClient.Network.Const;
using FlatBuffers;
using K4os.Compression.LZ4;
using Newtonsoft.Json.Linq;
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
    public class ClientSocket : Socket
    {
        private byte[] m_buff= new byte[NetworkGlobalConst.MAX_PACKET_BINARY_SIZE];

        private int m_nHostID;
        public int HostID
        {
            get { return m_nHostID; }
            set { m_nHostID = value; }
        }

        private int m_nConnectServerID;
        public int ConnectServerID
        {
            get { return m_nConnectServerID; }
        }

        private string m_sConnectServerName;
        public string ConnectServerName
        {
            get { return m_sConnectServerName; }
        }

        private bool m_bIsAddedGridRow;
        public bool AddedGridRow
        {
            get { return m_bIsAddedGridRow; }
            set { m_bIsAddedGridRow = value; }
        }

        public ClientSocket() : base(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp)
        {
        }

        public void Connect(JToken _serverInfo)
        {
            m_nConnectServerID = int.Parse(_serverInfo["ServerID"].ToString());
            m_sConnectServerName = _serverInfo["Name"].ToString();

            if(!Connected)
                base.BeginConnect(new IPEndPoint(IPAddress.Parse(_serverInfo["BindAddress"].ToString()), int.Parse(_serverInfo["BindPort"].ToString())), ConnectComplete, this);
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

            MainForm.form.SetSocketConnectStatus(m_nHostID, ConnectServerName + "(" +ConnectServerID+ ")", ((Connected) ? "Connected" : "Disconnected") );

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

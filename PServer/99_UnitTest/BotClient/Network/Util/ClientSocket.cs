using BotClient.Network.Const;
using BotClient.Network.Data;
using FlatBuffers;
using K4os.Compression.LZ4;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;

using System.Runtime.InteropServices;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BotClient.Network.Util
{
    public class ClientSocket : Socket
    {
        public event SocketConnect socketConnectEvt;

        private byte[] m_buff = new byte[NetworkGlobalConst.MAX_PACKET_BINARY_SIZE];
        //<time, content>
        private Dictionary<int, JObject> m_oSendPacketLogList = new Dictionary<int, JObject>();
        private Dictionary<int, JObject> m_oReceivePacketLogList = new Dictionary<int, JObject>();

        private int m_nPacketReceiveCount = 0;
        private int m_nPacketSendCount = 0;

        public Dictionary<int, JObject> SendPacketLogList
        {
            get { return m_oSendPacketLogList; }
        }

        public Dictionary<int, JObject> ReceivePacketLogList
        {
            get { return m_oReceivePacketLogList; }
        }


        VOBuilder m_oVOb = VOBuilder.Instance;

        public static event EventHandler Disconnected;

        private static void DisconnectEvent()
        {
            EventHandler lHandler = Disconnected;
            if (lHandler != null)
            {
                lHandler(null, EventArgs.Empty);
            }
        }

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
            try
            {
                base.BeginConnect(new IPEndPoint(IPAddress.Parse(_serverInfo["BindAddress"].ToString()), int.Parse(_serverInfo["BindPort"].ToString())), ConnectComplete, this);
            }
            catch (SocketException _se)
            {
                Console.WriteLine("HostID: {0}, {1}", m_nHostID, _se.Message);
            }
            catch (Exception _e)
            {
                Console.WriteLine("HostID: {0}, {1}", m_nHostID, _e.Message);
            }
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
            if (Connected)
            {
                base.EndConnect(_rslt);
                socketConnectEvt(m_nHostID, ConnectServerName + "(" + ConnectServerID + ")", ((Connected) ? "Connected" : "Disconnected"));
                base.BeginReceive(m_buff, 0, m_buff.Length, SocketFlags.None, Receive, this);
            }
        }

        private void Receive(IAsyncResult _rslt)
        {
            try
            {
                SocketError lSE;
                int receiveLen = base.EndReceive(_rslt, out lSE);

                if (receiveLen <= 0)
                {
                    SocketClose();
                    return;
                }

                if (lSE != SocketError.Success)
                {
                    SocketClose();
                    return;
                }

                //ReceiveTime
                string lReceiveTime = DateTime.Now.ToString();

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
                ByteBuffer bb = new ByteBuffer(msgBody);

                JObject jObj = m_oVOb.ConvertByteBufferToJObject((int)msgID, bb);
                jObj.Add("Time", lReceiveTime);

                AddPacketLog(false, jObj);
            }
            catch (ArgumentException _ae)
            {
                Console.WriteLine("ArgumentException - " + _ae.Message);
                SocketClose();
            }
            catch (Exception _e)
            {
                Console.WriteLine("ClientSocket ReceivedCallback - " + _e.Message);
                SocketClose();
            }
        }

        public void AddPacketLog(bool _isSend, JObject _packetData)
        {
            if (_isSend)
            {
                ++m_nPacketSendCount;
                m_oSendPacketLogList.Add(m_nPacketSendCount, _packetData);
            }
            else
            {
                ++m_nPacketReceiveCount;
                m_oReceivePacketLogList.Add(m_nPacketReceiveCount, _packetData);
            }
        }

        private void SocketClose()
        {
            m_oReceivePacketLogList.Clear();
            m_oSendPacketLogList.Clear();
            base.Dispose();
            base.Close();
        }
    }
}

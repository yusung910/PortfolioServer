using FlatBuffers;
using K4os.Compression.LZ4;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Net;
using System.Net.Http.Headers;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections;

using BotClient.Network.Const;
using BotClient.Network.Util;
using System.Threading;
using Newtonsoft.Json;
using System.IO;
using Newtonsoft.Json.Linq;
using BotClient.Log;
using BotClient.Config;
using BotClient.Util;
using BotClient.Network.Data;
//using BotClient.Network.Util;
//https://nowonbun.tistory.com/685
namespace BotClient.Network
{
    public class NetworkManager : Singleton<NetworkManager>
    {
        PacketDataBuilder m_oPacketBuilder = PacketDataBuilder.Instance;
        VOBuilder m_oVOb = VOBuilder.Instance;
        private string m_sIp;
        public string IP
        {
            get { return m_sIp; }
            set { m_sIp = value; }
        }

        private int m_nPort;
        public int PORT
        {
            get { return m_nPort; }
            set { m_nPort = value; }
        }

        List<ClientSocket> m_ClientSocketList = new List<ClientSocket>();

        private int m_nConnectCount;
        public int ConnectCount
        {
            get { return m_nConnectCount; }
            set { m_nConnectCount = value; }
        }



        public NetworkManager() { }

        public void Connect()
        {
            int lBeginHostID = m_ClientSocketList.Count;
            for (int i = 0; i < m_nConnectCount; i++)
            {
                var lSocket = new ClientSocket(m_sIp, m_nPort);
                lSocket.HostID = i + lBeginHostID;
                m_ClientSocketList.Add(lSocket);
            }
        }

        public void Send(EPacketProtocol _msgID)
        {
            //패킷 구조체(strucT)별로 데이터를 생성하고 생성된 builder를 반환한다.
            for (int i = 0; i < m_ClientSocketList.Count; i++)
            {
                FlatBufferBuilder fbb = m_oPacketBuilder.SetPacketBuildData(_msgID, m_oVOb.GenerateVO(_msgID, i));
                Packet.Instance.SetPacketData(_msgID, fbb);
                m_ClientSocketList[i].Send(Packet.Instance.Buffer);
            }
        }

        public void Disconnect()
        {
        }
    }
}

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

namespace BotClient.Network
{
    public class NetworkManager : Singleton<NetworkManager>
    {
        PacketDataBuilder m_oPacketBuilder = PacketDataBuilder.Instance;
        VOBuilder m_oVOb = VOBuilder.Instance;

        private List<ClientSocket> m_ClientSocketList = new List<ClientSocket>();

        public List<ClientSocket> ClientSocketList
        {
            get { return m_ClientSocketList; }
        }

        private int m_nConnectCount;
        public int ConnectCount
        {
            get { return m_nConnectCount; }
            set { m_nConnectCount = value; }
        }

        public NetworkManager() { }

        public void GenerateSocket()
        {
            int lBeginHostID = m_ClientSocketList.Count;
            for (int i = 0; i < m_nConnectCount; i++)
            {
                var lSocket = new ClientSocket();
                lSocket.HostID = i + lBeginHostID;
                m_ClientSocketList.Add(lSocket);
            }
        }

        public void ConnectSockets(JToken _serverInfo)
        {
            foreach (var lSocket in m_ClientSocketList)
            {
                lSocket.Connect(_serverInfo);
            }
        }

        public void Send(EPacketProtocol _msgID, int _hostID)
        {
            //패킷 구조체(strucT)별로 데이터를 생성하고 생성된 builder를 반환한다.
            if (m_ClientSocketList.Count == 0)
            {
                MessageBox.Show("Generate Socket!");
                return;
            }


            List<ClientSocket> _socketList = new List<ClientSocket>();

            if (_hostID > -1)
            {
                foreach (var lSocket in m_ClientSocketList)
                {
                    if (lSocket.HostID == _hostID)
                    {
                        _socketList.Add(lSocket);
                    }
                }
            }
            else
            {
                _socketList = m_ClientSocketList;
            }

            Broadcast(_msgID, _socketList);
        }


        public void Broadcast(EPacketProtocol _msgID, List<ClientSocket> _socketList)
        {
            //패킷 구조체(strucT)별로 데이터를 생성하고 생성된 builder를 반환한다.
            if (_socketList.Count == 0)
            {
                MessageBox.Show("Generate Socket!");
                return;
            }

            foreach (var lSocket in _socketList)
            {
                InnerPacketVO lInnerPacket = new InnerPacketVO((int)_msgID);

                m_oVOb.Socket = lSocket;
                var lVo = m_oVOb.GenerateVO(_msgID);
                FlatBufferBuilder fbb = m_oPacketBuilder.SetPacketBuildData(_msgID, lVo);
                Packet.Instance.SetPacketData(_msgID, fbb);

                lSocket.Send(Packet.Instance.Buffer);
            }
        }

        public void Disconnect()
        {
        }
    }
}

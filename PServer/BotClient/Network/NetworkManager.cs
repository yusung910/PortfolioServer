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
//using BotClient.Network.Util;

namespace BotClient.Network
{
    public class NetworkManager
    {
        bool m_IsConnected = false;

        Socket m_oSocket;
        Packet mPacket;

        public NetworkManager() {}

        public void Connect()
        {
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 35201);
            m_oSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            Packet args = new Packet();
            args.Completed += ConnectComplete;
            args.RemoteEndPoint = endPoint;

            m_oSocket.ConnectAsync(args);

            m_IsConnected = true;
        }

        public void ConnectComplete(object _obj, SocketAsyncEventArgs _args)
        {
            if (_args.SocketError == SocketError.Success)
            {
                //데이터 송신 전용 객체
                mPacket = new Packet();
                mPacket.Completed += OnSendCompleted;

                //데이터 수신 객체
                Packet recvArgs = new Packet();
                recvArgs.SetBuffer(new byte[NetworkGlobalConst.MAX_PACKET_DATA_SIZE], 0, NetworkGlobalConst.MAX_PACKET_DATA_SIZE);
                recvArgs.Completed += new EventHandler<SocketAsyncEventArgs>(OnRecvComplete);

                bool isPending = m_oSocket.ReceiveAsync(recvArgs);
                if (isPending == false)
                    OnRecvComplete(null, recvArgs);

            }
            else
            {
                MessageBox.Show("Socket Error!");
            }

        }

        public void OnRecvComplete(object obj, SocketAsyncEventArgs _args)
        {
            if (_args.BytesTransferred > 0 && _args.SocketError == SocketError.Success)
            {

                //압축 여부는 패킷의 맨 앞에 설정 되어 있음.
                //LZ4를 이용해서 크기에 따라 압축, 해제 작업이 필요하다.
                //https://github.com/IonKiwi/lz4.net
                //buff.Get(0~3)->패킷 사이즈
                //buff.Get(4~7)->MessageID
                //others -> 데이터
                ByteBuffer buff = new ByteBuffer(_args.Buffer);
                
                //패킷 헤더
                byte[] packetHeader = buff.ToArray(0, 4);
                BitArray packetHeaderBit = new BitArray(packetHeader);

                //패킷 압죽 여부 확인 비트
                bool isCompress = packetHeaderBit.Get(31);

                //패킷 메세지 id
                byte[] msgID = buff.ToArray(4, 4);

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
                
                // 새로운 데이터 수신을 준비합니다.
                bool pending = m_oSocket.ReceiveAsync(_args);
                if (pending == false)
                    OnRecvComplete(null, _args);
            }
        }

        public void OnSendCompleted(object obj, SocketAsyncEventArgs _args)
        {
            if (_args.BytesTransferred > 0 && _args.SocketError == SocketError.Success)
                mPacket.BufferList = null;
        }

        public void Send()
        {
            if (mPacket == null)
            {
                MessageBox.Show("Server Connect is Fail");
                return;
            }

            string[] data = { "yusung910@nate.com", "ABCDEFGABC" };
            mPacket.SetPacketData(EPacketProtocol.CS_AuthReq, data);

            m_oSocket.SendAsync(mPacket);
        }

        public void Disconnect()
        {
            if (m_oSocket != null)
            {
                m_oSocket.Disconnect(true);
                m_oSocket.Close();
                m_oSocket = null;
            }
        }
    }
}

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

namespace BotClient.Network
{
    public class NetworkManager
    {

        private static int MAX_PACKET_BINARY_SIZE = 8192 * 2;              // 패킷 바이너리 최대 크기 (Payload)
        private static int PACKET_HEADER_SIZE = 8;                 // size 4 + protocol 4
        private static int MAX_PACKET_DATA_SIZE = MAX_PACKET_BINARY_SIZE - PACKET_HEADER_SIZE;	// 패킷에 들어갈수 있는 최대 데이터 크기
        private static int DEFAULT_PACKET_COMPRESS_START_SIZE = 60;	// 패킷 압축 최소 기준
        private static uint PACKET_COMPRESS_MASK = 0x80000000;
        bool m_IsConnected = false;

        Socket m_oSocket;

        SocketAsyncEventArgs m_oSendSAArgs;

        Packet mPacket;

        public NetworkManager() {}

        public void Connect()
        {
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 35201);
            m_oSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            //SocketAsyncEventArgs args = new SocketAsyncEventArgs();
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
                recvArgs.SetBuffer(new byte[MAX_PACKET_DATA_SIZE], 0, MAX_PACKET_DATA_SIZE);
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

                byte[] packetSize = buff.ToArray(0, 4);
                byte[] msgID = buff.ToArray(4, 4);

                bool isCompress = BitConverter.ToBoolean(packetSize, 0);

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

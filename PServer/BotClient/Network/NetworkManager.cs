using FlatBuffers;
using System;
using System.Collections.Generic;
using System.ComponentModel;
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

        bool m_IsConnected = false;

        Socket m_oSocket;

        SocketAsyncEventArgs m_oSendSAArgs;

        public NetworkManager() { }

        public void Connect()
        {
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 35201);
            m_oSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            SocketAsyncEventArgs args = new SocketAsyncEventArgs();
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
                m_oSendSAArgs = new SocketAsyncEventArgs();
                m_oSendSAArgs.Completed += OnSendCompleted;

                //데이터 수신 객체
                SocketAsyncEventArgs recvArgs = new SocketAsyncEventArgs();
                recvArgs.SetBuffer(new byte[MAX_PACKET_DATA_SIZE], 0 , MAX_PACKET_DATA_SIZE);
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
                //buff.Get(0~3)->패킷 사이즈
                //buff.Get(4~7)->MessageID
                //others -> 데이터
                ByteBuffer buff = new ByteBuffer(_args.Buffer);
                //패킷 사이즈
                int payLoadSize = 0;
                IFlatbufferObject packetObj = null;
                EPacketProtocol msgID = (EPacketProtocol)13;
                switch (msgID)
                {
                    case EPacketProtocol.SC_AuthRes:

                        break;
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
                m_oSendSAArgs.BufferList = null;
        }

        public void Send()
        {
            if (m_oSendSAArgs == null)
            {
                MessageBox.Show("Server Connect is Fail");
                return;
            }


            //Flatbuffer 데이터 빌드 할 때 각각 패킷에 들어갈 데이터 먼저 빌드 해줘야한다(CreateString 등등을 먼저 해야한다)
            FlatBufferBuilder builder = new FlatBufferBuilder(1);
            var id = builder.CreateString("Test");
            var pw = builder.CreateString("1234");

            CSAuthReq.StartCSAuthReq(builder);

            CSAuthReq.AddAccountid(builder, id);
            CSAuthReq.AddAccountpw(builder, pw);
            
            var endoffset = CSAuthReq.EndCSAuthReq(builder);
            
            builder.Finish(endoffset.Value);
            var tmp = builder.SizedByteArray();

            int payloadSize = builder.SizedByteArray().Length + PACKET_HEADER_SIZE;
            byte[] payloadSizeByte = BitConverter.GetBytes(payloadSize);


            int tmpMsgID = (int)EPacketProtocol.CS_AuthReq;
            byte[] msgIDBytes = new byte[4];
            for (int i = 0; i < msgIDBytes.Length; i++)
            {
                msgIDBytes[i] = (byte)(tmpMsgID >> 24 - (i * 8));
            }

            byte[] packet = new byte[payloadSize];

            Array.Copy(payloadSizeByte, 0, packet, 0, msgIDBytes.Length);

            Array.Copy(msgIDBytes, 0, packet, 4, msgIDBytes.Length);


            Array.Copy(tmp, 0, packet, PACKET_HEADER_SIZE, tmp.Length);


            m_oSendSAArgs.SetBuffer(packet, 0, packet.Length);
            
            m_oSocket.SendAsync(m_oSendSAArgs);
        }
    }

}

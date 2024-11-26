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
        private static uint PACKET_COMPRESS_MASK = 0x80000000;
        private static int DEFAULT_PACKET_COMPRESS_START_SIZE = 60;
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
                //패킷 압축 여부에 따라 lz4를 이용해서 압축, 해제 한다
                //https://github.com/IonKiwi/lz4.net
                //buff.Get(0~3)->패킷 사이즈
                //buff.Get(4~7)->MessageID
                //others -> 데이터
                Byte firstByte = _args.Buffer[0];
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
            var id = builder.CreateString("yusung910@nate.com");
            var pw = builder.CreateString("1234123AB!!@");

            CSAuthReq.StartCSAuthReq(builder);

            CSAuthReq.AddAccountid(builder, id);
            CSAuthReq.AddAccountpw(builder, pw);
            
            var endoffset = CSAuthReq.EndCSAuthReq(builder);
            
            builder.Finish(endoffset.Value);
            //flatbuffer byte
            var tmp = builder.SizedByteArray();

            //body 사이즈
            int packetBody = builder.SizedByteArray().Length;

            //패킷 총 크기
            int payloadSize = packetBody + PACKET_HEADER_SIZE;

            //패킷 payload byte 배열
            byte[] payloadSizeByte = new byte[4];

            //패킷 body 사이즈에 따라 압축 여부 지정
            if (packetBody > DEFAULT_PACKET_COMPRESS_START_SIZE)
            {
                payloadSizeByte = BitConverter.GetBytes(payloadSize | PACKET_COMPRESS_MASK);
            }
            else
            {
                payloadSizeByte = BitConverter.GetBytes(payloadSize & ~PACKET_COMPRESS_MASK);
            }

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

            if (packetBody > DEFAULT_PACKET_COMPRESS_START_SIZE)
            {
                //패킷 암호화
                packet = LZ4.LZ4Codec.Wrap(packet);
            }

            m_oSendSAArgs.SetBuffer(packet, 0, packet.Length);
            
            m_oSocket.SendAsync(m_oSendSAArgs);
        }
    }

}

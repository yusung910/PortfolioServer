using FlatBuffers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
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
            var builder = new FlatBufferBuilder(1);
            var msg = HostConnect.CreateHostConnect(builder, builder.CreateString("127.0.0.1"), 35201, 1, (int)EPacketProtocol.Host_Connect);
            
            byte[] buffer = System.Text.Encoding.UTF8.GetBytes(builder.DataBuffer.ToString());
            m_oSendSAArgs.SetBuffer(buffer, 0, buffer.Length);
                
            m_oSocket.SendAsync(m_oSendSAArgs);
        }
    }

}

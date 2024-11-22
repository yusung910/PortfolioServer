using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


//https://nowonbun.tistory.com/246
//C#, .Net을 이용해서 BotClient를 시각적으로 이용하기 쉽게 만든다
namespace BotClient.Network
{
    class Client : SocketAsyncEventArgs
    {
        private static char CR = (char)0x0D;
        private static char LF = (char)0x0A;

        private static int MAX_PACKET_BINARY_SIZE = 8192;              // 패킷 바이너리 최대 크기 (Payload)
        private static int PACKET_HEADER_SIZE = 8;                 // size 4 + protocol 4
        private static int MAX_PACKET_DATA_SIZE = MAX_PACKET_BINARY_SIZE - PACKET_HEADER_SIZE;	// 패킷에 들어갈수 있는 최대 데이터 크기

        private Socket m_oSocket;

        // buffer
        private StringBuilder m_sb = new StringBuilder();

        //
        public Client(EndPoint pep)
        {
            RemoteEndPoint = pep;

            //접속 되었을 경우 발생  이벤트 등록
            base.Completed += Connected_Completed;
        }

        private void Connected_Completed(object sender, SocketAsyncEventArgs e)
        {
            //이벤트 해제
            base.Completed -= Connected_Completed;

            //접속 소켓 설정
            this.m_oSocket = e.ConnectSocket;
            base.UserToken = this.m_oSocket;

            //버퍼 설정
            base.SetBuffer(new byte[MAX_PACKET_DATA_SIZE], 0, MAX_PACKET_DATA_SIZE);

            // 수신 이벤트 등록
            base.Completed += Client_Completed;

            //메시지 도착 했을 때 이벤트 발생
            if(null != m_oSocket) this.m_oSocket.ReceiveAsync(this);
        }

        //메세지가 도착했을 경우
        private void Client_Completed(object sender, SocketAsyncEventArgs e)
        {
            if (m_oSocket.Connected && base.BytesTransferred > 0)
            {
                //수신 데이터는 SocketAsyncEventArgs.Buffer에 있음
                byte[] data = e.Buffer;
                //데이터를 형 변환, 추후 LZ4 라이브러리를 이용해서 압축된 데이터를 해제 시키는 처리가 필요하다
                string msg = Encoding.UTF8.GetString(data);

                //메모리 버퍼 초기화
                base.SetBuffer(new byte[MAX_PACKET_DATA_SIZE], 0, MAX_PACKET_DATA_SIZE);

                // 메세지의 끝이 이스케이그 \r\n과 >의 형태면 클라이언트에 표시
                if (m_sb.Length >= 3
                    && m_sb[m_sb.Length - 3] == CR
                    && m_sb[m_sb.Length - 2] == LF
                    && m_sb[m_sb.Length - 1] == '>')
                {
                    msg = m_sb.ToString();
                    m_sb.Clear();
                }

                this.m_oSocket.ReceiveAsync(this);
            }
            else
            {
                //접속 종료
                var remoteAddr = (IPEndPoint)m_oSocket.RemoteEndPoint;
                //
                Console.WriteLine($"Disconnected :  (From: {remoteAddr.Address.ToString()}:{remoteAddr.Port}, Connection time: {DateTime.Now})");
            }
        }

        //Send
        public void Send(String _msg)
        {
            byte[] msg = Encoding.UTF8.GetBytes(_msg);

            m_oSocket.Send(msg, msg.Length, SocketFlags.None);
        }
    }
}

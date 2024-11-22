using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

//https://nowonbun.tistory.com/246
//C#, .Net을 이용해서 BotClient를 시각적으로 이용하기 쉽게 만든다
namespace BotClient.Network
{
    class Client : SocketAsyncEventArgs
    {
        private static char CR = (char)0x0D;
        private static char LF = (char)0x0A;

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

        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

//https://nowonbun.tistory.com/246
namespace BotClient.Network
{
    class Client : SocketAsyncEventArgs
    {
        private static char CR = (char)0x0D;
        private static char LF = (char)0x0A;

        private Socket m_oSocket;

        // buffer
        private StringBuilder m_sb = new StringBuilder();
    }
}

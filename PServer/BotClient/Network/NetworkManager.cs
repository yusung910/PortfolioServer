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
    public class NetworkManager: Socket
    {
        public NetworkManager() : base(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp)
        {
            var client = new Client(new IPEndPoint(IPAddress.Parse("127.0.0.1"), 35201));
            base.ConnectAsync(client);

        }
    }
}

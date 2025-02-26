using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BotClient.Define.GStruct
{
    struct SocketListRow
    {
        public string HostID;
        public string ConnectedServer;
        public string ConnectStatus;

        public SocketListRow(string _hostID, string _connectedServer, string _connectStatus)
        {
            HostID = _hostID;
            ConnectedServer = _connectedServer;
            ConnectStatus = _connectStatus;
        }
    }
}

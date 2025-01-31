using BotClient.Network.Util;
using BotClient.Util;
using System;
using System.Collections.Generic;
using System.Data.SqlTypes;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace BotClient.Network.Data
{
    public class VOBuilder : Singleton<VOBuilder>
    {
        public VOBuilder() { }

        private ClientSocket m_oSocket = null;
        public ClientSocket Socket
        {
            get { return m_oSocket; }
            set { m_oSocket = value; }
        }
        


        public IInnerPacketVO GenerateVO(EPacketProtocol _msgID)
        {
            IInnerPacketVO ret = null;

            switch (_msgID)
            {
                case EPacketProtocol.CL_AuthReq:
                    {
                        ret = new LCAuthReqVO();
                        ((LCAuthReqVO)ret).AccountToken = "BOT_CLIENT_" + Socket.HostID;
                        ((LCAuthReqVO)ret).LoginPlatformType = 99;
                        ((LCAuthReqVO)ret).ClientType = 21;
                        ((LCAuthReqVO)ret).AppVersion = 55;
                    }
                    break;
            }
            return ret;
        }
    }
}

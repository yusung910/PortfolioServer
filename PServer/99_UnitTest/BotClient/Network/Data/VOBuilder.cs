using BotClient.Util;
using System;
using System.Collections.Generic;
using System.Data.SqlTypes;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BotClient.Network.Data
{
    public class VOBuilder : Singleton<VOBuilder>
    {
        public VOBuilder() { }


        public InnerPacketVO GenerateVO(EPacketProtocol _msgID, int _hostID)
        {
            InnerPacketVO ret = null;
            switch (_msgID)
            {
                case EPacketProtocol.CL_AuthReq:
                    {
                        ret = new LCAuthReqVO();
                        ((LCAuthReqVO)ret).AccountToken = "BOT_CLIENT_" + _hostID;
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

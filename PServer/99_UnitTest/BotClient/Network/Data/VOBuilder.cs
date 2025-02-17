using BotClient.Network.Util;
using BotClient.Util;
using FlatBuffers;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
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
                        ((LCAuthReqVO)ret).AccountID = "ABCDEF_ASDFE!@#_!@%$!@#%!@$%__#@$^%@#$^%#$__#$^#$^_#&DSFG_DRFGYE%$R__SEDFGASED_BOT_CLIENT_" + m_oSocket.HostID;
                        ((LCAuthReqVO)ret).LoginPlatformType = 99;
                        ((LCAuthReqVO)ret).ClientType = 21;
                        ((LCAuthReqVO)ret).AppVersion = 55;
                    }
                    break;
            }
            return ret;
        }

        public JObject ConvertByteBufferToJObject(int _msgID, ByteBuffer _buff)
        {
            IFlatbufferObject obj = null;
            JObject retObj = new JObject();
            retObj.Add("PacketID", _msgID.ToString());
            retObj.Add("PacketName", Enum.GetName(typeof(EPacketProtocol), _msgID));

            switch ((EPacketProtocol)_msgID)
            {
                case EPacketProtocol.LC_AuthRes:
                    obj = LCAuthRes.GetRootAsLCAuthRes(_buff);
                    break;

                case EPacketProtocol.SC_IntegrationErrorNotification:
                    obj = SCIntegrationErrorNotification.GetRootAsSCIntegrationErrorNotification(_buff);
                    break;  
            }

            retObj.Add("Data", JObject.Parse(JsonConvert.SerializeObject(obj)));
            return retObj;
        }
    }
}

using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace BotClient.Network.Data
{
    public class InnerPacketVO
    {
        public InnerPacketVO(int _msgID)
        {
            PacketID = (int)_msgID;
            PacketName = Enum.GetName(typeof(EPacketProtocol), _msgID);
        }

        public string PacketName = "";
        public int PacketID = -1;
    }

    public class LCAuthReqVO : InnerPacketVO
    {
        //플랫폼(google, naver, facebook 등등에서 얻어오는 토큰값)
        //또는 클라에서 접속 할 때 사용할 기본키, 계정처럼 사용되는 값
        public string AccountToken;
        //로그인 플랫폼(google, naver, facebook 등등)
        public int LoginPlatformType;
        // 클라 유형(pc, mobile 등등)
        public int ClientType;
        // 어플 버전
        public int AppVersion;

        public LCAuthReqVO(int _msgID) : base(_msgID) { }

        public override string ToString()
        {
            return JsonConvert.SerializeObject(this);
        }
    }
}

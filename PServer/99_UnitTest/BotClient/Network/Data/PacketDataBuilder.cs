using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using BotClient.Util;
using FlatBuffers;

namespace BotClient.Network.Data
{
    public class PacketDataBuilder : Singleton<PacketDataBuilder>
    {
        public PacketDataBuilder() { }

        public FlatBufferBuilder SetPacketBuildData(EPacketProtocol _msgID, IInnerPacketVO _args)
        {
            FlatBufferBuilder retBuilder = new FlatBufferBuilder(1);

            retBuilder.Clear();

            switch (_msgID)
            {
                case EPacketProtocol.CL_AuthReq:
                    var data = (LCAuthReqVO)_args;
                    var accountToken = retBuilder.CreateString(data.AccountToken);

                    CLAuthReq.StartCLAuthReq(retBuilder);
                    CLAuthReq.AddAccountToken(retBuilder, accountToken);
                    CLAuthReq.AddLoginPlatformType(retBuilder, data.LoginPlatformType);
                    CLAuthReq.AddClientType(retBuilder, data.ClientType);
                    CLAuthReq.AddAppVersion(retBuilder, data.AppVersion);

                    var endoffset = CLAuthReq.EndCLAuthReq(retBuilder);
                    retBuilder.Finish(endoffset.Value);
                    break;
            }

            return retBuilder;
        }
    }
}

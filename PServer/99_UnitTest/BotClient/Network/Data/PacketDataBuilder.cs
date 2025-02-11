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

        public FlatBufferBuilder SetPacketBuildData(EPacketProtocol _msgID, InnerPacketVO _args)
        {
            FlatBufferBuilder retBuilder = new FlatBufferBuilder(1);

            retBuilder.Clear();

            switch (_msgID)
            {
                case EPacketProtocol.CL_AuthReq:
                    var lData = (LCAuthReqVO)_args.Data;
                    var accountID = retBuilder.CreateString(lData.AccountID);

                    CLAuthReq.StartCLAuthReq(retBuilder);
                    CLAuthReq.AddAccountID(retBuilder, accountID);
                    CLAuthReq.AddLoginPlatformType(retBuilder, lData.LoginPlatformType);
                    CLAuthReq.AddClientType(retBuilder, lData.ClientType);
                    CLAuthReq.AddAppVersion(retBuilder, lData.AppVersion);

                    var endoffset = CLAuthReq.EndCLAuthReq(retBuilder);
                    retBuilder.Finish(endoffset.Value);
                    break;
            }

            return retBuilder;
        }
    }
}

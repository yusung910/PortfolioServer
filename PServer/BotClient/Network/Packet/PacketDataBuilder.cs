using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using BotClient.Util;
using FlatBuffers;

namespace BotClient.Network
{
    public class PacketDataBuilder : Singleton<PacketDataBuilder>
    {
        public PacketDataBuilder() { }

        public FlatBufferBuilder SetPacketBuildData(EPacketProtocol _msgID, string[] _args)
        {
            FlatBufferBuilder retBuilder = new FlatBufferBuilder(1);

            retBuilder.Clear();

            switch (_msgID)
            {
                case EPacketProtocol.CS_AuthReq:
                    var id = retBuilder.CreateString(_args[0]);
                    var pw = retBuilder.CreateString(_args[1]);

                    CSAuthReq.StartCSAuthReq(retBuilder);

                    CSAuthReq.AddAccountid(retBuilder, id);
                    CSAuthReq.AddAccountpw(retBuilder, pw);

                    var endoffset = CSAuthReq.EndCSAuthReq(retBuilder);
                    retBuilder.Finish(endoffset.Value);
                    break;
            }

            return retBuilder;
        }

        public string[] GetPacketBuildData(EPacketProtocol _msgID, byte[] _msg)
        {
            ByteBuffer bb = new ByteBuffer(_msg);
            string[] ret = new string[0];
            switch (_msgID)
            {
                case EPacketProtocol.SC_AuthRes:
                    SCAuthRes res = SCAuthRes.GetRootAsSCAuthRes(bb);
                    res.ToString();
                    break;
            }


            return ret;
        }

    }
}

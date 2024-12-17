using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FlatBuffers;

namespace BotClient.Network
{
    public class PacketDataBuilder
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

    }
}

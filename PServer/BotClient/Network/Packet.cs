using FlatBuffers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BotClient.Network
{
    public class Packet
    {
        FlatBufferBuilder builder;
        public Packet()
        {
            builder = new FlatBufferBuilder(1);
        }

        EPacketProtocol m_MsgID
        {
            get { return m_MsgID; }
            set { m_MsgID = value; }
        }

        public void SetPacketData()
        {
            //Offset<SCAuthRes> test;
        }

    }
}

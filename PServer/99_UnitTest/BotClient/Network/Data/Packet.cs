using FlatBuffers;
using System;
using BotClient.Network.Const;
using BotClient.Network.Util;
using System.Collections;
using BotClient.Util;

using PacketCompPlugin;

namespace BotClient.Network.Data
{
    public class Packet : Singleton<Packet>
    {

        private bool m_bIsCompress = false;
        private int m_nHostID = 0;

        public int ClientHostID
        {
            get { return m_nHostID; } set { m_nHostID = value; }
        }

        private byte[] m_buff;
        public byte[] Buffer
        {
            get { return m_buff; }
        }

        public Packet() { }

        public unsafe void SetPacketData(EPacketProtocol _msgID, FlatBufferBuilder builder)
        {
            //body 패킷
            var lBodyPacket = builder.SizedByteArray();
            int lBodyPacketLen = lBodyPacket.Length;
            //패킷 압축 여부 비트 지정
            m_bIsCompress = (lBodyPacket.Length > NetworkGlobalConst.DEFAULT_PACKET_COMPRESS_START_SIZE);

            //byte[] msgIDBytes = new byte[4];
            byte[] msgIDBytes = BitArrayConverter.BitArrayToByteArray(new BitArray(new int[] { (int)_msgID }));

            //패킷 크기를 저장하는 byte
            byte[] lPayloadSizeByte = new byte[4];

            //
            int PacketbodyLen = 0;

            if (m_bIsCompress)
            {
                PacketCompPlugin.Compressor compressor = new PacketCompPlugin.Compressor();

                sbyte[] tmpBodyPacket = Array.ConvertAll(lBodyPacket, b => unchecked((sbyte)b));
                //int* 
                //bool bIsComp = compressor.Compress(tmpBodyPacket, );
                ////패킷 압축에 사용될 임시 변수
                //byte[] compressPacketData = new byte[LZ4Codec.MaximumOutputSize(lBodyPacket.Length)];
                ////
                //lPayloadSizeByte = BitConverter.GetBytes((uint)(lBodyPacket.Length + NetworkGlobalConst.PACKET_HEADER_SIZE) | NetworkGlobalConst.PACKET_COMPRESS_MASK);
                ////패킷 암축
                //PacketbodyLen = LZ4Codec.Encode(lBodyPacket, 0, lBodyPacket.Length, compressPacketData, 0, compressPacketData.Length, LZ4Level.L09_HC);

                //lBodyPacket = compressPacketData;
            }
            else
            {
                lPayloadSizeByte = BitConverter.GetBytes((uint)(lBodyPacket.Length + NetworkGlobalConst.PACKET_HEADER_SIZE) & ~NetworkGlobalConst.PACKET_COMPRESS_MASK);
                PacketbodyLen = lBodyPacket.Length;
            }

            //패킷 데이터 세팅
            m_buff = new byte[PacketbodyLen + NetworkGlobalConst.PACKET_HEADER_SIZE];

            Array.Copy(lPayloadSizeByte, 0, m_buff, 0, msgIDBytes.Length);

            Array.Copy(msgIDBytes, 0, m_buff, 4, msgIDBytes.Length);
            
            Array.Copy(lBodyPacket, 0, m_buff, NetworkGlobalConst.PACKET_HEADER_SIZE, PacketbodyLen);
        }
    }
}

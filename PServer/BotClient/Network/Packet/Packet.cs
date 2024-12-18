using FlatBuffers;
using K4os.Compression.LZ4;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using BotClient.Network.Const;
using BotClient.Network.Util;
using System.Collections;
namespace BotClient.Network
{
    public class Packet : SocketAsyncEventArgs
    {
        private bool m_bIsCompress = false;

        PacketDataBuilder m_PacketBuilder = new PacketDataBuilder();

        public Packet() { }

        public void SetPacketData(EPacketProtocol _msgID, string[] _args)
        {
            //패킷 구조체(strucT)별로 데이터를 생성하고 생성된 builder를 반환한다.
            FlatBufferBuilder builder = m_PacketBuilder.SetPacketBuildData(_msgID, _args);

            //body 패킷
            var lBodyPacket = builder.SizedByteArray();

            //패킷 압축 여부 비트 지정
            m_bIsCompress = (lBodyPacket.Length > NetworkGlobalConst.DEFAULT_PACKET_COMPRESS_START_SIZE);

            //byte[] msgIDBytes = new byte[4];
            byte[] msgIDBytes = BitArrayConverter.IntToBitArray((int)_msgID);

            //패킷 크기를 저장하는 byte
            byte[] lPayloadSizeByte = new byte[4];

            //
            int PacketbodyLen = 0;

            if (m_bIsCompress)
            {
                //패킷 압축에 사용될 임시 변수
                byte[] compressPacketData = new byte[LZ4Codec.MaximumOutputSize(lBodyPacket.Length)];
                //
                lPayloadSizeByte = BitConverter.GetBytes((uint)(lBodyPacket.Length + NetworkGlobalConst.PACKET_HEADER_SIZE) | NetworkGlobalConst.PACKET_COMPRESS_MASK);
                //패킷 암호화
                PacketbodyLen = LZ4Codec.Encode(lBodyPacket, 0, lBodyPacket.Length, compressPacketData, 0, compressPacketData.Length);

                lBodyPacket = compressPacketData;
            }
            else
            {
                lPayloadSizeByte = BitConverter.GetBytes((uint)(lBodyPacket.Length + NetworkGlobalConst.PACKET_HEADER_SIZE) & ~NetworkGlobalConst.PACKET_COMPRESS_MASK);
                PacketbodyLen = lBodyPacket.Length;
            }

            //패킷 데이터 세팅
            byte[] buf = new byte[PacketbodyLen + NetworkGlobalConst.PACKET_HEADER_SIZE];

            Array.Copy(lPayloadSizeByte, 0, buf, 0, msgIDBytes.Length);

            Array.Copy(msgIDBytes, 0, buf, 4, msgIDBytes.Length);
            
            Array.Copy(lBodyPacket, 0, buf, NetworkGlobalConst.PACKET_HEADER_SIZE, PacketbodyLen);

            SetBuffer(buf, 0, buf.Length);
        }
    }
    
}

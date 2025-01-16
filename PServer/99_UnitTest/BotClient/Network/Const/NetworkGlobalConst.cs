using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BotClient.Network.Const
{
    public static class NetworkGlobalConst
    {
        public const int MAX_PACKET_BINARY_SIZE = 8192 * 2;              // 패킷 바이너리 최대 크기 (Payload)
        public const int PACKET_HEADER_SIZE = 8;                 // size 4 + protocol 4
        public const int MAX_PACKET_DATA_SIZE = MAX_PACKET_BINARY_SIZE - PACKET_HEADER_SIZE;	// 패킷에 들어갈수 있는 최대 데이터 크기
        public const int DEFAULT_PACKET_COMPRESS_START_SIZE = 60;	// 패킷 압축 최소 기준
        public const uint PACKET_COMPRESS_MASK = 0x80000000;
    }
}

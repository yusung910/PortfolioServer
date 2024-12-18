using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BotClient.Network.Util
{
    public static class BitArrayConverter
    {

        public static byte[] BitArrayToByteArray(BitArray bits)
        {
            byte[] ret = new byte[(bits.Length - 1) / 8 + 1];
            bits.CopyTo(ret, 0);
            return ret;
        }

        public static int BitArrayToInt32(BitArray bits)
        {
            if (bits.Length > 32)
                throw new ArgumentException("Argument length shall be at most 32 bits.");

            int[] array = new int[1];
            bits.CopyTo(array, 0);
            return array[0];
        }
    }
}

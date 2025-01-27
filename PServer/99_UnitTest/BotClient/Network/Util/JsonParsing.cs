using BotClient.Util;
using FlatBuffers;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BotClient.Network.Util
{
    public class JsonParsing : Singleton<JsonParsing>
    {
        public JsonParsing() { }

        public JObject ConvertToJsonObj(EPacketProtocol msgID, byte[] data)
        {
            ByteBuffer bb = new ByteBuffer(data);
            string tmpStr = null;
            //switch (msgID)
            //{
            //    //case EPacketProtocol.SC_AuthRes:
            //    //    var tmp = CSAuthReq.GetRootAsCSAuthReq(bb);
            //    //    tmpStr = JsonConvert.SerializeObject(tmp);
            //    //    break;
            //    default:
            //        Console.WriteLine("MsgID is not valid");
            //        return null;
            //}

            return JObject.Parse(tmpStr);
        }

        public T ConvertToFlatBuffer<T>(string json) where T : struct
        {
            return JsonConvert.DeserializeObject<T>(json);
        }
    }
}

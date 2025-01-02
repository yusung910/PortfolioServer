using BotClient.Network;
using BotClient.Util;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BotClient.Log
{
    public class LogManager : Singleton<LogManager>
    {
        public LogManager() { }


        public void Log(JObject _data)
        {
            if (_data != null)
            {
                IEnumerator<KeyValuePair<string, JToken>> _ienum = _data.GetEnumerator();
                while (true == _ienum.MoveNext())
                {
                    if (_ienum.Current.Key != "ByteBuffer")
                    { 
                        Console.WriteLine(_ienum.Current.ToString());  
                    }
                }
            }
            
        }
    }
}

using BotClient.Network;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BotClient.Util
{
    public class Singleton<T> where T : Singleton<T>, new()
    {
        //singleton
        private static readonly Lazy<T> _Instance = new Lazy<T>(() => new T());

        public static T Instance
        {
            get { return _Instance.Value; }
        }
    }
}

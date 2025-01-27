using BotClient.Network.Util;
using BotClient.Util;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BotClient.Config
{
    class LoadServerConfig : Singleton<LoadServerConfig>
    {
        private JArray m_sServerConfigList = new JArray();
        public JArray ServerConfigList
        {
            get { return m_sServerConfigList; }
        }

        //모든 서버의 ServerConfig 파일을 호출하고 Json Object로 파싱한다.
        public LoadServerConfig()
        {
            String lSolutionRoot = Directory.GetParent(Environment.CurrentDirectory).Parent.FullName + "\\..\\..";
            string[] lAllFiles = Directory.GetFiles(lSolutionRoot, "ServerConfig.json", SearchOption.AllDirectories);
            if (lAllFiles.Length > 0)
            {
                foreach (string sc in lAllFiles)
                {
                    var lMainListener = JObject.Parse(File.ReadAllText(sc))["MainListener"];
                    if(lMainListener["Name"].ToString() != "Login")
                        continue;
                    m_sServerConfigList.Add(lMainListener);
                }
            }
        }

        public JToken FindServerInfo(string _serverID)
        {
            JToken retObj = null;
            foreach (var jobj in m_sServerConfigList)
            {
                if (jobj["ServerID"].ToString() == _serverID)
                {
                    retObj = jobj;
                }
            }
            return retObj;
        }
    }
}

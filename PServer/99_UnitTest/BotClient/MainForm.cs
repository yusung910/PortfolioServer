using BotClient.Config;
using BotClient.Network;
using BotClient.Network.Data;
using FlatBuffers;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace BotClient
{
    public partial class MainForm : Form
    {
        NetworkManager m_oManager = NetworkManager.Instance;
        LoadServerConfig m_oServerConfig = LoadServerConfig.Instance;
        PacketDataBuilder m_oPacketBuilder = PacketDataBuilder.Instance;
        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            lbPacketList.DataSource = Enum.GetValues(typeof(EPacketProtocol));

            Dictionary<string, string> list = new Dictionary<string, string>();

            foreach (var info in m_oServerConfig.ServerConfigList)
            {
                list.Add(info["Name"].ToString(), info["ServerID"].ToString());
            }

            lbServerList.DataSource = new BindingSource(list, null);
            lbServerList.DisplayMember = "key";
            lbServerList.ValueMember = "value";

            txtSessionCount_Leave(sender, e);
        }

        private void lbPacketList_SelectedIndexChanged(object sender, EventArgs e)
        {
        }

        private void btnServerConnect_Click(object sender, EventArgs e)
        {
            string lstrCount = txtSessionCount.Text;
            decimal lCount;
            if (decimal.TryParse(lstrCount, out lCount))
            {
                m_oManager.ConnectCount = Decimal.ToInt32(lCount);
                m_oManager.Connect();
            }
            else
            {
                MessageBox.Show("Generate Count is not a Number!");
            }
        }

        private void btnPacketSend_Click(object sender, EventArgs e)
        {
            int msgID = (int)lbPacketList.SelectedValue;
            
            m_oManager.Send((EPacketProtocol)msgID);
        }

        private void btnLogClear_Click(object sender, EventArgs e)
        {

        }

        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            m_oManager.Disconnect();
        }

        private void lbServerList_SelectedIndexChanged(object sender, EventArgs e)
        {
            KeyValuePair<string, string> lSelectedMap = (KeyValuePair<string, string>)lbServerList.SelectedItem;
            string id = lSelectedMap.Value;

            var info = m_oServerConfig.FindServerInfo(id);

            m_oManager.IP = info["BindAddress"].ToString();
            m_oManager.PORT = int.Parse(info["BindPort"].ToString());

        }

        private void txtSessionCount_Enter(object sender, EventArgs e)
        {
            if (txtSessionCount.Text == "Count")
            {
                txtSessionCount.Text = "";
                txtSessionCount.ForeColor = Color.Black;
            }
        }

        private void txtSessionCount_Leave(object sender, EventArgs e)
        {
            if (txtSessionCount.Text == "")
            {
                txtSessionCount.Text = "Count";
                txtSessionCount.ForeColor = Color.Gray;
            }
        }

        private void txtSessionCount_TextChanged(object sender, EventArgs e)
        {

        }
    }
}

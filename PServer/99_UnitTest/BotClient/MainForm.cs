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

        public static MainForm form;

        public MainForm()
        {
            form = this;
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
            KeyValuePair<string, string> lSelectedMap = (KeyValuePair<string, string>)lbServerList.SelectedItem;
            string id = lSelectedMap.Value;

            m_oManager.ConnectSockets(m_oServerConfig.FindServerInfo(id));
        }

        private void btnPacketBroadcast_Click(object sender, EventArgs e)
        {
            int msgID = (int)lbPacketList.SelectedValue;
            
            m_oManager.Broadcast((EPacketProtocol)msgID);
        }

        private void btnLogClear_Click(object sender, EventArgs e)
        {

        }

        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            m_oManager.Disconnect();
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

        private void lbSocketList_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void btnGenerateSocketList_Click(object sender, EventArgs e)
        {
            string lstrCount = txtSessionCount.Text;
            decimal lCount;

            if (decimal.TryParse(lstrCount, out lCount))
            {
                m_oManager.ConnectCount = Decimal.ToInt32(lCount);
                m_oManager.GenerateSocket();

                foreach (var socket in m_oManager.ClientSocketList)
                {
                    if (socket.AddedGridRow == true) continue; 

                    string lHID = socket.HostID.ToString();
                    dgSocketList.Rows.Add(lHID, "none", "none");
                    socket.AddedGridRow = true;
                }
            }
            else
            {
                MessageBox.Show("Generate Count is not a Number!");
            }
        }

        public void SetSocketConnectStatus(int _hostID, string _connectServerName, string _connectStatus)
        {
            foreach (DataGridViewRow row in dgSocketList.Rows)
            {
                if (row.Cells["HostID"].Value == null)
                    continue;

                if (row.Cells["HostID"].Value.ToString() == _hostID.ToString())
                {
                    row.Cells["ConnectedServer"].Value = _connectServerName;
                    row.Cells["ConnectStatus"].Value = _connectStatus;
                }
            }
        }
    }
}

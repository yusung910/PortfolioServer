using BotClient.Network;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BotClient
{
    public partial class MainForm : Form
    {
        NetworkManager m_oManager = null;
        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            lbPacketList.DataSource = Enum.GetValues(typeof(EPacketProtocol));
            m_oManager = new NetworkManager();
        }

        private void lbPacketList_SelectedIndexChanged(object sender, EventArgs e)
        {
            int msgID = (int)lbPacketList.SelectedValue;
        }

        private void btnServerConnect_Click(object sender, EventArgs e)
        {
            m_oManager.Connect();
        }

        private void btnPacketSend_Click(object sender, EventArgs e)
        {
            m_oManager.Send();
        }
    }
}

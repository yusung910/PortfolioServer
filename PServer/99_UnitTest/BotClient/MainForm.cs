using BotClient.Config;
using BotClient.Define;
using BotClient.Define.GStruct;
using BotClient.Network;
using BotClient.Network.Util;
using BotClient.Service;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading;
using System.Windows.Controls;
using System.Windows.Forms;


namespace BotClient
{
    public delegate void SocketConnect(int _hostID, string _serverName, string _status);
    public delegate void SocketReceive(int _recvHostID);
    public delegate void SocketDisconnect(int _hostID);

    public delegate void SocketPacketLogList(int _hostID);

    public partial class MainForm : Form
    {
        NetworkManager m_oManager = NetworkManager.Instance;
        LoadServerConfig m_oServerConfig = LoadServerConfig.Instance;

        //ClientSocket m_oSelectedSocket = null;

        //패킷 검색
        Dictionary<EPacketProtocol, string> m_PacketSearchResult = new Dictionary<EPacketProtocol, string>();

        //패킷 로그 검색
        Dictionary<EPacketProtocol, string> m_PacketLogSearchResult = new Dictionary<EPacketProtocol, string>();

        //패킷 수신 관련 스레드
        private PacketRecvService m_recvService = new PacketRecvService();

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

        }

        private void lbPacketList_SelectedIndexChanged(object sender, EventArgs e)
        {
        }
        private void btnPacketBroadcast_Click(object sender, EventArgs e)
        {
            int msgID = (int)lbPacketList.SelectedValue;
            m_oManager.Send((EPacketProtocol)msgID, -1);
        }

        private void btnLogClear_Click(object sender, EventArgs e)
        {

        }

        private void btnDisconnect_Click(object sender, EventArgs e)
        {

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
                //Server info
                KeyValuePair<string, string> lSelectedMap = (KeyValuePair<string, string>)lbServerList.SelectedItem;
                string id = lSelectedMap.Value;
                m_oManager.ConnectCount = Decimal.ToInt32(lCount);
                m_oManager.GenerateSockets();

                foreach (var socket in m_oManager.ClientSocketList)
                {
                    if (socket.AddedGridRow == true) continue;

                    string lHID = socket.HostID.ToString();

                    dgSocketList.Rows.Add(lHID, "none", "none");

                    socket.AddedGridRow = true;
                    socket.socketConnectEvt += ShowSocketConnected;

                    socket.socketReceiveEvt += ShowSocketReceive;

                    socket.socketDisconnectEvt += ShowSocketDisconnected;
                }

                m_oManager.ConnectSockets(m_oServerConfig.FindServerInfo(id));

                m_recvService.Start();

                if (dgSocketList.SelectedRows[0].Cells[0].Value != null)
                {
                    string hostID = dgSocketList.SelectedRows[0].Cells [0].Value.ToString();
                    m_recvService.SetClientSocket(m_oManager.GetSocket(int.Parse(hostID)));
                }

                m_recvService.socketPacketLogList += ShowSocketPacketLogList;
            }
            else
            {
                MessageBox.Show("Generate Count is not a Number!");
            }
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            int msgID = (int)lbPacketList.SelectedValue;
            string hostID = dgSocketList.SelectedRows[0].Cells[0].Value.ToString();

            if (hostID == null)
            {
                MessageBox.Show("HostID is not a Number!");
                return;
            }

            m_oManager.Send((EPacketProtocol)msgID, int.Parse(hostID));
        }

        private void dgSocketList_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            try
            {
                var hostIDVal = dgSocketList.Rows[e.RowIndex].Cells["HostID"].Value;
                if(null == hostIDVal) return;

                string hostID = hostIDVal.ToString();

                m_recvService.SetClientSocket(m_oManager.GetSocket(int.Parse(hostID)));
                //소켓 통신 패킷 로그 리스트 제거
                if(lbSocketTelecomLog.Items.Count > 0) 
                    lbSocketTelecomLog.Items.Clear();
                //패킷 내부 데이터(node) 클리어
                if (trvLogData.Nodes != null)
                    trvLogData.Nodes.Clear();
            }
            catch(ArgumentOutOfRangeException _e)
            {
                Console.WriteLine(_e.Message);
            }

        }

        private void ShowSocketPacketLogList(int _hostID)
        {
            //Console.WriteLine("Selected Socket HostID : {0}", _hostID);
            if (lbSocketTelecomLog.InvokeRequired)
            {
                lbSocketTelecomLog.BeginInvoke(
                    (System.Action)(() =>
                    {
                        var logList = m_oManager.GetSocket(_hostID).PacketViewList;
                        var lbList = lbSocketTelecomLog.Items;

                        foreach (var log in logList)
                        {
                            if (false == lbList.Contains(log))
                            {
                                lbSocketTelecomLog.Items.Add(log);
                            }
                        }
                    })
                );
            }

        }

        private void ShowSocketConnected(int _hostID, string _serverName, string _status)
        {
            foreach (DataGridViewRow row in dgSocketList.Rows)
            {
                if (row.Cells["HostID"].Value == null)
                    continue;

                if (row.Cells["HostID"].Value.ToString() == _hostID.ToString())
                {
                    row.Cells["ConnectedServer"].Value = _serverName;
                    row.Cells["ConnectStatus"].Value = _status;
                }
            }
        }

        private void ShowSocketReceive(int _recvHostID)
        {
        }

        private void ShowSocketDisconnected(int _hostID)
        {
            m_oManager.Disconnect(_hostID);

            foreach (DataGridViewRow row in dgSocketList.Rows)
            {
                if(row.Cells["HostID"].Value == null)
                    continue;
                if (row.Cells["HostID"].Value.ToString() == _hostID.ToString())
                {
                    row.Cells["ConnectedServer"].Value = "None";
                    row.Cells["ConnectStatus"].Value = "Disconnected";
                }
            }
        }

        private void AddNode(JToken _token, TreeNode _node)
        {
            if (_token == null) return;

            if (_token is JValue)
            {
                var lChildNode = _node.Nodes[_node.Nodes.Add(new TreeNode(_token.ToString()))];
                lChildNode.Tag = _token;
            }
            else if (_token is JObject)
            {
                var obj = (JObject)_token;
                foreach (var prop in obj.Properties())
                {
                    var lChildNode = _node.Nodes[_node.Nodes.Add(new TreeNode(prop.Name))];
                    lChildNode.Tag = prop;
                    AddNode(prop.Value, lChildNode);
                }

            }
            else if (_token is JArray)
            {
                var arry = (JArray)_token;
                for (int i = 0; i < arry.Count; i++)
                {
                    var lChildNode = _node.Nodes[_node.Nodes.Add(new TreeNode(i.ToString()))];
                    lChildNode.Tag = arry[i];
                    AddNode(arry[i], lChildNode);
                }
            }
            else
            {
                Debug.WriteLine(string.Format("{0} not Implemented", _token.Type));
            }
        }

        private void tbPacketSearch_KeyUp(object sender, KeyEventArgs e)
        {
            string lSearchKeyword = tbPacketSearch.Text;
            Regex lRegex = new Regex(@lSearchKeyword);

            if (lSearchKeyword != null)
            {
                m_PacketSearchResult.Clear();
            }

            foreach (EPacketProtocol packetID in Enum.GetValues(typeof(EPacketProtocol)))
            {
                string PacketName = Enum.GetName(typeof(EPacketProtocol), packetID);
                bool isExist = m_PacketSearchResult.TryGetValue(packetID, out var extPacketName);

                if (isExist) continue;

                if (lRegex.IsMatch(PacketName))
                {
                    m_PacketSearchResult.Add(packetID, PacketName);
                }
            }

            if (m_PacketSearchResult.Count > 0)
            {
                lbPacketList.DataSource = new BindingSource(m_PacketSearchResult, null);
                lbPacketList.DisplayMember = "key";
                lbPacketList.ValueMember = "value";
            }
        }
        private void tbPacketLogSearch_KeyUp(object sender, KeyEventArgs e)
        {
            string lSearchKeyword = tbPacketLogSearch.Text;
            Regex lRegex = new Regex(@lSearchKeyword);

            if (lSearchKeyword != null)
            {
                m_PacketLogSearchResult.Clear();
            }

            //if (m_oSelectedSocket == null) return;

            //foreach (var item in lbSocketTelecomLog.Items)
            //{

            //}
        }

        private void tbRecvDataSearch_KeyUp(object sender, KeyEventArgs e)
        {

        }


        private void tbPacketSearch_Enter(object sender, EventArgs e)
        {
            frmTextboxPlaceHolder(tbPacketSearch, "", tbPacketSearch.Text);
        }
        private void tbPacketSearch_Leave(object sender, EventArgs e)
        {
            frmTextboxPlaceHolder(tbPacketSearch, "Search Keyword", tbPacketSearch.Text);
        }

        private void txtSessionCount_Enter(object sender, EventArgs e)
        {
            frmTextboxPlaceHolder(txtSessionCount, "", txtSessionCount.Text);
        }

        private void txtSessionCount_Leave(object sender, EventArgs e)
        {
            if (txtSessionCount != null) return;
            frmTextboxPlaceHolder(txtSessionCount, "Count", txtSessionCount.Text);
        }

        private void tbPacketLogSearch_Enter(object sender, EventArgs e)
        {
            frmTextboxPlaceHolder(tbPacketLogSearch, "", tbPacketLogSearch.Text);
        }

        private void tbPacketLogSearch_Leave(object sender, EventArgs e)
        {
            frmTextboxPlaceHolder(tbPacketLogSearch, "Search Keyword", tbPacketLogSearch.Text);
        }

        private void tbRecvDataSearch_Enter(object sender, EventArgs e)
        {
            frmTextboxPlaceHolder(tbRecvDataSearch, "", tbRecvDataSearch.Text);
        }

        private void tbRecvDataSearch_Leave(object sender, EventArgs e)
        {
            frmTextboxPlaceHolder(tbRecvDataSearch, "Search Keyword", tbRecvDataSearch.Text);
        }

        private void frmTextboxPlaceHolder(System.Windows.Forms.TextBox _tb, string _placeholder, string _inputTxt)
        {
            if (_tb.Text == _placeholder)
            {
                _tb.Text = "";
                _tb.ForeColor = Color.Black;
            }
            else
            {
                _tb.Text = _placeholder;
                _tb.ForeColor = Color.Gray;
            }
        }

        private void lbSocketTelecomLog_Click(object sender, EventArgs e)
        {
            if (lbSocketTelecomLog.SelectedItem == null)
                return;

            KeyValuePair<string, int> lSelectedLog = (KeyValuePair<string, int>)lbSocketTelecomLog.SelectedItem;

            int lSelectedPacketLogID = lSelectedLog.Value;

            JToken logDataToken = m_recvService.GetTargetClientSocket().PacketLogList[lSelectedPacketLogID]["Data"];

            trvLogData.BeginUpdate();

            if (logDataToken == null) return;

            try
            {
                trvLogData.Nodes.Clear();
                var tNode = trvLogData.Nodes[trvLogData.Nodes.Add(new TreeNode("Data"))];
                tNode.Tag = logDataToken["Data"];

                AddNode(logDataToken, tNode);

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            finally
            {
                trvLogData.ExpandAll();
                trvLogData.EndUpdate();
            }
        }
    }
}

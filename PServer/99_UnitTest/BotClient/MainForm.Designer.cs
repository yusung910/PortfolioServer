namespace BotClient
{
    partial class MainForm
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.tbPacketLogSearch = new System.Windows.Forms.TextBox();
            this.lbSocketTelecomLog = new System.Windows.Forms.ListBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btnSend = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.tbPacketSearch = new System.Windows.Forms.TextBox();
            this.lbPacketList = new System.Windows.Forms.ListBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.dgSocketList = new System.Windows.Forms.DataGridView();
            this.HostID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ConnectedServer = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ConnectStatus = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.lbServerList = new System.Windows.Forms.ListBox();
            this.btnPacketBroadcast = new System.Windows.Forms.Button();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.btnGenerateSocketList = new System.Windows.Forms.Button();
            this.txtSessionCount = new System.Windows.Forms.TextBox();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.tbRecvDataSearch = new System.Windows.Forms.TextBox();
            this.trvLogData = new System.Windows.Forms.TreeView();
            this.groupBox4.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgSocketList)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.groupBox7.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.tbPacketLogSearch);
            this.groupBox4.Controls.Add(this.lbSocketTelecomLog);
            this.groupBox4.Location = new System.Drawing.Point(594, 12);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(554, 693);
            this.groupBox4.TabIndex = 7;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "PacketLog";
            // 
            // tbPacketLogSearch
            // 
            this.tbPacketLogSearch.ForeColor = System.Drawing.Color.Gray;
            this.tbPacketLogSearch.Location = new System.Drawing.Point(6, 20);
            this.tbPacketLogSearch.Name = "tbPacketLogSearch";
            this.tbPacketLogSearch.Size = new System.Drawing.Size(542, 21);
            this.tbPacketLogSearch.TabIndex = 2;
            this.tbPacketLogSearch.Text = "Search Keyword";
            this.tbPacketLogSearch.Enter += new System.EventHandler(this.tbPacketLogSearch_Enter);
            this.tbPacketLogSearch.KeyUp += new System.Windows.Forms.KeyEventHandler(this.tbPacketLogSearch_KeyUp);
            this.tbPacketLogSearch.Leave += new System.EventHandler(this.tbPacketLogSearch_Leave);
            // 
            // lbSocketTelecomLog
            // 
            this.lbSocketTelecomLog.FormattingEnabled = true;
            this.lbSocketTelecomLog.ItemHeight = 12;
            this.lbSocketTelecomLog.Location = new System.Drawing.Point(8, 47);
            this.lbSocketTelecomLog.Name = "lbSocketTelecomLog";
            this.lbSocketTelecomLog.Size = new System.Drawing.Size(540, 640);
            this.lbSocketTelecomLog.TabIndex = 1;
            this.lbSocketTelecomLog.SelectedIndexChanged += new System.EventHandler(this.lbSocketTelecomLog_SelectedIndexChanged);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.btnSend);
            this.groupBox3.Controls.Add(this.groupBox1);
            this.groupBox3.Controls.Add(this.groupBox5);
            this.groupBox3.Controls.Add(this.groupBox2);
            this.groupBox3.Controls.Add(this.btnPacketBroadcast);
            this.groupBox3.Controls.Add(this.groupBox6);
            this.groupBox3.Location = new System.Drawing.Point(12, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(576, 693);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "SocketControl";
            // 
            // btnSend
            // 
            this.btnSend.Location = new System.Drawing.Point(397, 663);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(75, 25);
            this.btnSend.TabIndex = 8;
            this.btnSend.Text = "Send";
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.btnSend_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.tbPacketSearch);
            this.groupBox1.Controls.Add(this.lbPacketList);
            this.groupBox1.Location = new System.Drawing.Point(6, 187);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(226, 465);
            this.groupBox1.TabIndex = 7;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "PacketList";
            // 
            // tbPacketSearch
            // 
            this.tbPacketSearch.ForeColor = System.Drawing.Color.Gray;
            this.tbPacketSearch.Location = new System.Drawing.Point(7, 17);
            this.tbPacketSearch.Name = "tbPacketSearch";
            this.tbPacketSearch.Size = new System.Drawing.Size(211, 21);
            this.tbPacketSearch.TabIndex = 1;
            this.tbPacketSearch.Text = "Search Keyword";
            this.tbPacketSearch.Enter += new System.EventHandler(this.tbPacketSearch_Enter);
            this.tbPacketSearch.KeyUp += new System.Windows.Forms.KeyEventHandler(this.tbPacketSearch_KeyUp);
            this.tbPacketSearch.Leave += new System.EventHandler(this.tbPacketSearch_Leave);
            // 
            // lbPacketList
            // 
            this.lbPacketList.FormattingEnabled = true;
            this.lbPacketList.ItemHeight = 12;
            this.lbPacketList.Location = new System.Drawing.Point(6, 45);
            this.lbPacketList.Name = "lbPacketList";
            this.lbPacketList.Size = new System.Drawing.Size(212, 412);
            this.lbPacketList.TabIndex = 0;
            this.lbPacketList.SelectedIndexChanged += new System.EventHandler(this.lbPacketList_SelectedIndexChanged);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.dgSocketList);
            this.groupBox5.Location = new System.Drawing.Point(244, 20);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(326, 632);
            this.groupBox5.TabIndex = 6;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "SocketList";
            // 
            // dgSocketList
            // 
            this.dgSocketList.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgSocketList.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.HostID,
            this.ConnectedServer,
            this.ConnectStatus});
            this.dgSocketList.Location = new System.Drawing.Point(6, 20);
            this.dgSocketList.Name = "dgSocketList";
            this.dgSocketList.ReadOnly = true;
            this.dgSocketList.RowTemplate.Height = 23;
            this.dgSocketList.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgSocketList.Size = new System.Drawing.Size(313, 604);
            this.dgSocketList.TabIndex = 1;
            this.dgSocketList.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dgSocketList_CellClick);
            // 
            // HostID
            // 
            this.HostID.HeaderText = "HostID";
            this.HostID.Name = "HostID";
            this.HostID.ReadOnly = true;
            this.HostID.Width = 50;
            // 
            // ConnectedServer
            // 
            this.ConnectedServer.HeaderText = "ConnectedServer";
            this.ConnectedServer.Name = "ConnectedServer";
            this.ConnectedServer.ReadOnly = true;
            this.ConnectedServer.Width = 110;
            // 
            // ConnectStatus
            // 
            this.ConnectStatus.HeaderText = "ConnectStatus";
            this.ConnectStatus.Name = "ConnectStatus";
            this.ConnectStatus.ReadOnly = true;
            this.ConnectStatus.Width = 110;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.lbServerList);
            this.groupBox2.Location = new System.Drawing.Point(6, 20);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(226, 106);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Server";
            // 
            // lbServerList
            // 
            this.lbServerList.FormattingEnabled = true;
            this.lbServerList.ItemHeight = 12;
            this.lbServerList.Location = new System.Drawing.Point(6, 20);
            this.lbServerList.Name = "lbServerList";
            this.lbServerList.Size = new System.Drawing.Size(213, 76);
            this.lbServerList.TabIndex = 4;
            // 
            // btnPacketBroadcast
            // 
            this.btnPacketBroadcast.Location = new System.Drawing.Point(478, 663);
            this.btnPacketBroadcast.Name = "btnPacketBroadcast";
            this.btnPacketBroadcast.Size = new System.Drawing.Size(85, 25);
            this.btnPacketBroadcast.TabIndex = 2;
            this.btnPacketBroadcast.Text = "Broadcast";
            this.btnPacketBroadcast.UseVisualStyleBackColor = true;
            this.btnPacketBroadcast.Click += new System.EventHandler(this.btnPacketBroadcast_Click);
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.btnGenerateSocketList);
            this.groupBox6.Controls.Add(this.txtSessionCount);
            this.groupBox6.Location = new System.Drawing.Point(6, 132);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(226, 49);
            this.groupBox6.TabIndex = 6;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "SocketGenerate";
            // 
            // btnGenerateSocketList
            // 
            this.btnGenerateSocketList.Location = new System.Drawing.Point(91, 18);
            this.btnGenerateSocketList.Name = "btnGenerateSocketList";
            this.btnGenerateSocketList.Size = new System.Drawing.Size(127, 24);
            this.btnGenerateSocketList.TabIndex = 6;
            this.btnGenerateSocketList.Text = "Connect";
            this.btnGenerateSocketList.UseVisualStyleBackColor = true;
            this.btnGenerateSocketList.Click += new System.EventHandler(this.btnGenerateSocketList_Click);
            // 
            // txtSessionCount
            // 
            this.txtSessionCount.ForeColor = System.Drawing.Color.Gray;
            this.txtSessionCount.Location = new System.Drawing.Point(7, 18);
            this.txtSessionCount.Name = "txtSessionCount";
            this.txtSessionCount.Size = new System.Drawing.Size(78, 21);
            this.txtSessionCount.TabIndex = 5;
            this.txtSessionCount.Text = "Count";
            this.txtSessionCount.TextChanged += new System.EventHandler(this.txtSessionCount_TextChanged);
            this.txtSessionCount.Enter += new System.EventHandler(this.txtSessionCount_Enter);
            this.txtSessionCount.Leave += new System.EventHandler(this.txtSessionCount_Leave);
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.tbRecvDataSearch);
            this.groupBox7.Controls.Add(this.trvLogData);
            this.groupBox7.Location = new System.Drawing.Point(1155, 12);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Size = new System.Drawing.Size(368, 693);
            this.groupBox7.TabIndex = 8;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "ReceiveData";
            // 
            // tbRecvDataSearch
            // 
            this.tbRecvDataSearch.ForeColor = System.Drawing.Color.Gray;
            this.tbRecvDataSearch.Location = new System.Drawing.Point(6, 20);
            this.tbRecvDataSearch.Name = "tbRecvDataSearch";
            this.tbRecvDataSearch.Size = new System.Drawing.Size(356, 21);
            this.tbRecvDataSearch.TabIndex = 1;
            this.tbRecvDataSearch.Text = "Search Keyword";
            this.tbRecvDataSearch.Enter += new System.EventHandler(this.tbRecvDataSearch_Enter);
            this.tbRecvDataSearch.KeyUp += new System.Windows.Forms.KeyEventHandler(this.tbRecvDataSearch_KeyUp);
            this.tbRecvDataSearch.Leave += new System.EventHandler(this.tbRecvDataSearch_Leave);
            // 
            // trvLogData
            // 
            this.trvLogData.Location = new System.Drawing.Point(6, 47);
            this.trvLogData.Name = "trvLogData";
            this.trvLogData.Size = new System.Drawing.Size(356, 640);
            this.trvLogData.TabIndex = 0;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1538, 713);
            this.Controls.Add(this.groupBox7);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Name = "MainForm";
            this.Text = "BotClient";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgSocketList)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.groupBox7.ResumeLayout(false);
            this.groupBox7.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.ListBox lbPacketList;
        private System.Windows.Forms.Button btnPacketBroadcast;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.ListBox lbServerList;
        private System.Windows.Forms.TextBox tbPacketSearch;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.TextBox txtSessionCount;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.ListBox lbSocketTelecomLog;
        private System.Windows.Forms.Button btnGenerateSocketList;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.DataGridView dgSocketList;
        private System.Windows.Forms.DataGridViewTextBoxColumn HostID;
        private System.Windows.Forms.DataGridViewTextBoxColumn ConnectedServer;
        private System.Windows.Forms.DataGridViewTextBoxColumn ConnectStatus;
        private System.Windows.Forms.Button btnSend;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.TreeView trvLogData;
        private System.Windows.Forms.TextBox tbPacketLogSearch;
        private System.Windows.Forms.TextBox tbRecvDataSearch;
    }
}


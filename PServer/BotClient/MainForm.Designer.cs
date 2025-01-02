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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.grbPacketParameters = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btnPacketSearch = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.lbPacketList = new System.Windows.Forms.ListBox();
            this.btnPacketSend = new System.Windows.Forms.Button();
            this.btnServerConnect = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btnDisconnect = new System.Windows.Forms.Button();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.btnLogSearch = new System.Windows.Forms.Button();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.btnLogClear = new System.Windows.Forms.Button();
            this.lbLogs = new System.Windows.Forms.ListBox();
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.grbPacketParameters);
            this.groupBox1.Controls.Add(this.groupBox3);
            this.groupBox1.Controls.Add(this.btnPacketSend);
            this.groupBox1.Location = new System.Drawing.Point(234, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(809, 263);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Protocol";
            // 
            // grbPacketParameters
            // 
            this.grbPacketParameters.Location = new System.Drawing.Point(295, 20);
            this.grbPacketParameters.Name = "grbPacketParameters";
            this.grbPacketParameters.Size = new System.Drawing.Size(508, 208);
            this.grbPacketParameters.TabIndex = 4;
            this.grbPacketParameters.TabStop = false;
            this.grbPacketParameters.Text = "Packet Parameters";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.btnPacketSearch);
            this.groupBox3.Controls.Add(this.textBox1);
            this.groupBox3.Controls.Add(this.lbPacketList);
            this.groupBox3.Location = new System.Drawing.Point(16, 20);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(273, 208);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "PacketList";
            // 
            // btnPacketSearch
            // 
            this.btnPacketSearch.Location = new System.Drawing.Point(211, 17);
            this.btnPacketSearch.Name = "btnPacketSearch";
            this.btnPacketSearch.Size = new System.Drawing.Size(56, 23);
            this.btnPacketSearch.TabIndex = 2;
            this.btnPacketSearch.Text = "Search";
            this.btnPacketSearch.UseVisualStyleBackColor = true;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(6, 17);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(198, 21);
            this.textBox1.TabIndex = 1;
            // 
            // lbPacketList
            // 
            this.lbPacketList.FormattingEnabled = true;
            this.lbPacketList.ItemHeight = 12;
            this.lbPacketList.Location = new System.Drawing.Point(6, 44);
            this.lbPacketList.Name = "lbPacketList";
            this.lbPacketList.Size = new System.Drawing.Size(261, 160);
            this.lbPacketList.TabIndex = 0;
            this.lbPacketList.SelectedIndexChanged += new System.EventHandler(this.lbPacketList_SelectedIndexChanged);
            // 
            // btnPacketSend
            // 
            this.btnPacketSend.Location = new System.Drawing.Point(718, 231);
            this.btnPacketSend.Name = "btnPacketSend";
            this.btnPacketSend.Size = new System.Drawing.Size(85, 26);
            this.btnPacketSend.TabIndex = 2;
            this.btnPacketSend.Text = "Send";
            this.btnPacketSend.UseVisualStyleBackColor = true;
            this.btnPacketSend.Click += new System.EventHandler(this.btnPacketSend_Click);
            // 
            // btnServerConnect
            // 
            this.btnServerConnect.Location = new System.Drawing.Point(134, 234);
            this.btnServerConnect.Name = "btnServerConnect";
            this.btnServerConnect.Size = new System.Drawing.Size(76, 24);
            this.btnServerConnect.TabIndex = 3;
            this.btnServerConnect.Text = "Connect";
            this.btnServerConnect.UseVisualStyleBackColor = true;
            this.btnServerConnect.Click += new System.EventHandler(this.btnServerConnect_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnDisconnect);
            this.groupBox2.Controls.Add(this.listBox1);
            this.groupBox2.Controls.Add(this.btnServerConnect);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(216, 263);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "ServerList";
            // 
            // btnDisconnect
            // 
            this.btnDisconnect.Location = new System.Drawing.Point(44, 233);
            this.btnDisconnect.Name = "btnDisconnect";
            this.btnDisconnect.Size = new System.Drawing.Size(84, 24);
            this.btnDisconnect.TabIndex = 5;
            this.btnDisconnect.Text = "Disconnect";
            this.btnDisconnect.UseVisualStyleBackColor = true;
            this.btnDisconnect.Click += new System.EventHandler(this.btnDisconnect_Click);
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.ItemHeight = 12;
            this.listBox1.Location = new System.Drawing.Point(6, 20);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(201, 208);
            this.listBox1.TabIndex = 4;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.btnLogSearch);
            this.groupBox4.Controls.Add(this.textBox2);
            this.groupBox4.Controls.Add(this.btnLogClear);
            this.groupBox4.Controls.Add(this.lbLogs);
            this.groupBox4.Location = new System.Drawing.Point(12, 281);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(1031, 406);
            this.groupBox4.TabIndex = 5;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Log";
            // 
            // btnLogSearch
            // 
            this.btnLogSearch.Location = new System.Drawing.Point(384, 17);
            this.btnLogSearch.Name = "btnLogSearch";
            this.btnLogSearch.Size = new System.Drawing.Size(75, 23);
            this.btnLogSearch.TabIndex = 3;
            this.btnLogSearch.Text = "Search";
            this.btnLogSearch.UseVisualStyleBackColor = true;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(7, 17);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(371, 21);
            this.textBox2.TabIndex = 2;
            // 
            // btnLogClear
            // 
            this.btnLogClear.Location = new System.Drawing.Point(465, 17);
            this.btnLogClear.Name = "btnLogClear";
            this.btnLogClear.Size = new System.Drawing.Size(84, 23);
            this.btnLogClear.TabIndex = 1;
            this.btnLogClear.Text = "Clear";
            this.btnLogClear.UseVisualStyleBackColor = true;
            this.btnLogClear.Click += new System.EventHandler(this.btnLogClear_Click);
            // 
            // lbLogs
            // 
            this.lbLogs.FormattingEnabled = true;
            this.lbLogs.ItemHeight = 12;
            this.lbLogs.Location = new System.Drawing.Point(5, 44);
            this.lbLogs.Name = "lbLogs";
            this.lbLogs.Size = new System.Drawing.Size(1019, 352);
            this.lbLogs.TabIndex = 0;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1064, 699);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "MainForm";
            this.Text = "BotClient";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ListBox lbPacketList;
        private System.Windows.Forms.Button btnPacketSend;
        private System.Windows.Forms.Button btnServerConnect;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox grbPacketParameters;
        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.ListBox lbLogs;
        private System.Windows.Forms.Button btnLogClear;
        private System.Windows.Forms.Button btnPacketSearch;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Button btnLogSearch;
        private System.Windows.Forms.Button btnDisconnect;
    }
}


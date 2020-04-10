namespace SampleCS
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.button_Stop = new System.Windows.Forms.Button();
            this.button_Play = new System.Windows.Forms.Button();
            this.button_Logout = new System.Windows.Forms.Button();
            this.button_Login = new System.Windows.Forms.Button();
            this.textBox_ServerIP = new System.Windows.Forms.TextBox();
            this.label_ServerIP = new System.Windows.Forms.Label();
            this.listView_CameraID = new System.Windows.Forms.ListView();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.textBox_msg = new System.Windows.Forms.TextBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.checkBox2 = new System.Windows.Forms.CheckBox();
            this.checkBox3 = new System.Windows.Forms.CheckBox();
            this.Setting = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.textbox_RecvTimeout = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textbox_ReportInterval = new System.Windows.Forms.TextBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.checkBox4 = new System.Windows.Forms.CheckBox();
            this.checkBox5 = new System.Windows.Forms.CheckBox();
            this.checkBox6 = new System.Windows.Forms.CheckBox();
            this.ComboPlay = new System.Windows.Forms.Button();
            this.CheckBox_ExternDraw = new System.Windows.Forms.CheckBox();
            this.CheckBox_Transmit = new System.Windows.Forms.CheckBox();
            this.button_UpdateOpAssist = new System.Windows.Forms.Button();
            this.comboBox_mode = new System.Windows.Forms.ComboBox();
            this.SwitchScreen = new System.Windows.Forms.Button();
            this.pictureBox5 = new System.Windows.Forms.PictureBox();
            this.QueryRecord = new System.Windows.Forms.Button();
            this.PlayRecord = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.axAVPlayer1 = new AxAVPlayerLib.AxAVPlayer();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox5)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.axAVPlayer1)).BeginInit();
            this.SuspendLayout();
            // 
            // button_Stop
            // 
            this.button_Stop.Enabled = false;
            this.button_Stop.Location = new System.Drawing.Point(383, 3);
            this.button_Stop.Name = "button_Stop";
            this.button_Stop.Size = new System.Drawing.Size(64, 23);
            this.button_Stop.TabIndex = 17;
            this.button_Stop.Text = "Stop";
            this.button_Stop.UseVisualStyleBackColor = true;
            this.button_Stop.Click += new System.EventHandler(this.button_Stop_Click);
            // 
            // button_Play
            // 
            this.button_Play.Enabled = false;
            this.button_Play.Location = new System.Drawing.Point(314, 3);
            this.button_Play.Name = "button_Play";
            this.button_Play.Size = new System.Drawing.Size(64, 23);
            this.button_Play.TabIndex = 18;
            this.button_Play.Text = "Play";
            this.button_Play.UseVisualStyleBackColor = true;
            this.button_Play.Click += new System.EventHandler(this.button_Play_Click);
            // 
            // button_Logout
            // 
            this.button_Logout.Enabled = false;
            this.button_Logout.Location = new System.Drawing.Point(245, 3);
            this.button_Logout.Name = "button_Logout";
            this.button_Logout.Size = new System.Drawing.Size(64, 23);
            this.button_Logout.TabIndex = 19;
            this.button_Logout.Text = "Logout";
            this.button_Logout.UseVisualStyleBackColor = true;
            this.button_Logout.Click += new System.EventHandler(this.button_Logout_Click);
            // 
            // button_Login
            // 
            this.button_Login.Location = new System.Drawing.Point(176, 3);
            this.button_Login.Name = "button_Login";
            this.button_Login.Size = new System.Drawing.Size(64, 23);
            this.button_Login.TabIndex = 20;
            this.button_Login.Text = "Login";
            this.button_Login.UseVisualStyleBackColor = true;
            this.button_Login.Click += new System.EventHandler(this.button_Login_Click);
            // 
            // textBox_ServerIP
            // 
            this.textBox_ServerIP.Location = new System.Drawing.Point(64, 4);
            this.textBox_ServerIP.Name = "textBox_ServerIP";
            this.textBox_ServerIP.Size = new System.Drawing.Size(108, 21);
            this.textBox_ServerIP.TabIndex = 16;
            // 
            // label_ServerIP
            // 
            this.label_ServerIP.AutoSize = true;
            this.label_ServerIP.Location = new System.Drawing.Point(5, 7);
            this.label_ServerIP.Name = "label_ServerIP";
            this.label_ServerIP.Size = new System.Drawing.Size(53, 12);
            this.label_ServerIP.TabIndex = 15;
            this.label_ServerIP.Text = "ServerIP";
            // 
            // listView_CameraID
            // 
            this.listView_CameraID.CheckBoxes = true;
            this.listView_CameraID.Location = new System.Drawing.Point(900, 55);
            this.listView_CameraID.Name = "listView_CameraID";
            this.listView_CameraID.Size = new System.Drawing.Size(246, 567);
            this.listView_CameraID.TabIndex = 14;
            this.listView_CameraID.UseCompatibleStateImageBehavior = false;
            this.listView_CameraID.View = System.Windows.Forms.View.Details;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox1.Location = new System.Drawing.Point(1, 60);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(449, 282);
            this.pictureBox1.TabIndex = 13;
            this.pictureBox1.TabStop = false;
            // 
            // textBox_msg
            // 
            this.textBox_msg.Location = new System.Drawing.Point(3, 628);
            this.textBox_msg.Multiline = true;
            this.textBox_msg.Name = "textBox_msg";
            this.textBox_msg.Size = new System.Drawing.Size(1142, 172);
            this.textBox_msg.TabIndex = 22;
            // 
            // pictureBox2
            // 
            this.pictureBox2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox2.Location = new System.Drawing.Point(450, 60);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(449, 282);
            this.pictureBox2.TabIndex = 13;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox3.Location = new System.Drawing.Point(1, 342);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(449, 282);
            this.pictureBox3.TabIndex = 13;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox4
            // 
            this.pictureBox4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox4.Location = new System.Drawing.Point(450, 342);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(449, 282);
            this.pictureBox4.TabIndex = 13;
            this.pictureBox4.TabStop = false;
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Enabled = false;
            this.checkBox1.Location = new System.Drawing.Point(538, 6);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(96, 16);
            this.checkBox1.TabIndex = 23;
            this.checkBox1.Text = "Extend Play1";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // checkBox2
            // 
            this.checkBox2.AutoSize = true;
            this.checkBox2.Enabled = false;
            this.checkBox2.Location = new System.Drawing.Point(641, 6);
            this.checkBox2.Name = "checkBox2";
            this.checkBox2.Size = new System.Drawing.Size(96, 16);
            this.checkBox2.TabIndex = 23;
            this.checkBox2.Text = "Extend Play2";
            this.checkBox2.UseVisualStyleBackColor = true;
            this.checkBox2.CheckedChanged += new System.EventHandler(this.checkBox2_CheckedChanged);
            // 
            // checkBox3
            // 
            this.checkBox3.AutoSize = true;
            this.checkBox3.Enabled = false;
            this.checkBox3.Location = new System.Drawing.Point(740, 6);
            this.checkBox3.Name = "checkBox3";
            this.checkBox3.Size = new System.Drawing.Size(96, 16);
            this.checkBox3.TabIndex = 23;
            this.checkBox3.Text = "Extend Play3";
            this.checkBox3.UseVisualStyleBackColor = true;
            this.checkBox3.CheckedChanged += new System.EventHandler(this.checkBox3_CheckedChanged);
            // 
            // Setting
            // 
            this.Setting.Location = new System.Drawing.Point(1046, 5);
            this.Setting.Name = "Setting";
            this.Setting.Size = new System.Drawing.Size(75, 23);
            this.Setting.TabIndex = 24;
            this.Setting.Text = "Setting";
            this.Setting.UseVisualStyleBackColor = true;
            this.Setting.Click += new System.EventHandler(this.Setting_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(841, 5);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(107, 12);
            this.label1.TabIndex = 25;
            this.label1.Text = "Recv Timeout(ms):";
            // 
            // textbox_RecvTimeout
            // 
            this.textbox_RecvTimeout.Location = new System.Drawing.Point(970, 0);
            this.textbox_RecvTimeout.Name = "textbox_RecvTimeout";
            this.textbox_RecvTimeout.Size = new System.Drawing.Size(70, 21);
            this.textbox_RecvTimeout.TabIndex = 26;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(841, 25);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(125, 12);
            this.label2.TabIndex = 25;
            this.label2.Text = "Report Interval(ms):";
            // 
            // textbox_ReportInterval
            // 
            this.textbox_ReportInterval.Location = new System.Drawing.Point(970, 22);
            this.textbox_ReportInterval.Name = "textbox_ReportInterval";
            this.textbox_ReportInterval.Size = new System.Drawing.Size(70, 21);
            this.textbox_ReportInterval.TabIndex = 26;
            // 
            // checkBox4
            // 
            this.checkBox4.AutoSize = true;
            this.checkBox4.Location = new System.Drawing.Point(538, 21);
            this.checkBox4.Name = "checkBox4";
            this.checkBox4.Size = new System.Drawing.Size(84, 16);
            this.checkBox4.TabIndex = 23;
            this.checkBox4.Text = "Hide Play1";
            this.checkBox4.UseVisualStyleBackColor = true;
            this.checkBox4.CheckedChanged += new System.EventHandler(this.checkBox4_CheckedChanged);
            // 
            // checkBox5
            // 
            this.checkBox5.AutoSize = true;
            this.checkBox5.Location = new System.Drawing.Point(641, 21);
            this.checkBox5.Name = "checkBox5";
            this.checkBox5.Size = new System.Drawing.Size(84, 16);
            this.checkBox5.TabIndex = 23;
            this.checkBox5.Text = "Hide Play2";
            this.checkBox5.UseVisualStyleBackColor = true;
            this.checkBox5.CheckedChanged += new System.EventHandler(this.checkBox5_CheckedChanged);
            // 
            // checkBox6
            // 
            this.checkBox6.AutoSize = true;
            this.checkBox6.Location = new System.Drawing.Point(740, 21);
            this.checkBox6.Name = "checkBox6";
            this.checkBox6.Size = new System.Drawing.Size(84, 16);
            this.checkBox6.TabIndex = 23;
            this.checkBox6.Text = "Hide Play3";
            this.checkBox6.UseVisualStyleBackColor = true;
            this.checkBox6.CheckedChanged += new System.EventHandler(this.checkBox6_CheckedChanged);
            // 
            // ComboPlay
            // 
            this.ComboPlay.Enabled = false;
            this.ComboPlay.Location = new System.Drawing.Point(452, 3);
            this.ComboPlay.Name = "ComboPlay";
            this.ComboPlay.Size = new System.Drawing.Size(80, 23);
            this.ComboPlay.TabIndex = 17;
            this.ComboPlay.Text = "ComboPlay";
            this.ComboPlay.UseVisualStyleBackColor = true;
            this.ComboPlay.Click += new System.EventHandler(this.button_ComboPlay_Click);
            // 
            // CheckBox_ExternDraw
            // 
            this.CheckBox_ExternDraw.AutoSize = true;
            this.CheckBox_ExternDraw.Location = new System.Drawing.Point(538, 40);
            this.CheckBox_ExternDraw.Name = "CheckBox_ExternDraw";
            this.CheckBox_ExternDraw.Size = new System.Drawing.Size(120, 16);
            this.CheckBox_ExternDraw.TabIndex = 23;
            this.CheckBox_ExternDraw.Text = "Operation Assist";
            this.CheckBox_ExternDraw.UseVisualStyleBackColor = true;
            this.CheckBox_ExternDraw.CheckedChanged += new System.EventHandler(this.CheckBox_ExternDraw_CheckedChanged);
            // 
            // CheckBox_Transmit
            // 
            this.CheckBox_Transmit.AutoSize = true;
            this.CheckBox_Transmit.Location = new System.Drawing.Point(641, 40);
            this.CheckBox_Transmit.Name = "CheckBox_Transmit";
            this.CheckBox_Transmit.Size = new System.Drawing.Size(72, 16);
            this.CheckBox_Transmit.TabIndex = 23;
            this.CheckBox_Transmit.Text = "Transmit";
            this.CheckBox_Transmit.UseVisualStyleBackColor = true;
            this.CheckBox_Transmit.CheckedChanged += new System.EventHandler(this.checkBox4_CheckedChanged);
            // 
            // button_UpdateOpAssist
            // 
            this.button_UpdateOpAssist.Location = new System.Drawing.Point(5, 33);
            this.button_UpdateOpAssist.Name = "button_UpdateOpAssist";
            this.button_UpdateOpAssist.Size = new System.Drawing.Size(155, 23);
            this.button_UpdateOpAssist.TabIndex = 28;
            this.button_UpdateOpAssist.Text = "UpdateAssistConfigure";
            this.button_UpdateOpAssist.UseVisualStyleBackColor = true;
            this.button_UpdateOpAssist.Click += new System.EventHandler(this.button_UpdateOpAssist_Click);
            // 
            // comboBox_mode
            // 
            this.comboBox_mode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_mode.FormattingEnabled = true;
            this.comboBox_mode.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6"});
            this.comboBox_mode.Location = new System.Drawing.Point(274, 35);
            this.comboBox_mode.Name = "comboBox_mode";
            this.comboBox_mode.Size = new System.Drawing.Size(70, 20);
            this.comboBox_mode.TabIndex = 30;
            // 
            // SwitchScreen
            // 
            this.SwitchScreen.Location = new System.Drawing.Point(169, 33);
            this.SwitchScreen.Name = "SwitchScreen";
            this.SwitchScreen.Size = new System.Drawing.Size(99, 23);
            this.SwitchScreen.TabIndex = 32;
            this.SwitchScreen.Text = "SwitchScreen";
            this.SwitchScreen.UseVisualStyleBackColor = true;
            this.SwitchScreen.Click += new System.EventHandler(this.SwitchScreen_Click);
            // 
            // pictureBox5
            // 
            this.pictureBox5.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox5.Location = new System.Drawing.Point(1080, 40);
            this.pictureBox5.Name = "pictureBox5";
            this.pictureBox5.Size = new System.Drawing.Size(41, 34);
            this.pictureBox5.TabIndex = 13;
            this.pictureBox5.TabStop = false;
            // 
            // QueryRecord
            // 
            this.QueryRecord.Location = new System.Drawing.Point(350, 32);
            this.QueryRecord.Name = "QueryRecord";
            this.QueryRecord.Size = new System.Drawing.Size(89, 23);
            this.QueryRecord.TabIndex = 34;
            this.QueryRecord.Text = "Query Record";
            this.QueryRecord.UseVisualStyleBackColor = true;
            this.QueryRecord.Click += new System.EventHandler(this.QueryRecord_Click);
            // 
            // PlayRecord
            // 
            this.PlayRecord.Location = new System.Drawing.Point(445, 32);
            this.PlayRecord.Name = "PlayRecord";
            this.PlayRecord.Size = new System.Drawing.Size(89, 23);
            this.PlayRecord.TabIndex = 34;
            this.PlayRecord.Text = "Play Record";
            this.PlayRecord.UseVisualStyleBackColor = true;
            this.PlayRecord.Click += new System.EventHandler(this.PlayRecord_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // axAVPlayer1
            // 
            this.axAVPlayer1.Enabled = true;
            this.axAVPlayer1.Location = new System.Drawing.Point(999, 35);
            this.axAVPlayer1.Name = "axAVPlayer1";
            this.axAVPlayer1.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axAVPlayer1.OcxState")));
            this.axAVPlayer1.Size = new System.Drawing.Size(48, 48);
            this.axAVPlayer1.TabIndex = 35;
            this.axAVPlayer1.Visible = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1147, 799);
            this.Controls.Add(this.axAVPlayer1);
            this.Controls.Add(this.PlayRecord);
            this.Controls.Add(this.QueryRecord);
            this.Controls.Add(this.SwitchScreen);
            this.Controls.Add(this.comboBox_mode);
            this.Controls.Add(this.button_UpdateOpAssist);
            this.Controls.Add(this.textbox_ReportInterval);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textbox_RecvTimeout);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.Setting);
            this.Controls.Add(this.checkBox6);
            this.Controls.Add(this.checkBox5);
            this.Controls.Add(this.checkBox3);
            this.Controls.Add(this.CheckBox_Transmit);
            this.Controls.Add(this.CheckBox_ExternDraw);
            this.Controls.Add(this.checkBox4);
            this.Controls.Add(this.checkBox2);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.textBox_msg);
            this.Controls.Add(this.ComboPlay);
            this.Controls.Add(this.button_Stop);
            this.Controls.Add(this.button_Play);
            this.Controls.Add(this.button_Logout);
            this.Controls.Add(this.button_Login);
            this.Controls.Add(this.textBox_ServerIP);
            this.Controls.Add(this.label_ServerIP);
            this.Controls.Add(this.listView_CameraID);
            this.Controls.Add(this.pictureBox4);
            this.Controls.Add(this.pictureBox3);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox5);
            this.Controls.Add(this.pictureBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox5)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.axAVPlayer1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button_Stop;
        private System.Windows.Forms.Button button_Play;
        private System.Windows.Forms.Button button_Logout;
        private System.Windows.Forms.Button button_Login;
        private System.Windows.Forms.TextBox textBox_ServerIP;
        private System.Windows.Forms.Label label_ServerIP;
        private System.Windows.Forms.ListView listView_CameraID;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.TextBox textBox_msg;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBox4;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.CheckBox checkBox2;
        private System.Windows.Forms.CheckBox checkBox3;
        private System.Windows.Forms.Button Setting;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textbox_RecvTimeout;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textbox_ReportInterval;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.CheckBox checkBox4;
        private System.Windows.Forms.CheckBox checkBox5;
        private System.Windows.Forms.CheckBox checkBox6;
        private System.Windows.Forms.Button ComboPlay;
        private System.Windows.Forms.CheckBox CheckBox_ExternDraw;
        private System.Windows.Forms.CheckBox CheckBox_Transmit;
        private System.Windows.Forms.Button button_UpdateOpAssist;
        private System.Windows.Forms.ComboBox comboBox_mode;
        private System.Windows.Forms.Button SwitchScreen;
        bool m_bFitFrame = false;
       // private AxAVPlayerLib.AxAVPlayer axAVPlayer1;
        private System.Windows.Forms.PictureBox pictureBox5;
        private System.Windows.Forms.Button QueryRecord;
        private System.Windows.Forms.Button PlayRecord;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private AxAVPlayerLib.AxAVPlayer axAVPlayer1;
    }
}


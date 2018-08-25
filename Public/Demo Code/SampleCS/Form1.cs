using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace SampleCS
{
    // 使用 GetErrorString 函数可以取得对应错误代码具体详细描述,取得详细描述后,
    // 应该使用FreeString函数回收描述信息所占的内存
    enum AVStatus
    {
        AvError_Succeed                  = 0,				         // 操作成功
        AvError_base                     = -1024,
        AvError_InvalidParameters        = AvError_base - 1,		// 参数无效
        AvError_Invalid_ServerAddress    = AvError_base - 2,	    // 服务器地址不是一个有效的IP地址	
        AvError_Invliad_ServerPort       = AvError_base - 3,		// 服务器端口无效
        AvError_Invalid_AccountOrPassword= AvError_base - 4,        // 用户名或密码无效
        AvError_ConnectServerFailed      = AvError_base - 5,		// 连接服务器失败
        AvError_LoginFailed              = AvError_base - 6,		// 登录服务器失败
        AvError_NotLogintoServer         = AvError_base - 7,		// 尚未登录到服务器
        AvError_InvalidWindow            = AvError_base - 8,		// 用于播放的窗口句柄指示的不是一个有效的窗口
        AvError_DeviceNotExist           = AvError_base - 9,		// 请求的设备不存在
        AvError_ConnectDeviceFailed      = AvError_base - 10,	    // 连接设备失败
        AvError_DeviceInPlaying          = AvError_base - 11,		// 设备正在播放中，请匆复播放
        AvError_DBException              = AvError_base - 12,		// 数据库访问异常
        AvError_DeviceNotInPlaying       = AvError_base - 13,		// 设备不在播放中
        AVError_BufferOverflow           = AvError_base - 14,		// 缓存溢出,提供的内存空间不足以容纳所请求的数据
        AvError_WindowNotPlaying         = AvError_base - 15,		// 窗口尚未播放任何设备	
        AvError_ExternalError            = AvError_base - 253,		// 内部错误
        AvError_InsufficentMemory        = AvError_base - 254,		// 内存不足
        AvError_UnknownException         = AvError_base - 255		// 未知异常 
    };
    struct DevicePlay
    {
        public  string  strDevice;
        public  int     hWnd;
        public  double  nTime;
    }

    public partial class Form1 : Form
    {
        List<DevicePlay> m_PlayList = new List<DevicePlay>();
        // 精确计时类,需要引入System.Diagnostics
        Stopwatch  sw = new Stopwatch();

        public Form1()
        {
            InitializeComponent();
            sw.Start();
            listView_CameraID.Columns.Add("NO.");
            listView_CameraID.Columns.Add("DeviceID");
            listView_CameraID.Columns[0].Width = 60;
            listView_CameraID.Columns[1].Width = 150;
            listView_CameraID.FullRowSelect = true;
            listView_CameraID.MultiSelect = false;
            textBox_ServerIP.Text = "192.168.2.192";
            //textBox_ServerIP.Text = "172.27.18.101";
            //textBox_ServerIP.Text = "192.168.10.128";
            // 获取数据读取超时值，单位为毫秒
            textbox_RecvTimeout.Text = axAVPlayer1.RecvTimeout.ToString();
            // 获取报告超时间隔值，单位为毫秒，即每隔一段时间发送一次超报告
            textbox_ReportInterval.Text = axAVPlayer1.ReportInterval.ToString();
        }
              

        private void button_Login_Click(object sender, EventArgs e)
        {
            int nResult = axAVPlayer1.Login(textBox_ServerIP.Text, 9000, "admin", "123456", 0);
            if (nResult == 0)
            {
                listView_CameraID.Items.Clear();
                string strDeviceIDList = "";
                string strErrorMsg = "";
                int nDeviceCount = 0;
                // 从服务器取得所有设备的ID
                int nErrorCode = axAVPlayer1.GetDeviceID(ref strDeviceIDList, ref nDeviceCount);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                    textBox_msg.Text += strErrorMsg;
                    // 释放strErrormsg所占内存
                    axAVPlayer1.FreeString(ref strErrorMsg);
                    return;
                }
                
                string[] IDList = strDeviceIDList.Split(';');
                int nIndex = 0;
                listView_CameraID.BeginUpdate();
                foreach (string ID in IDList)
                {
                    ListViewItem lvi = new ListViewItem();
                    lvi.ImageIndex = nIndex++;
                    lvi.Text = nIndex.ToString();
                    lvi.SubItems.Add(ID);
                    listView_CameraID.Items.Add(lvi);
                }
                // strDeviceIDList是控件内部申请的内存，需要使用FreeString手动释放
                axAVPlayer1.FreeString(ref strDeviceIDList);
            
                listView_CameraID.EndUpdate();
                button_Login.Enabled = false;
                button_Logout.Enabled = true;
                button_Play.Enabled = true;
                ComboPlay.Enabled = true;
                button_Stop.Enabled = false;
                checkBox1.Enabled = true;
                checkBox2.Enabled = true;
                checkBox3.Enabled = true;

            }
        }

        private void button_Logout_Click(object sender, EventArgs e)
        {
            axAVPlayer1.Logout();
            button_Login.Enabled = true;
            button_Logout.Enabled = false;
            button_Play.Enabled = false;
            button_Stop.Enabled = false;
            // 扩展显示视频选项更新为未选中
            checkBox1.Checked = false;
            checkBox2.Checked = false;
            checkBox3.Checked = false;
            checkBox1.Enabled = false;
            checkBox2.Enabled = false;
            checkBox3.Enabled = false;
        }

        private void button_Play_Click(object sender, EventArgs e)
        {
            if (listView_CameraID.CheckedItems.Count == 0)
            {
                MessageBox.Show("Plz select a device to play.");
                return;
            }
            string strDeviceID = listView_CameraID.CheckedItems[0].SubItems[1].Text;
            int nEnalbeHWAccel = 0;     // 禁用硬解码
            int nErrorCode = 0;
            string strErrorMsg = "";
            
            nErrorCode = axAVPlayer1.PlayStream(strDeviceID, (int)pictureBox1.Handle, nEnalbeHWAccel);
            if (nErrorCode != (int)AVStatus.AvError_Succeed)
            {
                axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                textBox_msg.Text += strErrorMsg;
                textBox_msg.Text += "\r\n";
                // 释放strErrormsg所占内存
                axAVPlayer1.FreeString(ref strErrorMsg);
                return;
            } 
            button_Play.Enabled = false;
            button_Stop.Enabled = true;
            // 获取设备正在播放的窗口句柄
            int[] hWndArray = new int[16];
            int nArraySize = 16;
            nErrorCode = axAVPlayer1.GetDeviceWindow(strDeviceID,ref hWndArray[0],ref nArraySize);
            if (nErrorCode != (int)AVStatus.AvError_Succeed)
            {
                axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                textBox_msg.Text += strErrorMsg;
                textBox_msg.Text += "\r\n";
                // 释放strErrormsg所占内存
                axAVPlayer1.FreeString(ref strErrorMsg);
                return;
            }
            
        }

        private void button_ComboPlay_Click(object sender, EventArgs e)
        {
            if (listView_CameraID.CheckedItems.Count <2)
            {
                MessageBox.Show("Plz select two devices to play.");
                return;
            }
            string strDeviceID1 = listView_CameraID.CheckedItems[0].SubItems[1].Text;
            string strDeviceID2 = listView_CameraID.CheckedItems[1].SubItems[1].Text;

            
           
            int nEnalbeHWAccel = 0;     // 禁用硬解码
            int nErrorCode = 0;
            string strErrorMsg = "";

            nErrorCode = axAVPlayer1.PlayComboStream(strDeviceID1, strDeviceID2, (int)pictureBox1.Handle, nEnalbeHWAccel,1);
            if (nErrorCode != (int)AVStatus.AvError_Succeed)
            {
                axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                textBox_msg.Text += strErrorMsg;
                textBox_msg.Text += "\r\n";
                // 释放strErrormsg所占内存
                axAVPlayer1.FreeString(ref strErrorMsg);
                return;
            } 
            button_Play.Enabled = false;
            button_Stop.Enabled = true;
            // 获取设备正在播放的窗口句柄
            int[] hWndArray = new int[16];
            int nArraySize = 16;
            nErrorCode = axAVPlayer1.GetDeviceWindow(strDeviceID1,ref hWndArray[0],ref nArraySize);
            if (nErrorCode != (int)AVStatus.AvError_Succeed)
            {
                axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                textBox_msg.Text += strErrorMsg;
                textBox_msg.Text += "\r\n";
                // 释放strErrormsg所占内存
                axAVPlayer1.FreeString(ref strErrorMsg);
                return;
            }
            
        }

        
        private void button_Stop_Click(object sender, EventArgs e)
        {
            int nCount = listView_CameraID.CheckedItems.Count;
            for (int i = 0; i < nCount; i++)
            {
                string strDeviceID = listView_CameraID.CheckedItems[i].SubItems[1].Text;
                // 终止所有窗口的播放
                // StopPlay的第二个参数为窗口句柄，关闭在指定窗口显示的视频
                // 若该参数为0，则关闭所有窗口的句柄
                axAVPlayer1.StopPlay(strDeviceID,0);
            }

            // 扩展显示视频选项更新为未选中
            checkBox1.Checked = false;
            checkBox2.Checked = false;
            checkBox3.Checked = false;

            button_Play.Enabled = true;
            button_Stop.Enabled = false;
        }
               

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked == true)
            {
                if (listView_CameraID.SelectedItems.Count == 0)
                {
                    MessageBox.Show("Plz select a device to play.");
                    return;
                }
                string strDeviceID = listView_CameraID.SelectedItems[0].SubItems[1].Text;
                int nEnalbeHWAccel = 0;     // 禁用硬解码
                int nErrorCode = 0;
                string strErrorMsg = "";
                nErrorCode = axAVPlayer1.PlayStream(strDeviceID, (int)pictureBox2.Handle, nEnalbeHWAccel);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                    textBox_msg.Text += strErrorMsg;
                    textBox_msg.Text += "\r\n";
                    // 释放strErrormsg所占内存
                    axAVPlayer1.FreeString(ref strErrorMsg);
                    return;
                }

                // 获取设备正在播放的窗口句柄
                int[] hWndArray = new int[16];
                int nArraySize = 16;
                nErrorCode = axAVPlayer1.GetDeviceWindow(strDeviceID, ref hWndArray[0], ref nArraySize);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                    textBox_msg.Text += strErrorMsg;
                    textBox_msg.Text += "\r\n";
                    // 释放strErrormsg所占内存
                    axAVPlayer1.FreeString(ref strErrorMsg);
                    return;
                }
                textBox_msg.Text += (strDeviceID + "已在 " + nArraySize.ToString() + " 个窗口上播放.\r\n");
               
            }
            else 
            {
                string strDeviceID = "";
                string strErrorMsg = "";
                //  直接从播放图像的窗口句柄获取设备ID
                int nErrorCode = axAVPlayer1.GetWindowDevice((int)pictureBox2.Handle, ref strDeviceID);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                    textBox_msg.Text += strErrorMsg;
                    textBox_msg.Text += "\r\n";
                    // 释放strErrormsg所占内存
                    axAVPlayer1.FreeString(ref strErrorMsg);
                    return;
                }
                axAVPlayer1.StopPlay(strDeviceID, (int)pictureBox2.Handle);
                axAVPlayer1.FreeString(ref strDeviceID);
            }
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox2.Checked == true)
            {
                if (listView_CameraID.SelectedItems.Count == 0)
                {
                    MessageBox.Show("Plz select a device to play.");
                    return;
                }
                string strDeviceID = listView_CameraID.SelectedItems[0].SubItems[1].Text;

                int nEnalbeHWAccel = 0;     // 禁用硬解码
                int nErrorCode = 0;
                string strErrorMsg = "";
                nErrorCode = axAVPlayer1.PlayStream(strDeviceID, (int)pictureBox3.Handle, nEnalbeHWAccel);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                    textBox_msg.Text += strErrorMsg;
                    textBox_msg.Text += "\r\n";
                    // 释放strErrormsg所占内存
                    axAVPlayer1.FreeString(ref strErrorMsg);
                    return;
                }
                // 获取设备正在播放的窗口句柄
                int[] hWndArray = new int[16];
                int nArraySize = 16;
                nErrorCode = axAVPlayer1.GetDeviceWindow(strDeviceID, ref hWndArray[0], ref nArraySize);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                    textBox_msg.Text += strErrorMsg;
                    textBox_msg.Text += "\r\n";
                    // 释放strErrormsg所占内存
                    axAVPlayer1.FreeString(ref strErrorMsg);
                    return;
                }
                textBox_msg.Text += (strDeviceID + "已在 " + nArraySize.ToString() + " 个窗口上播放.\r\n");
            }
            else
            {
                string strDeviceID = "";
                string strErrorMsg = "";
                //  直接从播放图像的窗口句柄获取设备ID
                int nErrorCode = axAVPlayer1.GetWindowDevice((int)pictureBox3.Handle, ref strDeviceID);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                    textBox_msg.Text += strErrorMsg;
                    textBox_msg.Text += "\r\n";
                    // 释放strErrormsg所占内存
                    axAVPlayer1.FreeString(ref strErrorMsg);
                    return;
                }
                axAVPlayer1.StopPlay(strDeviceID, (int)pictureBox3.Handle);
                axAVPlayer1.FreeString(ref strDeviceID);

            }
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox3.Checked == true)
            {
                if (listView_CameraID.SelectedItems.Count == 0)
                {
                    MessageBox.Show("Plz select a device to play.");
                    return;
                }
                string strDeviceID = listView_CameraID.SelectedItems[0].SubItems[1].Text;
                int nEnalbeHWAccel = 0;     // 禁用硬解码
                int nErrorCode = 0;
                string strErrorMsg = "";
                nErrorCode = axAVPlayer1.PlayStream(strDeviceID, (int)pictureBox4.Handle, nEnalbeHWAccel);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                    textBox_msg.Text += strErrorMsg;
                    textBox_msg.Text += "\r\n";
                    // 释放strErrormsg所占内存
                    axAVPlayer1.FreeString(ref strErrorMsg);
                    return;
                }

                // 获取设备正在播放的窗口句柄
                int[] hWndArray = new int[16];
                int nArraySize = 16;
                nErrorCode = axAVPlayer1.GetDeviceWindow(strDeviceID, ref hWndArray[0], ref nArraySize);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                    textBox_msg.Text += strErrorMsg;
                    textBox_msg.Text += "\r\n";
                    // 释放strErrormsg所占内存
                    axAVPlayer1.FreeString(ref strErrorMsg);
                    return;
                }
                textBox_msg.Text += (strDeviceID + "已在 " + nArraySize.ToString() + " 个窗口上播放.\r\n");
            }
            else
            {
                string strDeviceID = "";
                string strErrorMsg = "";
                //  直接从播放图像的窗口句柄获取设备ID
                int nErrorCode = axAVPlayer1.GetWindowDevice((int)pictureBox4.Handle, ref strDeviceID);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                    textBox_msg.Text += strErrorMsg;
                    textBox_msg.Text += "\r\n";
                    // 释放strErrormsg所占内存
                    axAVPlayer1.FreeString(ref strErrorMsg);
                    return;
                }
                axAVPlayer1.StopPlay(strDeviceID, (int)pictureBox4.Handle);
                axAVPlayer1.FreeString(ref strDeviceID);
            }
        }
       
        private void Setting_Click(object sender, EventArgs e)
        {
            // 设置接收视频数据超时间隔，单位为毫秒
            axAVPlayer1.RecvTimeout = Convert.ToInt32(textbox_RecvTimeout.Text);
            // 设置报靠收接数据超时间隔，单位为毫秒
            axAVPlayer1.ReportInterval = Convert.ToInt32(textbox_ReportInterval.Text);

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // 启动定时器
            timer1.Enabled = true;
            timer1.Interval = 200;
            timer1.Tick += new EventHandler(Timer1_Tick);
        }
        private void Timer1_Tick(object Sender, EventArgs e)
        {
            // 重新播放时间为10秒
            int nRePlayTime = 10000;
            for (int i = m_PlayList.Count - 1; i >= 0 ; i--)
            {
                double curTime = (double)(sw.ElapsedTicks / (decimal)Stopwatch.Frequency);
                if ((curTime - m_PlayList[i].nTime)*1000 > nRePlayTime)
                {
                    // 重新播放该设备
                    axAVPlayer1.PlayStream(m_PlayList[i].strDevice, m_PlayList[i].hWnd, 0);
                    textBox_msg.Text += "重新播放设备:";
                    textBox_msg.Text += m_PlayList[i].strDevice;
                    textBox_msg.Text += "\r\n";
                    m_PlayList.RemoveAt(i);
                }
            }
        }

        private void axAVPlayer1_RecvTimeoutEvent(object sender, AxAVPlayerLib._DAVPlayerEvents_RecvTimeoutEvent e)
        {
            // 设备e.strDevice接收数据超时，可能断线, 此时可以把窗口刷黑
            // 隔一段时间之后，再重连该相机
            // 可以设置一个队列，把摄像机ID和窗口句柄都放队列中，建立一个线程或定时器，用于重连相机
            // 在这里仅作停止播放处理
            // e.hWnd 为发生问题的窗口ID
            axAVPlayer1.StopPlay(e.strDevice, 0);
            // 保存设备和窗口信息
            DevicePlay dp = new DevicePlay();
            dp.strDevice = e.strDevice;
            dp.hWnd = e.hWnd; 
            dp.nTime = (double)(sw.ElapsedTicks / (decimal)Stopwatch.Frequency);
            // 存入队列中
            m_PlayList.Add(dp);

            textBox_msg.Text += e.strDevice;
            textBox_msg.Text += " 可能已经掉线.\r\n";
            axAVPlayer1.FreeString(ref e.strDevice);
        }

        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox4.Checked == true)
                pictureBox2.Hide();
            else
                pictureBox2.Show();

        }

        private void checkBox5_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox5.Checked == true)
                pictureBox3.Hide();
            else
                pictureBox3.Show();
        }

        private void checkBox6_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox6.Checked == true)
                pictureBox4.Hide();
            else
                pictureBox4.Show();
        }
       
    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Collections;



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
        AvError_InvlaidPtzCommand        = AvError_base - 16,		// 无效的PTZ命令
        AvError_Invalid_PtzValue         = AvError_base - 17,		// 无效的PTZ命令值
        AvError_LoadAssitConfigFailed    = AvError_base - 18,		// 加载辅助操作配置文件失败
        AvError_DeivceNotConfigAssist    = AvError_base - 19,		// 设置未配置为操作辅助设备
        AvError_FailedEnableAssist       = AvError_base - 20,		// 启用辅助设置失败
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
     public struct RECT
    {
        public int left;
        public int top;
        public int right;
        public int bottom;
    };


    // HWND hWnd, HDC hDc, RECT rtDC, void *pUserPtr
    [UnmanagedFunctionPointerAttribute(CallingConvention.StdCall, CharSet = CharSet.Unicode)]
     // public delegate void ExternDCDrawDelegate(long pUserPtr, RECT rtDC, long hDc, long hWnd);
    public  delegate void ExternDCDrawDelegate(int hWnd, int hDc, RECT rtDC, int pUserPtr); 
   

    public partial class Form1 : Form
    {
        List<DevicePlay> m_PlayList = new List<DevicePlay>();
        // 精确计时类,需要引入System.Diagnostics
        Stopwatch  sw = new Stopwatch();
        public ExternDCDrawDelegate ExternDCDrawEvent;

        public CDrawInfo []m_ExternDraw;
       

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
            textBox_ServerIP.Text = "192.168.2.99";
           
                //textBox_ServerIP.Text = "172.27.18.101";
                //textBox_ServerIP.Text = "192.168.10.128";
                // 获取数据读取超时值，单位为毫秒
              textbox_RecvTimeout.Text = axAVPlayer1.RecvTimeout.ToString();
            // 获取报告超时间隔值，单位为毫秒，即每隔一段时间发送一次超报告
            textbox_ReportInterval.Text = axAVPlayer1.ReportInterval.ToString();
            ExternDCDrawEvent = new ExternDCDrawDelegate(ExternDCDraw);
            m_ExternDraw = new CDrawInfo[4];
            for(int i = 0;i < 4; i++)
                m_ExternDraw[i] = new CDrawInfo();
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
            IntPtr[] HandleArray = new IntPtr[4];
            HandleArray[0] = pictureBox1.Handle;
            HandleArray[1] = pictureBox2.Handle;
            HandleArray[2] = pictureBox3.Handle;
            HandleArray[3] = pictureBox4.Handle;
            for(int i = 0;i < listView_CameraID.CheckedItems.Count;i ++)
            {
                string strDeviceID = listView_CameraID.CheckedItems[i].SubItems[1].Text;
                int nEnalbeHWAccel = 0;     // 禁用硬解码
                int nErrorCode = 0;
                string strErrorMsg = "";
            
                
                nErrorCode = axAVPlayer1.PlayStream(strDeviceID, (int)HandleArray[i], nEnalbeHWAccel);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    axAVPlayer1.GetErrorMessage(nErrorCode, ref strErrorMsg);
                    textBox_msg.Text += strErrorMsg;
                    textBox_msg.Text += "\r\n";
                    // 释放strErrormsg所占内存
                    axAVPlayer1.FreeString(ref strErrorMsg);
                    return;
                }
                DevicePlay pDev = new DevicePlay();
                pDev.hWnd = (int)HandleArray[i];
                pDev.strDevice = strDeviceID;
                m_PlayList.Add(pDev);
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
            foreach(var dev in m_PlayList)
            {
                axAVPlayer1.StopPlay(dev.strDevice,0);
            }
            m_PlayList.Clear();

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
            //timer1.Enabled = true;
            //timer1.Interval = 200;
            //timer1.Tick += new EventHandler(Timer1_Tick);
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
        /*
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
        */
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

        private void CheckBox_ExternDraw_CheckedChanged(object sender, EventArgs e)
        {
            if (CheckBox_ExternDraw.Checked)
            {
                int i = 0;
                foreach (var dev in m_PlayList)
                {
                    //IntPtr pDCCall = Marshal.GetFunctionPointerForDelegate(ExternDCDrawEvent);
                    //axAVPlayer1.SetExternDCDraw(dev.strDevice,(int)pDCCall ,i);
                    axAVPlayer1.EnableOperationAssist(dev.strDevice, 1);
                    i++;
                }
            }
            else
            {
                foreach (var dev in m_PlayList)
                {
                    axAVPlayer1.EnableOperationAssist(dev.strDevice, 0);
                }
            }
        }
        public void ExternDCDraw(int hWnd, int hDc, RECT rtDC, int pUserPtr)
        {
            /*System.Console.WriteLine("ExternDC.\n");*/
            Graphics pGraph = Graphics.FromHdc((IntPtr)hDc);
            int nWidth = (int)(rtDC.right - rtDC.left);
            int nHeight = (int)(rtDC.bottom - rtDC.top);

            int nCenterX = (int)(rtDC.right + rtDC.left) / 2;
            int nCenterY = (int)(rtDC.top + rtDC.bottom) / 2;

            // Pen newPen = new Pen(Color.Red,2);
            //CPen *oldPen = dc.SelectObject(&newPen);

            // 可以使用任何DC的绘图函数进行绘图
            RECT rtCorner;			// 决定边角大小的矩形
            rtCorner.left = 0;
            rtCorner.top = 0;
            rtCorner.right = rtCorner.left + 200;
            rtCorner.bottom = rtCorner.top + 150;

            int nWidthCorner = (int)(rtCorner.right - rtCorner.left);
            int nHeightCorner = (int)(rtCorner.bottom - rtCorner.top);

            int nThickCornerV = 10;		// 垂直方向厚度
            int nThickCornerH = 10;		// 水平方向厚度

            int nDistanceV = 300;
            int nDistanceH = 400;
            int nGraphIndex = pUserPtr;
            ;

            switch (nGraphIndex)
            {
                case 0:      // 左上
                    {
                        int nStartX = (int)rtDC.left + nDistanceH;
                        int nStartY = (int)rtDC.top + nDistanceV;


                        m_ExternDraw[nGraphIndex].apt[0].X = nStartX;
                        m_ExternDraw[nGraphIndex].apt[0].Y = nStartY;


                        m_ExternDraw[nGraphIndex].apt[1].X = nStartX + nWidthCorner;
                        m_ExternDraw[nGraphIndex].apt[1].Y = nStartY;

                        m_ExternDraw[nGraphIndex].apt[2].X = m_ExternDraw[nGraphIndex].apt[1].X - nThickCornerH;
                        m_ExternDraw[nGraphIndex].apt[2].Y = m_ExternDraw[nGraphIndex].apt[1].Y + nThickCornerH;

                        m_ExternDraw[nGraphIndex].apt[3].X = nStartX + nThickCornerH;
                        m_ExternDraw[nGraphIndex].apt[3].Y = nStartY + nThickCornerV;

                        m_ExternDraw[nGraphIndex].apt[4].X = nStartX + nThickCornerV;
                        m_ExternDraw[nGraphIndex].apt[4].Y = nStartY + nHeightCorner - nThickCornerV;

                        m_ExternDraw[nGraphIndex].apt[5].X = nStartX;
                        m_ExternDraw[nGraphIndex].apt[5].Y = nStartY + nHeightCorner;
                    }
                    break;
                case 1:      // 右上
                    {
                        int nStartX = rtDC.right - nDistanceH - nWidthCorner;
                        int nStartY = rtDC.top + nDistanceV;

                        m_ExternDraw[nGraphIndex].apt[0].X = nStartX;
                        m_ExternDraw[nGraphIndex].apt[0].Y = nStartY;

                        m_ExternDraw[nGraphIndex].apt[1].X = nStartX + nWidthCorner;
                        m_ExternDraw[nGraphIndex].apt[1].Y = nStartY;

                        m_ExternDraw[nGraphIndex].apt[2].X = nStartX + nWidthCorner;
                        m_ExternDraw[nGraphIndex].apt[2].Y = nStartY + nHeightCorner;

                        m_ExternDraw[nGraphIndex].apt[3].X = nStartX + nWidthCorner - nThickCornerH;
                        m_ExternDraw[nGraphIndex].apt[3].Y = nStartY + nHeightCorner - nThickCornerH;

                        m_ExternDraw[nGraphIndex].apt[4].X = nStartX + nWidthCorner - nThickCornerH;
                        m_ExternDraw[nGraphIndex].apt[4].Y = nStartY + nThickCornerV;

                        m_ExternDraw[nGraphIndex].apt[5].X = nStartX + nThickCornerH;
                        m_ExternDraw[nGraphIndex].apt[5].Y = nStartY + nThickCornerV;
                    }
                    break;
                case 2:      // 左下
                    {
                        int nStartX = rtDC.left + nDistanceH;
                        int nStartY = rtDC.bottom - nDistanceV - nHeightCorner;

                        m_ExternDraw[nGraphIndex].apt[0].X = nStartX;
                        m_ExternDraw[nGraphIndex].apt[0].Y = nStartY;

                        m_ExternDraw[nGraphIndex].apt[1].X = nStartX + nThickCornerH;
                        m_ExternDraw[nGraphIndex].apt[1].Y = nStartY + nThickCornerH;

                        m_ExternDraw[nGraphIndex].apt[2].X = nStartX + nThickCornerH;
                        m_ExternDraw[nGraphIndex].apt[2].Y = nStartY + nHeightCorner - nThickCornerV;

                        m_ExternDraw[nGraphIndex].apt[3].X = nStartX + nWidthCorner - nThickCornerH;
                        m_ExternDraw[nGraphIndex].apt[3].Y = nStartY + nHeightCorner - nThickCornerV;

                        m_ExternDraw[nGraphIndex].apt[4].X = nStartX + nWidthCorner;
                        m_ExternDraw[nGraphIndex].apt[4].Y = nStartY + nHeightCorner;

                        m_ExternDraw[nGraphIndex].apt[5].X = nStartX;
                        m_ExternDraw[nGraphIndex].apt[5].Y = nStartY + nHeightCorner;
                    }
                    break;
                case 3:      // 右下
                    {
                        int nStartX = rtDC.right - nDistanceH - nWidthCorner;
                        int nStartY = rtDC.bottom - nDistanceV - nHeightCorner;

                        m_ExternDraw[nGraphIndex].apt[0].X = nStartX + nWidthCorner;
                        m_ExternDraw[nGraphIndex].apt[0].Y = nStartY;

                        m_ExternDraw[nGraphIndex].apt[1].X = nStartX + nWidthCorner;
                        m_ExternDraw[nGraphIndex].apt[1].Y = nStartY + nHeightCorner;

                        m_ExternDraw[nGraphIndex].apt[2].X = nStartX;
                        m_ExternDraw[nGraphIndex].apt[2].Y = nStartY + nHeightCorner;

                        m_ExternDraw[nGraphIndex].apt[3].X = nStartX + nThickCornerH;
                        m_ExternDraw[nGraphIndex].apt[3].Y = nStartY + nHeightCorner - nThickCornerV;

                        m_ExternDraw[nGraphIndex].apt[4].X = nStartX + nWidthCorner - nThickCornerH;
                        m_ExternDraw[nGraphIndex].apt[4].Y = nStartY + nHeightCorner - nThickCornerV;

                        m_ExternDraw[nGraphIndex].apt[5].X = nStartX + nWidthCorner - nThickCornerH;
                        m_ExternDraw[nGraphIndex].apt[5].Y = nStartY + nThickCornerV;

                    }
                    break;
                default:
                    break;
            }
            pGraph.DrawPolygon(m_ExternDraw[nGraphIndex].pen, m_ExternDraw[nGraphIndex].apt);
            pGraph.FillPolygon(m_ExternDraw[nGraphIndex].brush, m_ExternDraw[nGraphIndex].apt);
        }

        private void button_UpdateOpAssist_Click(object sender, EventArgs e)
        {
            if (CheckBox_ExternDraw.Checked)
            {
                foreach (var dev in m_PlayList)
                    axAVPlayer1.EnableOperationAssist(dev.strDevice, 0);
            }
            axAVPlayer1.LoadOpAssistConfigure();
            if (CheckBox_ExternDraw.Checked)
                foreach (var dev in m_PlayList)
                    axAVPlayer1.EnableOperationAssist(dev.strDevice, 1);
        }

 
        private void SwitchScreen_Click(object sender, EventArgs e)
        {
            if (!m_bFitFrame)
            {
                pictureBox1.Hide();
                pictureBox2.Hide();
                pictureBox3.Hide();
                pictureBox4.Hide();

                // 调整控件窗口尺寸
                axAVPlayer1.Left = pictureBox1.Left;
                axAVPlayer1.Top = pictureBox1.Top;

                axAVPlayer1.Width = pictureBox5.Width;
                axAVPlayer1.Height = pictureBox5.Height;

                axAVPlayer1.Show();
            } 
            int nMode = Convert.ToInt32(comboBox_mode.SelectedItem.ToString());
            int nCrane = 1;         // 配置文件只配置了1和2号Crane
            //axAVPlayer1.SwitchScreen(nCrane, nMode);
            m_bFitFrame = true;     

        }

        private void QueryRecord_Click(object sender, EventArgs e)
        {

        }

        private void PlayRecord_Click(object sender, EventArgs e)
        {

        }
    };
    public class CDrawInfo
    {
        public Point[] apt;
        public Pen pen;
        public SolidBrush brush;
        public CDrawInfo()
        {
            apt = new Point[6];
            pen = new Pen(Color.Red, 3);
            brush = new SolidBrush(Color.Red);
        }
    };
}

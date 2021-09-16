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
        AvError_Crane_notExist           = AvError_base - 21,		// 吊机设备不存在
        AvError_ScreenMode_notExist      = AvError_base - 22,		// 指定的模式不存在
        AvError_NotSingleFramePlayer     = AvError_base - 23,		// 不是一单帧播放器
        AvError_OutofPlayingRange        = AvError_base - 24,		// 指定的时间点超出播放器的时间范围
        AvError_AS300ServiceIsDisabled   = AvError_base - 25,		// AS300服务尚未开启,需修改Configure.xml文件
        AvError_ExternalError            = AvError_base - 253,		// 内部错误
        AvError_InsufficentMemory        = AvError_base - 254,		// 内存不足
        AvError_UnknownException         = AvError_base - 255		// 未知异常 
    };

    enum IPCPLAY_Status
    {
        IPC_Succeed = (0),	///< 操作成功
        IPC_Error_InvalidParameters          = (-1),	///< 无效的参数
        IPC_Error_NotVideoFile               = (-2),	///< 非视频录像文件
        IPC_Error_NotInputStreamHeader       = (-3),	///< 未输入视频录像文件头
        IPC_Error_InvalidSDKVersion          = (-4),	///< 录像文件头中的的SDK版本无效
        IPC_Error_PlayerNotStart             = (-5),	///< 播放器尚未启动,无法取得播放过程的信息或属性
        IPC_Error_PlayerHasStart             = (-6),	///< 播放器已经启动，不能执行初始化或其它设置操作
        IPC_Error_NotFilePlayer              = (-7),	///< 这不是一个文件播放对象
        IPC_Error_InvalidFrame               = (-8),	///< 无效的帧
        IPC_Error_InvalidFrameType           = (-9),	///< 无效的帧类型
        IPC_Error_SummaryNotReady            = (-10),	///< 文件摘要信息尚未准备好
        IPC_Error_FrameCacheIsFulled         = (-11),	///< 视频帧缓冲区已经满
        IPC_Error_FileNotOpened              = (-12),	///< 尚未打开视频文件
        IPC_Error_MaxFrameSizeNotEnough      = (-13),	///< 最大帧尺寸不足，可能视频文件中存在超过256K的帧数据,应调用SetMaxFrameSize设置新的帧尺寸上限
        IPC_Error_InvalidPlayRate            = (-14),	///< 无效的播放倍率
        IPC_Error_BufferSizeNotEnough        = (-15),	///< 提供的缓冲区长度不足
        IPC_Error_VideoThreadNotRun          = (-16),	///< 视频解码线程尚未启动或已经退出
        IPC_Error_AudioThreadNotRun          = (-17),	///< 音频频解码线程尚未启动或已经退出
        IPC_Error_ReadFileFailed             = (-18),	///< 读文件失败
        IPC_Error_FileNotExist               = (-19),	///< 文件不存在
        IPC_Error_InvalidTimeOffset          = (-20),	///< 无效的时间偏移或时间超出文件长度范围
        IPC_Error_DecodeFailed               = (-21),	///< 解码失败
        IPC_Error_InvalidWindow              = (-22),	///< 无效的窗口句柄
        IPC_Error_AudioDeviceNotReady        = (-23),	///< 音频播放初始化失败(播放设备未就绪),
        IPC_Error_DxError                    = (-24),	///< DirectX 错误
        IPC_Error_PlayerIsNotPaused          = (-25),	///< 播放器尚未暂停
        IPC_Error_VideoThreadStartFailed     = (-26),	///< 播放线程启动失败
        IPC_Error_VideoThreadAbnormalExit    = (-27),	///< 播放线程异常退出
        IPC_Error_WindowNotAssigned          = (-28),	///< 未指定显示窗口,无法截图
        IPC_Error_SnapShotProcessNotRun      = (-29),	///< 截图进程未运行
        IPC_Error_SnapShotProcessFileMissed  = (-30),	///< 截图程序文件丢失
        IPC_Error_SnapShotProcessStartFailed = (-31),///< 截图进程启动失败
        IPC_Error_SnapShotFailed             = (-32),	///< 截图进程未运行
        IPC_Error_PlayerHasStop              = (-33),	///< 播放器已经停止，不能执行初始化或其它设置操作
        IPC_Error_InvalidCacheSize           = (-34),	///< 播放器已经启动，不能执行初始化或其它设置操作
        IPC_Error_UnsupportHaccel            = (-35),	///< 当前系统不支持硬解码功能
        IPC_Error_UnsupportedCodec           = (-36),	///< 不支持的编码格式
        IPC_Error_RenderWndOverflow          = (-37),	///< 渲染窗口超限
        IPC_Error_RocateNotWork              = (-38),	///< 图像旋转不适用，可能是启用了硬解码
        IPC_Error_BufferOverflow             = (-39),	///< 缓存溢出,可能提供的缓存空间不足以容纳所请求的数据
        IPC_Error_DXRenderInitialized        = (-40),	///< DirectX渲染器已经初始化
        IPC_Error_ParserNotFound             = (-41),	///< 找不到匹配的解析器
        IPC_Error_AllocateCodecContextFailed = (-42),	///< 分配编码上下文失败
        IPC_Error_StreamParserExisted        = (-43),	///< 流解析器已经存在
        IPC_Error_StreamParserNotStarted     = (-44),	///< 流解析器尚未启动
        IPC_Error_DXRenderNotInitialized     = (-45),	///< DirectX渲染器尚未初始化
        IPC_Error_NotAsyncPlayer             = (-46),	///< 当前播放器未启用异步渲染功能
        IPC_Error_MediaFileHeaderError       = (-47),	///< 文件件头有错误
        IPC_Error_UnsupportedFormat          = (-48),	///< 不支持的图像格式
        IPC_Error_OpenCodecFailed            = (-49),	///< 分配编码上下文失败
        IPC_Error_InsufficentMemory          = (-255)	///< 内存不足
    };
    struct DevicePlay
    {
        public string  strDevice;
        public int hWnd;
        public double nTime;
        public bool bPlaying;
        
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

        int nCurPictureIndex = 0;

        public string ipcplay_GetErrorMessage(int nErrorCode)
        {
            IPCPLAY_Status nStatus = (IPCPLAY_Status)nErrorCode;
            switch(nStatus)
            {
                case IPCPLAY_Status.IPC_Error_AllocateCodecContextFailed:
                    break;
            }
            switch (nStatus)
            {
                case IPCPLAY_Status.IPC_Succeed:
                    return "Succeed.";
                case IPCPLAY_Status.IPC_Error_InvalidParameters:
                    return "Invalid Parameters.";
                case IPCPLAY_Status.IPC_Error_NotVideoFile:
                    return "The input file is not a video file or it's a unknow video file.";
                case IPCPLAY_Status.IPC_Error_NotInputStreamHeader:
                    return "No  StreamHeader is Input.";
                case IPCPLAY_Status.IPC_Error_InvalidSDKVersion:
                    return "Thie SDK Version of StreamHeader is invalid.";
                case IPCPLAY_Status.IPC_Error_PlayerNotStart:
                    return "The Player not started";
                case IPCPLAY_Status.IPC_Error_PlayerHasStart:
                    return "The player is started.";
                case IPCPLAY_Status.IPC_Error_NotFilePlayer:
                    return "The input handle is not a FilePlayer.";
                case IPCPLAY_Status.IPC_Error_InvalidFrame:
                    return "A invalid frame is input.";
                case IPCPLAY_Status.IPC_Error_InvalidFrameType:
                    return "The Frame type is invalid.";
                case IPCPLAY_Status.IPC_Error_SummaryNotReady:
                    return "The Summary is not ready.";
                case IPCPLAY_Status.IPC_Error_FrameCacheIsFulled:
                    return "The frame cache is fulled";
                case IPCPLAY_Status.IPC_Error_FileNotOpened:
                    return "The file is not opened.";
                case IPCPLAY_Status.IPC_Error_MaxFrameSizeNotEnough:
                    return "the size of MaxFrame is not Enough,please specified a larger one.";
                case IPCPLAY_Status.IPC_Error_InvalidPlayRate:
                    return "The play rate is Invalid .";
                case IPCPLAY_Status.IPC_Error_BufferSizeNotEnough:
                    return "The Buffer Size Not Enough.";
                case IPCPLAY_Status.IPC_Error_VideoThreadNotRun:
                    return "The Video Thread Not Run.";
                case IPCPLAY_Status.IPC_Error_AudioThreadNotRun:
                    return "The Audio Thread Not Run.";
                case IPCPLAY_Status.IPC_Error_ReadFileFailed:
                    return "Failed in read file.";
                case IPCPLAY_Status.IPC_Error_FileNotExist:
                    return "The specified file is not exist.";
                case IPCPLAY_Status.IPC_Error_InvalidTimeOffset:
                    return "The time offset is invalid.";
                case IPCPLAY_Status.IPC_Error_DecodeFailed:
                    return "Failed in decoding.";
                case IPCPLAY_Status.IPC_Error_InvalidWindow:
                    return "Input a invalid window";
                case IPCPLAY_Status.IPC_Error_AudioDeviceNotReady:
                    return "Audio Device is Not Ready.";
                case IPCPLAY_Status.IPC_Error_DxError:
                    return "DirectX Error.";
                case IPCPLAY_Status.IPC_Error_PlayerIsNotPaused:
                    return "The Player is not paused.";
                case IPCPLAY_Status.IPC_Error_VideoThreadStartFailed:
                    return "The Video Thread start failed.";
                case IPCPLAY_Status.IPC_Error_VideoThreadAbnormalExit:
                    return "The video thread exit abnormal.";
                case IPCPLAY_Status.IPC_Error_MediaFileHeaderError:
                    return "There is a error in Media file header.";
                case IPCPLAY_Status.IPC_Error_WindowNotAssigned:
                    return "Please specify a window to show video.";
                case IPCPLAY_Status.IPC_Error_SnapShotProcessNotRun:
                    return "SnapShot Process Not Run.";
                case IPCPLAY_Status.IPC_Error_SnapShotProcessFileMissed:
                    return "SnapShot Process File Missed.";
                case IPCPLAY_Status.IPC_Error_SnapShotProcessStartFailed:
                    return "SnapShot Process Start Failed.";
                case IPCPLAY_Status.IPC_Error_SnapShotFailed:
                    return "SnapShot Failed.";
                case IPCPLAY_Status.IPC_Error_PlayerHasStop:
                    return "The Player Has Stopped.";
                case IPCPLAY_Status.IPC_Error_InvalidCacheSize:
                    return "Invalid Cache Size";
                case IPCPLAY_Status.IPC_Error_UnsupportHaccel:
                    return "Unsupport Haccel.";
                case IPCPLAY_Status.IPC_Error_UnsupportedFormat:
                    return "Unsupported Photo Format.";
                case IPCPLAY_Status.IPC_Error_UnsupportedCodec:
                    return "Unsupported Video Codec.";
                case IPCPLAY_Status.IPC_Error_RenderWndOverflow:
                    return "Render window Overflow.";
                case IPCPLAY_Status.IPC_Error_RocateNotWork:
                    return "Rocate Not Work";
                case IPCPLAY_Status.IPC_Error_BufferOverflow:
                    return "Buffer Over flow";
                case IPCPLAY_Status.IPC_Error_DXRenderInitialized:
                    return "DirectX Render has been Initialized.";
                case IPCPLAY_Status.IPC_Error_ParserNotFound:
                    return "Parser Not Found.";
                case IPCPLAY_Status.IPC_Error_AllocateCodecContextFailed:
                    return "Allocate Codec Context Failed.";
                case IPCPLAY_Status.IPC_Error_OpenCodecFailed:
                    return "Open Codec Failed.";
                case IPCPLAY_Status.IPC_Error_StreamParserExisted:
                    return "Stream Parser Existed.";
                case IPCPLAY_Status.IPC_Error_StreamParserNotStarted:
                    return "Stream Parser Not Started.";
                case IPCPLAY_Status.IPC_Error_DXRenderNotInitialized:
                    return "DirectX Render Not Initialized";
                case IPCPLAY_Status.IPC_Error_NotAsyncPlayer:
                    return "Not a AsyncPlayer";
                case IPCPLAY_Status.IPC_Error_InsufficentMemory:
                    return "Insufficent Memory";
                default:
                    return "Unknown Error.";
            }
        }
        public string GetErrorMessage(int nErrorCode)
        {
	        if (nErrorCode == 0)
		        return "Succeed.";
	        
	        else if (nErrorCode < 0 && nErrorCode >-1000)
		        return ipcplay_GetErrorMessage(nErrorCode);
	       
	        else if (nErrorCode < -1000 && nErrorCode >= -2000)
	        {
                AVStatus nStatus = (AVStatus)nErrorCode;
                switch (nStatus)
		        {
                    case AVStatus.AvError_Succeed:
			            return "Succeed.";
                    case AVStatus.AvError_InvalidParameters:
			            return "Invalid Parameters.";
                    case AVStatus.AvError_Invalid_ServerAddress:
			            return "Invalid Servere address.";	
                    case AVStatus.AvError_Invliad_ServerPort:
			            return "Invliad Server port.";	
                    case AVStatus.AvError_Invalid_AccountOrPassword:		
			            return "Account or password is Invliad.";	
                    case AVStatus.AvError_ConnectServerFailed:		
			            return "Failed in connect Server.";	
                    case AVStatus.AvError_LoginFailed:
			            return "Failed in login to the server.";	
                    case AVStatus.AvError_NotLogintoServer:
			            return "Not login to the Server.";	
                    case AVStatus.AvError_InvalidWindow:
                        return "The handle of window to play video is invalid.";
                    case AVStatus.AvError_DeviceNotExist:
			            return "The sepecified device it not exist.";
                    case AVStatus.AvError_ConnectDeviceFailed:
			            return "Failed in connect the sepecified device.";	
                    case AVStatus.AvError_DeviceInPlaying:
			            return "The sepecified device is being played now,please don't play again.";	
                    case AVStatus.AvError_DBException:
			            return "An exception occured when access database.";
		            case AVStatus.AvError_DeviceNotInPlaying:
			            return "The device is not in playing.";
		            case AVStatus.AVError_BufferOverflow:
			            return "Buffer over flow." ;// 缓存溢出,提供的内存空间不足以容纳所请求的数据";
		            case AVStatus.AvError_WindowNotPlaying:
			            return "There is no any devices been played on the windows."; //窗口尚未播放任何设备";
		            case AVStatus.AvError_InvlaidPtzCommand:
			            return "Invalid PTZ Command.";
		            case AVStatus.AvError_Invalid_PtzValue:
			            return "Invalid PTZ Value.";
		            case AVStatus.AvError_LoadAssitConfigFailed:
			            return "Failed to load OperationAssist.xml.";
		            case AVStatus.AvError_DeivceNotConfigAssist:
			            return "The Device is not specified in the OperationAssist.xml.";
		            case AVStatus.AvError_FailedEnableAssist:
			            return "Failed in enable Operation Assist.";
		            case AVStatus.AvError_Crane_notExist:
			            return "The specified Crane ID is not exist.";
		            case AVStatus.AvError_ScreenMode_notExist:
			            return "The specified mode ID is not exist.";
		            case AVStatus.AvError_OutofPlayingRange:
			            return "Out of Player Timer Ranage";
		            case AVStatus.AvError_AS300ServiceIsDisabled:
			            return "AS300 Service is Disabled,Please Edit Configure.xml to active it.";
		            case AVStatus.AvError_ExternalError:
			            return "External error.";
                    case AVStatus.AvError_InsufficentMemory:
			            return "Insufficent Memory.";	
                    case AVStatus.AvError_UnknownException:
			            return "Unknown Exception.";
		            default:
			            return "Can't locate the error code.";
		        }
	        }
            else
                return "Unkonw Error code";
        }

        public Form1()
        {
            InitializeComponent();
            PlayWndArray = new PlayWndInfo[12];            
            PlayWndArray[0].PlayWnd = pictureBox1;
            PlayWndArray[1].PlayWnd = pictureBox2;
            PlayWndArray[2].PlayWnd = pictureBox3;
            PlayWndArray[3].PlayWnd = pictureBox4;
            PlayWndArray[4].PlayWnd = pictureBox5;
            PlayWndArray[5].PlayWnd = pictureBox6;
            PlayWndArray[6].PlayWnd = pictureBox7;
            PlayWndArray[7].PlayWnd = pictureBox8;
            PlayWndArray[8].PlayWnd = pictureBox9;
            PlayWndArray[9].PlayWnd = pictureBox10;
            PlayWndArray[10].PlayWnd = pictureBox11;
            PlayWndArray[11].PlayWnd = pictureBox12;

            for (int i = 0; i < 12; i++)
            {
                PlayWndArray[i].bPlaying = false;
                PlayWndArray[i].hPlayHandle = 0;
            }
                nCurPictureIndex = 0;
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.Fixed3D;
           

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
                int nDeviceCount = 0;
                // 从服务器取得所有设备的ID
                int nErrorCode = axAVPlayer1.GetDeviceID(ref strDeviceIDList, ref nDeviceCount);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    textBox_msg.Text += GetErrorMessage(nErrorCode);
                    textBox_msg.Text += "\r\n";
                    // 释放strErrormsg所占内存
                    //axAVPlayer1.FreeString(ref strErrorMsg);
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
            
        }

        private void button_Play_Click(object sender, EventArgs e)
        {
            if (listView_CameraID.CheckedItems.Count == 0)
            {
                MessageBox.Show("Plz select a device to play.");
                return;
            }
           
            int nEnalbeHWAccel = 0;     // 禁用硬解码
            if (checkBox_HACCEL.Checked)
                nEnalbeHWAccel = 1;
            for(int i = 0;i < listView_CameraID.CheckedItems.Count;i ++)
            {
                string strDeviceID = listView_CameraID.CheckedItems[i].SubItems[1].Text;
               
                int nErrorCode = 0;         
                
                nErrorCode = axAVPlayer1.PlayStream(strDeviceID, PlayWndArray[i].PlayWnd.Handle.ToInt32(), nEnalbeHWAccel);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    textBox_msg.Text += GetErrorMessage(nErrorCode);
                    textBox_msg.Text += "\r\n";
                    return;
                }
                DevicePlay pDev = new DevicePlay();
                pDev.strDevice = strDeviceID;
                m_PlayList.Add(pDev);
                button_Play.Enabled = false;
                button_Stop.Enabled = true;

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

            nErrorCode = axAVPlayer1.PlayComboStream(strDeviceID1, strDeviceID2, (int)pictureBox1.Handle, nEnalbeHWAccel,1);
            if (nErrorCode != (int)AVStatus.AvError_Succeed)
            {
                textBox_msg.Text += GetErrorMessage(nErrorCode);
                textBox_msg.Text += "\r\n";
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
                textBox_msg.Text += GetErrorMessage(nErrorCode);
                textBox_msg.Text += "\r\n";
                return;
            }
            
        }
        
        private void button_Stop_Click(object sender, EventArgs e)
        {
            int nCount = listView_CameraID.CheckedItems.Count;
                       
            foreach(var dev in m_PlayList)
            {
                
                axAVPlayer1.StopPlay(dev.strDevice,0);
            }
            m_PlayList.Clear();

        

            button_Play.Enabled = true;
            button_Stop.Enabled = false;
        }
               

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            //if (checkBox1.Checked == true)
            {
                if (listView_CameraID.SelectedItems.Count == 0)
                {
                    MessageBox.Show("Plz select a device to play.");
                    return;
                }
                string strDeviceID = listView_CameraID.SelectedItems[0].SubItems[1].Text;
                int nEnalbeHWAccel = 0;     // 禁用硬解码
                int nErrorCode = 0;
                nErrorCode = axAVPlayer1.PlayStream(strDeviceID, (int)pictureBox2.Handle, nEnalbeHWAccel);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    textBox_msg.Text += GetErrorMessage(nErrorCode);
                    textBox_msg.Text += "\r\n";
                    return;
                }

                // 获取设备正在播放的窗口句柄
                int[] hWndArray = new int[16];
                int nArraySize = 16;
                nErrorCode = axAVPlayer1.GetDeviceWindow(strDeviceID, ref hWndArray[0], ref nArraySize);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    textBox_msg.Text += GetErrorMessage(nErrorCode);
                    textBox_msg.Text += "\r\n";
                    return;
                }
                textBox_msg.Text += (strDeviceID + "已在 " + nArraySize.ToString() + " 个窗口上播放.\r\n");
               
            }
            //else 
            {
                string strDeviceID = "";
                //  直接从播放图像的窗口句柄获取设备ID
                int nErrorCode = axAVPlayer1.GetWindowDevice((int)pictureBox2.Handle, ref strDeviceID);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    textBox_msg.Text += GetErrorMessage(nErrorCode);
                    textBox_msg.Text += "\r\n";
                    return;
                }
                axAVPlayer1.StopPlay(strDeviceID, (int)pictureBox2.Handle);
                axAVPlayer1.FreeString(ref strDeviceID);
            }
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            //if (checkBox2.Checked == true)
            {
                if (listView_CameraID.SelectedItems.Count == 0)
                {
                    MessageBox.Show("Plz select a device to play.");
                    return;
                }
                string strDeviceID = listView_CameraID.SelectedItems[0].SubItems[1].Text;

                int nEnalbeHWAccel = 0;     // 禁用硬解码
                int nErrorCode = 0;
                nErrorCode = axAVPlayer1.PlayStream(strDeviceID, (int)pictureBox3.Handle, nEnalbeHWAccel);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    textBox_msg.Text += GetErrorMessage(nErrorCode);
                    textBox_msg.Text += "\r\n";
                    return;
                }
                // 获取设备正在播放的窗口句柄
                int[] hWndArray = new int[16];
                int nArraySize = 16;
                nErrorCode = axAVPlayer1.GetDeviceWindow(strDeviceID, ref hWndArray[0], ref nArraySize);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    textBox_msg.Text += GetErrorMessage(nErrorCode);
                    textBox_msg.Text += "\r\n";
                    return;
                }
                textBox_msg.Text += (strDeviceID + "已在 " + nArraySize.ToString() + " 个窗口上播放.\r\n");
            }
            //else
            {
                string strDeviceID = "";
                //  直接从播放图像的窗口句柄获取设备ID
                int nErrorCode = axAVPlayer1.GetWindowDevice((int)pictureBox3.Handle, ref strDeviceID);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    textBox_msg.Text += GetErrorMessage(nErrorCode);
                    textBox_msg.Text += "\r\n";
                    return;
                }
                axAVPlayer1.StopPlay(strDeviceID, (int)pictureBox3.Handle);
                axAVPlayer1.FreeString(ref strDeviceID);

            }
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            //if (checkBox3.Checked == true)
            {
                if (listView_CameraID.SelectedItems.Count == 0)
                {
                    MessageBox.Show("Plz select a device to play.");
                    return;
                }
                string strDeviceID = listView_CameraID.SelectedItems[0].SubItems[1].Text;
                int nEnalbeHWAccel = 0;     // 禁用硬解码
                int nErrorCode = 0;
                nErrorCode = axAVPlayer1.PlayStream(strDeviceID, (int)pictureBox4.Handle, nEnalbeHWAccel);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    textBox_msg.Text += GetErrorMessage(nErrorCode);
                    textBox_msg.Text += "\r\n";
                    return;
                }

                // 获取设备正在播放的窗口句柄
                int[] hWndArray = new int[16];
                int nArraySize = 16;
                nErrorCode = axAVPlayer1.GetDeviceWindow(strDeviceID, ref hWndArray[0], ref nArraySize);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    textBox_msg.Text += GetErrorMessage(nErrorCode);
                    textBox_msg.Text += "\r\n";
                    return;
                }
                textBox_msg.Text += (strDeviceID + "已在 " + nArraySize.ToString() + " 个窗口上播放.\r\n");
            }
            //else
            {
                string strDeviceID = "";
                //  直接从播放图像的窗口句柄获取设备ID
                int nErrorCode = axAVPlayer1.GetWindowDevice((int)pictureBox4.Handle, ref strDeviceID);
                if (nErrorCode != (int)AVStatus.AvError_Succeed)
                {
                    textBox_msg.Text += GetErrorMessage(nErrorCode);
                    textBox_msg.Text += "\r\n";
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
            //if (checkBox4.Checked == true)
            //    pictureBox2.Hide();
            //else
            //    pictureBox2.Show();

        }

        private void checkBox5_CheckedChanged(object sender, EventArgs e)
        {
            //if (checkBox5.Checked == true)
            //    pictureBox3.Hide();
            //else
            //    pictureBox3.Show();
        }

        private void checkBox6_CheckedChanged(object sender, EventArgs e)
        {
            //if (checkBox6.Checked == true)
            //    pictureBox4.Hide();
            //else
            //    pictureBox4.Show();
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

        private void pictureBox4_Click(object sender, EventArgs e)
        {
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.FixedSingle;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
            nCurPictureIndex = 3;
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.Fixed3D;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.FixedSingle;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
            nCurPictureIndex = 2;
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.Fixed3D;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
           
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.FixedSingle;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
            nCurPictureIndex = 1;
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.Fixed3D;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.FixedSingle;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
            nCurPictureIndex = 0;
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.Fixed3D;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
        }

        private void pictureBox9_Click(object sender, EventArgs e)
        {
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.FixedSingle;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
            nCurPictureIndex = 8;
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.Fixed3D;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
        }

        private void pictureBox8_Click(object sender, EventArgs e)
        {
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.FixedSingle;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
            nCurPictureIndex = 7;
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.Fixed3D;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
        }

        private void pictureBox7_Click(object sender, EventArgs e)
        {
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.FixedSingle;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
            nCurPictureIndex = 6;
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.Fixed3D;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
        }

        private void pictureBox6_Click(object sender, EventArgs e)
        {
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.FixedSingle;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
            nCurPictureIndex = 5;
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.Fixed3D;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
        }   

        private void pictureBox12_Click(object sender, EventArgs e)
        {
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.FixedSingle;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
            nCurPictureIndex = 11;
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.Fixed3D;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
        }

        private void pictureBox11_Click(object sender, EventArgs e)
        {
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.FixedSingle;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
            nCurPictureIndex = 10;
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.Fixed3D;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
        }

        private void pictureBox10_Click(object sender, EventArgs e)
        {
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.FixedSingle;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
            nCurPictureIndex = 9;
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.Fixed3D;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
        }

        private void pictureBox5_Click(object sender, EventArgs e)
        {
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.FixedSingle;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
            nCurPictureIndex = 4;
            PlayWndArray[nCurPictureIndex].PlayWnd.BorderStyle = BorderStyle.Fixed3D;
            PlayWndArray[nCurPictureIndex].PlayWnd.Refresh();
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

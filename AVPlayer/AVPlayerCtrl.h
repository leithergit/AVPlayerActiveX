#pragma once

// AVPlayerCtrl.h : Declaration of the CAVPlayerCtrl ActiveX Control class.

//#include <boost/smart_ptr.hpp>
//using namespace boost;
#include "MySQLConnector.h"
#include "Utility.h"
#include "Runlog.h"
#include "TimeUtility.h"
#include "AutoLock.h"
#include "CriticalSectionAgent.h"
#include "SimpleWnd.h"
#include "decSPS.h"
#include "../RTSP/rtsp.h"
#include "VS300ClientSDK.h"
#include "Stat.h"
#ifdef _DEBUG
#pragma comment(lib,"../debug/rtsp.lib")
#pragma comment(lib,"VS300ClientSDKD")
#else
#pragma comment(lib,"../release/rtsp.lib")
#pragma comment(lib,"VS300ClientSDK")
#endif
#include "../ipcplaysdk/IPCPlaySDK.h"
#pragma comment(lib,"../ipcplaysdk/ipcplaysdk.lib")
#include <list>
#include <map>
#include "VideoFrame.h"
#include "DhStreamParser.h"
#include "TimeUtility.h"

using namespace  std;

#define rtsp_TCP	1
#define rtsp_UDP	0
enum AVStatus
{
	AvError_Succeed = 0,				// 操作成功
	AvError_base = -1000,				
	AvError_InvalidParameters			 = AvError_base - 1,		// 参数无效
	AvError_Invalid_ServerAddress		 = AvError_base - 2,		// 服务器地址不是一个有效的IP地址	
	AvError_Invliad_ServerPort			 = AvError_base - 3,		// 服务器端口无效
	AvError_Invalid_AccountOrPassword	 = AvError_base - 4,		// 用户名或密码无效
	AvError_ConnectServerFailed			 = AvError_base - 5,		// 连接服务器失败
	AvError_LoginFailed					 = AvError_base - 6,		// 登录服务器失败
	AvError_NotLogintoServer			 = AvError_base - 7,		// 尚未登录到服务器
	AvError_InvalidWindow				 = AvError_base - 8,		// 用于播放的窗口句柄指示的不是一个有效的窗口
	AvError_DeviceNotExist				 = AvError_base - 9,		// 请求的设备不存在
	AvError_ConnectDeviceFailed			 = AvError_base - 10,		// 连接设备失败
	AvError_DeviceInPlaying				 = AvError_base - 11,		// 设备正在播放中，请匆复播放
	AvError_DBException					 = AvError_base - 12,		// 数据库访问异常
	AvError_DeviceNotInPlaying			 = AvError_base - 13,		// 设备不在播放中
	AVError_BufferOverflow				 = AvError_base	- 14,		// 缓存溢出,提供的内存空间不足以容纳所请求的数据
	AvError_WindowNotPlaying			 = AvError_base - 15,		// 窗口尚未播放任何设备	
	AvError_InvlaidPtzCommand			 = AvError_base - 16,		// 无效的PTZ命令
	AvError_Invalid_PtzValue			 = AvError_base - 17,		// 无效的PTZ命令值
	AvError_LoadAssitConfigFailed		 = AvError_base - 18,		// 加载辅助操作配置文件失败
	AvError_DeivceNotConfigAssist		 = AvError_base - 19,		// 设置未配置为操作辅助设备
	AvError_FailedEnableAssist			 = AvError_base - 20,		// 启用辅助设置失败
	AvError_Crane_notExist				 = AvError_base - 21,		// 吊机设备不存在
	AvError_ScreenMode_notExist			 = AvError_base - 22,		// 指定的模式不存在
	AvError_NotSingleFramePlayer		 = AvError_base - 23,		// 不是一单帧播放器
	AvError_OutofPlayingRange			 = AvError_base - 24,		// 指定的时间点超出播放器的时间范围
	AvError_AS300ServiceIsDisabled		 = AvError_base - 25,		// AS300服务尚未开启,需修改Configure.xml文件
	AvError_ExternalError				 = AvError_base - 253,		// 内部错误
	AvError_InsufficentMemory			 = AvError_base - 254,		// 内存不足
	AvError_UnknownException			 = AvError_base - 255,		// 未知异常 
	AvError_AS300_Error = -2000
};

enum Position
{
	Pos_Left,
	Pos_Top,
	Pos_Right,
	Pos_Bottom
};

struct PosParam
{
	int nLeft;
	int nTop;
	int nRight;
	int nBottom;
	float fSpreadHeiMin;
	float fSpreadHeiMax;
};
typedef list<PosParam>	ListPos;

enum PtzCode
{
	Ptz_Center		 = 0,
	Ptz_Areazoom,
	Ptz_ImageWidth,
	Ptz_ImageHeight,
	Ptz_Move		= 4,
	// 以下操作皆存在绝对和相对两种模式
	Ptz_Pan			 = 5,	// 水平转动
	Ptz_Tilt,				// 垂直转动
	Ptz_Zoom,
	Ptz_Focus,
	Ptz_Iris,
	Ptz_Brightness,

	Ptz_AutoFocus	=11,
	Ptz_AutoIris,
	Ptz_ContinuousPanTiltMove,
	Ptz_ContinuousFocusMove,
	Ptz_ContinuousZoomMove,
	Ptz_ContinuousIrisMove,
	Ptz_ContinuousBrightnessMove,
	Ptz_GogoServerPresetNo,
	Ptz_Speed,
	Ptz_IrcutFilter,
	Ptz_BackLight,
};

enum Ptz_MoveOption
{
	Mov_Home,
	Mov_Up,
	Mov_Down,
	Mov_Left,
	Mov_Right,
	Mov_Upleft,
	Mov_UPright,
	Mov_Downleft,
	Mov_Downright,
	Mov_Stop
};

enum PtzOption
{
	Opt_Abstract = 1,
	Opt_Relative = 0
};



struct SimpleStream 
{
private:
	explicit SimpleStream()
	{
	}
public:
	SimpleStream(HWND hParent,string strDevice,Position nPos)
	{
		::GetClientRect(hParent,&rtBorder);
		switch(nPos)
		{
		case Pos_Left:
			this->rtBorder.right = (rtBorder.right + rtBorder.left)/2;
			break;
		case Pos_Top:
			this->rtBorder.bottom = (rtBorder.top + rtBorder.bottom)/2;
			break;
		case Pos_Right:
			this->rtBorder.left = (rtBorder.right + rtBorder.left)/2;
			break;
		case Pos_Bottom:
			this->rtBorder.top = (rtBorder.top + rtBorder.bottom)/2;
			break;
		}
		pSimpleWnd = new CSimpleWnd(&this->rtBorder,hParent);
		if (pSimpleWnd)
			pSimpleWnd->ShowWindow();
		this->strDevice = strDevice;
	}
	~SimpleStream()
	{
		if (pSimpleWnd)
			delete pSimpleWnd;
	}
	HWND GetSimpleWnd()
	{
		return pSimpleWnd == NULL?NULL:pSimpleWnd->GetSafeHwnd();
	}
	string		strDevice;
	CSimpleWnd	*pSimpleWnd;
	HWND		hParent;
	IPC_PLAYHANDLE	hPlayer;
	RECT		rtBorder;
	Position	nPostion;
};

typedef shared_ptr<SimpleStream> SimpleStreamPtr;

struct CameraUrl
{
	CString strURL;
	CString strAccount;
	CString strPassword;
	WORD    nPort;
};
typedef shared_ptr<CameraUrl> CameraUrlPtr;

enum Orientation
{
	LeftTop = 0,
	RightTop,
	LeftBottom,
	RightBottom,
};

struct PlayBackStatus
{
	long	nSeekFrame;		// 是否支持单步播放
	time_t  tStartTime;
	time_t  tStopTime;
	time_t	tFrameTimeStamp;

	DhStreamParser* pStreamParser;
	PlayBackStatus()
	{
		ZeroMemory(this, sizeof(PlayBackStatus));
		pStreamParser = new DhStreamParser();
	}
	~PlayBackStatus()
	{
		delete pStreamParser;
		pStreamParser = nullptr;
	}
};

typedef shared_ptr<PlayBackStatus> PlayBackStatusPtr;
struct OperationAssist
{
	Orientation nOrientation;
	int			nStartX;
	int			nStartY;
	int			nWidth;
	int			nHeight;
	int			nThickVerical;
	int			nThickHorizontal;
	DWORD		nColor;
	POINT		ptArray[6];
	WORD		nVertexIndex[6];
	int			nCount;
	long		hPolygon;
	OperationAssist()
	{
		ZeroMemory(this, sizeof(OperationAssist));
	}
	void BuildPolygon()
	{
		nCount = 6;
		hPolygon = 0;
		switch (nOrientation)
		{
		case LeftTop:
		{
			ptArray[0].x = nStartX;
			ptArray[0].y = nStartY;

			ptArray[1].x = nStartX + nWidth;
			ptArray[1].y = nStartY;

			ptArray[2].x = ptArray[1].x - nThickVerical;
			ptArray[2].y = ptArray[1].y + nThickVerical;

			ptArray[3].x = nStartX + nThickHorizontal;
			ptArray[3].y = nStartY + nThickVerical;

			ptArray[4].x = nStartX + nThickHorizontal;
			ptArray[4].y = nStartY + nHeight - nThickHorizontal;

			ptArray[5].x = nStartX;
			ptArray[5].y = nStartY + nHeight;
			for (int i = 0; i < 6; i++)
				nVertexIndex[i] = i;
		}
		break;
		case RightTop:
		{
			ptArray[0].x = nStartX;
			ptArray[0].y = nStartY;

			ptArray[1].x = nStartX - nWidth;
			ptArray[1].y = nStartY;

			ptArray[2].x = ptArray[1].x + nThickVerical;
			ptArray[2].y = ptArray[1].y + nThickVerical;

			ptArray[3].x = nStartX - nThickHorizontal;
			ptArray[3].y = nStartY + nThickVerical;

			ptArray[4].x = ptArray[3].x;
			ptArray[4].y = nStartY + nHeight - nThickHorizontal;

			ptArray[5].x = nStartX;
			ptArray[5].y = nStartY + nHeight;
			nVertexIndex[0] = 0;
			for (int i = 1; i <6 ; i++)
				nVertexIndex[i] = 6 - i;
		}
		break;
		case RightBottom:
		{
			ptArray[0].x = nStartX;
			ptArray[0].y = nStartY;

			ptArray[1].x = nStartX - nWidth;
			ptArray[1].y = nStartY;

			ptArray[2].x = ptArray[1].x + nThickVerical;
			ptArray[2].y = ptArray[1].y - nThickVerical;

			ptArray[3].x = nStartX - nThickHorizontal;
			ptArray[3].y = nStartY - nThickVerical;

			ptArray[4].x = ptArray[3].x;
			ptArray[4].y = nStartY - nHeight + nThickVerical;

			ptArray[5].x = nStartX;
			ptArray[5].y = nStartY - nHeight;
			for (int i = 0; i < 6; i++)
				nVertexIndex[i] = i;
		}
		break;
		case LeftBottom:
		{
			ptArray[0].x = nStartX;
			ptArray[0].y = nStartY;

			ptArray[1].x = nStartX + nWidth;
			ptArray[1].y = nStartY;

			ptArray[2].x = ptArray[1].x - nThickVerical;
			ptArray[2].y = ptArray[1].y - nThickVerical;

			ptArray[3].x = nStartX + nThickHorizontal;
			ptArray[3].y = nStartY - nThickVerical;

			ptArray[4].x = ptArray[3].x;
			ptArray[4].y = nStartY - nHeight + nThickHorizontal;

			ptArray[5].x = nStartX;
			ptArray[5].y = nStartY - nHeight;
			nVertexIndex[0] = 0;
			for (int i = 1; i < 6; i++)
				nVertexIndex[i] = 6 - i;
		}
		break;
		default:
			break;
		}
	}
};
typedef shared_ptr<OperationAssist> OperationAssistPtr;
class CAVPlayerCtrl;


// struct ComboStream
// {
// 	ComboStream(HWND hParent,SimpleStream *pStream)
// 	{
// 
// 	}
// 	SimpleStream	*pStreamArray[16];
// 	int nCount;
// };
// 
// typedef shared_ptr<ComboStream> ComboStreamPtr;

// CAVPlayerCtrl : See AVPlayerCtrl.cpp for implementation.
// 解发数据接收超时消息
#define WM_FIRE_RECVTIMEOUT		WM_USER + 1024

struct PlayPanel
{
	PlayPanel(CString strCamera, long hWndInput)
	{
		strCameraID = strCamera;
		hWnd = hWndInput;
	}
	CString strCameraID;
	long	hWnd;
};

typedef shared_ptr<PlayPanel> PlayPanelPtr;

struct DisplayMode
{
	UINT	nModeID;
	vector<PlayPanelPtr> vecPanel;
	DisplayMode(UINT nModeID)
	{
		this->nModeID = nModeID;
	}
};

typedef shared_ptr<DisplayMode> DisplayModePtr;

struct Crane
{
	map<long, DisplayModePtr> ScreenMode;
};
typedef shared_ptr<Crane> CranePtr;

typedef vector<OperationAssistPtr> OperationAssistArray;

struct  CItemStatus;
typedef shared_ptr<CItemStatus> ItemStatusPtr;
struct  CItemStatus
{
	bool bItemStatus;
	void* pItemValue;
private:
	CItemStatus()
	{
		ZeroMemory(this, sizeof(CItemStatus));
	}
public:
	CItemStatus(void* hPlayHandle)
	{
		this->pItemValue = hPlayHandle;
		bItemStatus = true;
	}
};

class CFreeFinder
{
public:
	CFreeFinder()
	{
	}
	bool operator()(ItemStatusPtr & InputItem)
	{
		return !InputItem->bItemStatus;
	}
};

class CItemFinder
{
public:
	CItemFinder(void* hInputHandle)
		:pItemValue(hInputHandle)
	{
	}
	bool operator()(ItemStatusPtr & InputItem)
	{
		return (pItemValue == InputItem->pItemValue);
	}
	void* pItemValue;
};

typedef list<ItemStatusPtr>ItemStatusList;

struct _IPCConnection;
typedef shared_ptr<_IPCConnection> IPCConnectionPtr;
class CAVPlayerCtrl : public COleControl
{
	DECLARE_DYNCREATE(CAVPlayerCtrl)

// Constructor
public:
	CAVPlayerCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	map<CString,CameraUrlPtr> m_mapCameraUrl;
	CRITICAL_SECTION m_csOperationAssist;
	map<CString, OperationAssistArray> m_mapOperationAssist;
	INT		m_nLogSaveDays = 30;
	INT		m_nYUVFrameCacheSize = 50;
	bool	m_bEnableCameraPostion = true;
	bool	m_bEnableAS300 = true;
	
	void LogManager()
	{
		CFileFind finder;
		TCHAR szLogPath[1024] = {0};
		GetAppPath(szLogPath,1024);
		_tcscat(szLogPath,_T("\\log\\*.log"));
		BOOL bWorking = finder.FindFile(szLogPath);
		CTime tNow = CTime::GetCurrentTime();
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			CTime tFile;
			finder.GetCreationTime(tFile);
			CTimeSpan ts = tNow - tFile;
			if (ts.GetDays() < m_nLogSaveDays)
				continue;
			CString strFile = finder.GetFilePath();
			DeleteFile(strFile);
		} 
	}
// Implementation
protected:
	~CAVPlayerCtrl();

	DECLARE_OLECREATE_EX(CAVPlayerCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CAVPlayerCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CAVPlayerCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CAVPlayerCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
#if 0
		dispidPlayBackPreview = 31L,
#endif
		dispidQueryRecord = 30L,
		dispidAdjustPanels = 29L,
		dispidCreateFrameWnd = 28L,
		dispidSeekTime = 27L,
		dispidStopPlayBack = 26L,
		dispidPlayBack = 25L,
		dispidSwitchScreen = 24L,
		dispidConfigureScreenMode = 23L,
		dispidLoadOpAssistConfigure = 21L,
		dispidEnableOperationAssist = 20L,
		dispidPlaySrvStream = 19L,
		dispidSetExternDCDraw = 18L,
		dispidEnalbeCameraPostion = 17L,		
		dispidSendPtzCommand = 15L,
		dispidSetPlayComboBorderEx = 14L,
		dispidSetPlayComboBorder = 13L,
		dispidPlayComboStream = 12L,
		dispidGetWindowDevice = 11L,
		dispidGetDeviceWindow = 10L,		
		dispidFreeString = 9L,
		dispidGetErrorMessage = 8L,
		dispidReportInterval = 7,
		dispidRecvTimeout = 6,
		eventidRecvTimeout = 1L,
		dispidGetActiveTime = 6L,
		dispidStopPlay = 5L,
		dispidGetDeviceID = 4L,
		dispidPlayStream = 3L,
		dispidLogout = 2L,
		dispidLogin = 1L
	};
private:
	CRITICAL_SECTION	m_csDBConnector;
	shared_ptr<CMySQLConnector> m_pDBConnector;
	
	// 相机连接集
	CRITICAL_SECTION m_csMapConnection;
	map<string, IPCConnectionPtr> m_MapConnection;
	CRITICAL_SECTION m_csMapSession;
	map<long, IPCConnectionPtr> m_MapSession;
	map<string,SimpleStreamPtr> m_mapComboStream;
	bool LoadConfigure();
protected:
	CString m_strAccount;
	CString m_strServerIP;
	// 启用摄像机位置信息表，默认关闭
	bool	m_bEnalbeCameraPostion;
	LONG Login(LPCTSTR strServerIP, USHORT nServerPort, LPCTSTR strAccount, LPCTSTR strPassword, LONG nUserPtr);
	void Logout(void);
	LONG PlayStream(LPCTSTR strDeviceID, LONG hWnd,LONG nEnalbeHWAccel);
	LONG GetDeviceID(BSTR* strDeviceList, LONG* nDeviceCount);
	void StopPlay(LPCTSTR strDeviceID,LONG hWnd);
	void PausePlay(LPCTSTR strDeviceID, LONG hWnd);
	
// 	typedef struct SubClassInfo
// 	{
// 		HWND hPartnerWnd;
// 		WNDPROC	pOldProcWnd;
// 	};
// 	typedef shared_ptr<SubClassInfo> SubClassInfoPtr;
// 	static CRITICAL_SECTION m_csMapSubclassWnd;
// 	static map<HWND, SubClassInfoPtr> m_MapSubclassWnd;
// 	static LRESULT SubClassProc(HWND, UINT, WPARAM, LPARAM);
	shared_ptr<CRunlog> m_pRunlog;
	//HANDLE	m_hThreadCheckRecvTime;
	volatile bool m_bThreadCheckRecvTimeRun;
	long  m_nRecvTimeout;			// 接收数据超时值,默认值为5000毫秒
	long  m_nReConnectInterval;		// 重连间隔时间,默认值为5000毫秒
	static  UINT _stdcall ThreadCheckRecvTime(void *p)
	{
		CAVPlayerCtrl *pThis = (CAVPlayerCtrl *)p;
		return pThis->ThreadCheckRecvTimeRun();
	}

	UINT ThreadCheckRecvTimeRun();
	
	LRESULT OnFireRecvTimeout(WPARAM w,LPARAM l);

	void RecvTimeout(LPCTSTR strDevice,LONG hWnd)
	{
		//FireEvent(eventidRecvTimeout, EVENT_PARAM(VTS_I4 VTS_BSTR), hWnd, strDevice);
		FireEvent(eventidRecvTimeout, EVENT_PARAM(VTS_BSTR VTS_I4), strDevice, hWnd);
	}

	LONG GetRecvTimeout(void);
	void SetRecvTimeout(LONG newVal);
	LONG GetReportInterval(void);
	void SetReportInterval(LONG newVal);
	void FreeString(BSTR* strString);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:

	// 获取正在播放设备的窗口列表
	// strDevice	设备ID
	// hWndArray	句柄数组
	// nArraySize	hWndArray数组能容纳窗口的数量，并返回strDeviceID实际播放的窗口数量
	// 若设备strDevice正在播放，则返回正在播放该设备的窗口数量，否则返回0
	LONG GetDeviceWindow(LPCTSTR strDeviceID, LONG* hWndArray, LONG* nArraySize);
	// 获取在指定窗口上的播放的设备ID，
	// 若该窗口尚未播放任何设备，则返回0，否则返回AvError_Succeed，并且strDeviceID带回该窗口正在播放的设备ID
	
	LONG GetWindowDevice(LONG hWnd, BSTR* strDeviceID);
	LONG PlayComboStream(LPCTSTR strDevice1, LPCTSTR strDevice2, LONG hWnd, LONG nEnableHWAccel,LONG nArrangeMode);
	LONG SetPlayComboBorder(DOUBLE dwSpreadHei);

public:
	 
	map<string, ListPos> m_mapDevPos;
	//map<string,string> m_mapDeviceID;

protected:
	LONG SetPlayComboBorderEx(LPCTSTR strDevice1, LPCTSTR strDevice2, DOUBLE fSpreadHei);
	// 执行PTZ命令
	// strDeviceID	设备ID
	// nPtzCommand	命令代码
	// nPtzValue	与命令代码对应的命令值，如转动角度，变焦值等
	// nOpType		操作类型，0为相对动作，1为绝对动作，默认为相对动作
	LONG SendPtzCommand(LPCTSTR strDeviceID, LONG nPtzCommand,LONG nPtzValue1,LONG nPtzValue2,LONG nOpType);
	
	LONG EnalbeCameraPostion(LONG bEnalbeFlag);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

protected:
	LONG SetExternDCDraw(LPCTSTR szDeviceID, LONG pDCDraw, LONG pUserPtr);
public:
	// AS300 Client SDK
	static void CALLBACK AS300LiveCallBack(int nSessionId, char* buf, int len, void* user)
	{
		if (!nSessionId)
			return;
		CAVPlayerCtrl *pThis = (CAVPlayerCtrl *)user;
		if (nSessionId == -1 )
		{
			if (pThis->m_pRunlog)
				pThis->m_pRunlog->Runlog(_T("%s videotype: %d, url: %s"), __FUNCTIONW__, len, _UnicodeString(buf, CP_ACP));
		}
		else
		{
			pThis->OnAS300LiveData(nSessionId, buf, len);
		}
	}

	/// 这是一个超大的坑
	/// 返回值类型必须定义为BOOL型，若使用说明文档或函数原因定义的使用bool类型，则无法控制播放速度，操蛋！！！！
	static BOOL CALLBACK AS300PlayBackCallBack(int nSessionId, char* buf, int len, void* user)
	{
		if (!nSessionId)
			return FALSE;
		CAVPlayerCtrl *pThis = (CAVPlayerCtrl *)user;
		if (nSessionId == -1)
			return FALSE;
		return pThis->OnAS300PlayBack(nSessionId, buf, len);
	}
	static void CALLBACK DevStatusCallBack(long login_id, char* szDevId, int nStatus, void* user)
	{
		CAVPlayerCtrl *pThis = (CAVPlayerCtrl *)user;
		pThis->OnDevStatus(szDevId, nStatus);
	}
	static void CALLBACK RespondCallBack(uint32 nSequence, long res, VSParamInfo *paramInfo, void* pUser)//响应结果回调
	{
		
	}

	static void CALLBACK EventInfoCallback(long login_id, long nEventType, char* szId, int nParam1, int nParam2, void* user)
	{
		if (user)
		{
			CAVPlayerCtrl *pThis = (CAVPlayerCtrl *)user;
			pThis->OnAS300Event(nEventType, szId, nParam1, nParam2);
		}
	}

	//static int CALLBACK RecordQueryCallBack(int nQueryId, int nCount, void* pUser, long res);//响应结果回调

	void   OnRespondCallback(int nSessionId);	
	void   OnDevStatus(char* szDevId, int nStatus);
	void   OnAS300LiveData(LONG nSessionId, char* buf, int nLen);
	BOOL   OnAS300PlayBack(LONG nSessionId, char* buf, int nLen);

	//////////////////////////////////////////////////////////////////////////
	// OnAS300PlayBackPos
	// 参数说明：
	// nSessionId 		回放 / 下载会话ID,
	// pos		如果是按文件回放该值为已播放文件大小单位kb
	// 			如果是下载该值为最新frame的timestamp，如果是 - 1表示下载完成
	// total	如果是按文件回放该值为播放文件总大小单位kb
	//			如果是下载该值为 - 1
	// pUser		用户数据
	//////////////////////////////////////////////////////////////////////////
	void   OnAS300PlayBackPos(int nSessionID, int nPos, int nTotal);
	void   OnAS300Event(long nEventType, char* szId, int nParam1, int nParam2);
	static void CALLBACK PlayBackPosCallBack(int nSessionID, int nPos, int nTotal, void *pUserData)
	{
		if (!nSessionID)
			return;
		CAVPlayerCtrl *pThis = (CAVPlayerCtrl *)pUserData;
		if (nSessionID == -1)
			return;
			pThis->OnAS300PlayBackPos(nSessionID, nPos, nTotal);
	}

	long	m_nLoginID = -1;

	long   _QueryDeviceRecord(LPCTSTR strDeviceID,long nChannel, long nStartTime, long nStopTime, PVSRecord_Info_t  lpRecordArray,long &nRecordCount,long &nBufferSize)
	{
		if (!strDeviceID || !nStartTime || !nStopTime)
			return AvError_InvalidParameters;
		if (nStartTime <= nStopTime)
			return AvError_InvalidParameters;
		char szDeviceID[32] = { 0 };

		sprintf_s((char *)szDeviceID, 32, "%s%02d", _AnsiString(strDeviceID, CP_ACP), nChannel);
		VSRecord_Info_t RecordArray[1024] = {0};
		VSQuery_Record_Info_t QueryInfo;
		QueryInfo.nChannelNo	 = 1;
		QueryInfo.nSource		 = Record_Source_SS;
		QueryInfo.nRecordType	 = Record_all;
		QueryInfo.nStartTime	 = nStartTime;
		QueryInfo.nEndTime		 = nStopTime;
		strcpy_s((char *)QueryInfo.szCameraId,MAX_ID_LENGTH, (char *)szDeviceID);
		nRecordCount = 0;
		long nResult = SDK_CUQueryRecord(m_nLoginID, QueryInfo, (PVSRecord_Info_t)lpRecordArray, (int &)nRecordCount, nBufferSize, 15000);
		if (nRecordCount > 0)
		{
			return AvError_Succeed;
		}
		else
			return (AvError_AS300_Error + nResult);
	}

	LONG   PlaySrvStream(LPCTSTR strDeviceID, LONG hWnd, LONG nEnableHWAccel);
	
public:
	afx_msg void OnDestroy();

	LONG EnableOperationAssist(LPCTSTR strDevice, LONG nEnable);
	LONG LoadOpAssistConfigure();
	bool LoadOPAssistXConfigure();
	map<long, CranePtr> m_mapCrane;
	bool LoadScreenMode();
	LONG ConfigureScreenMode();

	LONG SwitchScreen(LONG nCraneID , LONG nScreenMode,LONG hWnd);
	long	m_nCurrentCrane = -1;
	LONG	m_nCurrentMode = -1;
	int		m_nSwitchFrame = 0;

	//CVideoFrame *m_pVideoFrame =  nullptr ;
	map<LONG, CVideoFrame *> m_mapVideoFrame;
	int m_nLastPanel = -1;
	RECT m_rtClip;
// 	LRESULT OnFrameLButtonDown(WPARAM W, LPARAM L)
// 	{
// 		RECT rtNewClicp;
// 		m_nLastPanel = m_pVideoFrame->GetCurPanel();
// 		m_pVideoFrame->SetCapture();
// 		m_pVideoFrame->GetWindowRect(&rtNewClicp);
// 		GetClipCursor(&m_rtClip);
// 		ClipCursor(&rtNewClicp);
// 
// 		return 0;
// 	}

// 	LRESULT OnFrameLButtonUp(WPARAM W, LPARAM lParam)
// 	{
// 		int nCurPanel = m_pVideoFrame->GetCurPanel();
// 		if (nCurPanel != -1 &&
// 			m_nLastPanel != -1 &&
// 			nCurPanel != m_nLastPanel)
// 		{
// 			HWND hCurWnd = m_pVideoFrame->GetPanelWnd(nCurPanel);
// 			m_pVideoFrame->SwapPanel(nCurPanel, m_nLastPanel);
// 			::InvalidateRect(hCurWnd, nullptr, TRUE);
// 			m_pVideoFrame->RefreshSelectedPanel();
// 		}
// 		TraceMsgA("%s\tnCurPanel = %d.\n", __FUNCTION__, nCurPanel);
// 		ClipCursor(&m_rtClip);
// 		ReleaseCapture();
// 		return 0;
// 	}

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	LONG PlayBack(LONG hWnd, LPCTSTR strDeviceID,LONG nStartTime, LONG nStopTime,LONG nSeekFrame, LONG nTimeout);
	void StopPlayBack(LPCTSTR strDeviceID);
	// 移动到指定位置后，重新播放
	LONG SeekTime(LPCTSTR strDeviceID, LONGLONG nTime);
protected:
	LONG CreateFrameWnd(LONG hWnd,LONG nWndCount, LONG nFrameStyle, LONG* pFrameHandle);
	LONG AdjustPanels(LONG nWndCount, LONG nFrameStyle);
	LONG QueryRecord(LPCTSTR szDeviceID, LONG nStartTime, LONG nStopTime, LONG pRecordArray,LONG nBufferCount, LONG* nRecordCount);
	LONG GetErrorMessage(LONG nErrorCode, LPCTSTR strErrorMessage, LONG nBufferSize);
#if 0
	LONG PlayBackPreview(LONG hWnd, LPCTSTR strDeviceID, LONG nStartTime, LONG nStopTime, LONG nCacheFPS, LONG nCacheTime);
#endif
public:
	static CCriticalSectionAgent m_csMapDecoderPool;
	static map<string, ItemStatusList> m_mapDecoderPool;

	
};

struct FrameBuffer
{
	byte *pBuffer;
	int	 nLength;
	FrameBuffer(byte *pData, int nLen)
	{
		pBuffer = new byte[nLen];
		memcpy(pBuffer, pData, nLen);
		nLength = nLen;
	}
	~FrameBuffer()
	{
		if (pBuffer)
		{
			delete[]pBuffer;
			pBuffer = nullptr;
			nLength = 0;
		}
	}
};
typedef shared_ptr<FrameBuffer> FrameBufferPtr;
struct _IPCConnection
{
	IPC_PLAYHANDLE hPlayhandle;
	HWND	m_hWnd;
	byte	*m_pFrameBuffer;
	int		m_nBufferSize;
	int		m_nFrameLength;
	int		m_nWidth;
	int		m_nHeight;
	byte	m_nFPS;
	bool	m_bFillHeader;
	bool	m_bEnableHWAccel;
	long	m_hRtspSession;
	double	dfLastActiveTime;
	double  dfReConnectTime;
	char	szIP[32];
	char	szAccount[32];
	char	szPassword[32];
	string  strRtspURL;
	BSTR	bstrErrorString;
	void	*m_pRtspCallBack;
	SimpleStream* m_pSimpleStream;
	bool	bRunning;
	long	nRecvTimeout;
	long	nReConnectInterval;
	shared_ptr<CRunlog> pRunlog;
	HANDLE	hThread;
	// AS300转发变量
	bool	m_bIPCStart;
	
	MMRESULT hInputTimer = 0;
	int		nPlayBackFps = 25;
	time_t	nLastPlayBackFrameTime = 0;
	int		nElapsFrames = 0;
	DWORD	nTimeStart = 0;
	
	long	m_nPlaySession;		// 回放和转发播放的session
	bool	m_bPlayBackPrewiew;	// 回放预览，可单帧查看
	long	m_nLoginID;
	CHAR	m_strDeviceID[64];
	PlayBackStatusPtr	pPlayStatus;
	CCriticalSectionAgent csListFrame;
	shared_ptr<CStat> pStat = nullptr;
	double	dfLastStatTime = 0;
	
	list<SimpleStream*> listSimpleStream;
	list<FrameBufferPtr> listFrame;
	shared_ptr<VSRecord_Info_t> m_pRecordInfo = nullptr;
	
#ifdef _DEBUG
	static volatile LONG  nRefCount;
#endif
	//map<long,long>mapWnd;
	_IPCConnection()
	{
		ZeroMemory(this, offsetof(_IPCConnection, pPlayStatus));
		int nSize = sizeof(_IPCConnection);
		m_pFrameBuffer = new byte[128 * 1024];
		m_nBufferSize = 128 * 1024;
		nRecvTimeout = 15000;
		nReConnectInterval = 15000;
		dfReConnectTime = 0.0f;
		m_nLoginID = -1;
		nPlayBackFps = 25;
#ifdef _DEBUG
		InterlockedIncrement(&nRefCount);
#endif

	}

	int SetExternDCDraw(void *pCallBack, void *pUserPtr)
	{
		if (hPlayhandle)
		{
			return  ipcplay_SetExternDrawCallBack(hPlayhandle, pCallBack, pUserPtr);
		}
		else
			return AvError_DeviceNotInPlaying;

	}
	_IPCConnection(HWND hParent, string strDevice, Position nPos)
	{
		ZeroMemory(this, offsetof(_IPCConnection, pPlayStatus));
		m_pFrameBuffer = new byte[128 * 1024];
		m_nBufferSize = 128 * 1024;
		dfReConnectTime = 0.0f;
		nRecvTimeout = 15000;
		nReConnectInterval = 15000;
		m_pSimpleStream = new SimpleStream((HWND)hParent, strDevice, nPos);
		if (m_pSimpleStream)
			m_hWnd = m_pSimpleStream->GetSimpleWnd();
#ifdef _DEBUG
		InterlockedIncrement(&nRefCount);
#endif
	}
	void AddSimpleStream(SimpleStream* pSimleStream)
	{
		list<SimpleStream*>::iterator itFind = find(listSimpleStream.begin(), listSimpleStream.end(), pSimleStream);
		if (itFind != listSimpleStream.end())
			listSimpleStream.push_back(pSimleStream);
	}
	void RemoveSimpleStream(SimpleStream *pSimleStream)
	{
		list<SimpleStream *>::iterator itFind = find(listSimpleStream.begin(), listSimpleStream.end(), pSimleStream);
		if (itFind != listSimpleStream.end())
		{
			listSimpleStream.erase(itFind);
			delete pSimleStream;
		}
	}

	void ClearRenderWndow(IPC_PLAYHANDLE hPlayHandle)
	{
		if (hPlayHandle)
		{
			HWND hWndArray[16] = { 0 };
			int nArraySize = 16;
			if (ipcplay_GetRenderWindows(hPlayHandle, hWndArray, nArraySize) == IPC_Succeed)
			{
				if (nArraySize)
				{
					TraceMsgA("%s nArraySize = %d.\n", __FUNCTION__, nArraySize);
					for (int i = 0; i < nArraySize; i++)
						ipcplay_RemoveWindow(hPlayHandle, hWndArray[i]);
				}
			}
		}
	}
#define _RTP_Header		0	
#define _NALU			1
#define _FrameData		2
#define _FPSOffset		5
#define _WidthOffset	6
#define _HeightOffset	7
	void OnRtspCallBack(char *pBuffer, int nParam)
	{
		_RTSPParam *pRTSPParam = (_RTSPParam *)nParam;
		int nBufferLen = pRTSPParam->nLength;
		int nNalType = pRTSPParam->nNalType;
		dfLastActiveTime = GetExactTime();
		switch (pRTSPParam->nDataType)
		{
		case _RTP_Header:
		{
			if (!m_bFillHeader)
			{
				TraceMsgA("%s IP = %s.\n", __FUNCTION__, szIP);
				m_nWidth = (byte)pBuffer[_WidthOffset] * 8;
				m_nHeight = (byte)pBuffer[_HeightOffset] * 8;
				m_nFPS = pBuffer[_FPSOffset];
				if (m_nWidth >= 8 && m_nHeight > 8)
				{
					IPC_MEDIAINFO MediaHeader;
					MediaHeader.nVideoCodec = CODEC_H264;
					MediaHeader.nAudioCodec = CODEC_G711A;
					MediaHeader.nVideoWidth = m_nWidth;
					MediaHeader.nVideoHeight = m_nHeight;
					MediaHeader.nFps = m_nFPS;

					if (!hPlayhandle)
					{
						char szResolution[16] = { 0 };
						sprintf_s(szResolution, 16, "%d*%d", m_nWidth, m_nHeight);
						string strResolutoin = szResolution;
						EnterCriticalSection(CAVPlayerCtrl::m_csMapDecoderPool.Get());
						map<string, ItemStatusList>::iterator itFind = CAVPlayerCtrl::m_mapDecoderPool.find(strResolutoin);
						if (itFind != CAVPlayerCtrl::m_mapDecoderPool.end())
						{
							ItemStatusList &HandleList = itFind->second;
							ItemStatusList::iterator itFind2 = find_if(HandleList.begin(), HandleList.end(), CFreeFinder());
							if (itFind2 != HandleList.end())
							{
								TraceMsgA("%s Matched a ipcplay Handle for %s(%s)", __FUNCTION__, szIP, szResolution);
								hPlayhandle = (*itFind2)->pItemValue;
								ClearRenderWndow(hPlayhandle);
								ipcplay_AddWindow((*itFind2)->pItemValue, m_hWnd);
								(*itFind2)->bItemStatus = true;
							}
							else
							{
								IPC_PLAYHANDLE hTempHandle = ipcplay_OpenStream(m_hWnd, NULL, 0);
								TraceMsgA("%s Create New ipcplay Handle for %s(%s)", __FUNCTION__, szIP, szResolution);
								ipcplay_SetStreamHeader(hTempHandle, (byte *)&MediaHeader, sizeof(IPC_MEDIAINFO));
								ipcplay_SetMaxFrameSize(hTempHandle, 1024 * 1024);
								ipcplay_SetD3dShared(hTempHandle, m_bEnableHWAccel);
								ipcplay_Start(hTempHandle, false, true, m_bEnableHWAccel);
								ipcplay_EnableStreamParser(hTempHandle, CODEC_H264);
								HandleList.push_back(shared_ptr<CItemStatus>(new CItemStatus(hTempHandle)));
								hPlayhandle = hTempHandle;

							}
						}
						else
						{
							IPC_PLAYHANDLE hTempHandle = ipcplay_OpenStream(m_hWnd, NULL, 0);
							TraceMsgA("%s Create New ipcplay Handle for %s(%s)", __FUNCTION__, szIP, szResolution);
							ipcplay_SetStreamHeader(hTempHandle, (byte *)&MediaHeader, sizeof(IPC_MEDIAINFO));
							ipcplay_SetMaxFrameSize(hTempHandle, 1024 * 1024);
							ipcplay_SetD3dShared(hTempHandle, m_bEnableHWAccel);
							ipcplay_Start(hTempHandle, false, true, m_bEnableHWAccel);
							ipcplay_EnableStreamParser(hTempHandle, CODEC_H264);
							ItemStatusList HandleList;
							HandleList.push_back(shared_ptr<CItemStatus>(new CItemStatus(hTempHandle)));
							CAVPlayerCtrl::m_mapDecoderPool.insert(pair<string, list<ItemStatusPtr>>(strResolutoin, HandleList));
							hPlayhandle = hTempHandle;
						}
						LeaveCriticalSection(CAVPlayerCtrl::m_csMapDecoderPool.Get());
					}
					

					m_bFillHeader = true;
					m_nFrameLength = 0;
					memcpy(m_pFrameBuffer, pBuffer, nBufferLen);
					m_nFrameLength = nBufferLen;
					TraceMsgA("%s\tRecv a RTP Header.\n", __FUNCTIONW__);
				}
			}
			else
			{
				memcpy(m_pFrameBuffer, pBuffer, nBufferLen);
				m_nFrameLength = nBufferLen;
			}
		}
		break;
		case _NALU:
		{
			if (m_bFillHeader)
			{
				memcpy(&m_pFrameBuffer[m_nFrameLength], pBuffer, nBufferLen);
				m_nFrameLength += nBufferLen;
			}
		}
		break;
		case _FrameData:
		{
			if (!m_bFillHeader)
				break;
			if (m_nBufferSize < m_nFrameLength + nBufferLen)
			{
				int nNewBufferSize = m_nBufferSize;
				while (nNewBufferSize < (m_nFrameLength + nBufferLen))
					nNewBufferSize *= 2;
				byte *pTemp = new byte[nNewBufferSize];
				if (pTemp == NULL)
					return;
				memcpy(pTemp, m_pFrameBuffer, m_nFrameLength);
				delete[]m_pFrameBuffer;
				m_pFrameBuffer = pTemp;
				m_nBufferSize = nNewBufferSize;
			}

			memcpy(&m_pFrameBuffer[m_nFrameLength], pBuffer, nBufferLen);
			m_nFrameLength += nBufferLen;
			int nFrameType = IPC_P_FRAME;
			if (nNalType == 7 ||
				nNalType == 5)
				nFrameType = IPC_I_FRAME;

			//ipcplay_InputIPCStream(hPlayhandle, m_pFrameBuffer, nFrameType, m_nFrameLength, 0);
			ipcplay_InputStream2(hPlayhandle, m_pFrameBuffer, m_nFrameLength);
			m_nFrameLength = 0;

		}
		break;
		default:
			break;
		}
	}
	// 10 ms
	static void CALLBACK InputTimer(UINT  uTimerID,
		UINT      uMsgReserved,
		DWORD_PTR dwUser,
		DWORD_PTR dwReserved1,
		DWORD_PTR dwReserved2)
	{
		_IPCConnection *pThis = (_IPCConnection *)dwUser;
		if (!pThis->m_bIPCStart || !pThis->hPlayhandle)
			return;
		//if (!pThis->pStat)
		//{
		//	CHAR szTitle[256] = { 0 };
		//	sprintf_s(szTitle, 256, "InputTimer Interval");
		//	pThis->pStat = make_shared<CStat>(szTitle, 0,30*25);
		//	//_tprintf(_T("Start Stat:%s.\n"),pThis->szIP);
		//	pThis->dfLastStatTime = GetPerfTime();
		//}
		//else
		//{
		//	pThis->pStat->Stat(PerfTimeSpan(pThis->dfLastStatTime));
		//	pThis->dfLastStatTime = GetPerfTime();
		//	if (pThis->pStat->IsFull())
		//	{
		//		pThis->pStat->OutputStat();
		//		pThis->pStat->Reset();
		//	}
		//}

		AutoLockAgent(pThis->csListFrame);
		if (pThis->listFrame.size())
		{
			FrameBufferPtr pFrame = pThis->listFrame.front();
			pThis->listFrame.pop_front();
			ipcplay_InputStream2(pThis->hPlayhandle, pFrame->pBuffer, pFrame->nLength);
		}
	}
	time_t GetFrameUTC(DH_FRAME_INFO *pFrame)
	{
		if (!pFrame)
			return 0;
		SYSTEMTIME systime;
		systime.wYear = pFrame->nYear;
		systime.wMonth = pFrame->nMonth;
		systime.wDay = pFrame->nDay;
		systime.wHour = pFrame->nHour;
		systime.wMinute = pFrame->nMinute;
		systime.wSecond = pFrame->nSecond;
		time_t tUTC = 0;
		SystemTime2UTC(&systime, (UINT64 *)&tUTC);
		return tUTC;
	}
	// 
	BOOL InputStream(int nSessionId, byte *pData, int nLength)
	{
		if (nLength)
		{
			LineLockAgent(csListFrame);
			if (listFrame.size() >= 25)
				return FALSE;
		}
		
		DhStreamParser* pDHStreamParser = pPlayStatus->pStreamParser;
		pDHStreamParser->InputData(pData, nLength);
		while (true)
		{
			DH_FRAME_INFO *pFrame = pDHStreamParser->GetNextFrame();
			if (!pFrame)
				break;

			if (pFrame->nType == DH_FRAME_TYPE_VIDEO)
			{
				if (pFrame->nYear)
				{
					time_t nNewFrameTime = GetFrameUTC(pFrame);
					if (nLastPlayBackFrameTime && nLastPlayBackFrameTime > nNewFrameTime)
					{
						nPlayBackFps = nElapsFrames / (nNewFrameTime - nLastPlayBackFrameTime) ;
						nElapsFrames = 0;
					}
					nLastPlayBackFrameTime = nNewFrameTime;
					nTimeStart = timeGetTime();
					nElapsFrames++;
				}
				try
				{
					if (!m_bIPCStart)
					{
						IPC_MEDIAINFO MediaHeader;
						MediaHeader.nVideoCodec = CODEC_H264;
						MediaHeader.nAudioCodec = CODEC_UNKNOWN;
						MediaHeader.nVideoWidth = pFrame->nWidth;
						MediaHeader.nVideoHeight = pFrame->nHeight;
						MediaHeader.nFps = pFrame->nFrameRate;
						ipcplay_SetStreamHeader(hPlayhandle, (byte *)&MediaHeader, sizeof(IPC_MEDIAINFO));
						ipcplay_EnableStreamParser(hPlayhandle, CODEC_H264);
						ipcplay_Start(hPlayhandle);
						int nFrameRate = pFrame->nFrameRate;
						if (!nFrameRate)
							nFrameRate = 25;

						m_bIPCStart = true;
						//if (!m_bPlayBackPrewiew)	// 非预览播放，则按预设帧率播放				
							hInputTimer = timeSetEvent(40, 10, InputTimer, DWORD_PTR(this), TIME_PERIODIC | TIME_KILL_SYNCHRONOUS | TIME_CALLBACK_FUNCTION);
					}
					if (pFrame->nFrameLength > 0)
					{
						//if (!m_bPlayBackPrewiew)
						{
							AutoLockAgent(csListFrame);
							listFrame.push_back(make_shared<FrameBuffer>(pFrame->pContent, pFrame->nFrameLength));
						}
						/*else
						{
							ipcplay_InputIPCStream(hPlayhandle, pFrame->pContent, pFrame->nType == DH_FRAME_TYPE_VIDEO_I_FRAME ? IPC_IDR_FRAME : IPC_P_FRAME, pFrame->nLength, 0, GetFrameUTC(pFrame));
						}*/
					}
					
				}
				catch (std::exception &e)
				{
					TraceMsgA("%s Catch a exception:%s.\n", __FUNCTION__, e.what());
					return TRUE;
				}
			}
		}
		return TRUE;

	}
	~_IPCConnection()
	{
		bRunning = false;
#ifdef _DEBUG
		InterlockedDecrement(&nRefCount);
#endif
		if (hInputTimer)
		{
			timeKillEvent(hInputTimer);
			hInputTimer = 0;
		}
		if (hThread)
		{
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
			hThread = NULL;
		}

		if (m_nPlaySession)
		{
			assert(m_nLoginID != -1);
			if (!pPlayStatus)
				SDK_CUStopVideoRequest(m_nLoginID, (CHAR *)m_strDeviceID);
			else
				SDK_CUStopPlayback(m_nLoginID, m_nPlaySession);
			m_nLoginID = -1;
			m_nPlaySession = -1;
		}

		if (m_hRtspSession)
			rtsp_stop(m_hRtspSession);
		if (hPlayhandle)
		{
			PlayerInfo info;
			ipcplay_GetPlayerInfo(hPlayhandle, &info);
			ipcplay_RemoveWindow(hPlayhandle, m_hWnd);
			ipcplay_ClearCache(hPlayhandle);
			::InvalidateRect((HWND)m_hWnd, nullptr, true);

			char szResolutation[16] = { 0 };
			
			sprintf_s(szResolutation, 16, "%d*%d", info.nVideoWidth, info.nVideoHeight);
			EnterCriticalSection(CAVPlayerCtrl::m_csMapDecoderPool.Get());
			map<string, ItemStatusList>::iterator itFind = CAVPlayerCtrl::m_mapDecoderPool.find(szResolutation);
			if (itFind != CAVPlayerCtrl::m_mapDecoderPool.end())
			{
				ItemStatusList &HandleList = itFind->second;
				ItemStatusList::iterator itFind2 = find_if(HandleList.begin(), HandleList.end(), CItemFinder(hPlayhandle));
				if (itFind2 != HandleList.end())
					(*itFind2)->bItemStatus = false;
			}
			LeaveCriticalSection(CAVPlayerCtrl::m_csMapDecoderPool.Get());
			//ipcplay_Close(hPlayhandle);
		}
			
		if (m_pFrameBuffer)
		{
			delete[]m_pFrameBuffer;
			m_pFrameBuffer = nullptr;
		}
		if (m_pSimpleStream)
		{
			delete m_pSimpleStream;
			m_pSimpleStream = nullptr;
		}

		for (list<SimpleStream*>::iterator it = listSimpleStream.begin(); it != listSimpleStream.end();)
		{
			delete (*it);
			it = listSimpleStream.erase(it);
		}

	}

	LONG OpenAS300Session(LONG nLoginID, LPCTSTR szDeviceID)
	{
		strcpy_s(m_strDeviceID, 64, _AnsiString(szDeviceID, CP_ACP));
		m_nPlaySession = SDK_CUVideoRequest(nLoginID, (char *)m_strDeviceID, 0, 1, 5000, 0, 0);
		m_nLoginID = nLoginID;

		return m_nPlaySession;
	}
	LONG RtspConnect(char *szIP, char *szAccount, char *szPassword, map<CString, CameraUrlPtr> &mapCamera, PFRtspDataCallBack pRtspCallBack)
	{
		char szURL[512] = { 0 };
		map<CString, CameraUrlPtr>::iterator itFinder = mapCamera.find(CString(szIP));
		if (itFinder == mapCamera.end())
		{
			sprintf(szURL, "rtsp://%s:%s@%s/axis-media/media.amp?camera=1&videocodec=h264", szAccount, szPassword, szIP);
		}
		else
		{
			string strUrlFmt = _AnsiString((LPCTSTR)itFinder->second->strURL, CP_ACP);
			string strUser = _AnsiString((LPCTSTR)itFinder->second->strAccount, CP_ACP);
			string strPassword = _AnsiString((LPCTSTR)itFinder->second->strPassword, CP_ACP);
			sprintf(szURL, strUrlFmt.c_str(), strUser.c_str(), strPassword.c_str(), szIP);
		}
		if (pRunlog)
			pRunlog->Runlog(_T("%s RTSP URL = %s.\n"), __FUNCTIONW__, _UnicodeString(szURL, CP_ACP));
		m_hRtspSession = rtsp_play(szURL, "", "", rtsp_TCP, 0, (PFRtspDataCallBack)pRtspCallBack, NULL, this);
		if (!m_hRtspSession)
			return AvError_ConnectDeviceFailed;
		else
		{
			strRtspURL = szURL;
			strcpy(this->szIP, szIP);
			strcpy(this->szAccount, szAccount);
			strcpy(this->szPassword, szPassword);
			m_pRtspCallBack = pRtspCallBack;
			dfLastActiveTime = GetExactTime();
			return AvError_Succeed;
		}
	}

	LONG Reconnect()
	{
		if (m_hRtspSession)
		{
			rtsp_stop(m_hRtspSession);
			m_hRtspSession = 0;
		}

		int nWndCount = 0;
		HWND hWndArray[16] = { 0 };
		int nStatus = ipcplay_GetRenderWindows(hPlayhandle, hWndArray, nWndCount);
		if (nStatus == IPC_Succeed  &&
			nWndCount > 0)
		{
			for (int i = 0; i < nWndCount; i++)
				InvalidateRect(hWndArray[i], NULL, TRUE);
		}

		m_hRtspSession = rtsp_play(strRtspURL.c_str(), "", "", rtsp_TCP, 0, (PFRtspDataCallBack)m_pRtspCallBack, NULL, this);
		if (!m_hRtspSession)
			return AvError_ConnectDeviceFailed;
		else
			return AvError_Succeed;
	}

	void StartCheckThread()
	{
		bRunning = true;
		hThread = (HANDLE)_beginthreadex(NULL, 128, ThreadCheckRecvTime, this, 0, 0);
	}

	static  UINT _stdcall ThreadCheckRecvTime(void *p)
	{
		_IPCConnection *pConnection = (_IPCConnection *)p;
		double dfTFirst = GetExactTime();
		while (pConnection->bRunning)
		{
			if (TimeSpanEx(dfTFirst) >= 1.000f)
			{
				// 上一次的活动时间与当前的时间差超过m_nRecvTimeOut
				if ((TimeSpanEx(pConnection->dfLastActiveTime) * 1000) > pConnection->nRecvTimeout &&
					// 若尚未报告断线或者离上线报告时间超过
					(pConnection->dfReConnectTime == 0.0f || (TimeSpanEx(pConnection->dfReConnectTime) * 1000) > pConnection->nReConnectInterval))
				{
					if (pConnection->pRunlog)
						pConnection->pRunlog->Runlog(_T("%s (IP:%s)if offline,try to connect!\n"), __FUNCTIONW__, _UnicodeString(pConnection->szIP, CP_ACP));

					if (pConnection->Reconnect() == AvError_Succeed)
					{
						if (pConnection->pRunlog)
							pConnection->pRunlog->Runlog(_T("%s Camera(IP:%s)reconnect succeed!\n"), __FUNCTIONW__, _UnicodeString(pConnection->szIP, CP_ACP));
						pConnection->dfReConnectTime = GetExactTime();
					}
					else
					{
						if (pConnection->pRunlog)
							pConnection->pRunlog->Runlog(_T("%s device %s(IP:%s)reconnect failed,tried after %d second!\n"), __FUNCTIONW__, _UnicodeString(pConnection->szIP, CP_ACP), pConnection->nReConnectInterval);
					}
				}

				dfTFirst = GetExactTime();
			}
			Sleep(20);
		}
		return 0;
	}
};


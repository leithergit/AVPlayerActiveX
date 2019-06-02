#pragma once

// AVPlayerCtrl.h : Declaration of the CAVPlayerCtrl ActiveX Control class.

//#include <boost/smart_ptr.hpp>
//using namespace boost;
#include "MySQLConnector.h"
#include "Utility.h"
#include "Runlog.h"
#include "TimeUtility.h"
#include "AutoLock.h"
#include "SimpleWnd.h"
//#include "../RTSP/rtsp.h"
#include "VS300ClientSDK.h"
#ifdef _DEBUG
//#pragma comment(lib,"../debug/rtsp.lib")
#pragma comment(lib,"VS300ClientSDKD")
#else
//#pragma comment(lib,"../release/rtsp.lib")
#pragma comment(lib,"VS300ClientSDK")
#endif
#include "../ipcplaysdk/IPCPlaySDK.h"
#pragma comment(lib,"../ipcplaysdk/ipcplaysdk.lib")
#include <list>
#include <map>
#include "VideoFrame.h"
#include "DhStreamParser.h"
#include "WndEvent.h"


using namespace  std;

#define rtsp_TCP	1
#define rtsp_UDP	0
enum AVStatus
{
	AvError_Succeed = 0,				// �����ɹ�
	AvError_base = -1000,				
	AvError_InvalidParameters			 = AvError_base - 1,		// ������Ч
	AvError_Invalid_ServerAddress		 = AvError_base - 2,		// ��������ַ����һ����Ч��IP��ַ	
	AvError_Invliad_ServerPort			 = AvError_base - 3,		// �������˿���Ч
	AvError_Invalid_AccountOrPassword	 = AvError_base - 4,		// �û�����������Ч
	AvError_ConnectServerFailed			 = AvError_base - 5,		// ���ӷ�����ʧ��
	AvError_LoginFailed					 = AvError_base - 6,		// ��¼������ʧ��
	AvError_NotLogintoServer			 = AvError_base - 7,		// ��δ��¼��������
	AvError_InvalidWindow				 = AvError_base - 8,		// ���ڲ��ŵĴ��ھ��ָʾ�Ĳ���һ����Ч�Ĵ���
	AvError_DeviceNotExist				 = AvError_base - 9,		// ������豸������
	AvError_ConnectDeviceFailed			 = AvError_base - 10,		// �����豸ʧ��
	AvError_DeviceInPlaying				 = AvError_base - 11,		// �豸���ڲ����У���Ҹ�����
	AvError_DBException					 = AvError_base - 12,		// ���ݿ�����쳣
	AvError_DeviceNotInPlaying			 = AvError_base - 13,		// �豸���ڲ�����
	AVError_BufferOverflow				 = AvError_base	- 14,		// �������,�ṩ���ڴ�ռ䲻�������������������
	AvError_WindowNotPlaying			 = AvError_base - 15,		// ������δ�����κ��豸	
	AvError_InvlaidPtzCommand			 = AvError_base - 16,		// ��Ч��PTZ����
	AvError_Invalid_PtzValue			 = AvError_base - 17,		// ��Ч��PTZ����ֵ
	AvError_LoadAssitConfigFailed		 = AvError_base - 18,		// ���ظ������������ļ�ʧ��
	AvError_DeivceNotConfigAssist		 = AvError_base - 19,		// ����δ����Ϊ���������豸
	AvError_FailedEnableAssist			 = AvError_base - 20,		// ���ø�������ʧ��
	AvError_Crane_notExist				 = AvError_base - 21,		// �����豸������
	AvError_ScreenMode_notExist			 = AvError_base - 22,		// ָ����ģʽ������
	AvError_NotSingleFramePlayer		 = AvError_base - 23,		// ����һ��֡������
	AvError_OutofPlayingRange			 = AvError_base - 24,		// ָ����ʱ��㳬����������ʱ�䷶Χ
	AvError_PlayProcessNotExist			 = AvError_base - 25,		// ���Ž����쳣�˳�
	AvError_PlayProcessListIsEmpty		 = AvError_base - 26,		// ���Ž��̱�Ϊ��
	AvError_ExternalError				 = AvError_base - 253,		// �ڲ�����
	AvError_InsufficentMemory			 = AvError_base - 254,		// �ڴ治��
	AvError_UnknownException			 = AvError_base - 255,		// δ֪�쳣 
	
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
	// ���²����Դ��ھ��Ժ��������ģʽ
	Ptz_Pan			 = 5,	// ˮƽת��
	Ptz_Tilt,				// ��ֱת��
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
	long	nSeekFrame;
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
// �ⷢ���ݽ��ճ�ʱ��Ϣ
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
	
	// ������Ӽ�
	CRITICAL_SECTION m_csMapConnection;
	map<string, IPCConnectionPtr> m_MapConnection;
	CRITICAL_SECTION m_csMapSession;
	map<long, IPCConnectionPtr> m_MapSession;
	map<string,SimpleStreamPtr> m_mapComboStream;
protected:
	CString m_strAccount;
	CString m_strServerIP;
	// ���������λ����Ϣ��Ĭ�Ϲر�
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
	long  m_nRecvTimeout;			// �������ݳ�ʱֵ,Ĭ��ֵΪ5000����
	long  m_nReConnectInterval;		// �������ʱ��,Ĭ��ֵΪ5000����
// 	static  UINT _stdcall ThreadCheckRecvTime(void *p)
// 	{
// 		CAVPlayerCtrl *pThis = (CAVPlayerCtrl *)p;
// 		return pThis->ThreadCheckRecvTimeRun();
// 	}
// 
// 	UINT ThreadCheckRecvTimeRun();
	
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

	// ��ȡ���ڲ����豸�Ĵ����б�
	// strDevice	�豸ID
	// hWndArray	�������
	// nArraySize	hWndArray���������ɴ��ڵ�������������strDeviceIDʵ�ʲ��ŵĴ�������
	// ���豸strDevice���ڲ��ţ��򷵻����ڲ��Ÿ��豸�Ĵ������������򷵻�0
	LONG GetDeviceWindow(LPCTSTR strDeviceID, LONG* hWndArray, LONG* nArraySize);
	// ��ȡ��ָ�������ϵĲ��ŵ��豸ID��
	// ���ô�����δ�����κ��豸���򷵻�0�����򷵻�AvError_Succeed������strDeviceID���ظô������ڲ��ŵ��豸ID
	
	LONG GetWindowDevice(LONG hWnd, BSTR* strDeviceID);
	LONG PlayComboStream(LPCTSTR strDevice1, LPCTSTR strDevice2, LONG hWnd, LONG nEnableHWAccel,LONG nArrangeMode);
	LONG SetPlayComboBorder(DOUBLE dwSpreadHei);

public:
	 
	map<string, ListPos> m_mapDevPos;
	//map<string,string> m_mapDeviceID;

protected:
	LONG SetPlayComboBorderEx(LPCTSTR strDevice1, LPCTSTR strDevice2, DOUBLE fSpreadHei);
	// ִ��PTZ����
	// strDeviceID	�豸ID
	// nPtzCommand	�������
	// nPtzValue	����������Ӧ������ֵ����ת���Ƕȣ��佹ֵ��
	// nOpType		�������ͣ�0Ϊ��Զ�����1Ϊ���Զ�����Ĭ��Ϊ��Զ���
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

	static bool CALLBACK AS300PlayBackCallBack(int nSessionId, char* buf, int len, void* user)
	{
		if (!nSessionId)
			return false;
		CAVPlayerCtrl *pThis = (CAVPlayerCtrl *)user;
		if (nSessionId == -1)
			return false;
		return pThis->OnAS300PlayBack(nSessionId, buf, len);
	}
	static void CALLBACK DevStatusCallBack(long login_id, char* szDevId, int nStatus, void* user)
	{
		CAVPlayerCtrl *pThis = (CAVPlayerCtrl *)user;
		pThis->OnDevStatus(szDevId, nStatus);
	}
	static void CALLBACK RespondCallBack(uint32 nSequence, long res, VSParamInfo *paramInfo, void* pUser)//��Ӧ����ص�
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

	//static int CALLBACK RecordQueryCallBack(int nQueryId, int nCount, void* pUser, long res);//��Ӧ����ص�

	void   OnRespondCallback(int nSessionId);	
	void   OnDevStatus(char* szDevId, int nStatus);
	void   OnAS300LiveData(LONG nSessionId, char* buf, int nLen);
	bool   OnAS300PlayBack(LONG nSessionId, char* buf, int nLen);

	//////////////////////////////////////////////////////////////////////////
	// OnAS300PlayBackPos
	// ����˵����
	// nSessionId 		�ط� / ���ػỰID,
	// pos		����ǰ��ļ��طŸ�ֵΪ�Ѳ����ļ���С��λkb
	// 			��������ظ�ֵΪ����frame��timestamp������� - 1��ʾ�������
	// total	����ǰ��ļ��طŸ�ֵΪ�����ļ��ܴ�С��λkb
	//			��������ظ�ֵΪ - 1
	// pUser		�û�����
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
	// �ƶ���ָ��λ�ú����²���
	LONG SeekTime(LPCTSTR strDeviceID, LONGLONG nTime);
protected:
	LONG CreateFrameWnd(LONG hWnd,LONG nWndCount, LONG nFrameStyle, LONG* pFrameHandle);
	LONG AdjustPanels(LONG nWndCount, LONG nFrameStyle);
	LONG QueryRecord(LPCTSTR szDeviceID, LONG nStartTime, LONG nStopTime, LONG pRecordArray,LONG nBufferCount, LONG* nRecordCount);
	LONG GetErrorMessage(LONG nErrorCode, LPCTSTR strErrorMessage, LONG nBufferSize);
public:
	static CCriticalSectionAgent m_csMapDecoderPool;
	static map<string, ItemStatusList> m_mapDecoderPool;
	//////////////////////////////////////////////////////////////////////////
	// ���ڶ������ʾ��Ƶ�Ĵ��� ��ʼ
	DWORD  StartPlayProcess(INT nIndex);
	void   StopPlayProcess(DWORD dwPID);
	CCriticalSectionAgent m_csListProcess;
	list<PlayProcess> m_listProcess;
	LRESULT OnRegisterWnd(WPARAM w, LPARAM l);
	LRESULT OnUnRegisterWnd(WPARAM w, LPARAM l);
	LRESULT OnCreatePlayer(WPARAM w, LPARAM l);
	// ���ڶ������ʾ��Ƶ�Ĵ��� ����
	//////////////////////////////////////////////////////////////////////////
};

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
	//long	m_hRtspSession;
	HWND	hPlayProcessWnd;		// �������Ž��̵�������
	bool	bStopAll;
	double	dfLastActiveTime;
	double  dfReConnectTime;
	char	szCameraIP[32];
	char	szAccount[32];
	char	szPassword[32];
	char    szRtspURL[512];
	BSTR	bstrErrorString;
	void	*m_pRtspCallBack;
	SimpleStream* m_pSimpleStream;
	bool	bRunning;
	long	nRecvTimeout;
	long	nReConnectInterval;
	shared_ptr<CRunlog> pRunlog;
	HANDLE	hThread;
	// AS300ת������
	bool	m_bIPCStart;
	long	m_nAS300Session;	// �طź�ת�����ŵ�session
	bool	m_bPlayBack;	// �طű�־���ط�ʱΪTRUE
	long	m_nLoginID;
	CHAR	m_strDeviceID[64];
	PlayBackStatusPtr	pPlayStatus;
	list<SimpleStream*> listSimpleStream;
	map<long,HWND> mapPlayerWnd;
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
#ifdef _DEBUG
		InterlockedIncrement(&nRefCount);
#endif
	}
	_IPCConnection(HWND hWnd,char *szIP,WORD nPort,char* szAccount,char *szPassword,char *szURL)
	{
		ZeroMemory(this, offsetof(_IPCConnection, pPlayStatus));
		strcpy_s(szCameraIP, 32, szIP);
		strcpy_s(szRtspURL,512, szURL);
		mapPlayerWnd.insert(pair<long,HWND>((long)hWnd,hWnd));
		int nSize = sizeof(_IPCConnection);
		m_pFrameBuffer = new byte[128 * 1024];
		m_nBufferSize = 128 * 1024;
		nRecvTimeout = 15000;
		nReConnectInterval = 15000;
		dfReConnectTime = 0.0f;
		m_nLoginID = -1;
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
	/*
	void OnSDPNotify(void *pData)
	{

	}
#define _RTP_Header		0	
#define _NALU			1
#define _FrameData		2
#define _FPSOffset		5
#define _WidthOffset	6
#define _HeightOffset	7
	void OnCalBack(char *pBuffer, int nParam)
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

					// 	ipcplay_SetStreamHeader(hPlayhandle, (byte *)&MediaHeader, sizeof(MediaHeader));
					// 	ipcplay_SetMaxFrameSize(hPlayhandle, 1024 * 1024);
					{
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

									ipcplay_Start(hTempHandle, false, true, false);
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

								ipcplay_Start(hTempHandle, false, true, false);
								ipcplay_EnableStreamParser(hTempHandle, CODEC_H264);
								ItemStatusList HandleList;
								HandleList.push_back(shared_ptr<CItemStatus>(new CItemStatus(hTempHandle)));
								CAVPlayerCtrl::m_mapDecoderPool.insert(pair<string, list<ItemStatusPtr>>(strResolutoin, HandleList));
								hPlayhandle = hTempHandle;
							}
							LeaveCriticalSection(CAVPlayerCtrl::m_csMapDecoderPool.Get());
						}
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
	*/
	~_IPCConnection()
	{
		bRunning = false;
#ifdef _DEBUG
		InterlockedDecrement(&nRefCount);
#endif
		if (hThread)
		{
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
			hThread = NULL;
		}

		if (m_nAS300Session)
		{
			assert(m_nLoginID != -1);
			if (!pPlayStatus)
				SDK_CUStopVideoRequest(m_nLoginID, (CHAR *)m_strDeviceID);
			else
				SDK_CUStopPlayback(m_nLoginID, m_nAS300Session);
			m_nLoginID = -1;
			m_nAS300Session = -1;
		}

// 		if (m_hRtspSession)
// 			rtsp_stop(m_hRtspSession);
		if (hPlayProcessWnd && IsWindow(hPlayProcessWnd))
		{
			COPYDATASTRUCT cds;
			PlayEvent WndEvent;
			WndEvent.nCommand = CDS_STOP;
			strcpy_s(WndEvent.szCameraIP,32, szCameraIP);
			cds.dwData = CDS_STOP;
			cds.cbData = sizeof(PlayEvent);
			cds.lpData = &WndEvent;
			::SendMessage(hPlayProcessWnd, WM_COPYDATA, 0, (LPARAM)&cds);
		}
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
		m_nAS300Session = SDK_CUVideoRequest(nLoginID, (char *)m_strDeviceID, 0, 1, 5000, 0, 0);
		m_nLoginID = nLoginID;

		return m_nAS300Session;
	}
	/*
	LONG RtspConnect(char *szIP, char *szAccount, char *szPassword, map<CString, CameraUrlPtr> &mapCamera/*, PFRtspDataCallBack pRtspCallBack)
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
		//m_hRtspSession = rtsp_play(szURL, "", "", rtsp_TCP, 0, (PFRtspDataCallBack)pRtspCallBack, NULL, this);
		if (!m_hRtspSession)
			return AvError_ConnectDeviceFailed;
		else
		{
			strcpy_s(szRtspURL,512,szURL);
			strcpy(this->szCameraIP, szIP);
			strcpy(this->szAccount, szAccount);
			strcpy(this->szPassword, szPassword);
			//m_pRtspCallBack = pRtspCallBack;
			dfLastActiveTime = GetExactTime();
			return AvError_Succeed;
		}
	}

	LONG Reconnect()
	{
// 		if (m_hRtspSession)
// 		{
// 			rtsp_stop(m_hRtspSession);
// 			m_hRtspSession = 0;
// 		}

		int nWndCount = 0;
		HWND hWndArray[16] = { 0 };
		int nStatus = ipcplay_GetRenderWindows(hPlayhandle, hWndArray, nWndCount);
		if (nStatus == IPC_Succeed  &&
			nWndCount > 0)
		{
			for (int i = 0; i < nWndCount; i++)
				InvalidateRect(hWndArray[i], NULL, TRUE);
		}

		//m_hRtspSession = rtsp_play(strRtspURL, "", "", rtsp_TCP, 0, (PFRtspDataCallBack)m_pRtspCallBack, NULL, this);
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
				// ��һ�εĻʱ���뵱ǰ��ʱ����m_nRecvTimeOut
				if ((TimeSpanEx(pConnection->dfLastActiveTime) * 1000) > pConnection->nRecvTimeout &&
					// ����δ������߻��������߱���ʱ�䳬��
					(pConnection->dfReConnectTime == 0.0f || (TimeSpanEx(pConnection->dfReConnectTime) * 1000) > pConnection->nReConnectInterval))
				{
					if (pConnection->pRunlog)
						pConnection->pRunlog->Runlog(_T("%s (IP:%s)if offline,try to connect!\n"), __FUNCTIONW__, _UnicodeString(pConnection->szCameraIP, CP_ACP));

					if (pConnection->Reconnect() == AvError_Succeed)
					{
						if (pConnection->pRunlog)
							pConnection->pRunlog->Runlog(_T("%s Camera(IP:%s)reconnect succeed!\n"), __FUNCTIONW__, _UnicodeString(pConnection->szCameraIP, CP_ACP));
						pConnection->dfReConnectTime = GetExactTime();
					}
					else
					{
						if (pConnection->pRunlog)
							pConnection->pRunlog->Runlog(_T("%s device %s(IP:%s)reconnect failed,tried after %d second!\n"), __FUNCTIONW__, _UnicodeString(pConnection->szCameraIP, CP_ACP), pConnection->nReConnectInterval);
					}
				}

				dfTFirst = GetExactTime();
			}
			Sleep(20);
		}
		return 0;
	}*/
};


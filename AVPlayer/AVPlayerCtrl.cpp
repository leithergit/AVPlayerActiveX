// AVPlayerCtrl.cpp : Implementation of the CAVPlayerCtrl ActiveX Control class.

#include "stdafx.h"
#include "AVPlayer.h"
#include "AVPlayerCtrl.h"
#include "AVPlayerPropPage.h"
#include "Markup.h"
#include "decSPS.h"
//#include "boost/smart_ptr/make_unique.hpp"
#include <process.h>
#include <Shlwapi.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_TIMER_DELETEFILE 1024
extern CAVPlayerApp theApp;
#ifdef _DEBUG
volatile LONG _IPCConnection::nRefCount = 0;
#endif

CCriticalSectionProxy  CAVPlayerCtrl::m_csMapDecoderPool;
map<string, ItemStatusList> CAVPlayerCtrl::m_mapDecoderPool;

IMPLEMENT_DYNCREATE(CAVPlayerCtrl, COleControl)

// Message map

BEGIN_MESSAGE_MAP(CAVPlayerCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_MESSAGE(WM_FIRE_RECVTIMEOUT,OnFireRecvTimeout)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//ON_MESSAGE(WM_FRAME_LBUTTONDOWN, &CAVPlayerCtrl::OnFrameLButtonDown)
	//ON_MESSAGE(WM_FRAME_LBUTTONUP, &CAVPlayerCtrl::OnFrameLButtonUp)
	//ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// Dispatch map
// #define DISP_FUNCTION_ID(theClass, szExternalName, dispid, pfnMember, vtRetVal, vtsParams)\
// 	{ _T(szExternalName), dispid, vtsParams, vtRetVal, (AFX_PMSG)(void (theClass::*)(void))&pfnMember, (AFX_PMSG)0, 0, afxDispCustom }, 

BEGIN_DISPATCH_MAP(CAVPlayerCtrl, COleControl)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "Login", dispidLogin, Login, VT_I4, VTS_BSTR VTS_UI2 VTS_BSTR VTS_BSTR VTS_I4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "Logout", dispidLogout, Logout, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "PlayStream", dispidPlayStream, PlayStream, VT_I4, VTS_BSTR VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "GetDeviceID", dispidGetDeviceID, GetDeviceID, VT_I4, VTS_PBSTR VTS_PI4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "StopPlay", dispidStopPlay, StopPlay, VT_EMPTY, VTS_BSTR VTS_I4)
	DISP_PROPERTY_EX_ID(CAVPlayerCtrl, "RecvTimeout", dispidRecvTimeout, GetRecvTimeout, SetRecvTimeout, VT_I4)
	DISP_PROPERTY_EX_ID(CAVPlayerCtrl, "ReportInterval", dispidReportInterval, GetReportInterval, SetReportInterval, VT_I4)
	//DISP_FUNCTION_ID(CAVPlayerCtrl, "GetErrorMessage", dispidGetErrorMessage, GetErrorMessage, VT_I4, VTS_I4 VTS_BSTR VT_I4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "FreeString", dispidFreeString, FreeString, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "GetDeviceWindow", dispidGetDeviceWindow, GetDeviceWindow, VT_I4, VTS_BSTR VTS_PI4 VTS_PI4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "GetWindowDevice", dispidGetWindowDevice, GetWindowDevice, VT_I4, VTS_I4 VTS_PBSTR)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "PlayComboStream", dispidPlayComboStream, PlayComboStream, VT_I4, VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "SetPlayComboBorder", dispidSetPlayComboBorder, SetPlayComboBorder, VT_I4, VTS_R8)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "SetPlayComboBorderEx", dispidSetPlayComboBorderEx, SetPlayComboBorderEx, VT_I4, VTS_BSTR VTS_BSTR VTS_R8)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "SendPtzCommand", dispidSendPtzCommand, SendPtzCommand, VT_I4, VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	
	DISP_FUNCTION_ID(CAVPlayerCtrl, "EnalbeCameraPostion", dispidEnalbeCameraPostion, EnalbeCameraPostion, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "SetExternDCDraw", dispidSetExternDCDraw, SetExternDCDraw, VT_I4, VTS_BSTR VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "PlaySrvStream", dispidPlaySrvStream, PlaySrvStream, VT_I4, VTS_BSTR VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "EnableOperationAssist", dispidEnableOperationAssist, EnableOperationAssist, VT_I4, VTS_BSTR VTS_I4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "LoadOpAssistConfigure", dispidLoadOpAssistConfigure, LoadOpAssistConfigure, VT_I4, VTS_NONE)

	DISP_FUNCTION_ID(CAVPlayerCtrl, "ConfigureScreenMode", dispidConfigureScreenMode, ConfigureScreenMode, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "SwitchScreen", dispidSwitchScreen, SwitchScreen, VT_I4, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "PlayBack", dispidPlayBack, PlayBack, VT_I4, VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "StopPlayBack", dispidStopPlayBack, StopPlayBack, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "SeekTime", dispidSeekTime, SeekTime, VT_I4, VTS_BSTR VTS_I8)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "CreateFrameWnd", dispidCreateFrameWnd, CreateFrameWnd, VT_I4, VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "AdjustPanels", dispidAdjustPanels, AdjustPanels, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "QueryRecord", dispidQueryRecord, QueryRecord, VT_I4, VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
	DISP_FUNCTION_ID(CAVPlayerCtrl, "GetErrorMessage", dispidGetErrorMessage, GetErrorMessage, VT_I4, VTS_I4 VTS_BSTR VTS_I4)
END_DISPATCH_MAP()


// Event map

BEGIN_EVENT_MAP(CAVPlayerCtrl, COleControl)
	EVENT_CUSTOM_ID("RecvTimeout", eventidRecvTimeout, RecvTimeout, VTS_BSTR VTS_I4)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CAVPlayerCtrl, 1)
	PROPPAGEID(CAVPlayerPropPage::guid)
END_PROPPAGEIDS(CAVPlayerCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CAVPlayerCtrl, "AVPLAYER.AVPlayerCtrl.1",
	0x7cb0d592, 0x1870, 0x4608, 0x81, 0x3b, 0x8d, 0xbb, 0xcd, 0x3d, 0xab, 0x57)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CAVPlayerCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DAVPlayer =
		{ 0x7FA42197, 0x3260, 0x4BBB, { 0x99, 0xF9, 0x28, 0x36, 0xE5, 0x3B, 0xB0, 0x3B } };
const IID BASED_CODE IID_DAVPlayerEvents =
		{ 0x789C77B5, 0x9570, 0x491B, { 0x9A, 0x70, 0x6B, 0xC7, 0x69, 0x71, 0xB5, 0x30 } };



// Control type information

static const DWORD BASED_CODE _dwAVPlayerOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CAVPlayerCtrl, IDS_AVPLAYER, _dwAVPlayerOleMisc)



// CAVPlayerCtrl::CAVPlayerCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CAVPlayerCtrl

BOOL CAVPlayerCtrl::CAVPlayerCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_AVPLAYER,
			IDB_AVPLAYER,
			afxRegApartmentThreading,
			_dwAVPlayerOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CAVPlayerCtrl::CAVPlayerCtrl - Constructor

CAVPlayerCtrl::CAVPlayerCtrl()
{
	InitializeIIDs(&IID_DAVPlayer, &IID_DAVPlayerEvents);
	m_nLogSaveDays = 30;
// 	TraceMsgA("offsetof(nVideoCodec) = %d\r\n", offsetof(PlayerInfo, nVideoCodec));
// 	TraceMsgA("offsetof(nAudioCodec) = %d\r\n", offsetof(PlayerInfo, nAudioCodec));
// 	TraceMsgA("offsetof(nVideoWidth) = %d\r\n", offsetof(PlayerInfo, nVideoWidth));
// 	TraceMsgA("offsetof(nVideoHeight) = %d\r\n", offsetof(PlayerInfo, nVideoHeight));
// 	TraceMsgA("offsetof(bAudioEnabled) = %d\r\n", offsetof(PlayerInfo, bAudioEnabled));
// 	TraceMsgA("offsetof(nTotalFrames) = %d\r\n", offsetof(PlayerInfo, nTotalFrames));
// 	TraceMsgA("offsetof(tTotalTime) = %d\r\n", offsetof(PlayerInfo, tTotalTime));
// 	TraceMsgA("offsetof(nCurFrameID) = %d\r\n", offsetof(PlayerInfo, nCurFrameID));
// 	TraceMsgA("offsetof(tFirstFrameTime) = %d\r\n", offsetof(PlayerInfo, tFirstFrameTime));
// 	TraceMsgA("offsetof(tCurFrameTime) = %d\r\n", offsetof(PlayerInfo, tCurFrameTime));
// 	TraceMsgA("offsetof(tTimeEplased) = %d\r\n", offsetof(PlayerInfo, tTimeEplased));
// 	TraceMsgA("offsetof(nFPS) = %d\r\n", offsetof(PlayerInfo, nFPS));
// 	TraceMsgA("offsetof(nPlayFPS) = %d\r\n", offsetof(PlayerInfo, nPlayFPS));
// 	TraceMsgA("offsetof(nCacheSize) = %d\r\n", offsetof(PlayerInfo, nCacheSize));
// 	TraceMsgA("offsetof(nCacheSize2) = %d\r\n", offsetof(PlayerInfo, nCacheSize2));
// 	TraceMsgA("offsetof(fPlayRate) = %d\r\n", offsetof(PlayerInfo, fPlayRate));
// 	TraceMsgA("offsetof(nSDKVersion) = %d\r\n", offsetof(PlayerInfo, nSDKVersion));
// 	TraceMsgA("offsetof(bFilePlayFinished) = %d\r\n", offsetof(PlayerInfo, bFilePlayFinished));
// 	TraceMsgA("offsetof(nReserver1) = %d\r\n", offsetof(PlayerInfo, nReserver1));
// 	TraceMsgA("offsetof(nReserver2) = %d\r\n", offsetof(PlayerInfo, nReserver2));
}


void StartCGI(char *szURL,char *szIP,WORD nPort,CHAR *szUser,char *szPassword)
{
	CURL *pCurl;
	CURLcode res;
	char szHost[128] = {0};
	sprintf(szHost,"Host: %s",szIP);
	pCurl = curl_easy_init();
	if(pCurl!=NULL)
	{
		curl_easy_setopt(pCurl,CURLOPT_URL,szURL);
		curl_easy_setopt(pCurl,CURLOPT_PORT,nPort);
		curl_easy_setopt(pCurl,CURLOPT_FOLLOWLOCATION,1L);
		curl_easy_setopt(pCurl,CURLOPT_HTTPGET,1L);
		curl_easy_setopt(pCurl,CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);
		curl_easy_setopt(pCurl,CURLOPT_USERNAME,szUser);
		curl_easy_setopt(pCurl,CURLOPT_PASSWORD,szPassword);
		struct curl_slist *pHeaders=NULL;
		pHeaders = curl_slist_append(pHeaders, szHost);
		pHeaders = curl_slist_append(pHeaders, "User-Agent: Mago");
		char szCookies[128] = {0};
		sprintf(szCookies,"Cookie: ptz_ctl_id=%s",szIP);
		pHeaders = curl_slist_append(pHeaders, szCookies);

		curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, pHeaders);

		res = curl_easy_perform(pCurl);
		if (res != CURLE_OK)
		{
			CHAR szError[1024] = {0};
			strcpy(szError,curl_easy_strerror(res));
			TraceMsgA("%s Error=%s.\n",__FUNCTION__,szError);
		}
		long retcode = 0;
		res = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE , &retcode);
		if (retcode == 401)
		{
			long nAuthorize;
			res = curl_easy_getinfo(pCurl, CURLINFO_HTTPAUTH_AVAIL, &nAuthorize);
			if(!res) 
			{
				if(!nAuthorize)
					TraceMsgA("%s No auth available, perhaps no 401?\n",__FUNCTION__);
				else 
				{
					TraceMsgA("%s Change authorize.\n",__FUNCTION__);
					if (nAuthorize & CURLAUTH_DIGEST)					
						curl_easy_setopt(pCurl,CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);					
					else if (nAuthorize & CURLAUTH_BASIC)
						curl_easy_setopt(pCurl,CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
					else if (nAuthorize & CURLAUTH_NEGOTIATE)
						curl_easy_setopt(pCurl,CURLOPT_HTTPAUTH, CURLAUTH_NEGOTIATE);
					else if (nAuthorize & CURLAUTH_NTLM)
						curl_easy_setopt(pCurl,CURLOPT_HTTPAUTH, CURLAUTH_NTLM);
					res = curl_easy_perform(pCurl);
					if (res != CURLE_OK)
					{
						CHAR szError[1024] = {0};
						strcpy(szError,curl_easy_strerror(res));
						TraceMsgA("%s Error=%s.\n",__FUNCTION__,szError);
					}
				}
			}
		}

		curl_slist_free_all(pHeaders);
		curl_easy_cleanup(pCurl); 
	}
}

// CAVPlayerCtrl::~CAVPlayerCtrl - Destructor

CAVPlayerCtrl::~CAVPlayerCtrl()
{
// 	if (m_hThreadCheckRecvTime)
// 	{
// 		m_bThreadCheckRecvTimeRun = false;
// 		if (m_pRunlog)
// 			m_pRunlog->Runlog(_T("%s 准备结束线程ThreadCheckRecvTime.\n"),__FUNCTIONW__);
// 		if (WaitForSingleObject(m_hThreadCheckRecvTime,5000) == WAIT_TIMEOUT)
// 		{
// 			DWORD dwThreadExitCode;
// 			GetExitCodeThread(m_hThreadCheckRecvTime, &dwThreadExitCode);
// 			if (dwThreadExitCode == STILL_ACTIVE)		// 线程仍在运行
// 			{
// 				if (m_pRunlog)
// 					m_pRunlog->Runlog(_T("%s 线程ThreadCheckRecvTime仍在运行,需要强制结束.\n"),__FUNCTIONW__);
// 				TerminateThread(m_hThreadCheckRecvTime,0);
// 			}
// 		}
// 	}

}


// CAVPlayerCtrl::OnDraw - Drawing function

void CAVPlayerCtrl::OnDraw(	CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;
	
	CBitmap BitmapLogo;
	CBitmap* pOldBitmap;
	CDC MemDC;
	MemDC.CreateCompatibleDC(pdc);
	BitmapLogo.LoadBitmap(IDB_AVPLAYER);
	BITMAP bitmapLogo;
	BitmapLogo.GetBitmap(&bitmapLogo);


	pOldBitmap = MemDC.SelectObject(&BitmapLogo);

	pdc->StretchBlt(0, 0, rcBounds.Width(), rcBounds.Height(), &MemDC, 0, 0, bitmapLogo.bmWidth, bitmapLogo.bmHeight, SRCCOPY);
	MemDC.SelectObject(pOldBitmap);

// 	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
// 	pdc->Ellipse(rcBounds);
}



// CAVPlayerCtrl::DoPropExchange - Persistence support

void CAVPlayerCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

}



// CAVPlayerCtrl::OnResetState - Reset control to default state

void CAVPlayerCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CAVPlayerCtrl::AboutBox - Display an "About" box to the user

void CAVPlayerCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_AVPLAYER);
	dlgAbout.DoModal();
}



// CAVPlayerCtrl message handlers
LRESULT CAVPlayerCtrl::OnFireRecvTimeout(WPARAM w,LPARAM l)
{
	// 不再发送事件，由控件内部处理重连事件
	//FireEvent(eventidRecvTimeout, EVENT_PARAM(VTS_BSTR VTS_I4), szDeviceW, l);
	//FireEvent(eventidRecvTimeout, EVENT_PARAM(VTS_I4 VTS_BSTR), l, szDeviceW);
	return 0;
}


LONG CAVPlayerCtrl::Login(LPCTSTR strServerIP, USHORT nServerPort, LPCTSTR strAccount, LPCTSTR strPassword, LONG nUserPtr)
{
	TraceFunction();
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!strServerIP)
		return AvError_Invalid_ServerAddress;
	if (!strAccount || !strPassword)
		return AvError_Invalid_AccountOrPassword;
	if (!nServerPort)
		return AvError_Invliad_ServerPort;

	if (!IsValidIPAddressW(strServerIP))
		return AvError_Invalid_ServerAddress;

	if (m_pRunlog)
		m_pRunlog->Runlog(_T("%s Account:%s trying to login to server:%s.\n"),__FUNCTIONW__,strAccount,strServerIP);
	
	EnterCriticalSection(&m_csDBConnector);
	if (m_pDBConnector)
	{
		LeaveCriticalSection(&m_csDBConnector);
		return S_OK;
	}
	else
	{
		// 暂停数据库访问，先解锁
		LeaveCriticalSection(&m_csDBConnector);
		try
		{
			m_nLoginID = -1;
			if (m_bEnableAS300)
			{
				long nStatus = SDK_CULogin(_AnsiString(strServerIP, CP_ACP), nServerPort, _AnsiString(strAccount, CP_ACP), _AnsiString(strPassword, CP_ACP), &m_nLoginID,0);
				if (nStatus == 0)
				{
					SDK_CUSetRealPlayCallBack(m_nLoginID, AS300LiveCallBack, this);
					SDK_CUSetPlaybackCallBack(m_nLoginID, AS300PlayBackCallBack, this);
					SDK_CUSetPlaybackPos(m_nLoginID, PlayBackPosCallBack, this);
					//SDK_CUSetResCallback(m_nLoginID, RespondCallBack, this);
				}
				else
				{
					if (m_pRunlog)
						m_pRunlog->Runlog(_T("%s Account:%s login to server:%s failed.\n"), __FUNCTIONW__, strAccount, strServerIP);
					return AvError_ConnectServerFailed;
				}
			}
			m_strAccount = strAccount;
			m_strServerIP = strServerIP;
			// 连接数据库，重新上锁
			CAutoLock dblock(&m_csDBConnector);
			m_pDBConnector = shared_ptr<CMySQLConnector>( new CMySQLConnector());
			if (!m_pDBConnector->Connect(_AnsiString(strServerIP, CP_ACP), "root", "password", "vms", 3406))
			{
				m_pDBConnector = nullptr;
				if (m_pRunlog)
					m_pRunlog->Runlog(_T("%s Failed to connect database.\n"), __FUNCTIONW__);
				return AvError_ConnectServerFailed;
			}	
			CMyResult res = m_pDBConnector->Query("select count(*) as result from `users` where `loginname`='%s' and `loginpass`='%s'", _AnsiString(strAccount,CP_ACP), _AnsiString(strPassword,CP_ACP));
			int nResult = res["result"];
			if (nResult < 1)
			{
				if (m_pRunlog) 
					m_pRunlog->Runlog(_T("%s Invalid account or password,login failed.\n"),__FUNCTIONW__);
				return 	AvError_LoginFailed;
			}
			// 数据库访问完成，解锁
			dblock.Unlock();
			LoadScreenMode();
			LoadOPAssistXConfigure();
			TCHAR szPath[MAX_PATH] = { 0 };
			TCHAR szTempPath[MAX_PATH] = {0};
			// 取得控件加载路径
			GetModuleFileName(theApp.m_hInstance,szTempPath,MAX_PATH);
			int nPos = _tcsReserverFind(szTempPath,_T('\\'));
			_tcsncpy_s(szPath,MAX_PATH,szTempPath,nPos);
			_tcscat_s(szPath, MAX_PATH, _T("\\Configuration.xml"));
			CMarkup xml;
			if (PathFileExists(szPath) &&  xml.Load(szPath))
			{
				// 配置文件的格式
				/*
				<?xml version="1.0" encoding="utf-8"?>
				<Configuration >
					<CameraPostion Enable ="true"/>
					<LogManager SaveDays ="30"/>
					<CameraList>			
						<Camera IP="192.168.1.26" User="root" Password="" Url="" Port =""/>
						<Camera IP="192.168.1.30" User="root" Password="" Url="" Port =""/>	
					</CameraList>	
				</Configuration>
				*/

				CString strValue;
				bool bEnable = false;

				if (xml.FindElem(_T("Configuration")))
				{
					if (xml.FindChildElem(_T("CameraList")))
					{
						xml.IntoElem();	
						while (xml.FindChildElem(_T("Camera")))
						{
							xml.IntoElem();
							CameraUrlPtr pCamera = make_shared<CameraUrl>();
							CString strIP = xml.GetAttrib(_T("IP"));	
							pCamera->strAccount = xml.GetAttrib(_T("User"));
							pCamera->strPassword = xml.GetAttrib(_T("Password"));
							pCamera->strURL = xml.GetAttrib(_T("Url"));
							pCamera->nPort = (WORD)_tcstolong(xml.GetAttrib(_T("Port")));
							m_mapCameraUrl.insert(pair<CString,CameraUrlPtr>(strIP,pCamera));
							if (m_pRunlog)
								m_pRunlog->Runlog(_T("%s CameraIP = %s\tAccount = %s\tPassword = %s\tURL = %s.\n"), __FUNCTIONW__,strIP,pCamera->strAccount,pCamera->strPassword,pCamera->strURL);

							xml.OutOfElem();
						}
						xml.OutOfElem();
					}
					if (xml.FindChildElem(_T("CameraPostion")))
					{
						xml.IntoElem();			
						strValue		 = xml.GetAttrib(_T("Enable"));	
						xml.OutOfElem();
						if (strValue.CompareNoCase(_T("true")) == 0)
						{
							bEnable = true;
						}
					}
					if (xml.FindChildElem(_T("LogManager")))
					{
						xml.IntoElem();			
						m_nLogSaveDays = _tcstolong((LPCTSTR)xml.GetAttrib(_T("SaveDays")));	
						if (m_nLogSaveDays < 7)
							m_nLogSaveDays = 7;
						xml.OutOfElem();
					}

					if (xml.FindChildElem(_T("AsyncPlayer")))
					{
						xml.IntoElem();
						m_nYUVFrameCacheSize = _tcstolong((LPCTSTR)xml.GetAttrib(_T("YUVFrameCacheSize")));
						if (m_nYUVFrameCacheSize <= 25)
							m_nLogSaveDays = 25;
						xml.OutOfElem();
					}
				}
				if (bEnable)
				{// 只有启用位置选项后,才查询数据库
					//	访问数据库，重新加锁
					dblock.Lock();
					res = m_pDBConnector->Query("select DISTINCT cameraID from screencut_position");
					if (res.RowCount() < 1)
					{
						if (m_pRunlog) 
						{
							m_pRunlog->Runlog(_T("Can't find any `CamerrID` in `screencut_position`!\n"));
						}
						return AvError_DeviceNotExist;
					}

					do 
					{
						char *szCameraID = res["cameraID"];
						CMyResult resSub = m_pDBConnector->Query("select * from screencut_position where cameraID=%s",szCameraID);
						if (resSub.RowCount() < 1)
						{
							if (m_pRunlog) 
							{
								m_pRunlog->Runlog(_T("error!\n"));
							}
							return AvError_DeviceNotExist;
						}

						ListPos listPos;
						do 
						{
							PosParam PosTemp;
							PosTemp.nLeft = resSub["left"];
							PosTemp.nTop = resSub["top"];
							PosTemp.nRight = resSub["right"];
							PosTemp.nBottom = resSub["bottom"];
							PosTemp.fSpreadHeiMin = resSub["spreadHeiMin"];
							PosTemp.fSpreadHeiMax = resSub["spreadHeiMax"];
							listPos.push_back(PosTemp);
						}
						while(++resSub);
						string strCamerID(szCameraID);

						m_mapDevPos.insert(pair<string,ListPos>(strCamerID,listPos));

					}
					while(++res);
				}
			}
			else
			{
				if (m_pRunlog)
					m_pRunlog->Runlog(_T("%s Can't load %s.\n"), __FUNCTIONW__, szPath);
			}
			//return S_OK;
		}	
		catch (CMySQLException& e)
		{
			if (m_pRunlog) 
				m_pRunlog->Runlog(_T("%s A DB Exception occured:\n\t%s.\n"),__FUNCTIONW__,e.whatW());
			// 发生异常后编译器会自动回收对象所占内存，因此对象指针可置空
			CAutoLock lock(&m_csDBConnector);
			m_pDBConnector.reset();
			return AvError_DBException;
		}
		catch (std::exception & e)
		{
			if (m_pRunlog) 
				m_pRunlog->Runlog(_T("%s A Unknown Exception occured:\n\t%s.\n"),__FUNCTIONW__,_UnicodeString(e.what(),CP_ACP));
			CAutoLock lock(&m_csDBConnector);
			m_pDBConnector.reset();
			return AvError_UnknownException;
		}
	}

	return 0;
}

void CAVPlayerCtrl::Logout(void)
{
	TraceFunction();
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		CAutoLock dblock(&m_csDBConnector);
		if (m_pDBConnector)
			m_pDBConnector.reset();
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s The user %s logout from %s.\n"),__FUNCTIONW__,m_strAccount,m_strAccount);

		EnterCriticalSection(&m_csMapConnection);
		m_MapConnection.clear();
		LeaveCriticalSection(&m_csMapConnection);

		EnterCriticalSection(&m_csMapSession);
		m_MapSession.clear();
		LeaveCriticalSection(&m_csMapSession);

		EnterCriticalSection(&m_csOperationAssist);
		m_mapOperationAssist.clear();
		LeaveCriticalSection(&m_csOperationAssist);
		
		m_mapCrane.clear();
		m_nCurrentMode = -1;
		m_nCurrentCrane = -1;

	}
	catch (...)
	{

	}
}


void   CAVPlayerCtrl::OnAS300PlayBackPos(int nSessionID, int nPos, int nTotal)
{
	//TraceMsgA("%s nSessionID = %d\tnPos = %d\tnTotal = %d.\n", __FUNCTION__, nSessionID, nPos, nTotal);
	if (nPos == -1)
	{
		TCHAR szDeviceID[32] = { 0 };
		EnterCriticalSection(&m_csMapSession);
		auto itFind = m_MapSession.find((long)nSessionID);
		if (itFind != m_MapSession.end())
			_tcscpy_s(szDeviceID, 32, _UnicodeString(itFind->second->m_strDeviceID, CP_ACP));
		LeaveCriticalSection(&m_csMapSession);
		if (_tcslen(szDeviceID))
			StopPlayBack(szDeviceID);
	}
}

UINT CAVPlayerCtrl::ThreadCheckRecvTimeRun()
{
	double dfTFirst = GetExactTime();
	while (m_bThreadCheckRecvTimeRun)
	{
		if (TimeSpanEx(dfTFirst) >= 0.200f)
		{
			CAutoLock lock(&m_csMapConnection);
			for (map<string, IPCConnectionPtr>::iterator it = m_MapConnection.begin();
				it != m_MapConnection.end();
				it++)
			{
				IPCConnectionPtr pConnection = it->second;
				// 上一次的活动时间与当前的时间差超过m_nRecvTimeOut
				if ((TimeSpanEx(pConnection->dfLastActiveTime) * 1000) > m_nRecvTimeout &&
					// 若尚未报告断线或者离上线报告时间超过
					(pConnection->dfReConnectTime == 0.0f || (TimeSpanEx(pConnection->dfReConnectTime) * 1000) > m_nReConnectInterval))
				{
					if (m_pRunlog)
						m_pRunlog->Runlog(_T("%s 设备 %s(IP:%s)掉线,尝试重连!\n"), __FUNCTIONW__, _UnicodeString(it->first.c_str(), CP_ACP), _UnicodeString(pConnection->szIP, CP_ACP));

					if (pConnection->Reconnect() == AvError_Succeed)
					{
						if (m_pRunlog)
							m_pRunlog->Runlog(_T("%s 设备 %s(IP:%s)重连成功!\n"), __FUNCTIONW__, _UnicodeString(it->first.c_str(), CP_ACP), _UnicodeString(pConnection->szIP, CP_ACP));
						pConnection->dfReConnectTime = GetExactTime();
					}
					else
					{
						if (m_pRunlog)
							m_pRunlog->Runlog(_T("%s 设备 %s(IP:%s)重连失败,%d秒后重试!\n"), __FUNCTIONW__, _UnicodeString(it->first.c_str(), CP_ACP), _UnicodeString(pConnection->szIP, CP_ACP), m_nReConnectInterval);
					}
				}
			}
			dfTFirst = GetExactTime();
		}
		Sleep(20);
	}
	return 0;
}

void RtspDataCallBack(long lHandle, char *pBuffer, int nParam, int nUser)
{
	_IPCConnection *pThis = (_IPCConnection *)nUser;
	pThis->OnCalBack(pBuffer, nParam);
}

// LRESULT CAVPlayerCtrl::SubClassProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
// {
// 	EnterCriticalSection(&m_csMapSubclassWnd);
// 	auto itFind = m_MapSubclassWnd.find(hWnd);
// 	if (itFind == m_MapSubclassWnd.end())
// 	{
// 		LeaveCriticalSection(&m_csMapSubclassWnd);
// 		return 0L;
// 	}
// 	LeaveCriticalSection(&m_csMapSubclassWnd);
// 	RECT rtPartner;
// 	::GetWindowRect(itFind->second->hPartnerWnd, &rtPartner);
// 	switch (nMessage)
// 	{
// 	case WM_MOVE:
// 	case WM_MOVING:
// 	case WM_SIZE:
// 	{
// 		::MoveWindow(hWnd, rtPartner.left, rtPartner.top, rtPartner.right - rtPartner.left, rtPartner.bottom - rtPartner.top, true);
// 	}
// 	}
// 	return itFind->second->pOldProcWnd(hWnd, nMessage, wParam, lParam);
// }

LONG CAVPlayerCtrl::PlayStream(LPCTSTR strDeviceID, LONG hWnd,LONG nEnalbeHWAccel)
{
	TraceFunction();
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!strDeviceID || !hWnd)
		return AvError_InvalidParameters;
	CAutoLock dblock(&m_csDBConnector);
	if (!m_pDBConnector)
		return AvError_NotLogintoServer;

	dblock.Unlock();
	// 从数据库获取相机的IP，端口，帐号，密码
	try
	{
		// 330106 100 0052
		char szChannelID[32] = {0};
		char szDeviceID[32] = { 0 };
		W2AHelper(strDeviceID, szChannelID, 32);
		strncpy(szDeviceID,szChannelID,13);
		
		if (!::IsWindow((HWND)hWnd))
		{
			if (m_pRunlog)
				m_pRunlog->Runlog(_T("%s Specialfied a invalid window!\n"),__FUNCTIONW__,strDeviceID);
			return AvError_InvalidWindow;
		}

		RECT rt;
		::GetWindowRect((HWND)hWnd,&rt);
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s Windows %08X Postion(%d,%d,%d,%d).\n"),__FUNCTIONW__,hWnd,rt.left,rt.right,rt.top,rt.bottom);
		CAutoLock ConnectionLock(&m_csMapConnection);
		map<string, IPCConnectionPtr>::iterator itFind = m_MapConnection.find(szDeviceID);
		if (itFind != m_MapConnection.end())
		{
			IPC_PLAYHANDLE &hPlayer = itFind->second->hPlayhandle;			
			if (ipcplay_AddWindow(hPlayer, (HWND)hWnd) != IPC_Succeed)
			{
				if (m_pRunlog)
					m_pRunlog->Runlog(_T("%s ipcplayhandle (%8x) can't add a window any more,hWnd (%08X)add failed..\n"), __FUNCTIONW__,hPlayer,hWnd);
				return IPC_Error_RenderWndOverflow;
			}
			int nWndCount = 16;
			HWND hArray[16] = {0};
			int nStatus = ipcplay_GetRenderWindows(hPlayer,hArray,nWndCount);
			if (m_pRunlog)
			{
				m_pRunlog->Runlog(_T("%s Device %s (%08X) is Playing,Extended to a new window %08X (WndCount = %d)!\n"),__FUNCTIONW__,strDeviceID,(long)hPlayer,hWnd,nWndCount);			
				m_pRunlog->Runlog(_T("%s MapConnection size = %d.\n"),__FUNCTIONW__,m_MapConnection.size());
			}
			return AvError_Succeed;
		}
		ConnectionLock.Unlock();

		dblock.Lock();
		CMyResult res = m_pDBConnector->Query("SELECT `ipaddress`,`port`,`loginname`,`loginpasswd` FROM `devices` where `deviceid` = '%s'", _AnsiString(strDeviceID,CP_ACP));
		if (res.RowCount() < 1)
		{
			if (m_pRunlog) 
				m_pRunlog->Runlog(_T("%s can't find the mismatched device with ID:%s!\n"),__FUNCTIONW__,strDeviceID);
			return AvError_DeviceNotExist;
		}
		char *szIP	 = res["ipaddress"];
		WORD nPort	 = res["port"];
		char *szUser = res["loginname"];
		char *szPass = res["loginpasswd"];
		// 不再访问数据库，手动解锁
		dblock.Unlock();
		
		IPCConnectionPtr pConnection = shared_ptr<_IPCConnection>(new _IPCConnection());
		pConnection->pRunlog = m_pRunlog;
		pConnection->m_hWnd = (HWND)hWnd;
		pConnection->m_bEnableHWAccel = (bool)nEnalbeHWAccel;
// 		pConnection->hPlayhandle = ipcplay_OpenStream(pConnection->m_hWnd, NULL, 0);
// 		
// 		ipcplay_SetD3dShared(pConnection->hPlayhandle, (bool)nEnalbeHWAccel);
// 		ipcplay_Start(pConnection->hPlayhandle, false, true, (bool)nEnalbeHWAccel);
// 		ipcplay_EnableStreamParser(pConnection->hPlayhandle, CODEC_H264);
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s Device %s (%08X) is playing on Window %08X.\n"),__FUNCTIONW__,strDeviceID,(long)pConnection->hPlayhandle,hWnd);
		
		//ipcplay_SetRocateAngle(pConnection->hPlayhandle,Rocate90);
		
		
		LONG nResult = pConnection->RtspConnect(szIP,szUser,szPass,m_mapCameraUrl,RtspDataCallBack);
		if (nResult != AvError_Succeed)
		{
			if (m_pRunlog) 
				m_pRunlog->Runlog(_T("%s Connect device failed,IP = %s.\n"),__FUNCTIONW__,szIP);
			return nResult;
		}
		pConnection->StartCheckThread();		
		CAutoLock lock2(&m_csMapConnection);
		m_MapConnection.insert(pair<string, IPCConnectionPtr>(szDeviceID, pConnection));
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s MapConnection size = %d.\n"),__FUNCTIONW__,m_MapConnection.size());

		return S_OK;
	}
	catch (CMySQLException& e)
	{
		if (m_pRunlog) 
			m_pRunlog->Runlog(_T("%s A DB Exception occured when playing device %s:%s.\n"),__FUNCTIONW__,strDeviceID,e.whatW());
		return AvError_DBException;
	}
	catch (std::exception &e)
	{
		if (m_pRunlog) 
			m_pRunlog->Runlog(_T("%s A Unknown Exception occured when playing device %s:%s.\n"),__FUNCTIONW__,strDeviceID,_UnicodeString(e.what(),CP_ACP));
		return AvError_UnknownException;
	}

	return 0;
}

LONG CAVPlayerCtrl::PlayComboStream(LPCTSTR szDevice1, LPCTSTR szDevice2, LONG hWnd, LONG nEnableHWAccel,LONG nArrangeMode)
{
	TraceFunction();
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!szDevice1||
		!szDevice2|| 
		!hWnd)
		return AvError_InvalidParameters;
	CAutoLock dblock(&m_csDBConnector);
	if (!m_pDBConnector)
		return AvError_NotLogintoServer;
	dblock.Unlock();
	if (nArrangeMode != 0 && nArrangeMode != 1)
		return AvError_InvalidParameters;

	//string strDev1(_AnsiString(szDevice1,CP_ACP));
	//string strDev2(_AnsiString(szDevice2,CP_ACP));
	
	//m_mapDeviceID.insert(make_pair<string,string>(strDev1,strDev1));
	//m_mapDeviceID.insert(make_pair<string,string>(strDev2,strDev2));
	
	// 从数据库获取相机的IP，端口，帐号，密码
	RECT rtBorderLR[2] = {{0,0,50,0},{50,0,0,0}};		// 左右排列的尺寸
	RECT rtBorderTB[2] = {{0,0,0,50},{0,50,0,0}};		// 上下排列的尺寸
	LPCTSTR szDeviceArray[2] = {szDevice1,szDevice2};
	Position posArrayLR[2] = {Pos_Left,Pos_Right};		// 左右排列的位置
	Position posArrayTB[2] = {Pos_Top,Pos_Bottom};		// 上下排列的位置
	RECT *pBorder = nullptr;
	Position *pPosition = nullptr;
	if (nArrangeMode == 1)								// nArrangeMode即排列方式为1时，则为上下排列,为0时则左右排列
	{
		pBorder = rtBorderTB;
		pPosition = posArrayTB;
	}
	else if (nArrangeMode == 0)
	{
		pBorder = rtBorderLR;
		pPosition = posArrayLR;
	}
	else 
		return AvError_InvalidParameters;
	int nIndex = 0;
	try
	{
		// 330106 100 0052
		if (m_pRunlog)
		{
			m_pRunlog->Runlog(_T("%s Try to Play Device %s and %s On Window(%08X),nArrangeMode = %d.\n"),__FUNCTIONW__,szDevice1,szDevice2,hWnd,nArrangeMode);
		}
		HWND hVideoWnd = (HWND)hWnd;
		if (!::IsWindow(hVideoWnd))
		{
			if (m_pRunlog)
				m_pRunlog->Runlog(_T("%s and Specialfied a invalid window!\n"),__FUNCTIONW__,szDeviceArray[nIndex]);
			return AvError_InvalidWindow;
		}
		RECT rtWindow;
		::GetWindowRect((HWND)hWnd,&rtWindow);
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s Windows %08X Postion(%d,%d,%d,%d).\n"),__FUNCTIONW__,hWnd,rtWindow.left,rtWindow.right,rtWindow.top,rtWindow.bottom);
		
		dblock.Lock();
		CMyResult res = m_pDBConnector->Query("SELECT `ipaddress`,`port`,`loginname`,`loginpasswd` FROM `devices` where `deviceid` = '%s' or `deviceid` = '%s'", _AnsiString(szDeviceArray[0],CP_ACP),_AnsiString(szDeviceArray[1],CP_ACP));
		if (res.RowCount() < 2)
		{
			if (m_pRunlog) 
				m_pRunlog->Runlog(_T("%s can't find the mismatched device with ID:%s or %s!\n"),__FUNCTIONW__,szDeviceArray[0],szDeviceArray[1]);
			return AvError_DeviceNotExist;
		}
		do 
		{
			bool bPlaying = false;				// the device already in playing
			EnterCriticalSection(&m_csMapConnection);
			map<string, IPCConnectionPtr>::iterator itFind = m_MapConnection.find(_AnsiString(szDeviceArray[nIndex],CP_ACP));
			if (itFind != m_MapConnection.end() )
			{
				if (m_pRunlog)
					m_pRunlog->Runlog(_T("%s Device %s (%08X) is Playing,!\n"),__FUNCTIONW__,szDeviceArray[nIndex],hWnd);
				LeaveCriticalSection(&m_csMapConnection);
				bPlaying = true;
			}
			LeaveCriticalSection(&m_csMapConnection);
			IPCConnectionPtr pConnection = shared_ptr<_IPCConnection>(new _IPCConnection((HWND)hVideoWnd,_AnsiString(szDeviceArray[nIndex],CP_ACP),pPosition[nIndex]));
			if (bPlaying)
			{
				IPC_PLAYHANDLE &hPlayer = itFind->second->hPlayhandle;			
				ipcplay_AddWindow(hPlayer,(HWND)pConnection->m_hWnd,&pBorder[nIndex],true);	
				int nWndCount = 16;
				HWND hArray[16] = {0};
				int nStatus = ipcplay_GetRenderWindows(hPlayer,hArray,nWndCount);
				if (m_pRunlog)
				{
					m_pRunlog->Runlog(_T("%s Device %s (%08X) is Playing,Extended to a new window %08X (WndCount = %d)!\n"),__FUNCTIONW__,_AnsiString(szDeviceArray[nIndex],CP_ACP),(long)hPlayer,hWnd,nWndCount);			
					m_pRunlog->Runlog(_T("%s MapConnection size = %d.\n"),__FUNCTIONW__,m_MapConnection.size());
				}
				itFind->second->AddSimpleStream(pConnection->m_pSimpleStream);
				pConnection->m_pSimpleStream = NULL;
			}
			else
			{
				char *szIP	 = res["ipaddress"];
				WORD nPort	 = res["port"];
				char *szUser = res["loginname"];
				char *szPass = res["loginpasswd"];
				// HWND hParent,string strDevice,Position nPos
				pConnection->hPlayhandle = ipcplay_OpenStream(pConnection->m_hWnd, NULL, 0);

				ipcplay_SetD3dShared(pConnection->hPlayhandle, (bool)nEnableHWAccel);
				ipcplay_Start(pConnection->hPlayhandle, false, true, (bool)nEnableHWAccel);
				
				ipcplay_SetBorderRect(pConnection->hPlayhandle,pConnection->m_hWnd,&pBorder[nIndex],true);

				if (m_pRunlog)
					m_pRunlog->Runlog(_T("%s Device %s (%08X) is playing on Window %08X On Rectangle(%d,%d,%d,%d).\n"),__FUNCTIONW__,szDeviceArray[nIndex],(long)pConnection->hPlayhandle,hWnd,pBorder[nIndex].left,pBorder[nIndex].top,pBorder[nIndex].right,pBorder[nIndex].bottom);

				//ipcplay_SetRocateAngle(pConnection->hPlayhandle,Rocate90);
				LONG nResult = pConnection->RtspConnect(szIP,szUser,szPass,m_mapCameraUrl,RtspDataCallBack);
				pConnection->StartCheckThread();
				if (nResult != AvError_Succeed)
				{
					if (m_pRunlog) 
						m_pRunlog->Runlog(_T("%s Connect device (%s) failed,IP = %s.\n"),__FUNCTIONW__,szDeviceArray[nIndex],szIP);
					return nResult;
				}
			}
			EnterCriticalSection(&m_csMapConnection);
			m_MapConnection.insert(pair<string, IPCConnectionPtr>(_AnsiString(szDeviceArray[nIndex],CP_ACP), pConnection));
			LeaveCriticalSection(&m_csMapConnection);
			nIndex ++;
// 			if (m_pRunlog)
// 				m_pRunlog->Runlog(_T("%s MapConnection size = %d.\n"),__FUNCTIONW__,m_MapConnection.size());
		} while (++res);

		return S_OK;
	}
	catch (CMySQLException& e)
	{
		if (m_pRunlog) 
			m_pRunlog->Runlog(_T("%s A DB Exception occured when playing device %s or %s:%s.\n"),__FUNCTIONW__,szDeviceArray[nIndex],szDevice2,e.whatW());
		return AvError_DBException;
	}
	catch (std::exception &e)
	{
		if (m_pRunlog) 
			m_pRunlog->Runlog(_T("%s A Unknown Exception occured when playing device %s:%s.\n"),__FUNCTIONW__,szDeviceArray[nIndex],szDevice2,_UnicodeString(e.what(),CP_ACP));
		return AvError_UnknownException;
	}

	return 0;

}

LONG CAVPlayerCtrl::PlaySrvStream(LPCTSTR strDeviceID, LONG hWnd, LONG nEnableHWAccel)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TraceFunction();

	if (!strDeviceID || !hWnd)
		return AvError_InvalidParameters;
		
	if (m_nLoginID == -1)
		return AvError_NotLogintoServer;
	// 从数据库获取相机的IP，端口，帐号，密码
	try
	{
		// 330106 100 0052
		char szChannelID[32] = { 0 };
		char szDeviceID[32] = { 0 };
		W2AHelper(strDeviceID, szChannelID, 32);
		strncpy(szDeviceID, szChannelID, 13);

		if (!::IsWindow((HWND)hWnd))
		{
			if (m_pRunlog)
				m_pRunlog->Runlog(_T("%s Specialfied a invalid window!\n"), __FUNCTIONW__, strDeviceID);
			return AvError_InvalidWindow;
		}

		RECT rt;
		::GetWindowRect((HWND)hWnd, &rt);
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s Windows %08X Postion(%d,%d,%d,%d).\n"), __FUNCTIONW__, hWnd, rt.left, rt.right, rt.top, rt.bottom);
		CAutoLock lock(&m_csMapConnection);
		map<string, IPCConnectionPtr>::iterator itFind = m_MapConnection.find(szDeviceID);
		if (itFind != m_MapConnection.end())
		{
			IPC_PLAYHANDLE &hPlayer = itFind->second->hPlayhandle;
			ipcplay_AddWindow(hPlayer, (HWND)hWnd);
			int nWndCount = 16;
			HWND hArray[16] = { 0 };
			int nStatus = ipcplay_GetRenderWindows(hPlayer, hArray, nWndCount);
			if (m_pRunlog)
			{
				m_pRunlog->Runlog(_T("%s Device %s (%08X) is Playing,Extended to a new window %08X (WndCount = %d)!\n"), __FUNCTIONW__, strDeviceID, (long)hPlayer, hWnd, nWndCount);
				m_pRunlog->Runlog(_T("%s MapConnection size = %d.\n"), __FUNCTIONW__, m_MapConnection.size());
			}
			return AvError_Succeed;
		}
		lock.Unlock();

		IPCConnectionPtr pConnection = make_shared<_IPCConnection>();
		pConnection->pRunlog = m_pRunlog;
		pConnection->m_hWnd = (HWND)hWnd;
		pConnection->hPlayhandle = ipcplay_OpenRTStream(pConnection->m_hWnd);

		ipcplay_SetD3dShared(pConnection->hPlayhandle, (bool)nEnableHWAccel);
		long hPlaySession = pConnection->OpenAS300Session(m_nLoginID, strDeviceID);

		EnterCriticalSection(&m_csMapConnection);
		m_MapConnection.insert(pair<string, IPCConnectionPtr>(szDeviceID, pConnection));
		LeaveCriticalSection(&m_csMapConnection);

		EnterCriticalSection(&m_csMapSession);
		m_MapSession.insert(pair<LONG, IPCConnectionPtr>(hPlaySession, pConnection));
		LeaveCriticalSection(&m_csMapSession);

		if (m_pRunlog)
		{
			m_pRunlog->Runlog(_T("%s Device %s (%08X) is playing on Window %08X.\n"), __FUNCTIONW__, strDeviceID, (long)pConnection->hPlayhandle, hWnd);
			m_pRunlog->Runlog(_T("%s MapSession size = %d.\n"), __FUNCTIONW__, m_MapSession.size());
		}

		return S_OK;
	}
	catch (CMySQLException& e)
	{
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s A DB Exception occured when playing device %s:%s.\n"), __FUNCTIONW__, strDeviceID, e.whatW());
		return AvError_DBException;
	}
	catch (std::exception &e)
	{
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s A Unknown Exception occured when playing device %s:%s.\n"), __FUNCTIONW__, strDeviceID, _UnicodeString(e.what(), CP_ACP));
		return AvError_UnknownException;
	}

}

LONG CAVPlayerCtrl::GetDeviceID(BSTR* pstrDeviceList, LONG* nDeviceCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAutoLock dblock(&m_csDBConnector);
	if (!m_pDBConnector)
		return AvError_NotLogintoServer;
	try
	{
		CMyResult res = m_pDBConnector->Query("SELECT `deviceid`  FROM `devices`");
		if (res.RowCount() < 1)
			return AvError_DeviceNotExist;
		string strDeviceList;
		do 
		{
			strDeviceList += (char *)res["deviceid"];
			strDeviceList += ';';
		} while (++res);
		*nDeviceCount = res.RowCount();

		BSTR bstrDevice = ::SysAllocString(_UnicodeString(strDeviceList.c_str(),CP_ACP));

		if (bstrDevice == NULL)
			return AvError_InsufficentMemory;
		*pstrDeviceList = bstrDevice;
		return 0;
	}
	catch (CMySQLException& e)
	{
		m_pDBConnector.reset();
		return AvError_DBException;

	}
	catch (...)
	{
		m_pDBConnector.reset();
		return AvError_UnknownException;
	}

	return 0;
}

bool CAVPlayerCtrl::LoadConfigure()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	TCHAR szTempPath[MAX_PATH] = { 0 };
	// 取得控件加载路径
	GetModuleFileName(theApp.m_hInstance, szTempPath, MAX_PATH);
	int nPos = _tcsReserverFind(szTempPath, _T('\\'));
	_tcsncpy_s(szPath, MAX_PATH, szTempPath, nPos);
	_tcscat_s(szPath, MAX_PATH, _T("\\Configuration.xml"));
	CMarkup xml;
	if (PathFileExists(szPath) && xml.Load(szPath))
	{
		// 配置文件的格式
		/*
		<?xml version="1.0" encoding="utf-8"?>
		<Configuration AS300="true">
		<CameraPostion Enable ="true"/>
		<LogManager SaveDays ="30"/>
		<CameraList>
		<Camera IP="192.168.1.26" User="root" Password="" Url="" Port =""/>
		<Camera IP="192.168.1.30" User="root" Password="" Url="" Port =""/>
		</CameraList>
		</Configuration>
		*/

		CString strValue;
		if (xml.FindElem(_T("Configuration")))
		{
			strValue = xml.GetAttrib(_T("AS300"));
			if (strValue.CompareNoCase(_T("false")) == 0)
				m_bEnableCameraPostion = false;
			else
				m_bEnableCameraPostion = true;

			if (xml.FindChildElem(_T("CameraList")))
			{
				xml.IntoElem();
				while (xml.FindChildElem(_T("Camera")))
				{
					xml.IntoElem();
					CameraUrlPtr pCamera = make_shared<CameraUrl>();
					CString strIP = xml.GetAttrib(_T("IP"));
					pCamera->strAccount = xml.GetAttrib(_T("User"));
					pCamera->strPassword = xml.GetAttrib(_T("Password"));
					pCamera->strURL = xml.GetAttrib(_T("Url"));
					pCamera->nPort = (WORD)_tcstolong(xml.GetAttrib(_T("Port")));
					m_mapCameraUrl.insert(pair<CString, CameraUrlPtr>(strIP, pCamera));
					if (m_pRunlog)
						m_pRunlog->Runlog(_T("%s CameraIP = %s\tAccount = %s\tPassword = %s\tURL = %s.\n"), __FUNCTIONW__, strIP, pCamera->strAccount, pCamera->strPassword, pCamera->strURL);

					xml.OutOfElem();
				}
				xml.OutOfElem();
			}
			if (xml.FindChildElem(_T("CameraPostion")))
			{
				xml.IntoElem();
				strValue = xml.GetAttrib(_T("Enable"));
				xml.OutOfElem();
				if (strValue.CompareNoCase(_T("true")) == 0)
				{
					m_bEnableCameraPostion = true;
				}
			}
			if (xml.FindChildElem(_T("LogManager")))
			{
				xml.IntoElem();
				m_nLogSaveDays = _tcstolong((LPCTSTR)xml.GetAttrib(_T("SaveDays")));
				if (m_nLogSaveDays < 7)
					m_nLogSaveDays = 7;
				xml.OutOfElem();
			}

			if (xml.FindChildElem(_T("AsyncPlayer")))
			{
				xml.IntoElem();
				m_nYUVFrameCacheSize = _tcstolong((LPCTSTR)xml.GetAttrib(_T("YUVFrameCacheSize")));
				if (m_nYUVFrameCacheSize <= 25)
					m_nLogSaveDays = 25;
				xml.OutOfElem();
			}
		}
		return true;
	}
	return false;
	
}

void CAVPlayerCtrl::PausePlay(LPCTSTR strDeviceID, LONG hWnd)
{
	if (!strDeviceID)
		return;

	char szDeviceID[32] = { 0 };
	W2AHelper(strDeviceID, szDeviceID, 32);
	CAutoLock lock(&m_csMapConnection);
	map<string, IPCConnectionPtr>::iterator itFind = m_MapConnection.find(szDeviceID);
	if (itFind != m_MapConnection.end())
	{
		IPC_PLAYHANDLE  hPlayer = itFind->second->hPlayhandle;
// 		if (!hWnd)			// 停止所有窗口显示, 关闭播放器
// 		{
// 			EnterCriticalSection(&m_csMapConnection);
// 			m_MapSession.erase(itFind->second->m_nPlaySession);
// 			LeaveCriticalSection(&m_csMapConnection);
// 			itFind->second.reset();
// 			m_MapConnection.erase(itFind);
// 			if (m_pRunlog)
// 			{
// 				m_pRunlog->Runlog(_T("%s Device %s Play handle is freed.\n"), __FUNCTIONW__, strDeviceID);
// 				m_pRunlog->Runlog(_T("%s MapConnection size = %d.\n"), __FUNCTIONW__, m_MapConnection.size());
// 			}
// 		}
// 		else
		{
			ipcplay_RemoveWindow(hPlayer, (HWND)hWnd);
			::InvalidateRect((HWND)hWnd, NULL, true);
// 			int nWndCount = 16;
// 			HWND hWndArray[16] = { 0 };
// 			int nStatus = ipcplay_GetRenderWindows(hPlayer, hWndArray, nWndCount);
// 			if (m_pRunlog)
// 				m_pRunlog->Runlog(_T("%s Device %s (%08X) Is stopped on Windows %08X (WndCount = %d).\n"), __FUNCTIONW__, strDeviceID, (long)hPlayer, hWnd, nWndCount);
// 			if (nStatus == IPC_Succeed  &&
// 				nWndCount == 0)
// 			{
// 				EnterCriticalSection(&m_csMapConnection);
// 				m_MapSession.erase(itFind->second->m_nPlaySession);
// 				LeaveCriticalSection(&m_csMapConnection);
// 				itFind->second.reset();
// 				lock.Lock();
// 				m_MapConnection.erase(itFind);
// 				if (m_pRunlog)
// 				{
// 					m_pRunlog->Runlog(_T("%s Device %s (%08X) Play handle is freed.\n"), __FUNCTIONW__, strDeviceID, (long)hPlayer);
// 					m_pRunlog->Runlog(_T("%s MapConnection size = %d.\n"), __FUNCTIONW__, m_MapConnection.size());
// 				}
// 			}
		}
	}
	else if (m_pRunlog)
	{
		m_pRunlog->Runlog(_T("%s device %s Is not playing.\n"), __FUNCTIONW__, strDeviceID);
		m_pRunlog->Runlog(_T("%s MapConnection size = %d.\n"), __FUNCTIONW__, m_MapConnection.size());
	}
}

void CAVPlayerCtrl::StopPlay(LPCTSTR strDeviceID,LONG hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!strDeviceID)
		return ;

	char szDeviceID[32] = { 0 };
	W2AHelper(strDeviceID, szDeviceID, 32);
	CAutoLock lock(&m_csMapConnection);
	map<string, IPCConnectionPtr>::iterator itFind = m_MapConnection.find(szDeviceID);
	if (itFind != m_MapConnection.end())
	{
		IPC_PLAYHANDLE  hPlayer = itFind->second->hPlayhandle;
		if (hPlayer)
		{
			if (!hWnd)			// 停止所有窗口显示, 关闭播放器
			{
				EnterCriticalSection(&m_csMapSession);
				m_MapSession.erase(itFind->second->m_nPlaySession);
				LeaveCriticalSection(&m_csMapSession);
				itFind->second.reset();
				m_MapConnection.erase(itFind);
				if (m_pRunlog)
				{
					m_pRunlog->Runlog(_T("%s Device %s Play handle is freed.\n"), __FUNCTIONW__, strDeviceID);
					m_pRunlog->Runlog(_T("%s MapConnection size = %d.\n"), __FUNCTIONW__, m_MapConnection.size());
				}
			}
			else
			{
				ipcplay_RemoveWindow(hPlayer, (HWND)hWnd);
				::InvalidateRect((HWND)hWnd, NULL, true);
				int nWndCount = 16;
				HWND hWndArray[16] = { 0 };
				int nStatus = ipcplay_GetRenderWindows(hPlayer, hWndArray, nWndCount);
				if (m_pRunlog)
					m_pRunlog->Runlog(_T("%s Device %s (%08X) Is stopped on Windows %08X (WndCount = %d).\n"), __FUNCTIONW__, strDeviceID, (long)hPlayer, hWnd, nWndCount);
				if (nStatus == IPC_Succeed  &&
					nWndCount == 0)
				{
					EnterCriticalSection(&m_csMapSession);
					m_MapSession.erase(itFind->second->m_nPlaySession);
					LeaveCriticalSection(&m_csMapSession);
					itFind->second.reset();
					//lock.Lock();
					m_MapConnection.erase(itFind);
					if (m_pRunlog)
					{
						m_pRunlog->Runlog(_T("%s Device %s (%08X) Play handle is freed.\n"), __FUNCTIONW__, strDeviceID, (long)hPlayer);
						m_pRunlog->Runlog(_T("%s MapConnection size = %d.\n"), __FUNCTIONW__, m_MapConnection.size());
					}
				}
			}
		}
		else
		{
			EnterCriticalSection(&m_csMapSession);
			m_MapSession.erase(itFind->second->m_nPlaySession);
			LeaveCriticalSection(&m_csMapSession);
			itFind->second.reset();
			//lock.Lock();
			m_MapConnection.erase(itFind);
			if (m_pRunlog)
			{
				m_pRunlog->Runlog(_T("%s Device %s (%08X) Play handle is freed.\n"), __FUNCTIONW__, strDeviceID, (long)hPlayer);
				m_pRunlog->Runlog(_T("%s MapConnection size = %d.\n"), __FUNCTIONW__, m_MapConnection.size());
			}
		}
	}
	else if (m_pRunlog) 
	{
		m_pRunlog->Runlog(_T("%s device %s Is not playing.\n"),__FUNCTIONW__,strDeviceID);
		m_pRunlog->Runlog(_T("%s MapConnection size = %d.\n"),__FUNCTIONW__,m_MapConnection.size());
	}
}

LONG CAVPlayerCtrl::GetRecvTimeout(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_nRecvTimeout;
}

void CAVPlayerCtrl::SetRecvTimeout(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (newVal <= 1000)
		m_nRecvTimeout = 1000;
	else
		m_nRecvTimeout = newVal;

	if (m_pRunlog) 
		m_pRunlog->Runlog(_T("%s m_nRecvTimeout = %d.\n"),__FUNCTIONW__,m_nRecvTimeout);
	SetModifiedFlag();
}

LONG CAVPlayerCtrl::GetReportInterval(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_nReConnectInterval;
}

void CAVPlayerCtrl::SetReportInterval(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (newVal <= 1000)
		m_nReConnectInterval = 1000;
	else
		m_nReConnectInterval = newVal;

	if (m_pRunlog) 
		m_pRunlog->Runlog(_T("%s m_nReportInterval = %d.\n"),__FUNCTIONW__,m_nReConnectInterval);
	SetModifiedFlag();
}

LONG CAVPlayerCtrl::GetErrorMessage(LONG nErrorCode, LPCTSTR strErrorMessage, LONG nBufferSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CString strErrMsg = L"";
	if (nErrorCode == 0)
	{
		strErrMsg = L"Succeed.";
	}
	else if (nErrorCode < 0 && nErrorCode >-1000)
	{
		return ipcplay_GetErrorMessageW(nErrorCode, (WCHAR *)strErrorMessage, nBufferSize);
	}
	else if (nErrorCode < -1000 && nErrorCode >= -2000)
	{
	switch(nErrorCode)
		{
		case	AvError_Succeed:
			strErrMsg = L"Succeed.";
			break;				
		case	AvError_InvalidParameters:
			strErrMsg = L"Invalid Parameters.";
			break;
		case	AvError_Invalid_ServerAddress:
			strErrMsg = L"Invalid Servere address.";	
			break;
		case	AvError_Invliad_ServerPort:
			strErrMsg = L"Invliad Server port.";	
			break;
		case	AvError_Invalid_AccountOrPassword:		
			strErrMsg = L"Account or password is Invliad.";	
			break;
		case	AvError_ConnectServerFailed:		
			strErrMsg = L"Failed in connect Server.";	
			break;
		case	AvError_LoginFailed:
			strErrMsg = L"Failed in login to the server.";	
			break;
		case	AvError_NotLogintoServer:
			strErrMsg = L"Not login to the Server.";	
			break;
		case	AvError_InvalidWindow:
			strErrMsg = L"The handle of window to play video is invalid.";	
			break;
		case	AvError_DeviceNotExist:
			strErrMsg = L"The sepecified device it not exist.";
			break;
		case	AvError_ConnectDeviceFailed:
			strErrMsg = L"Failed in connect the sepecified device.";	
			break;
		case	AvError_DeviceInPlaying	:
			strErrMsg = L"The sepecified device is being played now,please don't play again.";	
			break;
		case	AvError_DBException:
			strErrMsg = L"An exception occured when access database.";
			break;
		case AvError_DeviceNotInPlaying:
			strErrMsg = L"The device is not in playing.";
			break;
		case AVError_BufferOverflow:
			strErrMsg = L"Buffer over flow." ;// 缓存溢出,提供的内存空间不足以容纳所请求的数据";
			break;
		case AvError_WindowNotPlaying:
			strErrMsg = L"There is no any devices been played on the windows."; //窗口尚未播放任何设备";
			break;
		case AvError_InvlaidPtzCommand:
			strErrMsg = L"Invalid PTZ Command.";
			break;
		case AvError_Invalid_PtzValue:
			strErrMsg = L"Invalid PTZ Value.";
			break;
		case AvError_LoadAssitConfigFailed:
			strErrMsg = L"Failed to load OperationAssist.xml.";
			break;
		case AvError_DeivceNotConfigAssist:
			strErrMsg = L"The Device is not specified in the OperationAssist.xml.";
			break;
		case AvError_FailedEnableAssist:
			strErrMsg = L"Failed in enable Operation Assist.";
			break;
		case AvError_Crane_notExist:
			strErrMsg = L"The specified Crane ID is not exist.";
			break;
		case AvError_ScreenMode_notExist:
			strErrMsg = L"The specified mode ID is not exist.";
			break;
		case AvError_OutofPlayingRange:
			strErrMsg = L"Out of Player Timer Ranage";
			break;
		case AvError_AS300ServiceIsDisabled:
			strErrMsg = L"AS300 Service is Disabled,Please Edit Configure.xml to active it.";
			break;
		case AvError_ExternalError:
			strErrMsg = L"External error.";
			break;
		case	AvError_InsufficentMemory:
			strErrMsg = L"Insufficent Memory.";	
			break;
		case	AvError_UnknownException:
			strErrMsg = L"Unknown Exception.";
			break;
		default:
			strErrMsg = L"Can't locate the error code.";
			break;
		}
	}
	else if (nErrorCode < -2000 && nErrorCode > -3000)
	{

	}
	
	wcscpy_s((WCHAR*)strErrorMessage, nBufferSize, (LPCTSTR)strErrMsg);

	return 0;
}

void CAVPlayerCtrl::FreeString(BSTR* strString)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
// 	if (strString)
// 	{
// 		SysFreeString(*strString);
// 		*strString = NULL;
// 	}
	return;
}

int CAVPlayerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	OnActivateInPlace(TRUE, NULL);
	InitializeCriticalSection(&m_csDBConnector);
	InitializeCriticalSection(&m_csMapConnection);
	InitializeCriticalSection(&m_csMapSession);
	InitializeCriticalSection(&m_csOperationAssist);
	TCHAR szLog[128] = {0};
	_stprintf(szLog,_T("AvPlayer_%08X_"),GetSafeHwnd());
	//m_pRunlog = make_shared<CRunlog>(_T("AvPlayer"));
	m_pRunlog = shared_ptr<CRunlog>(new CRunlog(szLog));
	int nSize = m_MapConnection.size();
	// 清理日志，默认只保存30天日志
	LogManager();
	LoadConfigure();
#ifndef _DEBUG
	m_nRecvTimeout = 15000;
	m_nReConnectInterval = 15000;
	// 每6小时检查一次日志
	SetTimer(ID_TIMER_DELETEFILE,6*3600*1000,NULL);
	
#else
	m_nRecvTimeout = 30000;
	SetTimer(ID_TIMER_DELETEFILE,15*1000,NULL);
	m_nReConnectInterval = 30000;
	
#endif
	unsigned int nThreadAddr = 0;
	m_strAccount = _T("");
	m_strServerIP = _T("");
	m_nLoginID = -1;

	ShowWindow(SW_SHOW);
	
	// 初始化AS300客户端SDK
	if (m_bEnableAS300)
	{
		SDK_CUInit();
		SDK_CUSetDevStatusCallback(DevStatusCallBack, this);
		SDK_CUSetEventCallback(EventInfoCallback, this);
	}

	SetControlSize(48, 48);

// 	m_pVideoFrame = new CVideoFrame;
// 	CRect rtClient;
// 	GetClientRect(&rtClient);
// 	//rtClient.DeflateRect(20, 20);
// 	m_pVideoFrame->Create(1024, rtClient, 4, this);

// 	m_bThreadCheckRecvTimeRun = true;
// 	m_hThreadCheckRecvTime = (HANDLE)_beginthreadex(nullptr,0,ThreadCheckRecvTime,this,0,&nThreadAddr);
// 	if (!m_hThreadCheckRecvTime && 
// 		m_pRunlog)
// 	{
// 		m_pRunlog->Runlog(_T("%s Failed in Createing ThreadCheckRecvTime.\n"),__FUNCTIONW__);
// 	}
	return 0;
}

LONG CAVPlayerCtrl::GetDeviceWindow(LPCTSTR strDeviceID, LONG* hWndArray, LONG* nArraySize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!strDeviceID || !hWndArray || !nArraySize)
		return AvError_InvalidParameters;

	CAutoLock lock(&m_csMapConnection);
	HWND hWndArray1[16] = {0};
	int nArraySize1 = 16;
	map<string,IPCConnectionPtr>::iterator itFind = m_MapConnection.find(_AnsiString(strDeviceID,CP_ACP));
	if (itFind == m_MapConnection.end())	
		return AvError_DeviceNotInPlaying;
	int nIPCError = ipcplay_GetRenderWindows(itFind->second->hPlayhandle, hWndArray1, (int&)nArraySize1);
	if (nIPCError != IPC_Succeed)
	{
		return nIPCError;
	}
	if (*nArraySize < nArraySize1)
	{
		return AVError_BufferOverflow;
	}
	else
	{
		memcpy(hWndArray,hWndArray1,nArraySize1*sizeof(long));
		*nArraySize = nArraySize1;
		return AvError_Succeed;
	}
}

LONG CAVPlayerCtrl::GetWindowDevice(LONG hWnd, BSTR* strDeviceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (!strDeviceID || !hWnd)
		return AvError_InvalidParameters;
	CAutoLock lock(&m_csMapConnection);
	HWND hWndArray[16] = {0};
	int nArraySize = 16;
	for (map<string,IPCConnectionPtr>::iterator it = m_MapConnection.begin();
		it != m_MapConnection.end();
		it ++)
	{
		nArraySize = 16;
		if (ipcplay_GetRenderWindows(it->second->hPlayhandle,hWndArray,nArraySize) != IPC_Succeed)
			continue;

		for (int i = 0;i < nArraySize;i ++)
			if (hWnd == (long)hWndArray[i])
			{
				BSTR bstrDevice = ::SysAllocString(_UnicodeString(it->first.c_str(),CP_ACP));
				if (bstrDevice == NULL)
					return AvError_InsufficentMemory;
				*strDeviceID = bstrDevice;
				return AvError_Succeed;
			}
	}
	return AvError_WindowNotPlaying;
}

LONG CAVPlayerCtrl::SetPlayComboBorder(DOUBLE dwSpreadHei)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	/*m_pRunlog->Runlog(_T("SetPlayComboBorder Start dwSpreadHei=%f\r\n"),dwSpreadHei);
	map<string,string>::iterator ItrDevID = m_mapDeviceID.begin();
	m_pRunlog->Runlog(_T("m_mapDeviceID.size()==%d\r\n",m_mapDeviceID.size()));
	for (;ItrDevID != m_mapDeviceID.end();++ItrDevID)
	{
		string strDevID(ItrDevID->second);

		m_pRunlog->Runlog(_T("devID=%s\r\n"),_UnicodeString(strDevID.c_str(),CP_ACP));

		EnterCriticalSection(&m_csMapConnection);
		map<string, IPCConnectionPtr>::iterator itFind = m_MapConnection.find(strDevID);  //_AnsiString(szDevice1)
		if (itFind == m_MapConnection.end() )
		{
			m_pRunlog->Runlog(_T("m_MapConnection can not find the dev devID=%s\r\n"),_UnicodeString(strDevID.c_str(),CP_ACP));
			return AvError_InvalidParameters;
		}
		else
		{
			m_pRunlog->Runlog(_T("m_MapConnection find the dev devID=%s\r\n"),_UnicodeString(strDevID.c_str(),CP_ACP));
		}
		LeaveCriticalSection(&m_csMapConnection);

		map<string,list<PosParam>>::iterator itrFind = m_mapDevPos.find(strDevID);
		if (itrFind != m_mapDevPos.end())
		{
			m_pRunlog->Runlog(_T("m_mapDevPos find the dev devID=%s\r\n"),_UnicodeString(strDevID.c_str(),CP_ACP));

			list<PosParam> listPos = itrFind->second;
			list<PosParam>::iterator itrSub = listPos.begin();
			for (;itrSub != listPos.end();++itrSub)
			{
				PosParam PosTemp = *itrSub;
				if (dwSpreadHei >= PosTemp.fSpreadHeiMin && dwSpreadHei < PosTemp.fSpreadHeiMax)
				{
					RECT rect;
					rect.left = 0;
					rect.top = PosTemp.nTop;
					rect.right = 0;
					rect.bottom = PosTemp.nBottom;

					if(IPC_Succeed == ipcplay_SetBorderRect(itFind->second->hPlayhandle,itFind->second->m_hWnd,&rect,true))
					{
						m_pRunlog->Runlog(_T("ipcplay_SetBorderRect Successfull dwSpreadHei=%f \r\n"),dwSpreadHei);
					}
					else
					{
						m_pRunlog->Runlog(_T("ipcplay_SetBorderRect error dwSpreadHei=%f \r\n"),dwSpreadHei);
					}
				}
			}
		}

	}
	*/
	return 0;
}

LONG CAVPlayerCtrl::SetPlayComboBorderEx(LPCTSTR szDevice1, LPCTSTR szDevice2, DOUBLE fSpreadHei)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//m_pRunlog->Runlog(_T("SetPlayComboBorderEx Start dev1id=%s,dev2id=%s,dwSpreadHei=%f\r\n"),szDevice1,szDevice2,fSpreadHei);
	LPCTSTR szDeviceArray[2] = {szDevice1,szDevice2};
	for (int i =0;i<2;++i)
	{
		string strDevID(_AnsiString(szDeviceArray[i],CP_ACP));
		//m_pRunlog->Runlog(_T("devID=%s\r\n"),_UnicodeString(strDevID.c_str(),CP_ACP));
		EnterCriticalSection(&m_csMapConnection);
		map<string, IPCConnectionPtr>::iterator itFind = m_MapConnection.find(strDevID);  //_AnsiString(szDevice1)
		if (itFind == m_MapConnection.end() )
		{
            //m_pRunlog->Runlog(_T("m_MapConnection can not find the dev devID=%s\r\n"),_UnicodeString(strDevID.c_str(),CP_ACP));
			return AvError_InvalidParameters;
		}
		else
		{
			//m_pRunlog->Runlog(_T("m_MapConnection find the dev devID=%s\r\n"),_UnicodeString(strDevID.c_str(),CP_ACP));

		}
		LeaveCriticalSection(&m_csMapConnection);

		map<string,list<PosParam>>::iterator itrFind = m_mapDevPos.find(strDevID);
		if (itrFind != m_mapDevPos.end())
		{
			list<PosParam> listPos = itrFind->second;

			//m_pRunlog->Runlog(_T("%s range list.size()=%d\r\n"),_UnicodeString(strDevID.c_str(),CP_ACP),listPos.size());

			list<PosParam>::iterator itrSub = listPos.begin();

			for (;itrSub != listPos.end();++itrSub)
			{
				PosParam PosTemp = *itrSub;
				//m_pRunlog->Runlog(_T("range list find the range minHei=%f,maxHei=%f devID=%s\r\n"),PosTemp.fSpreadHeiMin,PosTemp.fSpreadHeiMax,_UnicodeString(strDevID.c_str(),CP_ACP));
				if (fSpreadHei >= PosTemp.fSpreadHeiMin && fSpreadHei < PosTemp.fSpreadHeiMax)
				{
					RECT rect;
					rect.left = 0;
					rect.top = PosTemp.nTop;
					rect.right = 0;
					rect.bottom = PosTemp.nBottom;
					if(IPC_Succeed == ipcplay_SetBorderRect(itFind->second->hPlayhandle,itFind->second->m_hWnd,&rect,true))
					{
						//m_pRunlog->Runlog(_T("ipcplay_SetBorderRect Successfull dwSpreadHei=%f \r\n"),fSpreadHei);
						break;
						
					}
					else
					{
						//m_pRunlog->Runlog(_T("ipcplay_SetBorderRect error dwSpreadHei=%f \r\n"),fSpreadHei);
					}
				}
			}
		}
		else
		{
			//m_pRunlog->Runlog(_T("m_mapDevPos is null,size=%d\r\n"),m_mapDevPos.size());

		}
	}

	//m_pRunlog->Runlog(_T("SetPlayComboBorderEx End\r\n"));

	return 0;
}

LONG CAVPlayerCtrl::SendPtzCommand(LPCTSTR strDeviceID, LONG nPtzCommand,LONG nPtzValue1,LONG nPtzValue2,LONG nOpType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!strDeviceID)
		return AvError_InvalidParameters;
	if (nPtzCommand < 0 || nPtzCommand > Ptz_BackLight)
		return AvError_InvlaidPtzCommand;
	CAutoLock dblock(&m_csDBConnector);
	if (!m_pDBConnector)
		return AvError_NotLogintoServer;;
	CMyResult res = m_pDBConnector->Query("SELECT `ipaddress`,`port`,`loginname`,`loginpasswd` FROM `devices` where `deviceid` = '%s'", _AnsiString(strDeviceID,CP_ACP));
	if (res.RowCount() < 1)
	{
		if (m_pRunlog) 
			m_pRunlog->Runlog(_T("%s can't find the mismatched device with ID:%s!\n"),__FUNCTIONW__,strDeviceID);
		return AvError_DeviceNotExist;
	}
	char *szIP	 = res["ipaddress"];
	WORD nPort	 = res["port"];
	char *szUser = res["loginname"];
	char *szPass = res["loginpasswd"];

	char szValue[128] = {0};
	CHAR szURL1[1024] = {0};
	TCHAR szCameraW[32] = {0};
	CHAR szCameraA[32] = {0};
	char *szPtzfmt= "http://%s/axis-cgi/com/ptz.cgi?%s=%s";
	char szOperation[64] = {0};

	PtzCode nCode = (PtzCode)nPtzCommand;
	switch(nCode)
	{
	case Ptz_Center:
		{
			strcpy(szOperation,"center");
			sprintf_s(szValue,"%d,%d",nPtzValue1,nPtzValue2);
			break;
		}
	case Ptz_Areazoom:
		{
			strcpy(szOperation,"areazoom");
			sprintf_s(szValue,"%d,%d,%d",(short)LOWORD(nPtzValue1),(short)HIWORD(nPtzValue1),nPtzValue2);
			break;
		}
	case Ptz_ImageWidth:
		{
			strcpy(szOperation,"imagewidth");
			strcpy(szValue,"1");
			break;
		}
	case Ptz_ImageHeight:
		{
			strcpy(szOperation,"imageheight");
			strcpy(szValue,"1");
			break;
		}
	case Ptz_Move:
		{
			strcpy(szOperation,"move");
			Ptz_MoveOption nMovOption = (Ptz_MoveOption)nPtzValue1;
			char *szMoveOption[] = {"home","up","down","left","right","upleft","upright","downleft","downright","stop"};
			if (nPtzValue1 >=0 && nPtzValue1 <= sizeof(szMoveOption)/sizeof(char*))
			{
				strcpy(szValue,szMoveOption[nPtzValue1]);
			}
			else
				return AvError_Invalid_PtzValue;
			break;
		}
	// 以下操作皆存在绝对和相对两种模式
	case Ptz_Pan:	// 水平转动
		{
			if (nOpType == Opt_Relative)
				strcpy(szOperation,"rpan");
			else if (nOpType == Opt_Abstract)
				strcpy(szOperation,"pan");
			sprintf_s(szValue,"%d",nPtzValue1);
			break;
		}
	case Ptz_Tilt:	// 垂直转动
		{
			if (nOpType == Opt_Relative)
				strcpy(szOperation,"rtilt");
			else if (nOpType == Opt_Abstract)
				strcpy(szOperation,"tilt");
			sprintf_s(szValue,"%d",nPtzValue1);
			break;
		}
	case Ptz_Zoom:
		{
			if (nOpType == Opt_Relative)
				strcpy(szOperation,"rzoom");
			else if (nOpType == Opt_Abstract)
				strcpy(szOperation,"zoom");
			sprintf_s(szValue,"%d",nPtzValue1);
			break;
		}
	case Ptz_Focus:
		{
			if (nOpType == Opt_Relative)
				strcpy(szOperation,"rfocus");
			else if (nOpType == Opt_Abstract)
				strcpy(szOperation,"focus");
			sprintf_s(szValue,"%d",nPtzValue1);
			break;
		}
	case Ptz_Iris:
		{
			if (nOpType == Opt_Relative)
				strcpy(szOperation,"riris");
			else if (nOpType == Opt_Abstract)
				strcpy(szOperation,"iris");
			sprintf_s(szValue,"%d",nPtzValue1);
			break;
		}
	case Ptz_Brightness:
		{
			if (nOpType == Opt_Relative)
				strcpy(szOperation,"rbrightness");
			else if (nOpType == Opt_Abstract)
				strcpy(szOperation,"brightness");
			sprintf_s(szValue,"%d",nPtzValue1);
			break;
		}

	case Ptz_AutoFocus:
		{
			strcpy(szOperation,"autofocus");
			if (nPtzValue1 == 0)
				strcpy(szValue,"off");
			else 
				strcpy(szValue,"on");
			break;
		}
	case Ptz_AutoIris:
		{
			strcpy(szOperation,"autoiris");
			if (nPtzValue1 == 0)
				strcpy(szValue,"off");
			else 
				strcpy(szValue,"on");
			break;
		}
	case Ptz_ContinuousPanTiltMove:
		{
			strcpy(szOperation,"continuouspantiltmove");
			sprintf_s(szValue,"%d,%d",nPtzValue1,nPtzValue2);
			break;
		}
	case Ptz_ContinuousFocusMove:
		{
			strcpy(szOperation,"continuousfocusmove");
			sprintf_s(szValue,"%d",nPtzValue1);
			break;
		}
	case Ptz_ContinuousZoomMove:
		{
			strcpy(szOperation,"continuouszoommove");
			sprintf_s(szValue,"%d",nPtzValue1);
			break;
		}
	case Ptz_ContinuousIrisMove:
		{
			strcpy(szOperation,"continuousirismove");
			sprintf_s(szValue,"%d",nPtzValue1);
			break;
		}
	case Ptz_ContinuousBrightnessMove:
		{
			strcpy(szOperation,"continuousbrightnessmove");
			sprintf_s(szValue,"%d",nPtzValue1);
			break;
		}
	case Ptz_GogoServerPresetNo:
		{
			strcpy(szOperation,"gotoserverpresetno");
			sprintf_s(szValue,"%d",nPtzValue1);
			break;
		}
	case Ptz_Speed:
		{
			strcpy(szOperation,"speed");
			sprintf_s(szValue,"%d",nPtzValue1);
			break;
		}
	case Ptz_IrcutFilter:
		{
			strcpy(szOperation,"ircutfilter");
			if (nPtzValue1 == 0)
				strcpy(szValue,"off");
			else if (nPtzValue1 == 1)
				strcpy(szValue,"on");
			else if(nPtzValue1 > 1)
				strcpy(szValue,"auto");
			else
				return AvError_Invalid_PtzValue;
			break;
		}
	case Ptz_BackLight:
		{
			strcpy(szOperation,"backlight");
			if (nPtzValue1 == 0)
				strcpy(szValue,"off");
			else if (nPtzValue1 == 1)
				strcpy(szValue,"on");
			break;
		}
	default:
		{
			break;
		}
	}
	
	sprintf(szURL1,szPtzfmt,szIP,szOperation,szValue);
	TraceMsgA("%s Ptz URL = %s.\n",__FUNCTION__,szURL1);
	StartCGI(szURL1,szIP,80,szUser,szPass);
	return 0;
}

LONG CAVPlayerCtrl::EnalbeCameraPostion(LONG bEnalbeFlag)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_bEnalbeCameraPostion = bEnalbeFlag;
	return 0;
}

void CAVPlayerCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == ID_TIMER_DELETEFILE)
	{
		LogManager();
	}
	COleControl::OnTimer(nIDEvent);
}

LONG CAVPlayerCtrl::SetExternDCDraw(LPCTSTR szDeviceID, LONG pDCDraw, LONG pUserPtr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TraceMsgW(_T("%s Device = %s\tpDCDraw = %08X.\n"), __FUNCTION__, szDeviceID, pDCDraw);
	if (!szDeviceID)
		return AvError_InvalidParameters;
	CAutoLock dblock(&m_csDBConnector);
	if (!m_pDBConnector)
		return AvError_NotLogintoServer;
	string strDeviceID = _AnsiString(szDeviceID, CP_ACP);
	CAutoLock lock(&m_csMapConnection);
	auto itFind = m_MapConnection.find(strDeviceID);
	if (itFind == m_MapConnection.end())
	{
		return AvError_DeviceNotInPlaying;
	}

	return itFind->second->SetExternDCDraw((void *)pDCDraw, (void *)pUserPtr);
}

void   CAVPlayerCtrl::OnDevStatus(char* szDevId, int nStatus)
{
}

void   CAVPlayerCtrl::OnAS300LiveData(LONG nSessionId, char* pBuffer, int nLen)
{
	IPCConnectionPtr pConnection = m_MapSession[nSessionId];
	if (pConnection)
	{
		if (!pConnection->m_bIPCStart && (pConnection->m_nFrameLength + nLen) < pConnection->m_nBufferSize)
		{
			memcpy(&pConnection->m_pFrameBuffer[pConnection->m_nFrameLength], pBuffer, nLen);
			pConnection->m_nFrameLength += nLen;
			if (pConnection->m_nFrameLength >= 64)
			{
				int nWidth = 0, nHeight = 0, nFramerate = 0;
				int nOffsetStart = 0;
				int nOffsetMatched = 0;
				if (Stream_Analyse(&pConnection->m_pFrameBuffer[nOffsetStart], pConnection->m_nFrameLength, &nWidth, &nHeight, &nFramerate, &nOffsetMatched) != -1)
				{
					IPC_MEDIAINFO MediaHeader;
					MediaHeader.nVideoCodec = CODEC_H264;
					MediaHeader.nAudioCodec = CODEC_UNKNOWN;
					MediaHeader.nVideoWidth = nWidth;
					MediaHeader.nVideoHeight = nHeight;
					MediaHeader.nFps = 25;
					ipcplay_SetStreamHeader(pConnection->hPlayhandle, (byte *)&MediaHeader, sizeof(IPC_MEDIAINFO));
					ipcplay_EnableStreamParser(pConnection->hPlayhandle, CODEC_H264);
					ipcplay_Start(pConnection->hPlayhandle);
					pConnection->m_bIPCStart = true;
				}
			}
		}
		if (pConnection->hPlayhandle)
			ipcplay_InputStream2(pConnection->hPlayhandle , (byte *)pBuffer, nLen);
	}
}

bool   CAVPlayerCtrl::OnAS300PlayBack(LONG nSessionId, char* pBuffer, int nLength)
{
	IPCConnectionPtr pConnection = m_MapSession[nSessionId];
	int nResult = -1;
	
	if (pConnection)
	{
		if (!pConnection->pPlayStatus)
			return false;
		
		if (!pConnection->pPlayStatus->pStreamParser)
			return false;
		PlayBackStatusPtr pPlayStatus = pConnection->pPlayStatus;
		DhStreamParser* pParser = pPlayStatus->pStreamParser;
		if (pParser->InputData((byte *)pBuffer, nLength) == 0)
		{
			while (true)
			{
				DH_FRAME_INFO *pFrame = pParser->GetNextFrame();
				if (!pFrame)
					break;
				else if (!pConnection->m_bIPCStart)
				{

					IPC_MEDIAINFO MediaHeader;
					MediaHeader.nVideoCodec = CODEC_H264;
					MediaHeader.nAudioCodec = CODEC_UNKNOWN;
					MediaHeader.nVideoWidth = pFrame->nWidth;
					MediaHeader.nVideoHeight = pFrame->nHeight;
					MediaHeader.nFps = pFrame->nFrameRate;
					ipcplay_SetStreamHeader(pConnection->hPlayhandle, (byte *)&MediaHeader, sizeof(IPC_MEDIAINFO));
					//ipcplay_EnableAsyncRender(pConnection->hPlayhandle);
					//ipcplay_EnablePlayOneFrame(pConnection->hPlayhandle);
					ipcplay_Start(pConnection->hPlayhandle);
					pConnection->m_bIPCStart = true;

				}
				
				if (pFrame->nType == DH_FRAME_TYPE_VIDEO)
				{
					time_t tTimeStamp = pFrame->nTimeStamp;
					
					int nFrameInterval = 40;
					tTimeStamp *= 1000;
					if (pFrame->nFrameRate)
						nFrameInterval = 1000 / pFrame->nFrameRate;
					else
						nFrameInterval = 40;
					
					if (pFrame->nSubType == DH_FRAME_TYPE_VIDEO_I_FRAME)
						pPlayStatus->tFrameTimeStamp = tTimeStamp;
					else
						pPlayStatus->tFrameTimeStamp += nFrameInterval;

					do
					{
						nResult = ipcplay_InputIPCStream(pConnection->hPlayhandle, pFrame->pContent,
							pFrame->nSubType == DH_FRAME_TYPE_VIDEO_I_FRAME ? IPC_I_FRAME : IPC_P_FRAME,
							pFrame->nLength,
							pFrame->nRequence,
							pPlayStatus->tFrameTimeStamp);
						if (IPC_Succeed == nResult)
							break;
						else if (IPC_Error_FrameCacheIsFulled == nResult)
						{
							Sleep(10);
							continue;
						}
					} while (true);
				}
			}
		}

	}
	return (IPC_Succeed ==nResult);
}

void   CAVPlayerCtrl::OnAS300Event(long nEventType, char* szId, int nParam1, int nParam2)
{
	EventCallbackType nType = (EventCallbackType)nEventType;
	switch (nType)
	{
	case EVENT_CMS_DISCONNECTED:	//服务器断线，此时应该清除所有的设备信息，重新登录服务器时，会通知外部重新获取设备列表
	{
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s EVENT_CMS_DISCONNECTED:cms disconnected!\n"),__FUNCTIONW__);
		EnumCmsDisconnected nsubType = (EnumCmsDisconnected)nParam1;
		
		if (nsubType == Disconnect_usererror)//用户被删除或者锁定，如果想登陆300服务器，需更换用户重新登录。
		{
			if (m_pRunlog)
				m_pRunlog->Runlog(_T("%s EVENT_CMS_DISCONNECTED:The user is locked or deleted!\n"), __FUNCTIONW__);
		}
	}
	break;
	case EVENT_CMS_RECONNECT:
	{
		int nResult = nParam1;
		if (nParam1 == 0)	//服务器重连失败
		{
			if (m_pRunlog)
				m_pRunlog->Runlog(_T("%s EVENT_CMS_RECONNECT:The relogin server error!\n"), __FUNCTIONW__);
		}
		else
		{
			if (m_pRunlog)
				m_pRunlog->Runlog(_T("%s EVENT_CMS_RECONNECT:The relogin server ok!\n"), __FUNCTIONW__);
		}
	}
	break;
	case EVENT_RELOAD_DEVICE://在服务器更改组权限之类的操作之后，需要将设备重新获取。
	{
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s EVENT_RELOAD_DEVICE:reload device!\n"), __FUNCTIONW__);
	}
	break;
	case EVENT_DEL_DEVICE:	//删除设备
	{
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s EVENT_RELOAD_DEVICE:del device!\n"), __FUNCTIONW__);
			//这边只需要停止解码，SDK内部自己会销毁所有的链接
	}
	break;
	case EVENT_VIDEO_EXCEPTION:	//视频异常时
	{
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s EVENT_VIDEO_EXCEPTION!\n"),__FUNCTIONW__);
		
		CString strErrorMsg;
		switch (nParam2)
		{
		case SDK_Error_Video_Opened:	// _EC(20) 视频已打开
			strErrorMsg = _T("The video has been opened.");
			break;
		case SDK_Error_Rtsp_Connect_Failed:	// _EC(21) 建立RTSP连接失败
			strErrorMsg = _T("RTSP Connect fail.");
			break;
		case SDK_Error_Rtsp_Ack_Failed:		// _EC(22) RTSP应答失败
			strErrorMsg = _T("RTSP Respond fail.");
			break;
		case SDK_Error_Rtp_Listen_Failed:	// _EC(23) RTP建立侦听失败
			strErrorMsg = _T("RTP Listen fail.");
			break;
		case SDK_Error_Rtp_Connect_Failed:	// _EC(24) 建立RTP连接失败
			strErrorMsg = _T("RTP Connect fail.");
			break;
		case SDK_Error_Player_Port_UseOut:	// _EC(25) 解码端口用完
			strErrorMsg = _T("Player port is run out.");
			break;
		case SDK_Error_No_Right:			// _EC(26) 无权限
			strErrorMsg = _T("Access denied.");
			break;
		case SDK_Error_Not_Found_Device:	// _EC(27) 找不到对应的设备
			strErrorMsg = _T("The device is not exist.");
			break;
		case SDK_Error_Device_Offline:		// _EC(28) 设备离线
			strErrorMsg = _T("The device is offline.");
			break;
		case SDK_Error_LoginDev_Failed:		// _EC(29) 设备登录失败
			strErrorMsg = _T("Failed in login device.");
			break;
		case SDK_Error_No_DevVisitorRight:	// _EC(30) 没有直连权限
			strErrorMsg = _T("No Device access right.");
			break;
		case SDK_Error_CMS_No_MTS_To_Use:	// _EC(32) 没有MTS可用
			strErrorMsg = _T("There is not any available MTS Server.");
			break;
		case SDK_Error_CMS_No_Device:		// _EC(33) 找不到设备
			strErrorMsg = _T("There is no device in CMS Server.");
			break;
		case SDK_Error_CMS_Media_Session_Exist:// _EC(34) 媒体会话已存在
			strErrorMsg = _T("There is already a media session of device in CMS .");
			break;
		case SDK_Error_CMS_Server_Reconnect:	// _EC(35) 功能服务正在重连
			strErrorMsg = _T("The CMS Server is reconnecting.");
			break;
		case SDK_Error_CMS_No_Right:			// _EC(36) 无权限
			strErrorMsg = _T("Access denied by CMS.");
			break;
		case SDK_Error_CMS_Not_Find_Child_Domain:// _EC(37) 没有找到下级域
			strErrorMsg = _T("CMS can't find the child domain.");
			break;
		case SDK_Error_CMS_Not_Login_Child_Domain:	// _EC(38) 未登陆下级域
			strErrorMsg = _T("CMS not login the child domain.");
			break;
		case SDK_Error_CMS_Child_Domain_Exception:	// _EC(39) 下级域异常
			strErrorMsg = _T("There is a exeception in CMS child domain.");
			break;
		case SDK_Error_CMS_Reach_System_Limit:		//
			strErrorMsg = _T("The connections of CMS is out ot range.");
			break;
		case SDK_Error_Chan:					//	 EC(41)			//通道数不正确(超出范围)
			strErrorMsg = _T("Channel is out of range.");
			break;
		case SDK_Error_Playback_Opened:			//	_EC(42)			//回放已打开
			strErrorMsg = _T("Playback is opened.");
			break;
		case SDK_Error_Open_File_Failed:		//	_EC(43)			//打开文件失败
			strErrorMsg = _T("Open File failed.");
			break;
		case SDK_Error_No_SS_To_Use:			//	_EC(44)			//没有SS
			strErrorMsg = _T("There is no available Storage Server");
			break;
		case SDK_Error_Playback_Limit:			//  _EC(45)			//达到回放限制
			strErrorMsg = _T("Reached the playback upper limit.");
			break;
		case SDK_Error_OtherUser_Playback:		//  _EC(46)			//其他用户正在回放
			strErrorMsg = _T("Other user is playback.");
			break;
		case SDK_Error_SSPlayback_Failed:		//	_EC(47)			//回放失败
			strErrorMsg = _T("Playback failed.");
			break;
		case SDK_Error_Rtsp_Over_Capability:	//	_EC(48)			//转发服务器超过路数
			strErrorMsg = _T("The connections of MTS is out of ranage.");
			break;
		case SDK_Error_Rtsp_Be_Kicked:			//	_EC(49)			//被高权限用户占用
			strErrorMsg = _T("Kicked by other use who owned higher privilege.");
			break;
		default:
			strErrorMsg = _T("Unkonw Error.");
			break;
		}
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s EVENT_VIDEO_EXCEPTION:%s\n"), __FUNCTIONW__, strErrorMsg);
		if (nParam2 < 0)
		{
			//这边只需要停止解码，SDK内部自己会销毁所有的链接
		}
	}
	break;
	case EVENT_PLAYBACK_STATUS:
	{
		EnumPlaybackStatus playStatus = (EnumPlaybackStatus)nParam2;
		switch (playStatus)
		{
		case Playback_SS_Unregistered:	//存储服务器断线
		{

		}
		break;
		case Playback_Right_Canceled://权限被删除
		{

		}
		break;
		case Playback_Dev_Removed:	//设备被删除
		{

		}
		break;
		case Playback_SessionRemoved:	//会话被删除，或者会话有异常
		case Playback_SessionAbnormal:
		{

		}
		break;
		case Playback_Finished:		//回放结束
		{
			
		}
		break;
		}
	}
	break;
	case EVENT_TALK_EXCEPTION:	//对讲异常，SDK内部会收回对讲相关内存，此处应用层需要将对讲的资源也回收
	{
		// char* szId, int nParam1, int nParam2
		// szId代表对讲设备ID，
		// nParam1为 EnumTalkException 对讲异常具体类型
		// nParam2为 lsessionId，对讲的句柄
	}
	break;
	}
}

void CAVPlayerCtrl::OnDestroy()
{
	COleControl::OnDestroy();
	
	EnterCriticalSection(&m_csMapConnection);
	m_MapConnection.clear();
	LeaveCriticalSection(&m_csMapConnection);

	EnterCriticalSection(&m_csMapSession);
	m_MapSession.clear();
	LeaveCriticalSection(&m_csMapSession);

	EnterCriticalSection(&m_csOperationAssist);
	m_mapOperationAssist.clear();
	LeaveCriticalSection(&m_csOperationAssist);
	if (m_bEnableAS300)
	{
		if (m_nLoginID != -1)
			SDK_CULogout(m_nLoginID);
		SDK_CUClear();
	}

	EnterCriticalSection(m_csMapDecoderPool.Get());
	for (map<string, ItemStatusList>::iterator itList = m_mapDecoderPool.begin();
		itList != m_mapDecoderPool.end();)
	{
		ItemStatusList &HandleList = itList->second;
		for (ItemStatusList::iterator it = HandleList.begin();
			it != HandleList.end();)
		{
			ipcplay_Close((*it)->pItemValue);
			it = HandleList.erase(it);
		}
		itList = m_mapDecoderPool.erase(itList);
	}
	LeaveCriticalSection(m_csMapDecoderPool.Get());
	DeleteCriticalSection(&m_csDBConnector);
	DeleteCriticalSection(&m_csMapConnection);
	DeleteCriticalSection(&m_csMapSession);
	DeleteCriticalSection(&m_csOperationAssist);
	
#ifdef _DEBUG
	TraceMsgA("%s _IPCConnection::nRefCount = %d.\n", _IPCConnection::nRefCount);
#endif

	if (m_pRunlog)
		m_pRunlog->Runlog(_T("%s The Control is destroyed.\n"), __FUNCTIONW__);
}

LONG CAVPlayerCtrl::EnableOperationAssist(LPCTSTR strDeviceID, LONG nEnable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!strDeviceID)
		return AvError_InvalidParameters;

	char szDeviceID[32] = { 0 };
	W2AHelper(strDeviceID, szDeviceID, 32);
	CAutoLock lock(&m_csMapConnection);
	auto itFind = m_MapConnection.find(szDeviceID);
	if (itFind == m_MapConnection.end())
	{
		if (m_pRunlog)
		{
			m_pRunlog->Runlog(_T("%s device %s Is not playing.\n"), __FUNCTIONW__, strDeviceID);
			m_pRunlog->Runlog(_T("%s MapConnection size = %d.\n"), __FUNCTIONW__, m_MapConnection.size());
		}
		return AvError_DeviceInPlaying;
	}
	
	IPC_PLAYHANDLE  hPlayer = itFind->second->hPlayhandle;
	CAutoLock lock2(&m_csOperationAssist);
	auto itFind2 = m_mapOperationAssist.find(strDeviceID);
	if (itFind2 == m_mapOperationAssist.end())
	{
		if (m_pRunlog)
		{
			m_pRunlog->Runlog(_T("%s device %s Is not playing.\n"), __FUNCTIONW__, strDeviceID);
			m_pRunlog->Runlog(_T("%s MapConnection size = %d.\n"), __FUNCTIONW__, m_MapConnection.size());
		}
		return AvError_DeivceNotConfigAssist;
	}
	OperationAssistArray pOpAssist = itFind2->second;
	if (nEnable)
	{
		long hPolygon = 0;
		for (auto it = pOpAssist.begin(); it != pOpAssist.end(); it++)
		{
			hPolygon = ipcplay_AddPolygon(hPlayer, (*it)->ptArray, (*it)->nCount, (*it)->nVertexIndex, (*it)->nColor);
			if (hPolygon == 0)
			{
				if (m_pRunlog)
					m_pRunlog->Runlog(_T("%s call 'ipcplay_AddPolygon' failed for device %s.\n"), __FUNCTIONW__, strDeviceID);
				return AvError_FailedEnableAssist;
			}
			(*it)->hPolygon = hPolygon;
		}
		
	}
	else
	{
		for (auto it = pOpAssist.begin(); it != pOpAssist.end(); it++)
		{
			if ((*it)->hPolygon)
			{
				ipcplay_RemovePolygon(hPlayer, (*it)->hPolygon);
				(*it)->hPolygon = 0;
			}
		}
	}
	
	return AvError_Succeed;
}

LONG CAVPlayerCtrl::LoadOpAssistConfigure()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (LoadOPAssistXConfigure())
	{
		return AvError_Succeed;
	}
	else
		return AvError_LoadAssitConfigFailed;

}

bool CAVPlayerCtrl::LoadOPAssistXConfigure()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	TCHAR szTempPath[MAX_PATH] = { 0 };
	// 取得控件加载路径
	GetModuleFileName(theApp.m_hInstance, szTempPath, MAX_PATH);
	int nPos = _tcsReserverFind(szTempPath, _T('\\'));
	_tcsncpy_s(szPath, MAX_PATH, szTempPath, nPos);
	_tcscat_s(szPath, MAX_PATH, _T("\\OperationAssist.xml"));
	CMarkup xml;
	if (PathFileExists(szPath) && xml.Load(szPath))
	{
		// 配置文件的格式
		/*
		<?xml version="1.0" encoding="gbk"?>
		<Configuration >
		<!-- 配置说明，请勿删除 -->
		<!--  -->
		<DeviceList>
		<!-- ID				为设备ID  -->
		<!-- Orientation	为方位，决定显示辅助解的方向-->
		<!-- StartX 		起始点X坐标，该坐标为图像坐标-->
		<!-- Width 			宽度 -->
		<!-- Height 		高度 -->
		<!-- ThickVerical	垂直方向厚度 -->
		<!-- ThickHorizontal 水平方向厚度 -->
		<Device ID = "33011234" Orientation = "LeftTop"  StartX = "100" StartY = "100" Width = "200" Height ="150" ThickVerical="10" ThickHorizontal = "12" />
		<Device ID = "33011234" Orientation = "RightTop"  StartX = "100" StartY = "100" Width = "200" Height ="150" ThickVerical="10" ThickHorizontal = "12" />
		<Device ID = "33011234" Orientation = "LeftBottom"  StartX = "100" StartY = "100" Width = "200" Height ="150" ThickVerical="10" ThickHorizontal = "12" />
		<Device ID = "33011234" Orientation = "RightBottom"  StartX = "100" StartY = "100" Width = "200" Height ="150" ThickVerical="10" ThickHorizontal = "12" />
		</DeviceList>
		</Configuration>
		*/

		CString strValue;
		bool bEnable = false;
		
		EnterCriticalSection(&m_csOperationAssist);
		m_mapOperationAssist.clear();
		LeaveCriticalSection(&m_csOperationAssist);

		if (xml.FindElem(_T("Configuration")))
		{
			if (xml.FindChildElem(_T("DeviceList")))
			{
				xml.IntoElem();
				while (xml.FindChildElem(_T("Device")))
				{
					xml.IntoElem();
					
					CString strID = xml.GetAttrib(_T("ID"));
					while (xml.FindChildElem(_T("Assist")))
					{
						xml.IntoElem();
						OperationAssistPtr pOAX = make_shared<OperationAssist>();
						pOAX->nOrientation		 = (Orientation)_tcstolong((LPCTSTR)xml.GetAttrib(_T("Orientation")));
						pOAX->nStartX			 = _tcstolong((LPCTSTR)xml.GetAttrib(_T("StartX")));
						pOAX->nStartY			 = _tcstolong((LPCTSTR)xml.GetAttrib(_T("StartY")));
						pOAX->nWidth				 = _tcstolong((LPCTSTR)xml.GetAttrib(_T("Width")));
						pOAX->nHeight			 = _tcstolong((LPCTSTR)xml.GetAttrib(_T("Height")));
						pOAX->nThickVerical		 = _tcstolong((LPCTSTR)xml.GetAttrib(_T("ThickVerical")));
						pOAX->nThickHorizontal	 = _tcstolong((LPCTSTR)xml.GetAttrib(_T("ThickHorizontal")));
						pOAX->nColor				 = _tcstolong((LPCTSTR)xml.GetAttrib(_T("Color")),16);
						pOAX->BuildPolygon();
						xml.OutOfElem();
						EnterCriticalSection(&m_csOperationAssist);
						auto itFind = m_mapOperationAssist.find(strID);
						if (itFind == m_mapOperationAssist.end())
						{
							OperationAssistArray opArray;
							opArray.push_back(pOAX);
							m_mapOperationAssist.insert(pair<CString, OperationAssistArray>(strID, opArray));
						}
						else
						{
							itFind->second.push_back(pOAX);
						}
						//m_mapOperationAssist.insert(pair<CString, OperationAssistPtr>(strID, pOAX));
						LeaveCriticalSection(&m_csOperationAssist);

					}
					
					TraceMsgW(_T("%s Load Device %s.\n"), __FUNCTIONW__, strID);

					xml.OutOfElem();
				}
				xml.OutOfElem();
			}
			else
			{
				if (m_pRunlog)
					m_pRunlog->Runlog(_T("%s Can't find Node 'DeviceList' in file.\n"), __FUNCTIONW__, szPath);
				return false;
			}
		}
		else
		{
			if (m_pRunlog)
				m_pRunlog->Runlog(_T("%s Can't find Node 'Configuration' in file.\n"), __FUNCTIONW__, szPath);
			return false;
		}
		return true;
	}
	else
	{
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s Can't find %s.\n"), __FUNCTIONW__, szPath);
		return false;
	}
}

bool CAVPlayerCtrl::LoadScreenMode()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	TCHAR szTempPath[MAX_PATH] = { 0 };
	// 取得控件加载路径
	GetModuleFileName(theApp.m_hInstance, szTempPath, MAX_PATH);
	int nPos = _tcsReserverFind(szTempPath, _T('\\'));
	_tcsncpy_s(szPath, MAX_PATH, szTempPath, nPos);
	_tcscat_s(szPath, MAX_PATH, _T("\\ScreenMode.xml"));
	CMarkup xml;
	if (PathFileExists(szPath) && xml.Load(szPath))
	{
		m_mapCrane.clear();
		// 配置文件的格式
		/*
		<?xml version="1.0" encoding="gbk"?>
		<Configuration >
		<ModeList CraneID = "1">
		<!-- ID 屏幕定义,对应相应的屏幕编号,值必须唯一-->
			<Mode ID="1" Camera1="3301061000002"/>
			<Mode ID="2" Camera1="3301061000003" Camera2="3301061000005"/>
			<Mode ID="3" Camera1="3301061000007" Camera2="3301061000009" Camera3="3301061000010"/>
			<Mode ID="4" Camera1="3301061000011" Camera2="3301061000012" Camera3="3301061000013" Camera4="3301061000014"/>
			<Mode ID="5" Camera1="3301061000015" Camera2="3301061000016" Camera3="3301061000017" Camera4="3301061000018" />
			<Mode ID="6" Camera1="3301061000003" Camera2="3301061000002" Camera3="3301061000005" Camera4="3301061000007" Camera5="3301061000009" Camera6="33010610000010" Camera7="3301061000020" Camera8="3301061000033" Camera9="3301061000034"/>
		</ModeList>
		<ModeList CraneID = "2">
		<!-- ID 屏幕定义,对应相应的屏幕编号,值必须唯一-->
		<Mode ID="1" Camera1="3301061000003"/>
		<Mode ID="2" Camera1="3301061000002" Camera2="3301061000005"/>
		<Mode ID="3" Camera1="3301061000007" Camera2="3301061000009" Camera3="3301061000010"/>
		<Mode ID="4" Camera1="3301061000011" Camera2="3301061000012" Camera3="3301061000013" Camera4="3301061000014"/>
		<Mode ID="5" Camera1="3301061000015" Camera2="3301061000016" Camera3="3301061000017" Camera4="3301061000018" />
		<Mode ID="6" Camera1="3301061000007" Camera2="3301061000009" Camera3="3301061000007" Camera4="3301061000005" Camera5="3301061000010" Camera6="33010610000009" Camera7="3301061000020" Camera8="3301061000033" Camera9="3301061000034"/>

		</ModeList>
		</Configuration>
		*/

		CString strValue;
		bool bEnable = false;
		m_mapCrane.clear();

		if (xml.FindElem(_T("Configuration")))
		{
			while (xml.FindChildElem(_T("Crane")))
			{
				xml.IntoElem();
				long nCraneID = _tcstolong(xml.GetAttrib(_T("ID")));
				CranePtr pCrane;
				auto itFindCrane = m_mapCrane.find(nCraneID);
				if (itFindCrane == m_mapCrane.end())
				{
					pCrane = make_shared<Crane>();
					m_mapCrane.insert(pair<long, CranePtr>(nCraneID, pCrane));
				}
				else
					pCrane = itFindCrane->second;
			
				while (xml.FindChildElem(_T("Mode")))
				{
					xml.IntoElem();
					TCHAR szAttrib[16] = { 0 };
					int nCount = 1;
					long nModeID = _tcstolong((LPCTSTR)xml.GetAttrib(_T("ID")));
						
					DisplayModePtr ModePtr;
					auto itFindMode = pCrane->ScreenMode.find(nModeID);
					if (itFindMode == pCrane->ScreenMode.end())
					{
						ModePtr = make_shared<DisplayMode>(nModeID);
						pCrane->ScreenMode.insert(pair<long, DisplayModePtr>(nModeID, ModePtr));
					}
					else
					{// 发现重复的模式值,需要报错
						ModePtr = itFindMode->second;
					}
					while (true)
					{
						_stprintf_s(szAttrib,16, _T("Camera%d"), nCount);
						CString strCameraID = xml.GetAttrib(szAttrib);
						if (strCameraID == _T(""))
						{
							break;
						}
						ModePtr->vecPanel.push_back(make_shared<PlayPanel>(strCameraID,0));
						nCount++;
					}

					xml.OutOfElem();
				}
				xml.OutOfElem();

			}
			
		}
		else
		{
			if (m_pRunlog)
				m_pRunlog->Runlog(_T("%s Can't find Node 'Configuration' in file.\n"), __FUNCTIONW__, szPath);
			return false;
		}
		return true;
	}
	else
	{
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s Can't find %s.\n"), __FUNCTIONW__, szPath);
		return false;
	}
}

LONG CAVPlayerCtrl::ConfigureScreenMode()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 0;
}

LONG CAVPlayerCtrl::SwitchScreen(LONG nCraneID, LONG nScreenMode,LONG hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (nCraneID < 0 || nScreenMode < 0 || !hWnd)
		return AvError_InvalidParameters;
	DWORD dwT1 = timeGetTime();

	CVideoFrame *pVideoFrame = nullptr;
	
	auto itFindFrame = m_mapVideoFrame.find(hWnd);
	if (itFindFrame != m_mapVideoFrame.end())
		pVideoFrame = itFindFrame->second;
	else
	{
		CRect rtClient;
		::GetClientRect((HWND)hWnd,&rtClient);
		pVideoFrame = new CVideoFrame;
		
		pVideoFrame->Create(1024 + m_mapVideoFrame.size(), rtClient, 4, CWnd::FromHandle((HWND)hWnd));
		pVideoFrame->ShowWindow(SW_SHOW);
		m_mapVideoFrame.insert(pair<LONG, CVideoFrame *>(hWnd, pVideoFrame));
	}

	if (nCraneID == m_nCurrentCrane && m_nCurrentMode == nScreenMode)
		return 0;
	map<long, DisplayModePtr>::iterator itLastMode;
	bool bLastMode = false;
	if (m_nCurrentCrane > 0 && m_nCurrentMode >= 0)
	{// 需要关闭上一个模式的图像
		// 需要以异步方式关闭之前的图像，同时需要准备两个VideoFrame；
		auto itLastCrane = m_mapCrane.find(m_nCurrentCrane);
		if (itLastCrane != m_mapCrane.end())
		{
			itLastMode = itLastCrane->second->ScreenMode.find(m_nCurrentMode);
			if (itLastMode != itLastCrane->second->ScreenMode.end())
			{
				bLastMode = true;
				DisplayModePtr pMode = itLastMode->second;
				for (auto it = pMode->vecPanel.begin(); it != pMode->vecPanel.end(); it++)
				{
					PausePlay((*it)->strCameraID, (LONG)(*it)->hWnd);
				}
			}
		}
	}

	auto itCrane = m_mapCrane.find(nCraneID);
	if (itCrane == m_mapCrane.end())
		return AvError_Crane_notExist;
	auto itMode = itCrane->second->ScreenMode.find(nScreenMode);
	if (itMode == itCrane->second->ScreenMode.end())
		return AvError_ScreenMode_notExist;
	
	
	DisplayModePtr pMode = itMode->second;
	if (pMode->vecPanel.size() != pVideoFrame->GetPanelCount())
	{
		if (pMode->vecPanel.size() ==3)
			pVideoFrame->AdjustPanels(pMode->vecPanel.size(), Style_2P1);
		else
			pVideoFrame->AdjustPanels(pMode->vecPanel.size());
	}
	int nIndex = 0;
	for (auto it = pMode->vecPanel.begin(); it != pMode->vecPanel.end(); it++)
	{
		(*it)->hWnd = (LONG)pVideoFrame->GetPanelWnd(nIndex++);
		int nError = PlayStream((*it)->strCameraID, (*it)->hWnd, 0);
		
		if (nError != AvError_Succeed)
		{
			if (m_pRunlog)
				m_pRunlog->Runlog(_T("%s Failed in Play Camera %s.\n"), __FUNCTIONW__, *it);
		}
	}

	DWORD dwTimeSpan = timeGetTime() - dwT1;
	TraceMsgA("%s TimeSpan = %d.\n", __FUNCTION__, dwTimeSpan);

	if (bLastMode)
	{
		DisplayModePtr pMode = itLastMode->second;
		for (auto it = pMode->vecPanel.begin(); it != pMode->vecPanel.end(); it++)
		{
			StopPlay((*it)->strCameraID, (LONG)(*it)->hWnd);
		}
	}

	m_nCurrentCrane = nCraneID;
	m_nCurrentMode = nScreenMode;
	return 0;
}

void CAVPlayerCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	COleControl::OnLButtonDown(nFlags, point);
}

BOOL CAVPlayerCtrl::PreTranslateMessage(MSG* pMsg)
{
	TraceMsgA("%s.\n", __FUNCTION__);
	return COleControl::PreTranslateMessage(pMsg);
}

int CAVPlayerCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	return COleControl::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

LONG CAVPlayerCtrl::PlayBack(LONG hWnd,LPCTSTR strDeviceID, LONG nStartTime,LONG nStopTime,LONG nSeekFrame ,LONG nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TraceFunction();
	if (!m_bEnableAS300)
		return AvError_AS300ServiceIsDisabled;
	if (!strDeviceID || !hWnd || !nStartTime || !nStopTime || !nTimeout || nStartTime >= nStopTime)
		return AvError_InvalidParameters;
	if (m_nLoginID == -1)
		return AvError_NotLogintoServer;

	// PVSRecord_Info_t pRecordInfo = (PVSRecord_Info_t )lRecordInfo;
	try
	{
		// 330106 100 0052
		char szChannelID[32] = { 0 };
		char szDeviceID[32] = { 0 };
		if (!::IsWindow((HWND)hWnd))
		{
			if (m_pRunlog)
				m_pRunlog->Runlog(_T("%s Specialfied a invalid window!\n"), __FUNCTIONW__);
			return AvError_InvalidWindow;
		}
		strcpy_s(szDeviceID, 32, _AnsiString(strDeviceID, CP_ACP));
		strcat_s(szDeviceID, 32, "01");

		RECT rt;
		::GetWindowRect((HWND)hWnd, &rt);
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s Windows %08X Postion(%d,%d,%d,%d).\n"), __FUNCTIONW__, hWnd, rt.left, rt.right, rt.top, rt.bottom);
		CAutoLock lock(&m_csMapConnection);
		
		map<string, IPCConnectionPtr>::iterator itFind = m_MapConnection.find(szDeviceID);
		if (itFind != m_MapConnection.end())
		{
			return AvError_DeviceInPlaying;
		}
		lock.Unlock();
		long nTimeSpan = nStopTime - nStartTime;
		int nMaxRecordCount = nTimeSpan % 1800;
		if (nMaxRecordCount * 1800 < nTimeSpan)
			nMaxRecordCount++;
		VSRecord_Info_t *pRecordArray = new VSRecord_Info_t[nMaxRecordCount];
		unique_ptr<VSRecord_Info_t[]> RecordArrayPtr(pRecordArray);
		
		VSQuery_Record_Info_t QueryInfo;
		QueryInfo.nChannelNo = 1;
		QueryInfo.nSource = Record_Source_SS;
		QueryInfo.nRecordType = Record_all;
		QueryInfo.nStartTime = nStartTime;
		QueryInfo.nEndTime = nStopTime;
		strcpy_s((char *)QueryInfo.szCameraId, MAX_ID_LENGTH, (char *)szDeviceID);
		int nRecordCount = 0;
		long nResult = SDK_CUQueryRecord(m_nLoginID, QueryInfo, pRecordArray, (int &)nRecordCount, nMaxRecordCount, 5000);
		if (nResult< 0)
			return (AvError_AS300_Error + nResult);

// 		pRecordArray[0].startTime = nStartTime;
// 		pRecordArray[0].endTime = nStopTime;
		IPCConnectionPtr pConnection = make_shared<_IPCConnection>();
		pConnection->pRunlog = m_pRunlog;
		pConnection->m_hWnd = (HWND)hWnd;
// 		pConnection->nSeekFrame = nSeekFrame;
// 		pConnection->tStartTime = nStartTime;
// 		pConnection->tStopTime = nStopTime;

		PlayBackStatusPtr pPlayStatus = make_shared<PlayBackStatus>();
		pPlayStatus->nSeekFrame = nSeekFrame;
		pPlayStatus->tStartTime = nStartTime;
		pPlayStatus->tStopTime = nStopTime;

		pConnection->hPlayhandle = ipcplay_OpenRTStream(pConnection->m_hWnd);
		pConnection->pPlayStatus = pPlayStatus;
		//ipcplay_EnableAsyncRender(pConnection->hPlayhandle,true,50);
		if (nSeekFrame)
			ipcplay_EnablePlayOneFrame(pConnection->hPlayhandle);
	
		ipcplay_SetD3dShared(pConnection->hPlayhandle,false);

		pRecordArray[0].startTime = nStartTime;
		if (nStopTime <= pRecordArray[0].endTime)
			pRecordArray[0].endTime = nStopTime;

		long hPlaySession = SDK_CUPlaybackByFile(m_nLoginID, &pRecordArray[0], nTimeout, "", 0);
		if (hPlaySession < 0)
			return (AvError_AS300_Error + hPlaySession);
		pConnection->m_nPlaySession = hPlaySession;
		pConnection->m_nLoginID = m_nLoginID;
		EnterCriticalSection(&m_csMapConnection);
		m_MapConnection.insert(pair<string, IPCConnectionPtr>(szDeviceID, pConnection));
		LeaveCriticalSection(&m_csMapConnection);

		EnterCriticalSection(&m_csMapSession);
		m_MapSession.insert(pair<LONG, IPCConnectionPtr>(hPlaySession, pConnection));
		LeaveCriticalSection(&m_csMapSession);

		if (m_pRunlog)
		{
			m_pRunlog->Runlog(_T("%s Device %s (%08X) is playing on Window %08X.\n"), __FUNCTIONW__, strDeviceID, (long)pConnection->hPlayhandle, hWnd);
			m_pRunlog->Runlog(_T("%s MapSession size = %d.\n"), __FUNCTIONW__, m_MapSession.size());
		}

		return S_OK;
	}
	catch (CMySQLException& e)
	{
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s A DB Exception occured when playing device %s:%s.\n"), __FUNCTIONW__, strDeviceID, e.whatW());
		return AvError_DBException;
	}
	catch (std::exception &e)
	{
		if (m_pRunlog)
			m_pRunlog->Runlog(_T("%s A Unknown Exception occured when playing device %s:%s.\n"), __FUNCTIONW__, strDeviceID, _UnicodeString(e.what(), CP_ACP));
		return AvError_UnknownException;
	}

	return 0;
}


void CAVPlayerCtrl::StopPlayBack(LPCTSTR strDeviceID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!strDeviceID)
		return ;
	TCHAR szDeviceID[64] = { 0 };
	_tcscpy_s(szDeviceID, 64, strDeviceID);
	_tcscat_s(szDeviceID, 64, _T("01"));
	StopPlay(szDeviceID, nullptr);
}


LONG CAVPlayerCtrl::SeekTime(LPCTSTR strDeviceID, LONGLONG nTime)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!strDeviceID || !nTime)
		return AvError_InvalidParameters;

	TCHAR szDeviceID[32] = { 0 };
	
	_tcscpy_s(szDeviceID, 32, strDeviceID);
	_tcscat_s(szDeviceID, 32, _T("01"));
	
	CAutoLock lock(&m_csMapConnection);
	map<string, IPCConnectionPtr>::iterator itFind = m_MapConnection.find(_AnsiString(szDeviceID,CP_ACP));
	if (itFind != m_MapConnection.end())
	{
		if (!itFind->second->hPlayhandle)
			return AvError_DeviceNotInPlaying;
		
		if (itFind->second->pPlayStatus)
		{
			if (itFind->second->pPlayStatus->tStartTime <= nTime && itFind->second->pPlayStatus->tStopTime >= nTime)
				return ipcplay_AsyncSeekFrame(itFind->second->hPlayhandle, nTime);
			else
				return AvError_OutofPlayingRange;
		}
		else
			return AvError_NotSingleFramePlayer;
	}
	return 0;
}


LONG CAVPlayerCtrl::CreateFrameWnd(LONG hWnd,LONG nWndCount, LONG nFrameStyle, LONG* pFrameHandle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 0;
}


LONG CAVPlayerCtrl::AdjustPanels(LONG nWndCount, LONG nFrameStyle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here

	return 0;
}


LONG CAVPlayerCtrl::QueryRecord(LPCTSTR strDeviceID, LONG nStartTime, LONG nStopTime, LONG pRecordArray, LONG nBufferCount, LONG* pRecordCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!m_bEnableAS300)
		return AvError_AS300ServiceIsDisabled;
	if (!strDeviceID || !nStartTime || !nStopTime || nStopTime <= nStartTime || !pRecordArray || !pRecordCount)
		return AvError_InvalidParameters;
	if (m_nLoginID == -1)
		return AvError_NotLogintoServer;
	

	if (nStartTime >= nStopTime)
		return AvError_InvalidParameters;
	char szDeviceID[32] = { 0 };

	strcpy_s((char *)szDeviceID, 32, _AnsiString(strDeviceID, CP_ACP));
	strcat_s((char *)szDeviceID, 32, "01");
	VSQuery_Record_Info_t QueryInfo;
	QueryInfo.nChannelNo = 1;
	QueryInfo.nSource = Record_Source_SS;
	QueryInfo.nRecordType = Record_all;
	QueryInfo.nStartTime = nStartTime;
	QueryInfo.nEndTime = nStopTime;
	strcpy_s((char *)QueryInfo.szCameraId, MAX_ID_LENGTH, (char *)szDeviceID);
	int nRecordCount = 0;
	long nResult = SDK_CUQueryRecord(m_nLoginID, QueryInfo, (PVSRecord_Info_t)pRecordArray, (int &)nRecordCount, nBufferCount, 15000);
	if (nRecordCount > 0)
	{
		*pRecordCount = nRecordCount;
		return AvError_Succeed;
	}
	else
		return (AvError_AS300_Error + nResult);

	return 0;
}



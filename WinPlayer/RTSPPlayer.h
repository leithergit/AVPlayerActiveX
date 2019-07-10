#pragma once

#include "resource.h"


#define rtsp_TCP	1
#define rtsp_UDP	0

#include "../RTSP/rtsp.h"
#ifdef _DEBUG
#pragma comment(lib,"../debug/rtspd.lib")
#else
#pragma comment(lib,"../release/rtsp.lib")
#endif
#include "../ipcplaysdk/IPCPlaySDK.h"
#pragma comment(lib,"../ipcplaysdk/ipcplaysdk.lib")
#include <list>
#include "Runlog.h"
#include "Utility.h"
#include "TimeUtility.h"
#include "CriticalSectionAgent.h"
#include <memory>
#include <process.h>

/*
struct PlayEvent
{
	HWND	hWnd;					// 显示图像的窗口句柄
// 	HANDLE	hStopEvent;				// 停止当前视频窗口播放事件
// 	CHAR	szStopEvent[32];		// 停止事件名称
// 	DWORD	dwPrimaryPID;			// 主进程ID
// 	DWORD	dwPrimaryTID;			// 主进程的主线程ID
// 	DWORD	dwChildPID;				// 当前播放进程ID	
// 	DWORD	dwChildTID;				// 子进程的主线程ID
	CHAR	szCameraIP[32];			// 摄像机IP
	char	szRTSP_URL[512];		// 
	bool	bEnableHAccel;
	WORD	nPort;					// 取码流的端口
};
*/


#define _TraceMemory

//#if defined(_DEBUG) && defined(_TraceMemory)
#define TraceTimeout		150
#define TraceFunction()	CTraceFunction Tx(__FUNCTION__);
#define TraceFunction1(szText)	CTraceFunction Tx(__FUNCTION__,szText);
// #else 
// #define TraceFunction()	
// #endif


/// @brief 跟踪函数执行过程的一些信息的类,主要用于调试
class CTraceFunction
{
	explicit CTraceFunction(){};
public:
	CTraceFunction(CHAR *szFunction, CHAR *szTxt = NULL)
	{
		ZeroMemory(this, sizeof(CTraceFunction));
		m_dfTimeIn = GetExactTime();
		strcpy(m_szFunction, szFunction);
	}
	~CTraceFunction()
	{
		CHAR szText[4096] = { 0 };
		if (strlen(m_szText) == 0)
			sprintf_s(szText, 4096, "%s\t_OUT_ %s \tTimeSpan = %.3f.\n", __FUNCTION__, m_szFunction, TimeSpanEx(m_dfTimeIn));
		else
			sprintf_s(szText, 4096, "%s\t_OUT_ %s %s\tTimeSpan = %.3f.\n", __FUNCTION__, m_szFunction, m_szText, TimeSpanEx(m_dfTimeIn));
		OutputDebugStringA(szText);

	}
private:
	double	m_dfTimeIn;
	CHAR	m_szFile[MAX_PATH];
	CHAR	m_szText[1024];
	CHAR	m_szFunction[256];
};


#define _MAX_VIDEO_WINDOW	(64)
#define _Max_Switch_Times	(1024)

#define		MemFileNameW	L"{09A1E875-5982-49DC-AAF1-8C7356040461}"
#define		MemFileNameA	("{09A1E875-5982-49DC-AAF1-8C7356040461}")

#ifdef _UNICODE
#define _MemFileName MemFileNameW
#else
#define _MemFileName MemFileNameA
#endif

using namespace  std;
using namespace  std::tr1;
void OnRTSPStream(long lHandle, char *pBuffer, int nParam, int nUser);
void OnSDPNofity(long hHandle, void *pData, int nUser);
void OnDisconnect(long hHandle, int nErorr, int nUser);
struct _RTSPConnection;
//////////////////////////////////////////////////////////////////////////
/// 用于重连RTSP的函数和数据结构
list<_RTSPConnection *> listConnection;
 CCriticalSectionAgent csListConnection;
 HANDLE hThreadEvent = nullptr;
 HANDLE hThreadConnect = nullptr;
 void PostConnection(void * pConnection);
 UINT __stdcall ThreadReconnect(void *p);
using namespace  std;
#pragma pack(push)
#pragma pack(1)
struct _RTSPConnection
{
	IPC_PLAYHANDLE hPlayhandle;
	IPC_MEDIAINFO *pMediaHeader;
	int		nSize;
	HWND	hWnd;
	bool	bHWAccel;
	byte	*m_pFrameBuffer;
	CRITICAL_SECTION csPlayHandle;
	
	int		nConnectType;
	int		nPort;
	char	szURL[1024];
	char	szPassword[32];
	char	szAccount[32];
	char	szIP[32];
	double  dfDisconnected;			// 摄像机断线时间
	int		nConnectTimeout;		// 连接摄像机超时间，超过这个时间后，摄像机无响应时，认为摄像机掉线，默认250ms
	int		nMaxFrameInterval;		// 最大帧间隔，超这个时间，则认为摄像机掉线,默认250ms
	int		nReconnectInterval;		// 断线重连间隔,默认5000ms
	CRITICAL_SECTION csConnect;		// 
	int		m_nBufferSize;
	int		m_nFrameLength;
	int		m_nIFrameLength;
	void	*pOnStream;
	void	*pSDPNotify;
	void	*pOnDisconect;
	long	m_hRtspSession;
	bool	bSPSInput;
	bool	bPPSInput;
	bool	bSEIInput;
	shared_ptr<CRunlog> pRunlog;
	RECT	rtZoomBorder;		// 缩放边界
	bool	bPercent;			// 缩放方式，为True时，使用则按百分缩放，否则按象素缩放
	_RTSPConnection(HWND hWnd, int nConnectTimeout =500, int nMaxFrameInterval=500, int nReconnectInterval=5000)
	{
		ZeroMemory(this, sizeof(_RTSPConnection));
		nSize = sizeof(_RTSPConnection);
		this->hWnd = hWnd;
		m_nBufferSize = 128 * 1024;
		m_pFrameBuffer = new byte[m_nBufferSize];	
		InitializeCriticalSection(&csPlayHandle);
		InitializeCriticalSection(&csConnect);
		this->nConnectTimeout = nConnectTimeout;
		this->nMaxFrameInterval = nMaxFrameInterval;
		this->nReconnectInterval = nReconnectInterval;
		pMediaHeader = new IPC_MEDIAINFO;
	}

	static void OnSDPNofity(long hHandle, void *pData, int nUser)
	{
		_RTSPConnection *pThis = (_RTSPConnection *)nUser;
		_SDPAttrib *pSDP = (_SDPAttrib *)pData;
		int nCodecID = 0, nWidth = 0, nHeiht = 0, nFPS = 0;
		if (!pThis->hPlayhandle)
		{
			if (!pSDP->pMediaName || strcmp(pSDP->pMediaName, "video"))
			{
				TraceMsgA("%s Not a video stream .\n", __FUNCTION__);
				return;
			}

			if (!pSDP->nHeight || !pSDP->nWidth || !pSDP->szCodec)
			{
				TraceMsgA("%s Not Unknown codec .\n", __FUNCTION__);
				return;
			}
			if (strcmp(pSDP->szCodec, "H264") == 0)
				pThis->pMediaHeader->nVideoCodec = CODEC_H264;
			else if (strcmp(pSDP->szCodec, "H265") == 0)
				pThis->pMediaHeader->nVideoCodec = CODEC_H265;
			pThis->pMediaHeader->nAudioCodec = CODEC_G711A;
			pThis->pMediaHeader->nVideoWidth = pSDP->nWidth;
			pThis->pMediaHeader->nVideoHeight = pSDP->nHeight;
			if (pSDP->nRate != 0)
				pThis->pMediaHeader->nFps = pSDP->nRate;
			else
				pThis->pMediaHeader->nFps = 25;
			pThis->hPlayhandle = ipcplay_OpenStream(pThis->hWnd, NULL, 0);
			ipcplay_SetStreamHeader(pThis->hPlayhandle, (byte *)pThis->pMediaHeader, sizeof(IPC_MEDIAINFO));
			ipcplay_SetMaxFrameSize(pThis->hPlayhandle, 1024 * 1024);
			if (pThis->bHWAccel)
				ipcplay_SetD3dShared(pThis->hPlayhandle, true);
			ipcplay_Start(pThis->hPlayhandle, false, true, pThis->bHWAccel);
		}
	}

	static void OnDisconnect(long hHandle, int nErorr, int nUser,char *szProc)
	{
		_RTSPConnection *pThis = (_RTSPConnection *)nUser;
		pThis->PostConnection(nErorr,szProc);
	}

	void PostConnection(int nError, char *szProc)
	{
		dfDisconnected = GetExactTime();
		if (pRunlog)
			pRunlog->Runlog("%s device %s Disconnected,Stack:%s\tError =%d.\n", __FUNCTION__, szIP,szProc,nError);
		csListConnection.Lock();
		listConnection.push_back(this);
		csListConnection.Unlock();
	}

	static void OnRTSPStream(long lHandle, char *pBuffer, int nBufferLen, int nUser)
	{
		_RTSPConnection *pThis = (_RTSPConnection *)nUser;
		byte szHeader[64] = { 0 };
		int nHeadersize = 64;
		int nNalType = pBuffer[0] & 0x1F;
		int nTotalLength = pThis->m_nFrameLength + nBufferLen + 4;
		if (pThis->m_nBufferSize < nTotalLength)
		{
			int nNewBufferSize = pThis->m_nBufferSize;
			while (nNewBufferSize < nTotalLength)
				nNewBufferSize *= 2;
			byte *pTemp = new byte[nNewBufferSize];
			if (pTemp == NULL)
				return;

			memcpy(pTemp, pThis->m_pFrameBuffer, pThis->m_nFrameLength);
			delete[]pThis->m_pFrameBuffer;
			pThis->m_pFrameBuffer = pTemp;
			pThis->m_nBufferSize = nNewBufferSize;

		}
		bool bInputFrame = true;
		switch (nNalType)
		{
		case 1:
		case 5:
			bInputFrame = true;
			break;
		case 6:
		{
			if (pThis->bSEIInput)
				bInputFrame = false;
			else
				pThis->bSEIInput = true;
		}
			break;
		case 7:
		{
			if (pThis->bSPSInput)
				bInputFrame = false;
			else
				pThis->bSPSInput = true;
		}
			break;
		case 8:
		{
			if (pThis->bPPSInput)
				bInputFrame = false;
			else
				pThis->bPPSInput = true;
		}

		default:
			break;
		}
					
		if (bInputFrame)
		{
			byte szStart_code[] = { 0, 0, 0, 1 };
			memcpy_s(&pThis->m_pFrameBuffer[pThis->m_nFrameLength], pThis->m_nBufferSize - pThis->m_nFrameLength, szStart_code, 4);
			pThis->m_nFrameLength += 4;
			memcpy_s(&pThis->m_pFrameBuffer[pThis->m_nFrameLength], pThis->m_nBufferSize - pThis->m_nFrameLength, pBuffer, nBufferLen);
			pThis->m_nFrameLength += nBufferLen;
			int nFrameType = IPC_P_FRAME;
			if (nNalType == 5)
				nFrameType = IPC_I_FRAME;
			if (TryEnterCriticalSection(&pThis->csPlayHandle))
			{
				if (pThis->hPlayhandle)
				{
					ipcplay_InputIPCStream(pThis->hPlayhandle, pThis->m_pFrameBuffer, nFrameType, pThis->m_nFrameLength, 0);
					pThis->m_nFrameLength = 0;
				}
				LeaveCriticalSection(&pThis->csPlayHandle);
			}
		}
	}
	~_RTSPConnection()
	{
		TraceFunction1(szIP);
		EnterCriticalSection(&csConnect);
		if (m_hRtspSession)
			rtsp_stop(m_hRtspSession);

		if (hPlayhandle)
			ipcplay_Close(hPlayhandle);

		if (m_pFrameBuffer)
			delete[]m_pFrameBuffer;

		if (pMediaHeader)
			delete pMediaHeader;
		LeaveCriticalSection(&csConnect);
		DeleteCriticalSection(&csConnect);
		DeleteCriticalSection(&csPlayHandle);
	}
	bool CheckConnectTime()
	{
		return ((TimeSpanEx(dfDisconnected)*1000) >= nReconnectInterval);
	}
	void Reset(bool bStopDecode = true)
	{
		if (m_hRtspSession)
		{
			rtsp_stop(m_hRtspSession);
			m_hRtspSession = 0;
			if (bStopDecode)
			{
				if (hPlayhandle)
				{
					ipcplay_Close(hPlayhandle);
					hPlayhandle = nullptr;
				}
			}
			else
			{
				if (hPlayhandle)
					ipcplay_Refresh(hPlayhandle);
			}
			int nSize = sizeof(_RTSPConnection) - offsetof(_RTSPConnection, m_hRtspSession);
			ZeroMemory(&m_hRtspSession, sizeof(_RTSPConnection) - offsetof(_RTSPConnection, m_hRtspSession));
		}
	}

	void AddWindow(HWND hWnd)
	{
		EnterCriticalSection(&csPlayHandle);
		if (hPlayhandle)
		{
			ipcplay_AddWindow(hPlayhandle, hWnd);
		}
		LeaveCriticalSection(&csPlayHandle);
	}
	// 返回余下窗口数
	int RemoveWindow(HWND hWnd)
	{
		int nWndCount = 0;
		int nArraySize = 4;
		HWND hWndArray[4] = { 0 };
		EnterCriticalSection(&csPlayHandle);
		if (hPlayhandle)
		{
			ipcplay_RemoveWindow(hPlayhandle, hWnd);			
			if (ipcplay_GetRenderWindows(hPlayhandle, nullptr, nArraySize) == IPC_Succeed)
			{
				nWndCount = nArraySize;
			}
		}
		LeaveCriticalSection(&csPlayHandle);
		return nWndCount;
	}
	LONG RtspConnect(char *szIP, int nPort, char *szAccount, char *szPassword, char *szURL)
	{
		TraceFunction1(szIP);
		if (!TryEnterCriticalSection(&csConnect))
			return 0;
		m_hRtspSession = rtsp_play(szURL, szAccount, szPassword, rtsp_TCP, nPort, OnSDPNofity, OnRTSPStream, OnDisconnect, this, nConnectTimeout,nMaxFrameInterval);
		LeaveCriticalSection(&csConnect);
		if (!m_hRtspSession)
		{
			if (pRunlog)
				pRunlog->Runlog(_T("%s Failed in connecting Device:%s via %s.\n"), __FUNCTIONW__, _UnicodeString(szIP, CP_ACP), _UnicodeString(szURL, CP_ACP));
			return -1010;
		}
		else
		{
			strcpy_s(this->szURL, 1024, szURL);
			strcpy_s(this->szIP,32, szIP);
			strcpy_s(this->szAccount,32, szAccount);
			strcpy_s(this->szPassword,32, szPassword);
			this->nPort = nPort;			
			return 0;
		}
	}
	LONG Reconnect()
	{
		TraceFunction1(szIP);
		if (!TryEnterCriticalSection(&csConnect))
		{
			if (pRunlog)
				pRunlog->Runlog("%s Device %s TryEnterCriticalSection failed.\n", __FUNCTION__, szIP);
		}
		if (nSize != sizeof(_RTSPConnection))
			return 0;
		Reset(false);
		if (pRunlog)
			pRunlog->Runlog("%s Try to connect Device:%s via %s.\n", __FUNCTION__, szIP, szURL);
	
		m_hRtspSession = rtsp_play(szURL, szAccount, szPassword, nConnectType, nPort, OnSDPNofity, OnRTSPStream,OnDisconnect, this,nConnectTimeout,nMaxFrameInterval);
		LeaveCriticalSection(&csConnect);
		if (!m_hRtspSession)
		{
			if (pRunlog)
				pRunlog->Runlog("%s Failed in connecting Device:%s via %s.\n", __FUNCTION__, szIP, szURL);
			return -1010;
		}
		else
			return 0;
	}

};
#pragma pack(pop)
typedef shared_ptr<_RTSPConnection> RTSPConnectionPtr;

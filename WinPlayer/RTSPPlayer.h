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
//////////////////////////////////////////////////////////////////////////
/// 用于重连RTSP的函数和数据结构
 list<LONG> listConnect;
 CCriticalSectionAgent csListConnect;
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
	int		m_nBufferSize;
	int		m_nFrameLength;
	int		m_nIFrameLength;
	void	*pOnStream;
	void	*pSDPNotify;
	void	*pOnDisconect;
	long	m_hRtspSession;
	CRunlog* pRunlog;
	RECT	rtZoomBorder;		// 缩放边界
	bool	bPercent;			// 缩放方式，为True时，使用则按百分缩放，否则按象素缩放

	_RTSPConnection(HWND hWnd)
	{
		ZeroMemory(this, sizeof(_RTSPConnection));
		this->hWnd = hWnd;
		m_nBufferSize = 128 * 1024;
		m_pFrameBuffer = new byte[m_nBufferSize];	
		InitializeCriticalSection(&csPlayHandle);
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
			ipcplay_Start(pThis->hPlayhandle, false, true, pThis->bHWAccel);
		}
	}

	static void OnDisconnect(long hHandle, int nErorr, int nUser)
	{
		_RTSPConnection *pThis = (_RTSPConnection *)nUser;
		pThis->PostConnection();
	}

	void PostConnection()
	{
		TraceMsgA("%s %d IP = %s.\n", __FUNCTION__,(int)time(0), szIP);
		csListConnect.Lock();
		listConnect.push_back((LONG)this);
		csListConnect.Unlock();
	}

	static void OnRTSPStream(long lHandle, char *pBuffer, int nBufferLen, int nUser)
	{
		_RTSPConnection *pThis = (_RTSPConnection *)nUser;
		byte szHeader[64] = { 0 };
		int nHeadersize = 64;
		int nNalType = pBuffer[0] & 0x1F;
		if (pThis->m_nBufferSize < pThis->m_nFrameLength + nBufferLen)
		{
			int nNewBufferSize = pThis->m_nBufferSize;
			while (nNewBufferSize <= (pThis->m_nFrameLength + nBufferLen))
				nNewBufferSize *= 2;
			byte *pTemp = new byte[nNewBufferSize];
			if (pTemp == NULL)
				return;

			memcpy(pTemp, pThis->m_pFrameBuffer, pThis->m_nFrameLength);
			delete[]pThis->m_pFrameBuffer;
			pThis->m_pFrameBuffer = pTemp;
			pThis->m_nBufferSize = nNewBufferSize;

		}
		if (nNalType == 5 || nNalType == 1 ||
			nNalType == 7 || nNalType == 8)
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
		if (m_hRtspSession)
			rtsp_stop(m_hRtspSession);

		if (hPlayhandle)
			ipcplay_Close(hPlayhandle);

		if (m_pFrameBuffer)
			delete[]m_pFrameBuffer;

		if (pMediaHeader)
			delete pMediaHeader;
		DeleteCriticalSection(&csPlayHandle);
	}
	void Reset(bool bStopDecode = true)
	{
		if (m_hRtspSession)
			rtsp_stop(m_hRtspSession);
		if (bStopDecode)
		{
			if (hPlayhandle)
				ipcplay_Close(hPlayhandle);
		}
		else
		{
			if (hPlayhandle)
				ipcplay_Refresh(hPlayhandle);
		}
		
		int nSize = sizeof(_RTSPConnection) - offsetof(_RTSPConnection, m_hRtspSession);
		ZeroMemory(&m_hRtspSession, sizeof(_RTSPConnection) - offsetof(_RTSPConnection, m_hRtspSession));
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
		m_hRtspSession = rtsp_play(szURL, szAccount, szPassword, rtsp_TCP, nPort, OnSDPNofity, OnRTSPStream, OnDisconnect, this, 2000);
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
		Reset(false);
		if (pRunlog)
			pRunlog->Runlog("%s Try to connect Device:%s via %s.\n", __FUNCTION__, szIP, szURL);
		TraceMsgA("%s %d Try to connect Device : %s via %s.\n", __FUNCTION__, (int)time(0), szIP, szURL);
		m_hRtspSession = rtsp_play(szURL, "", "", nConnectType, nPort, OnSDPNofity, OnRTSPStream,OnDisconnect, this,2000);
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

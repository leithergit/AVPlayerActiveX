#pragma once
#include <windows.h>

#ifndef _max
#define _max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef _min
#define _min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
enum CDS_Command
{
	CDS_START,
	CDS_STOP,
	CDS_ADDWND,
	CDS_REMOVEWND,
	CDS_STOPALL=1024
};
#pragma pack(push)
#pragma pack(1)
struct PlayEvent
{
	CDS_Command	nCommand;
	HWND	hWnd;					// 显示图像的窗口句柄
	CHAR	szCameraIP[32];			// 摄像机IP
	char	szRTSP_URL[512];		// 
	bool	bEnableHAccel;
	WORD	nPort;					// 取码流的端口
	PlayEvent()
	{
		ZeroMemory(this, sizeof(PlayEvent));
	}
};
struct PlayProcess
{
	DWORD	dwProcessID;
	HWND	hProcessWnd;
	void Reset()
	{
		ZeroMemory(this, sizeof(PlayProcess));
	}
};
#pragma pack(pop)


void RegisterMyMesssage();


// Register a window to primary Windows
// the WPARAM save the index of Windows
// the LPARAM save the hwnd of windows

#define _MAX_VIDEO_WINDOW	(64)
#define _Max_PlayProcess	(16)

#define		MemFileNameW	L"{09A1E875-5982-49DC-AAF1-8C7356040461}"
#define		MemFileNameA	("{09A1E875-5982-49DC-AAF1-8C7356040461}")

#ifdef _UNICODE
#define _MemFileName MemFileNameW
#else
#define _MemFileName MemFileNameA
#endif
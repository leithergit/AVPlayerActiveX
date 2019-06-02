// RTSPPlayer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "RTSPPlayer.h"
#include "WndEvent.h"
#include <map>
#include <memory>
#include "CriticalSectionAgent.h"
using namespace std;
using namespace std::tr1;

// Global Variables:
HINSTANCE g_hInstance;								// current instance

VOID WaitForAttach(bool bWait)
{
	while (bWait)
	{
		Sleep(100);
	}
}

HWND			g_hPrimaryWnd = nullptr;
HWND			*g_pWndArray = nullptr;
INT				g_nProcessIndex = -1;
INT				g_nMaxSwitchCount = 1024;
TCHAR			*szTitle = _T("Win Player");					// The title bar text
TCHAR			*szWindowClass = _T("Win Player");
ATOM			MyRegisterClass(HINSTANCE hInstance);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void			StartThreadConnect();
void			StopThreadConnect();
DWORD			g_nSwitchTimes = 0;								// 视频切换次数
CRunlog			*g_pRunlog = nullptr;
DWORD			g_dwPrimaryPID = -1;
DWORD			g_dwPrimaryTID = -1;
HWND			g_hWindow = nullptr;
HANDLE			hThreadWaitPrimaryProcessExit = nullptr;
UINT __stdcall ThreadWaitPrimaryProcessExit(void *);
UINT			WM_REGISTERWND = 0;
UINT			WM_UNREGISTERWND = 0;
UINT			WM_CREATEPLAYER = 0;
// CCriticalSectionAgent g_csWnd2Connection;
// map<HWND, IPCConnectionPtr> g_mapWnd2Connection;
CCriticalSectionAgent g_csIP2Connection;
map<LONG, RTSPConnectionPtr> g_mapIP2Connection;

void RegisterMyMesssage()
{
	WM_REGISTERWND = RegisterWindowMessage(_T("WM_REGISTERWND"));
	// Unregister a window to primary wnd ,the LPARAM save the hwnd of windows
	WM_UNREGISTERWND = RegisterWindowMessage(_T("WM_UNREGISTERWND"));
	WM_CREATEPLAYER = RegisterWindowMessage(_T("WM_CREATEPLAYER"));
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef _DEBUG
	//WaitForAttach(true);
#endif
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	TCHAR *szSampleString = _T("/Process:%02d /Wnd:%d /PID:%d /TID:%d /MaxCount:%d");
	if (_tcslen(lpCmdLine) <  _tcslen(szSampleString))
	{
		//MessageBox(nullptr, _T("命令行参数长度不足."), _T("RTSPPlayer"), MB_OK | MB_ICONSTOP);
		TraceMsg("%s The length of command line is not enough,CmdLine:%s\n", __FUNCTION__, lpCmdLine);
		return 0;
	}
	TCHAR szRTSP_URL[1024] = { 0 };	
	if (_stscanf_s(lpCmdLine, _T("/Process:%02d /Wnd:%d /PID:%d /TID:%d /MaxCount:%d"), &g_nProcessIndex, &g_hPrimaryWnd, &g_dwPrimaryPID, &g_dwPrimaryTID,&g_nMaxSwitchCount) < 5)
	{
		TraceMsg("%s the parameters of not enough ,CmdLine:%s\n", __FUNCTION__, lpCmdLine);
		return 0;
	}

	if (!IsWindow(g_hPrimaryWnd))
	{
		TraceMsg("%s the g_hPrimaryWnd attached a invalid window.\n", __FUNCTION__);
		return 0;
	}
	if (g_nProcessIndex < 0 || g_nProcessIndex > _Max_PlayProcess)
	{
		TraceMsg("%s The index of Process is out of bounds,CmdLine:%s\n", __FUNCTION__, lpCmdLine);
		return 0;
	}
	
// 	HANDLE hFile = CreateFileMapping(
// 		INVALID_HANDLE_VALUE,
// 		NULL,
// 		PAGE_READWRITE,
// 		0,
// 		sizeof(HWND)*_Max_PlayProcess,
// 		_MemFileName);
// 	if (hFile == NULL)
// 	{
// 		TraceMsg("%s CreateFileMapping FAILED.\n", __FUNCTION__);
// 		
// 		return 0;
// 	}
// 	g_pWndArray = (HWND *)MapViewOfFile(hFile,
// 		FILE_MAP_ALL_ACCESS,
// 		0,
// 		0,
// 		sizeof(HWND)*(_Max_PlayProcess));	
// 	PlayEvent WndEvent;
// 	if (!IsWindow(WndEvent.hWnd))
// 	{
// 		MessageBox(nullptr, _T("输入参数不是一个窗口句柄."), _T("RTSPPlayer"), MB_OK | MB_ICONSTOP);
// 		return 0;
// 	}
// 	WndEvent.hStopEvent = OpenEventA(EVENT_ALL_ACCESS, true, WndEvent.szStopEvent);
// 	do
// 	{
// 		if (WaitForMultipleObjects(EventCount, hExitEvent, FALSE, 0) == WAIT_TIMEOUT)
// 		{
// 		
// 			Sleep(40);
// 		}
// 		else
// 			break;
// 	} while (true);				
// 	CloseHandle(WndEvent.hStopEvent);	
	
	MyRegisterClass(hInstance);

	g_hInstance = hInstance; // Store instance handle in our global variable
	//#define CreateWindowA(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam) CreateWindowExA(0L, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
	g_hWindow = CreateWindow(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_BORDER | WS_GROUP|WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 0, 0, NULL, NULL, hInstance, NULL);

	if (!g_hWindow)
	{
		return FALSE;
	}
	RegisterMyMesssage();
	if (IsWindow(g_hPrimaryWnd))
		SendMessage(g_hPrimaryWnd, WM_REGISTERWND, (WPARAM)GetCurrentProcessId(), (LPARAM)g_hWindow);
	else
		TraceMsg("%s The hWnd of Primary is invalid,CmdLine:%s\n", __FUNCTION__, lpCmdLine);
#ifdef _DEBUG
	ShowWindow(g_hWindow, nCmdShow);
	UpdateWindow(g_hWindow);
	//WaitForAttach(true);
#endif

	StartThreadConnect();
	hThreadWaitPrimaryProcessExit = (HANDLE )_beginthreadex(0, 64, ThreadWaitPrimaryProcessExit, 0, 0, 0);
	CloseHandle(hThreadWaitPrimaryProcessExit);
	hThreadWaitPrimaryProcessExit = nullptr;
	MSG msg;
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	StopThreadConnect();

	return 0;
}

void StartThreadConnect()
{
	hThreadEvent = CreateEvent(nullptr,true,false,nullptr);
	hThreadConnect = (HANDLE)_beginthreadex(nullptr,256,ThreadReconnect,nullptr,0,0);
}

void StopThreadConnect()
{
	if (hThreadEvent)
		SetEvent(hThreadEvent);
	if (hThreadConnect)
	{
		WaitForSingleObject(hThreadConnect, INFINITE);
		CloseHandle(hThreadConnect);
		hThreadConnect = nullptr;
	}
	if (hThreadEvent)
	{
		CloseHandle(hThreadEvent);
		hThreadEvent = nullptr;
	}
	
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RTSPPLAYER));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_RTSPPLAYER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

/*
最多同时启动M+N个进程，M为屏幕窗口数，N备用线程,主进程同时启动这些线程，并记录线程ID和主窗口号
每个进程都会记录自己的视频切换次数，超过指定次数后，主进程将不再把任务分配给任务超限的进程(O))，在下一次切换
时，主进程将关闭进程（O），并将新任务发给备用进程，之后创建新的备用进程，保持备用进程数量为N
M和N都可配置
注意：若某IP已经在A进程中播放，不可再在其它进程中播放，否则会同时拉取两路连接，此时可以通加添加显示窗口命令，
为该IP增加一个视频显示窗口，停止该窗口视频时，应该先判断该IP是否还有其它窗口在显示，若还有则不应该关闭
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_CREATE:
	{
		// 创建闲置检查线程，若切换次数超过指定值，则关闭当前进程，系统使用下一进程
	}
		break;
	case WM_COPYDATA:
	{
		PCOPYDATASTRUCT pMyCDS = (PCOPYDATASTRUCT)lParam;
		if (pMyCDS->dwData == CDS_STOPALL)
		{
			if (g_nSwitchTimes >= g_nMaxSwitchCount )	// 超过切换次数进程退出,通知主进程开启新进程
				SendMessage(g_hPrimaryWnd, WM_UNREGISTERWND, (WPARAM)GetCurrentProcessId(), (LPARAM)hWnd);
			g_mapIP2Connection.clear();
			if (g_nSwitchTimes >= g_nMaxSwitchCount)
			{
				DestroyWindow(hWnd);
				delete g_pRunlog;
				g_pRunlog = nullptr;
			}
			break;
		}
		if (!g_pRunlog)
			g_pRunlog = new CRunlog();
		int nCount = pMyCDS->cbData / sizeof(PlayEvent);
		PlayEvent *pWndArray = (PlayEvent *)pMyCDS->lpData;
		for (int i = 0; i < nCount; i++)
		{
			PlayEvent& WndEvent = pWndArray[i];
			LONG nIP = inet_addr(WndEvent.szCameraIP);
			switch (WndEvent.nCommand)
			{
			case CDS_START:		// Start Session;
			{
				auto itFindIP = g_mapIP2Connection.find(nIP);
				if (itFindIP == g_mapIP2Connection.end())
				{
					RTSPConnectionPtr pConnection = make_shared<_RTSPConnection>(WndEvent.hWnd);
					pConnection->pRunlog = g_pRunlog;
					pConnection->RtspConnect(WndEvent.szCameraIP,WndEvent.nPort,"", "",	WndEvent.szRTSP_URL);
					strcpy(pConnection->szURL, WndEvent.szRTSP_URL);				
					g_mapIP2Connection.insert(pair<LONG, RTSPConnectionPtr>(nIP,pConnection));
					g_nSwitchTimes++;
				}
				else
				{
					itFindIP->second->AddWindow(WndEvent.hWnd);
				}
				break;
			}
			case CDS_STOP:		// Stop Session;
			{
				auto itFindIP = g_mapIP2Connection.find(nIP);
				if (itFindIP != g_mapIP2Connection.end())
				{
					if (WndEvent.hWnd)
					{
						if (itFindIP->second->RemoveWindow(WndEvent.hWnd) == 0)
							g_mapIP2Connection.erase(nIP);
					}
					else // 移除所有窗口，即关闭当前摄像机
						g_mapIP2Connection.erase(nIP);
					
				}
				if (g_nSwitchTimes >= g_nMaxSwitchCount && g_mapIP2Connection.size() == 0)
				{
					// 超过切换次数进程退出
					SendMessage(g_hPrimaryWnd, WM_UNREGISTERWND, (WPARAM)GetCurrentProcessId(), (LPARAM)hWnd);
					DestroyWindow(hWnd);
				}
				break;
			}
			case CDS_ADDWND:
			{
				break;
			}
				
			case CDS_REMOVEWND:
			{
				break;
			}
			}

		}
		TCHAR szText[128] = { 0 };
		_stprintf_s(szText, 128, _T("%s (%d)"), szTitle, g_nSwitchTimes);
		SetWindowText(hWnd, szText);
		
		break;
	}
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_SENDMESSAGE:
		{
			SendMessage(g_hPrimaryWnd, WM_REGISTERWND, (WPARAM)GetCurrentProcessId(), (LPARAM)g_hWindow);
		}
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
// 		TCHAR szText[128] = { 0 };
// 		_stprintf_s(szText, _T("Switch Times:%d"), g_nSwitchTimes);
// 		RECT rt;
// 		GetWindowRect(hWnd, &rt);
// 		InflateRect(&rt, 50, 50);
// 		DrawText(hdc, szText, _tcslen(szText), &rt, DT_CENTER);
		EndPaint(hWnd, &ps);
		break;
	}
		
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

UINT __stdcall ThreadReconnect(void *p)
{
	while (WaitForSingleObject(hThreadEvent, 25) == WAIT_TIMEOUT)
	{
		list<LONG> TempList;
		if (csListConnect.TryLock())
		{
			if (listConnect.size())
				listConnect.swap(TempList);
			csListConnect.Unlock();
		}
		if (!TempList.size())
			continue;
		
		TraceMsgA("%s %d TempList Size = %d.\n", __FUNCTION__,(int)time(0), TempList.size());
		for (auto it = TempList.begin(); it != TempList.end();)
		{
			double dfT1 = GetExactTime();
			((_RTSPConnection *)(*it))->Reconnect();
			if (TimeSpanEx(dfT1)> 1.0f)
				TraceMsgA("%s %d Timespan of Reconnect = %.3f.\n", __FUNCTION__,(int)time(0), TimeSpanEx(dfT1));
			it = TempList.erase(it);
		}
	} ;

	return 0;
}

UINT __stdcall ThreadWaitPrimaryProcessExit(void *p)
{
	HANDLE hPrimaryProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, g_dwPrimaryPID);
	HANDLE hPrimaryThread = OpenThread(THREAD_ALL_ACCESS, FALSE, g_dwPrimaryTID);
#define  EventCount	(2)
	HANDLE hExitEvent[EventCount] = { hPrimaryProcess, hPrimaryThread };
	do 
	{
		DWORD dwResult = WaitForMultipleObjects(EventCount, hExitEvent, FALSE, 40);
		if (dwResult != WAIT_TIMEOUT)
			break;
		if (!IsWindow(g_hPrimaryWnd))
			break;

	} while (true);
	PostMessage(g_hWindow, WM_DESTROY, 0, 0);
	
	CloseHandle(hPrimaryThread);
	CloseHandle(hPrimaryProcess);
	return 0;
}
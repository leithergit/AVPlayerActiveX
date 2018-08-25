
#pragma once
//#include "Common.h"
/// @brief 功能单一的简单窗口类，用于需要窗口，却不能或不需要显示窗口的场合，如DirectX和DirectSound的初始化
class  CSimpleWnd
{
public:
	CSimpleWnd(int nWidth = 0, int nHeight = 0,HWND hParent = NULL)
	{
		m_hWnd = CreateSimpleWindow(nullptr, nWidth, nHeight,hParent);
		if (!m_hWnd)
		{
			m_hWnd = nullptr;
			assert(false);
		}
		//::ShowWindow(m_hWnd, SW_HIDE);
	}

	CSimpleWnd(RECT* pRectWnd,HWND hParent = NULL)
	{
		m_hWnd = CreateSimpleWindow(nullptr, pRectWnd,hParent);
		if (!m_hWnd)
		{
			m_hWnd = nullptr;
			assert(false);
		}
		//::ShowWindow(m_hWnd, SW_HIDE);
	}
	void ShowWindow(bool bShow = true)
	{
		if (bShow)
			::ShowWindow(m_hWnd,SW_SHOW);
		else
			::ShowWindow(m_hWnd,SW_HIDE);
	}
	~CSimpleWnd()
	{
		if (m_hWnd)
		{
			DestroyWindow(m_hWnd);
			m_hWnd = nullptr;
		}
	}
	// 创建一个简单的不可见的窗口
	HWND CreateSimpleWindow(IN HINSTANCE hInstance = NULL, int nWidth = 0, int nHeight = 0,HWND hParentWnd = NULL)
	{
		WNDCLASSEX wcex;

		TCHAR *szWindowClass = _T("SimpleWnd");
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcex.lpfnWndProc = DefWindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = NULL;
		RegisterClassEx(&wcex);
		if (!nWidth || !nHeight)
		{
			nWidth = GetSystemMetrics(SM_CXSCREEN);
			nHeight = GetSystemMetrics(SM_CYSCREEN);
		}
		TraceMsgA("%s nWidth = %d\tnHeight = %d", __FUNCTION__, nWidth, nHeight);
		return ::CreateWindow(szWindowClass,			// 窗口类
			szWindowClass,				// 窗口标题 
			WS_VISIBLE|WS_CHILD,		// 窗口风格
			CW_USEDEFAULT, 				// 窗口左上角X坐标
			0, 							// 窗口左上解Y坐标
			nWidth, 					// 窗口宽度
			nHeight, 					// 窗口高度
			hParentWnd, 				// 父窗口句柄
			NULL,						// 菜单句柄
			hInstance,
			NULL);
	}
	// 创建一个简单的不可见的窗口
	HWND CreateSimpleWindow(IN HINSTANCE hInstance = NULL, RECT* pRtWnd = NULL,HWND hParentWnd = NULL)
	{
		WNDCLASSEX wcex;

		TCHAR *szWindowClass = _T("SimpleWnd");
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcex.lpfnWndProc = DefWindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = NULL;
		RegisterClassEx(&wcex);
		int nWidth = 0,nHeight = 0;
		int nLeft = 0,nTop = 0;
		if (!pRtWnd)
		{
			nWidth = GetSystemMetrics(SM_CXSCREEN);
			nHeight = GetSystemMetrics(SM_CYSCREEN);
		}
		else
		{
			nLeft	 = pRtWnd->left;
			nTop	 = pRtWnd->top;
			nWidth	 = pRtWnd->right - pRtWnd->left;
			nHeight	 = pRtWnd->bottom - pRtWnd->top;
		}
		return ::CreateWindow(szWindowClass,			// 窗口类
			szWindowClass,				// 窗口标题 
			WS_VISIBLE|WS_CHILD,	// 窗口风格
			nLeft, 						// 窗口左上角X坐标
			nTop, 						// 窗口左上解Y坐标
			nWidth, 					// 窗口宽度
			nHeight, 					// 窗口高度
			hParentWnd, 				// 父窗口句柄
			NULL,						// 菜单句柄
			hInstance,
			NULL);
	}
	const HWND &GetSafeHwnd()
	{
		if (!m_hWnd)
			assert(false);
		return m_hWnd;
	}
private:
	HWND m_hWnd;
};
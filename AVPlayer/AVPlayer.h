#pragma once

// AVPlayer.h : main header file for AVPlayer.DLL

#if !defined( __AFXCTL_H__ )
#error "include 'afxctl.h' before including this file"
#endif

#include "resource.h"       // main symbols
#include "TimeUtility.h"

// CAVPlayerApp : See AVPlayer.cpp for implementation.

class CAVPlayerApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;


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
		if (strlen(m_szText) ==0)
			sprintf_s(szText, 4096, "%s\t_OUT_ %s \tTimeSpan = %.3f.\n",__FUNCTION__, m_szFunction, TimeSpanEx(m_dfTimeIn));
		else
			sprintf_s(szText, 4096, "%s\t_OUT_ %s %s\tTimeSpan = %.3f.\n", __FUNCTION__, m_szFunction,m_szText, TimeSpanEx(m_dfTimeIn));
		OutputDebugStringA(szText);
		
	}
private:
	double	m_dfTimeIn;
	CHAR	m_szFile[MAX_PATH];
	CHAR	m_szText[1024];
	CHAR	m_szFunction[256];
};



// TestTaskQueue.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTestTaskQueueApp:
// See TestTaskQueue.cpp for the implementation of this class
//

class CTestTaskQueueApp : public CWinApp
{
public:
	CTestTaskQueueApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTestTaskQueueApp theApp;
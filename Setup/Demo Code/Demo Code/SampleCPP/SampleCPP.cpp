
// SampleCPP.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SampleCPP.h"
#include "SampleCPPDlg.h"
#include "BugTrap.h"
#include "Utility.h"
#pragma comment(lib,"BugTrap.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSampleCPPApp

BEGIN_MESSAGE_MAP(CSampleCPPApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSampleCPPApp construction

CSampleCPPApp::CSampleCPPApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSampleCPPApp object

CSampleCPPApp theApp;


static void SetupExceptionHandler()
{	
	BT_SetAppName(_T("SampleCPP"));
	BT_SetActivityType(BTA_SAVEREPORT);
	TCHAR szReportPath[1024] = {0};
	GetAppPath(szReportPath,1024);
	BT_SetReportFilePath(szReportPath);
	BT_SetFlags(BTF_DETAILEDMODE /*|BTF_RESTARTAPP*/);
	BT_InstallSehFilter();
}

// CSampleCPPApp initialization

BOOL CSampleCPPApp::InitInstance()
{
	//SetupExceptionHandler();
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CSampleCPPDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

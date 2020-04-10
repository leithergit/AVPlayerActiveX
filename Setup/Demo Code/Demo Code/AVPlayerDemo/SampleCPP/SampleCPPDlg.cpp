
// SampleCPPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SampleCPP.h"
#include "SampleCPPDlg.h"

#include "Utility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SWITCH	WM_USER + 1024
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSampleCPPDlg dialog

CSampleCPPDlg::CSampleCPPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleCPPDlg::IDD, pParent)
	, m_fHei(10)
	, m_strDev1(_T("3301061000000"))
	, m_strDev2(_T("3301061000001"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSampleCPPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AVPLAYERCTRL, m_AvPlayer);
	DDX_Text(pDX, IDC_EDIT_HEI, m_fHei);
	DDX_Text(pDX, IDC_EDIT_DEVID1, m_strDev1);
	DDX_Text(pDX, IDC_EDIT_DEVID2, m_strDev2);
}

BEGIN_MESSAGE_MAP(CSampleCPPDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CSampleCPPDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CSampleCPPDlg::OnBnClickedButtonLogout)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSampleCPPDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_PLAYCOMBO, &CSampleCPPDlg::OnBnClickedButtonPlaycombo)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CSampleCPPDlg::OnBnClickedButtonMove)
	ON_BN_CLICKED(IDC_BUTTON_MOVEEx, &CSampleCPPDlg::OnBnClickedButtonMoveex)
	ON_BN_CLICKED(IDC_BUTTON1, &CSampleCPPDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSampleCPPDlg message handlers

BOOL CSampleCPPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization
	m_ctlDeviceList.SubclassDlgItem(IDC_LIST_DEVICE,this);
	m_ctlDeviceList.SetExtendedStyle(m_ctlDeviceList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER|LVS_EX_CHECKBOXES);

	int nCols = 0;
	m_ctlDeviceList.InsertColumn(nCols++, _T("NO."), LVCFMT_LEFT, 60);
	m_ctlDeviceList.InsertColumn(nCols++, _T("Device ID"), LVCFMT_LEFT, 100);
	SetDlgItemText(IDC_IPADDRESS1,_T("127.0.0.1"));
	//ZeroMemory(m_bFrameused,sizeof(m_bFrameused));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSampleCPPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSampleCPPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSampleCPPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSampleCPPDlg::OnBnClickedButtonLogin()
{
	CString strServerIP;
	GetDlgItemText(IDC_IPADDRESS1,strServerIP);
	int nErrorCode = m_AvPlayer.Login(strServerIP,9000,_T("admin"),_T("123456"),0);
	if (nErrorCode == 0)
	{// 登录成功
		BSTR strDevice;
		LONG nDeviceCount = 0;
		m_AvPlayer.GetDeviceID(&strDevice,&nDeviceCount);
		if (nDeviceCount <= 0)
		{
			AfxMessageBox(_T("没有在线的设备."),MB_ICONSTOP);
			return ;
		}
		_bstr_t bstrDevice = strDevice;
		TCHAR *szDevice = bstrDevice;
		TCHAR szSeps[] = _T(";");
		TCHAR *szToken = NULL;
		TCHAR *szNextToken = NULL;
		 szToken = _tcstok_s( szDevice, szSeps, &szNextToken);  
		 int nCount = 0;
		 TCHAR szItemText[64] = {0};
		 while(szToken)
		 {
			 if (szToken)  
			 {  
				 _stprintf_s(szItemText,64,_T("%d"),nCount + 1);
				 m_ctlDeviceList.InsertItem(nCount,szItemText);	
				 m_ctlDeviceList.SetItemText(nCount,1,szToken);
				 szToken = _tcstok_s( NULL, szSeps, &szNextToken); 
				 nCount ++;
			 }  
		 }
		 GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
		 GetDlgItem(IDC_IPADDRESS1)->EnableWindow(FALSE);
		 GetDlgItem(IDC_BUTTON_LOGOUT)->EnableWindow(TRUE);
		 GetDlgItem(IDC_BUTTON_PLAYCOMBO)->EnableWindow(TRUE);
		 m_AvPlayer.FreeString(&strDevice);
	}
	else
	{
		BSTR strMessage;
		m_AvPlayer.GetErrorMessage(nErrorCode,&strMessage);
		AfxMessageBox(strMessage);
		m_AvPlayer.FreeString(&strMessage);
	}
}

void CSampleCPPDlg::OnBnClickedButtonLogout()
{
	m_AvPlayer.Logout();
	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_IPADDRESS1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOGOUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAYCOMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPADDRESS1)->EnableWindow(TRUE);
}

void CSampleCPPDlg::OnBnClickedButtonStop()
{
	if (m_vecDevicePlaying.size() > 0)
	{
		for (vector<wstring>::iterator it = m_vecDevicePlaying.begin();
			it != m_vecDevicePlaying.end();)
		{
			//HWND hWndArray[16] = {0};
			//long nArraySize = 16; 
			//int nErrorCode = m_AvPlayer.GetDeviceWindow(it->c_str(),(long *)hWndArray,&nArraySize);
			m_AvPlayer.StopPlay(it->c_str(),NULL);
			it = m_vecDevicePlaying.erase(it);
		}
	}
	//ZeroMemory(m_bFrameused,sizeof(m_bFrameused));

	GetDlgItem(IDC_BUTTON_PLAYCOMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
}

void CSampleCPPDlg::OnBnClickedButtonPlaycombo()
{
	int nCount = m_ctlDeviceList.GetItemCount();
	
	int nIndex = 0;
	for (int i = 0;i < nCount;i ++)
	{
		if (m_ctlDeviceList.GetCheck(i) )
		{
			m_ctlDeviceList.GetItemText(i,1,m_szDeviceID[nIndex ++],32);
			if (nIndex >=2)
			{
				break;
			}
		}
	}

	if (nIndex >=2)
	{
		HWND hVideoWnd = GetDlgItem(IDC_STATIC_FRAME)->GetSafeHwnd();
		m_AvPlayer.PlayComboStream(m_szDeviceID[0],m_szDeviceID[1],(long)hVideoWnd,false,1);
		m_vecDevicePlaying.push_back(m_szDeviceID[0]);
		m_vecDevicePlaying.push_back(m_szDeviceID[1]);
		GetDlgItem(IDC_BUTTON_PLAYCOMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	}
}

void CSampleCPPDlg::OnBnClickedButtonMove()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_AvPlayer.SetPlayComboBorder(m_fHei);
}

void CSampleCPPDlg::OnBnClickedButtonMoveex()
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
	//m_AvPlayer.SetPlayComboBorderEx(_T("3301061000004"),_T("3301061000007"),m_fHei);
	m_AvPlayer.SetPlayComboBorderEx(m_strDev1,m_strDev2,m_fHei);

}

void CSampleCPPDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	for(int i = 0;i< 100;i++)
	{
		m_AvPlayer.SetPlayComboBorderEx(_T("3301061000000"),_T("3301061000001"),10);
		Sleep(10);
		m_AvPlayer.SetPlayComboBorderEx(_T("3301061000000"),_T("3301061000001"),20);
		Sleep(10);
		m_AvPlayer.SetPlayComboBorderEx(_T("3301061000000"),_T("3301061000001"),30);
		Sleep(10);
		m_AvPlayer.SetPlayComboBorderEx(_T("3301061000000"),_T("3301061000001"),40);
		Sleep(10);
		m_AvPlayer.SetPlayComboBorderEx(_T("3301061000000"),_T("3301061000001"),50);
		Sleep(10);
	}
	//CreateThread(NULL,0,MonitorThread,this,0,&dwThreadID);
}


// SampleCPPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SampleCPP.h"
#include "SampleCPPDlg.h"

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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CSampleCPPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AVPLAYERCTRL1, m_AvPlayer);
}

BEGIN_MESSAGE_MAP(CSampleCPPDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CSampleCPPDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CSampleCPPDlg::OnBnClickedButtonLogout)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CSampleCPPDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSampleCPPDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_STARTSWITCH, &CSampleCPPDlg::OnBnClickedButtonStartswitch)
	ON_BN_CLICKED(IDC_BUTTON_STOPSWITCH, &CSampleCPPDlg::OnBnClickedButtonStopswitch)
	ON_MESSAGE(WM_SWITCH,OnSwitch)
	ON_WM_TIMER()
	ON_COMMAND(ID_FILE_OUTPUT, &CSampleCPPDlg::OnFileOutput)
	ON_BN_CLICKED(IDC_BUTTON_SWITCH, &CSampleCPPDlg::OnBnClickedButtonSwitch)
	ON_BN_CLICKED(IDC_CHECK1, &CSampleCPPDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CSampleCPPDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CSampleCPPDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_BUTTON_STOPEXTEND, &CSampleCPPDlg::OnBnClickedButtonStopextend)
	ON_BN_CLICKED(IDC_BUTTON_STOPEXTEND2, &CSampleCPPDlg::OnBnClickedButtonStopextend2)
	ON_BN_CLICKED(IDC_BUTTON_STOPEXTEND3, &CSampleCPPDlg::OnBnClickedButtonStopextend3)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &CSampleCPPDlg::OnBnClickedButtonShow)
	ON_BN_CLICKED(IDC_BUTTON_HIDE, &CSampleCPPDlg::OnBnClickedButtonHide)
	ON_BN_CLICKED(IDC_BUTTON_PLAYCOMBO, &CSampleCPPDlg::OnBnClickedButtonPlaycombo)
	ON_BN_CLICKED(IDC_BUTTON_PTZPannel, &CSampleCPPDlg::OnBnClickedButtonPtzpannel)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ACTIVATE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_DEVICE, &CSampleCPPDlg::OnNMClickListDevice)
END_MESSAGE_MAP()


// CSampleCPPDlg message handlers

BOOL CSampleCPPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

// 	CMenu* pSysMenu = GetSystemMenu(FALSE);
// 	if (pSysMenu != NULL)
// 	{
// 		BOOL bNameValid;
// 		CString strAboutMenu;
// 		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
// 		ASSERT(bNameValid);
// 		if (!strAboutMenu.IsEmpty())
// 		{
// 			pSysMenu->AppendMenu(MF_SEPARATOR);
// 			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
// 		}
// 	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_bIsLButtonDown = false;

	m_pDialogPTZ =new CDialogPTZ();
	m_pDialogPTZ->m_pAVPlayer = &m_AvPlayer;
	m_pDialogPTZ->Create(IDD_DIALOG_PTZ);
	m_ctlDeviceList.SubclassDlgItem(IDC_LIST_DEVICE,this);

	m_ctlDeviceList.SetExtendedStyle(m_ctlDeviceList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER|LVS_EX_CHECKBOXES);

	int nCols = 0;
	m_ctlDeviceList.InsertColumn(nCols++, _T("NO."), LVCFMT_LEFT, 60);
	m_ctlDeviceList.InsertColumn(nCols++, _T("Device ID"), LVCFMT_LEFT, 100);
	SetDlgItemText(IDC_IPADDRESS1,_T("192.168.189.129"));
	//SetDlgItemText(IDC_IPADDRESS1,_T("192.168.1.128"));
	m_nNextDevNo = 0;
	SetDlgItemText(IDC_EDIT_SWITCHTIME,_T("30"));
	SetDlgItemText(IDC_EDIT_NEXTSWITCHTIME,_T("30"));
	SetDlgItemText(IDC_EDIT_SWITCHCOUNT,_T("0"));
	ZeroMemory(m_bFrameused,sizeof(m_bFrameused));

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
		 GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
		 GetDlgItem(IDC_BUTTON_STARTSWITCH)->EnableWindow(TRUE);
		 GetDlgItem(IDC_BUTTON_STOPSWITCH)->EnableWindow(TRUE);
		 GetDlgItem(IDC_BUTTON_SWITCH)->EnableWindow(TRUE);
		 GetDlgItem(IDC_BUTTON_PLAYCOMBO)->EnableWindow(TRUE);
		 m_AvPlayer.FreeString(&strDevice);
		   
		 //GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
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
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	 GetDlgItem(IDC_BUTTON_SWITCH)->EnableWindow(FALSE);
	 GetDlgItem(IDC_IPADDRESS1)->EnableWindow(TRUE);
}

void CSampleCPPDlg::OnBnClickedButtonPlay()
{
	int nCount = m_ctlDeviceList.GetItemCount();
	UINT nFrameID[] = {IDC_STATIC_FRAME,IDC_STATIC_FRAME2,IDC_STATIC_FRAME3,IDC_STATIC_FRAME4};
	TCHAR szDeviceID[32] = {0};
	for (int i = 0;i < nCount;i ++)
	{
		if (m_ctlDeviceList.GetCheck(i) && m_vecDevicePlaying.size() < 4)
		{
			int nFrameIndex = -1;
			for (int k = 0;k < 4;k ++)
				if (!m_bFrameused[k])
				{
					nFrameIndex = k;
					break;
				}
			if (nFrameIndex == -1)
			{
				AfxMessageBox(_T("There no Free frame."));
				return;
			}
			m_bFrameused[nFrameIndex] = true;
			HWND hVideoWnd = GetDlgItem(nFrameID[nFrameIndex])->GetSafeHwnd();
			m_ctlDeviceList.GetItemText(i,1,szDeviceID,32);
			if (m_AvPlayer.PlayStream(szDeviceID,(long)hVideoWnd,0) ==0)
			{
				HWND hWndArray[16] = {0};
				long nArraySize = 16; 
				int nErrorCode = m_AvPlayer.GetDeviceWindow(szDeviceID,(long *)hWndArray,&nArraySize);
				m_vecDevicePlaying.push_back(szDeviceID);
			}
		}
	}
	if (m_vecDevicePlaying.size() > 0)
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
}

void CSampleCPPDlg::OnBnClickedButtonStop()
{
	if (m_vecDevicePlaying.size() > 0)
	{
		for (vector<wstring>::iterator it = m_vecDevicePlaying.begin();
			it != m_vecDevicePlaying.end();)
		{
			HWND hWndArray[16] = {0};
			long nArraySize = 16; 
			int nErrorCode = m_AvPlayer.GetDeviceWindow(it->c_str(),(long *)hWndArray,&nArraySize);


			m_AvPlayer.StopPlay(it->c_str(),NULL);
			it = m_vecDevicePlaying.erase(it);
		}
	}
	ZeroMemory(m_bFrameused,sizeof(m_bFrameused));

	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
}
#define ID_TIMER	1024
void CSampleCPPDlg::OnBnClickedButtonStartswitch()
{
	m_nNextDevNo = 0;
	m_nTimerToSwitch = GetDlgItemInt(IDC_EDIT_SWITCHTIME);
	SetDlgItemText(IDC_EDIT_SWITCHTIME,_T("30"));
	SetDlgItemText(IDC_EDIT_NEXTSWITCHTIME,_T("30"));
	SetDlgItemText(IDC_EDIT_SWITCHCOUNT,_T("0"));
	m_nSwitchCount = 0;

	SetTimer(ID_TIMER,1000,NULL);
	PostMessage(WM_SWITCH);
	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPADDRESS1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOGOUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STARTSWITCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOPSWITCH)->EnableWindow(TRUE);	
}

void CSampleCPPDlg::OnBnClickedButtonStopswitch()
{
	KillTimer(ID_TIMER);
	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPADDRESS1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOGOUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STARTSWITCH)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOPSWITCH)->EnableWindow(FALSE);	
}


void CSampleCPPDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == ID_TIMER)
	{
		if (m_nTimerToSwitch == 0)
		{
			PostMessage(WM_SWITCH);
			m_nTimerToSwitch = GetDlgItemInt(IDC_EDIT_SWITCHTIME);
		}
		else
		{
			m_nTimerToSwitch --;
			SetDlgItemInt(IDC_EDIT_SWITCHCOUNT,m_nSwitchCount);
			SetDlgItemInt(IDC_EDIT_NEXTSWITCHTIME,m_nTimerToSwitch);
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

LRESULT CSampleCPPDlg::OnSwitch(WPARAM wParam, LPARAM lParam)
{
	// 先停止当前正在播放的视频
	if (m_vecDevicePlaying.size() > 0)
	{
		for (vector<wstring>::iterator it = m_vecDevicePlaying.begin();
			it != m_vecDevicePlaying.end();)
		{
			m_AvPlayer.StopPlay(it->c_str(),NULL);
			it = m_vecDevicePlaying.erase(it);
		}
	}
	//KillTimer(ID_TIMER);
	UINT nFrameID[] = {IDC_STATIC_FRAME,IDC_STATIC_FRAME2,IDC_STATIC_FRAME3,IDC_STATIC_FRAME4};
	int nCount = m_ctlDeviceList.GetItemCount();
	
	if (m_nNextDevNo < (nCount -1))
	{
		TCHAR szDeviceID[32] = {0};
		while(m_vecDevicePlaying.size() < 4)
		{
			HWND hVideoWnd = GetDlgItem(nFrameID[m_vecDevicePlaying.size()])->GetSafeHwnd();
			m_ctlDeviceList.GetItemText(m_nNextDevNo,1,szDeviceID,32);
			if (m_AvPlayer.PlayStream(szDeviceID,(long)hVideoWnd,0) ==0)
			{
				m_vecDevicePlaying.push_back(szDeviceID);
				m_nNextDevNo ++;
				if (m_nNextDevNo == nCount)
					m_nNextDevNo = 0;
			}
		}
	}
	m_nSwitchCount ++;
	return 0;
}
void CSampleCPPDlg::OnFileOutput()
{
	//VLDReportLeaks ();
}

void CSampleCPPDlg::OnBnClickedButtonSwitch()
{
	m_nSwitchCount = 0;
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
	OnSwitch(0,0);
}
BEGIN_EVENTSINK_MAP(CSampleCPPDlg, CDialog)
	ON_EVENT(CSampleCPPDlg, IDC_AVPLAYERCTRL, 1, CSampleCPPDlg::RecvTimeoutAvplayerctrl, VTS_BSTR VTS_I4)
END_EVENTSINK_MAP()


void CSampleCPPDlg::OnBnClickedCheck1()
{
	TCHAR szDeviceID[32] = {0};
	int nCount = m_ctlDeviceList.GetItemCount();
	if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
	{
		for (int i = 0;i < nCount;i ++)
		{
			if (m_ctlDeviceList.GetCheck(i) )
			{
				m_ctlDeviceList.GetItemText(i,1,szDeviceID,32);
				HWND hVideoWnd = GetDlgItem(IDC_STATIC_FRAME2)->GetSafeHwnd();
				m_ctlDeviceList.GetItemText(i,1,szDeviceID,32);
				if (m_AvPlayer.PlayStream(szDeviceID,(long)hVideoWnd,1) ==0)
				{
					
				}
				break;
			}
		}
	}
	else
	{
		HWND hVideoWnd = GetDlgItem(IDC_STATIC_FRAME2)->GetSafeHwnd();
		BSTR bstrDeviceWnd;
		if (m_AvPlayer.GetWindowDevice((long)hVideoWnd,&bstrDeviceWnd) == AvError_Succeed)
		{
			_bstr_t bstrDevice = bstrDeviceWnd;
			TCHAR *szDeviceWnd = bstrDevice;
			m_AvPlayer.StopPlay(szDeviceWnd,(long)hVideoWnd);
			::InvalidateRect(hVideoWnd,NULL,TRUE);
		}
	}
}

void CSampleCPPDlg::OnBnClickedCheck2()
{
	TCHAR szDeviceID[32] = {0};
	int nCount = m_ctlDeviceList.GetItemCount();
	if (IsDlgButtonChecked(IDC_CHECK2) == BST_CHECKED)
	{
		for (int i = 0;i < nCount;i ++)
		{
			if (m_ctlDeviceList.GetCheck(i) )
			{
				m_ctlDeviceList.GetItemText(i,1,szDeviceID,32);
				HWND hVideoWnd = GetDlgItem(IDC_STATIC_FRAME3)->GetSafeHwnd();
				m_ctlDeviceList.GetItemText(i,1,szDeviceID,32);
				if (m_AvPlayer.PlayStream(szDeviceID,(long)hVideoWnd,1) ==0)
				{

				}
				break;
			}
		}
	}
	else
	{
		HWND hVideoWnd = GetDlgItem(IDC_STATIC_FRAME3)->GetSafeHwnd();
		WCHAR szDeviceWnd[32] = {0};
		if (m_AvPlayer.GetWindowDevice((long)hVideoWnd,(BSTR *)&szDeviceWnd) == AvError_Succeed)
		{
			m_AvPlayer.StopPlay(szDeviceWnd,(long)hVideoWnd);
			::InvalidateRect(hVideoWnd,NULL,TRUE);
		}
	}
}

void CSampleCPPDlg::OnBnClickedCheck3()
{
	TCHAR szDeviceID[32] = {0};
	int nCount = m_ctlDeviceList.GetItemCount();
	if (IsDlgButtonChecked(IDC_CHECK3) == BST_CHECKED)
	{
		for (int i = 0;i < nCount;i ++)
		{
			if (m_ctlDeviceList.GetCheck(i) )
			{
				m_ctlDeviceList.GetItemText(i,1,szDeviceID,32);
				HWND hVideoWnd = GetDlgItem(IDC_STATIC_FRAME4)->GetSafeHwnd();
				m_ctlDeviceList.GetItemText(i,1,szDeviceID,32);
				if (m_AvPlayer.PlayStream(szDeviceID,(long)hVideoWnd,1) ==0)
				{

				}
				break;
			}
		}
	}
	else
	{
		HWND hVideoWnd = GetDlgItem(IDC_STATIC_FRAME3)->GetSafeHwnd();
		WCHAR szDeviceWnd[32] = {0};
		if (m_AvPlayer.GetWindowDevice((long)hVideoWnd,(BSTR *)&szDeviceWnd) == AvError_Succeed)
		{
			m_AvPlayer.StopPlay(szDeviceWnd,(long)hVideoWnd);
			::InvalidateRect(hVideoWnd,NULL,TRUE);
		}
	}
}

void CSampleCPPDlg::OnBnClickedCheck4()
{

}

void CSampleCPPDlg::RecvTimeoutAvplayerctrl(LPCTSTR strDeviceID, long hWnd)
{
	OutputDebugStringA(__FUNCTION__);
	OutputDebugStringA("\n");
	m_AvPlayer.StopPlay(strDeviceID,NULL);
	SetTimer(1024,5000,NULL);
	HWND hVideoWnd = GetDlgItem(IDC_STATIC_FRAME)->GetSafeHwnd();
	m_AvPlayer.PlayStream(strDeviceID,(long)hVideoWnd,false);
}

void CSampleCPPDlg::OnBnClickedButtonStopextend()
{
	TCHAR szDeviceID[32] = {0};
	int nCount = m_ctlDeviceList.GetItemCount();
	for (int i = 0;i < nCount;i ++)
	{
		if (m_ctlDeviceList.GetCheck(i) )
		{

			m_ctlDeviceList.GetItemText(i,1,szDeviceID,32);
			HWND hVideoWnd = GetDlgItem(IDC_STATIC_FRAME2)->GetSafeHwnd();
			m_ctlDeviceList.GetItemText(i,1,szDeviceID,32);
			m_AvPlayer.StopPlay(szDeviceID,(long)hVideoWnd);
			::InvalidateRect(hVideoWnd,NULL,TRUE);

			break;
		}
	}
}

void CSampleCPPDlg::OnBnClickedButtonStopextend2()
{
	TCHAR szDeviceID[32] = {0};
	int nCount = m_ctlDeviceList.GetItemCount();
	for (int i = 0;i < nCount;i ++)
	{
		if (m_ctlDeviceList.GetCheck(i) )
		{
			m_ctlDeviceList.GetItemText(i,1,szDeviceID,32);
			HWND hVideoWnd = GetDlgItem(IDC_STATIC_FRAME3)->GetSafeHwnd();
			m_ctlDeviceList.GetItemText(i,1,szDeviceID,32);
			m_AvPlayer.StopPlay(szDeviceID,(long)hVideoWnd);
			::InvalidateRect(hVideoWnd,NULL,TRUE);

			break;
		}
	}
}

void CSampleCPPDlg::OnBnClickedButtonStopextend3()
{
	TCHAR szDeviceID[32] = {0};
	int nCount = m_ctlDeviceList.GetItemCount();
	for (int i = 0;i < nCount;i ++)
	{
		if (m_ctlDeviceList.GetCheck(i) )
		{
			m_ctlDeviceList.GetItemText(i,1,szDeviceID,32);
			HWND hVideoWnd = GetDlgItem(IDC_STATIC_FRAME4)->GetSafeHwnd();
			m_ctlDeviceList.GetItemText(i,1,szDeviceID,32);
			m_AvPlayer.StopPlay(szDeviceID,(long)hVideoWnd);
			::InvalidateRect(hVideoWnd,NULL,TRUE);

			break;
		}
	}
}

void CSampleCPPDlg::OnBnClickedButtonShow()
{
	UINT nFrameID[] = {IDC_STATIC_FRAME,IDC_STATIC_FRAME2,IDC_STATIC_FRAME3,IDC_STATIC_FRAME4};
	for (int i = 0;i < 4;i ++)
		GetDlgItem(nFrameID[i])->ShowWindow(SW_SHOW);

}

void CSampleCPPDlg::OnBnClickedButtonHide()
{
	UINT nFrameID[] = {IDC_STATIC_FRAME,IDC_STATIC_FRAME2,IDC_STATIC_FRAME3,IDC_STATIC_FRAME4};
	for (int i = 0;i < 4;i ++)
		GetDlgItem(nFrameID[i])->ShowWindow(SW_HIDE);
}

void CSampleCPPDlg::OnBnClickedButtonPlaycombo()
{
	int nCount = m_ctlDeviceList.GetItemCount();
	UINT nFrameID[] = {IDC_STATIC_FRAME,IDC_STATIC_FRAME2,IDC_STATIC_FRAME3,IDC_STATIC_FRAME4};
	TCHAR szDeviceID[2][32] = {0};
	int nIndex = 0;
	for (int i = 0;i < nCount;i ++)
	{
		if (m_ctlDeviceList.GetCheck(i) )
		{
			m_ctlDeviceList.GetItemText(i,1,szDeviceID[nIndex ++],32);
			if (nIndex >=2)
				break;
		}
	}
	if (nIndex >=2)
	{
		int nFrameIndex = -1;
		for (int i = 0;i < 4;i ++)
			if (!m_bFrameused[i])
			{
				nFrameIndex = i;
				break;
			}
		if (nFrameIndex == -1)
		{
			AfxMessageBox(_T("There no Free frame."));
			return;
		}
		m_bFrameused[nFrameIndex] = true;
		HWND hVideoWnd = GetDlgItem(nFrameID[nFrameIndex])->GetSafeHwnd();

		m_AvPlayer.PlayComboStream(szDeviceID[0],szDeviceID[1],(long)hVideoWnd,false,1);
		m_vecDevicePlaying.push_back(szDeviceID[0]);
		m_vecDevicePlaying.push_back(szDeviceID[1]);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	}
}

void CSampleCPPDlg::OnBnClickedButtonPtzpannel()
{
	RECT rtPTZ;
	m_pDialogPTZ->GetWindowRect(&rtPTZ);
	int nWidthPtz = rtPTZ.right - rtPTZ.left;
	int nHeightPtz = rtPTZ.bottom - rtPTZ.top;

	RECT rtMain;
	this->GetWindowRect(&rtMain);
	int nWidthMain = rtMain.right - rtMain.left;
	int nHeightMain = rtMain.bottom - rtMain.top;
	
	int nCX = GetSystemMetrics(SM_CXFULLSCREEN);
	int nCY = GetSystemMetrics(SM_CYFULLSCREEN);
	rtPTZ.top = rtMain.top;
	rtPTZ.bottom = rtPTZ.top + nHeightPtz;

	if ((nWidthPtz + nWidthMain) < nCX)
	{
		if ((rtMain.right + nWidthPtz ) < nCX)
		{
			rtPTZ.left = rtMain.right + 1;
			rtPTZ.right = rtPTZ.left + nWidthPtz;
		}
		else
		{
			rtPTZ.right = nCX;
			rtPTZ.left = rtPTZ.right - nWidthPtz;
			rtMain.right = rtPTZ.left - 1;
			rtMain.left = rtMain.right - nWidthMain;
			this->MoveWindow(&rtMain);
		}
	}
	else
	{
		rtMain.left = 0;
		rtMain.right = rtMain.left + nWidthMain;
		this->MoveWindow(&rtMain);
		rtPTZ.right = nCX;
		rtPTZ.left = nCX - nWidthPtz;

	}
	m_pDialogPTZ->MoveWindow(&rtPTZ);
	
	m_pDialogPTZ->ShowWindow(SW_SHOW);
}

void CSampleCPPDlg::OnDestroy()
{
	CDialog::OnDestroy();

	delete m_pDialogPTZ;
}

void CSampleCPPDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
// 	CRect   rectPic;  
// 
// 	POINT   ptPut   = point;  
// 	GetDlgItem(IDC_LIST_DEVICE)->GetWindowRect(rectPic);  
// 	ClientToScreen(&ptPut);  
// 	if(rectPic.PtInRect(ptPut))  
// 	{  
// 		CBitmap     bitmapTemp, *pOldBitmap;  
// 		CDC         *pDC    = GetDlgItem(IDC_LIST_DEVICE)->GetDC();
// 		*pMemDC = new CDC;  
// 		//创建位图内存  
// 
// 		bitmapTemp.CreateCompatibleBitmap(pDC, rectPic.Width(), rectPic.Height());  
// 		pMemDC->CreateCompatibleDC(pDC);  
// 		pOldBitmap  = pMemDC->SelectObject(&bitmapTemp); 
// 		pMemDC->BitBlt(0, 0, rectPic.Width(), rectPic.Height(), pDC, 0, 0, SRCCOPY);  
// 		pMemDC->SelectObject(pOldBitmap);  
// 		delete  pMemDC;  
// 		ReleaseDC(pDC);  
// 		m_bIsLButtonDown    = TRUE; 
// 		m_ptOffset.x    = ptPut.x-rectPic.left; 
// 		m_ptOffset.y    = ptPut.y-rectPic.top;  
// 		m_imgDrag.DeleteImageList();  
// 		m_imgDrag.Create(rectPic.Width(), rectPic.Height(), ILC_COLOR32|ILC_MASK, 0, 1);  
// 		m_imgDrag.Add(&bitmapTemp, RGB(0, 0, 0));  
// 		m_imgDrag.BeginDrag(0, m_ptOffset);  
// 		m_imgDrag.DragEnter(NULL, ptPut);  
// 		SetCapture();  
// 	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CSampleCPPDlg::OnMouseMove(UINT nFlags, CPoint point)
{
// 	if(m_bIsLButtonDown)  
// 	{  
// 		CRect       rtClient, rtList;  
// 		m_ptMove    = point;  
// 		GetDlgItem(IDC_LIST_DEVICE)->GetWindowRect(rtList);  
// 		GetClientRect(rtClient);  
// 		ClientToScreen(&rtClient); 
// 		ClientToScreen(&m_ptMove); 
// 		if(rtClient.left>m_ptMove.x-m_ptOffset.x)  
// 			m_ptMove.x  = rtClient.left+m_ptOffset.x;
// 		if(rtClient.top>m_ptMove.y-m_ptOffset.y) 
// 			m_ptMove.y  = rtClient.top+m_ptOffset.y;  
// 
// 		if(rtClient.right-rtList.Width())
// 			m_ptMove.x  = rtClient.right-rtList.Width()+m_ptOffset.x;  
// 
// 		if(rtClient.bottom-rtList.Height())
// 			m_ptMove.y  = rtClient.bottom-rtList.Height()+m_ptOffset.y;  
// 
// 		CImageList::DragMove(m_ptMove);  
// 
// 	}  

	CDialog::OnMouseMove(nFlags, point);
}

void CSampleCPPDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
// 	if(m_bIsLButtonDown)  
// 	{  
// 		CRect   rectPic;  
// 		CWnd*   pPic    = GetDlgItem(IDC_LIST_DEVICE); 
// 		ScreenToClient(&m_ptMove);  
// 		pPic->GetWindowRect(rectPic);
// 		pPic->MoveWindow(m_ptMove.x-m_ptOffset.x, m_ptMove.y-m_ptOffset.y, rectPic.Width(), rectPic.Height()); 
// 
// 		m_bIsLButtonDown    = FALSE; 
// 		CImageList::DragLeave(this); 
// 		CImageList::EndDrag(); 
// 		ReleaseCapture();
// 		pPic->Invalidate(); 
// 	} 
	CDialog::OnLButtonUp(nFlags, point);
}

void CSampleCPPDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

// 	if(nState==WA_INACTIVE)//当失去焦点后,
// 	{
// 		m_bIsLButtonDown    = FALSE;
// 
// 		CImageList::DragLeave(this);
// 		CImageList::EndDrag();
// 		ReleaseCapture();
// 	}
}

void CSampleCPPDlg::OnNMClickListDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem != -1 && pNMItemActivate->iSubItem != -1)
	{
		TCHAR szItem[128] = {0};
		m_ctlDeviceList.GetItemText(pNMItemActivate->iItem,pNMItemActivate->iSubItem,szItem,128);
		if (m_pDialogPTZ)
			m_pDialogPTZ->SetDlgItemText(IDC_EDIT_CAMERAID,szItem);
	}
	*pResult = 0;
}

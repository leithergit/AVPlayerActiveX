
// SampleCPPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SampleCPP.h"
#include "SampleCPPDlg.h"
#include "TimeUtility.h"
#include "Utility.h"
#include <MMSystem.h>
#include <Markup.h>

#pragma comment(lib,"winmm.lib")
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
	DDX_Control(pDX, IDC_DATEPICKER, m_Date);
	DDX_Control(pDX, IDC_TIMEPICKER1, m_StartTime);
	DDX_Control(pDX, IDC_TIMEPICKER2, m_StopTime);
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
	ON_MESSAGE(WM_HOTKEY, OnHotkey)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DEVICE, &CSampleCPPDlg::OnNMClickListDevice)
	ON_BN_CLICKED(IDC_CHECK_DCDRAW, &CSampleCPPDlg::OnBnClickedCheckDcdraw)
	ON_BN_CLICKED(IDC_BUTTON_UPDATEAssist, &CSampleCPPDlg::OnBnClickedButtonUpdateassist)
	ON_CBN_SELENDOK(IDC_COMBO_SWITCH, &CSampleCPPDlg::OnCbnSelendokComboSwitch)
	ON_WM_SIZE()
	ON_CBN_SELENDOK(IDC_COMBO_DIV, &CSampleCPPDlg::OnCbnSelendokComboDiv)
	ON_BN_CLICKED(IDC_BUTTON_QUERYRECORD, &CSampleCPPDlg::OnBnClickedButtonQueryrecord)
	ON_BN_CLICKED(IDC_BUTTON_PLAYRECORD, &CSampleCPPDlg::OnBnClickedButtonPlayrecord)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

struct HotKeyInfo
{
	TCHAR *szName;
	UINT  nID;
};
static HotKeyInfo HotKeyArray[] =
{
	{ _T("Alt + `"), ID_OEM_3 },
	{ _T("Alt + F1"), ID_F1 },
	{ _T("Alt + F2"), ID_F2 },
	{ _T("Alt + F3"), ID_F3 },
	{ _T("Alt + F4"), ID_F4 },
	{ _T("Alt + F5"), ID_F5 },
	{ _T("Alt + F6"), ID_F6 },
	{ _T("Alt + F12"), ID_F12 }
};

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
	m_pRunlog = new CRunlog(_T("Sample"));
	
	CRect rtClient;
	GetDlgItemRect(IDC_STATIC_FRAME, rtClient);
	m_pVideoFrame = new CVideoFrame;
	m_pVideoFrame->Create(1024, rtClient, 4, this);

	m_bIsLButtonDown = false;
	m_AvPlayer.ShowWindow(SW_SHOW);
	m_pDialogPTZ =new CDialogPTZ();
	m_pDialogPTZ->m_pAVPlayer = &m_AvPlayer;
	m_pDialogPTZ->Create(IDD_DIALOG_PTZ);
	m_ctlDeviceList.SubclassDlgItem(IDC_LIST_DEVICE,this);

	m_ctlDeviceList.SetExtendedStyle(m_ctlDeviceList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER|LVS_EX_CHECKBOXES);

	
	int nCols = 0;
	m_ctlDeviceList.InsertColumn(nCols++, _T("NO."), LVCFMT_LEFT, 60);
	m_ctlDeviceList.InsertColumn(nCols++, _T("Device ID"), LVCFMT_LEFT, 100);
	
	m_listRecord.SubclassDlgItem(IDC_LIST_RECORD, this);
	DWORD dwExtendedStyle = m_listRecord.GetExtendedStyle();
	dwExtendedStyle |= LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES;
	int nIndex = 0;
	m_listRecord.SetExtendedStyle(dwExtendedStyle);

	m_listRecord.InsertColumn(nIndex++, _T("No."), LVCFMT_LEFT, 30);
	m_listRecord.InsertColumn(nIndex++, _T("Start"), LVCFMT_LEFT, 80);
	m_listRecord.InsertColumn(nIndex++, _T("Stop"), LVCFMT_LEFT, 80);
	//m_listRecord.InsertColumn(nIndex++, _T("Type"), LVCFMT_LEFT, 30);
	//SetDlgItemText(IDC_IPADDRESS1,_T("192.168.2.99"));
	//SetDlgItemText(IDC_IPADDRESS1, _T("10.0.0.100"));
	//SetDlgItemText(IDC_IPADDRESS1, _T("192.168.254.129"));
	//SetDlgItemText(IDC_IPADDRESS1, _T("192.168.1.104"));
	//SetDlgItemText(IDC_IPADDRESS1,_T("10.28.254.98"));
	m_nNextDevNo = 0;
	SetDlgItemText(IDC_EDIT_SWITCHTIME,_T("30"));
	SetDlgItemText(IDC_EDIT_NEXTSWITCHTIME,_T("30"));
	SetDlgItemText(IDC_EDIT_SWITCHCOUNT,_T("0"));
	ZeroMemory(m_bFrameused,sizeof(m_bFrameused));

	m_bitmapMago.LoadBitmap(IDB_BITMAP_MAGO);
	m_bitmapZpmc.LoadBitmap(IDB_BITMAP_ZPMC);

	UpdateServer();


	::RegisterHotKey(m_hWnd, ID_F12, MOD_ALT | MOD_CONTROL, VK_HOME);
	m_bmpActived.LoadBitmap( IDB_ACTIVED);
	m_bmpUnActived.LoadBitmap(IDB_UNACTIVED);
	
	m_Font.CreateFont(/*32*/-48, // nHeight 
	0, // nWidth 
	0, // nEscapement 
	0, // nOrientation 
	FW_NORMAL, // nWeight 
	FALSE, // bItalic 
	FALSE, // bUnderline 
	0, // cStrikeOut 
	DEFAULT_CHARSET, // nCharSet 
	OUT_DEFAULT_PRECIS, // nOutPrecision 
	CLIP_DEFAULT_PRECIS, // nClipPrecision 
	DEFAULT_QUALITY, // nQuality 
	DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
	_T("微软雅黑"));

	CTime cur_time = CTime::GetCurrentTime();
	CTime bTime = CTime(cur_time.GetYear(), cur_time.GetMonth(), cur_time.GetDay(), 0, 0, 0);
	CTime eTime = CTime(cur_time.GetYear(), cur_time.GetMonth(), cur_time.GetDay(), 23, 59, 59);
	CTime begdateTime;

	begdateTime = CTime(bTime.GetYear(), bTime.GetMonth(), bTime.GetDay(), 0, 0, 0);
	m_Date.SetTime(&begdateTime);
	m_StartTime.SetTime(&bTime);
	m_StopTime.SetTime(&eTime);

	UINT nArrayLeftTop[] = { IDC_STATIC_Server,IDC_IPADDRESS1, IDC_BUTTON_LOGIN, IDC_BUTTON_LOGOUT, IDC_BUTTON_PLAY, IDC_BUTTON_PLAYCOMBO, IDC_BUTTON_STOP,
		IDC_BUTTON_STARTSWITCH, IDC_BUTTON_STOPSWITCH, IDC_CHECK1, IDC_CHECK2, IDC_CHECK3, IDC_STATIC_CONFIG, IDC_STATIC_CONFIGWND,
		IDC_STATIC_SWITCHTIME, IDC_EDIT_SWITCHTIME, IDC_STATIC_NEXTT, IDC_EDIT_NEXTSWITCHTIME, IDC_STATIC_SWITCHCOUNT, IDC_EDIT_SWITCHCOUNT,
		IDC_BUTTON_SHOW, IDC_BUTTON_HIDE, IDC_BUTTON_HIDE, IDC_BUTTON_PTZPannel, IDC_BUTTON_SWITCH, IDC_BUTTON_STOPEXTEND, IDC_BUTTON_STOPEXTEND2,
		IDC_BUTTON_STOPEXTEND3, IDC_STATIC_SWITCHMODE, IDC_COMBO_SWITCH, IDC_CHECK_DCDRAW, IDC_CHECK_ENABLETRANLATE, IDC_BUTTON_UPDATEAssist };
	UINT nIDArrayRight[] = { IDC_STATIC_DEV };
	UINT nIDArrayRightTopBottom[] = {  IDC_LIST_DEVICE };
	UINT nIDArrayRightBottom[] = { IDC_LIST_RECORD, IDC_DATEPICKER, IDC_TIMEPICKER1, IDC_TIMEPICKER2, IDC_BUTTON_QUERYRECORD, IDC_BUTTON_PLAYRECORD, IDC_CHECK_SEEKFRAME };
	UINT nIDArrayCenter[] = { IDC_STATIC_FRAME };

	RECT rtDialog;
	GetClientRect(&rtDialog);

	m_WndSizeManager.SetParentWnd(this);
	m_WndSizeManager.SaveWndPosition(nArrayLeftTop, sizeof(nArrayLeftTop) / sizeof(UINT), (DockType)(DockLeft ));
	m_WndSizeManager.SaveWndPosition(nIDArrayCenter, sizeof(nIDArrayCenter) / sizeof(UINT), DockCenter, false);
	m_WndSizeManager.SaveWndPosition(nIDArrayRight, sizeof(nIDArrayRight) / sizeof(UINT), DockRight);
	m_WndSizeManager.SaveWndPosition(nIDArrayRightBottom, sizeof(nIDArrayRightBottom) / sizeof(UINT), (DockType)(DockRight | DockBottom));
	m_WndSizeManager.SaveWndPosition(nIDArrayRightTopBottom, sizeof(nIDArrayRightTopBottom) / sizeof(UINT), (DockType)(DockRight |DockTop| DockBottom));

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
		m_pRunlog->Runlog(_T("登录成功.\n"));
		BSTR strDevice;
		LONG nDeviceCount = 0;
		m_AvPlayer.GetDeviceID(&strDevice,&nDeviceCount);
		if (nDeviceCount <= 0)
		{
			m_pRunlog->Runlog(_T("没有在线的设备.\n"));
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
		 // 从设备串中分离设备ID
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
		TCHAR strMessage[1024];
		_stprintf_s(strMessage, _T("Failed in login server,error code:%d."), nErrorCode);
		//m_AvPlayer.GetErrorMessage(nErrorCode,strMessage,1024);
		//m_pRunlog->Runlog(_T("登录失败 ErrorCode = %d,Message = %s.\n"), nErrorCode, strMessage);
		AfxMessageBox(strMessage);
		//m_AvPlayer.FreeString(&strMessage);
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
	TCHAR szDeviceID[32] = {0};
	for (int i = 0;i < nCount;i ++)
	{
		if (m_ctlDeviceList.GetCheck(i) && m_vecDevicePlaying.size() < 4)
		{
			int nFrameIndex = -1;
			for (int k = 0; k < 4; k++)
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
			HWND hVideoWnd = m_pVideoFrame->GetPanelWnd(nFrameIndex);
			m_ctlDeviceList.GetItemText(i, 1, szDeviceID, 32);
			if (IsDlgButtonChecked(IDC_CHECK_ENABLETRANLATE) == BST_CHECKED)
			{
				if (m_AvPlayer.PlaySrvStream(szDeviceID, (long)hVideoWnd, 0) == 0)
				{
					HWND hWndArray[16] = { 0 };
					long nArraySize = 16;
					int nErrorCode = m_AvPlayer.GetDeviceWindow(szDeviceID, (long *)hWndArray, &nArraySize);
					m_vecDevicePlaying.push_back(szDeviceID);
				}
			}
			else if (m_AvPlayer.PlayStream(szDeviceID,(long)hVideoWnd,0) ==0)
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
	if (!m_nTimerToSwitch)
	{
		SetDlgItemText(IDC_EDIT_SWITCHTIME, _T("15"));
		SetDlgItemText(IDC_EDIT_NEXTSWITCHTIME, _T("15"));
	}
		
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
	int nLastSize = 0;
	if (m_vecDevicePlaying.size() > 0)
	{
		nLastSize = m_vecDevicePlaying.size();
		for (vector<wstring>::iterator it = m_vecDevicePlaying.begin();
			it != m_vecDevicePlaying.end();)
		{
			m_AvPlayer.StopPlay(it->c_str(),NULL);
			it = m_vecDevicePlaying.erase(it);
		}
	}
	int nCount = m_ctlDeviceList.GetItemCount();
	
	if (m_nNextDevNo <= (nCount -1))
	{
		TCHAR szDeviceID[32] = {0};
		while (m_vecDevicePlaying.size() < nLastSize)
		{
			HWND hVideoWnd = m_pVideoFrame->GetPanelWnd(m_vecDevicePlaying.size());
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
// 	UINT nFrameID[] = {IDC_STATIC_FRAME,IDC_STATIC_FRAME2,IDC_STATIC_FRAME3,IDC_STATIC_FRAME4};
// 	for (int i = 0;i < 4;i ++)
// 		GetDlgItem(nFrameID[i])->ShowWindow(SW_SHOW);
	m_AvPlayer.ShowWindow(SW_SHOW);

}

void CSampleCPPDlg::OnBnClickedButtonHide()
{
	UINT nFrameID[] = {IDC_STATIC_FRAME,IDC_STATIC_FRAME2,IDC_STATIC_FRAME3,IDC_STATIC_FRAME4};
	for (int i = 0;i < 4;i ++)
		GetDlgItem(nFrameID[i])->ShowWindow(SW_HIDE);
	m_AvPlayer.ShowWindow(SW_HIDE);
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

bool CSampleCPPDlg::UpdateServer()
{
	CMarkup xml;
	TCHAR szAppPath[1024];
	GetAppPath(szAppPath, 1024);
	_tcscat_s(szAppPath, _T("\\Sample.xml"));
	if (xml.Load(szAppPath))
	{
		if (xml.FindElem(_T("Server")))
		{
			xml.IntoElem();
			if (xml.FindElem(_T("Host")))
			{
				CString strHost = xml.GetAttrib(_T("IP"));
				SetDlgItemText(IDC_IPADDRESS1, strHost);
			}
			xml.OutOfElem();
		}
		return true;
	}
	else
		return false;

}

void CSampleCPPDlg::OnDestroy()
{
	CDialog::OnDestroy();
	CMarkup xml;
	TCHAR szAppPath[1024];
	GetAppPath(szAppPath, 1024);
	_tcscat_s(szAppPath, _T("\\Sample.xml"));
	TCHAR *szDoc = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	xml.SetDoc(szDoc);


	xml.AddElem(_T("Server"));
	xml.IntoElem();	// Configuration	

	TCHAR szServer[64] = { 0 };
	GetDlgItemText(IDC_IPADDRESS1, szServer, 64);
	xml.AddElem(_T("Host"));
	xml.AddAttrib(_T("IP"), szServer);

	xml.OutOfElem();// Configuration
	xml.Save(szAppPath);

	
	if (m_pVideoFrame)
	{
		delete m_pVideoFrame;
	}
	delete m_pDialogPTZ;

	if (m_pRunlog)
		delete m_pRunlog;
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


void CSampleCPPDlg::OnBnClickedCheckDcdraw()
{
	//if (m_vecDevicePlaying.size() > 0)
	
		bool bEnable = IsDlgButtonChecked(IDC_CHECK_DCDRAW) == BST_CHECKED;
		TCHAR szDevice[32] = { 0 };
		for (int i = 0; i < m_ctlDeviceList.GetItemCount(); i++)
		{
			if (m_ctlDeviceList.GetCheck(i))
			{
				m_ctlDeviceList.GetItemText(i, 1, szDevice, 32);
				
				int nError = m_AvPlayer.EnableOperationAssist(szDevice, (long)bEnable);
			}
			
		}
	
}


void __stdcall CSampleCPPDlg::ExternDCDraw(HWND hWnd, HDC hDc, RECT rtDC, void *pUserPtr)
{
	DWORD dwTick1 = timeGetTime();
	CSampleCPPDlg *pThis = (CSampleCPPDlg *)pUserPtr;

	int nWidth = rtDC.right - rtDC.left;
	int nHeight = rtDC.bottom - rtDC.top;
	CDC dc;
	dc.Attach(hDc);
	CDC ImageMago, MemDC, ImageZpmc;

	MemDC.CreateCompatibleDC(&dc);
	MemDC.SetBkColor(RGB(0, 0, 0));
	MemDC.FillSolidRect(0, 0, nWidth, nHeight, RGB(255, 255, 255));
	MemDC.SetTextColor(RGB(255, 0, 0));

	CBitmap *pOldMapMemory;
	CBitmap mapMemory;
	mapMemory.CreateCompatibleBitmap(&dc, nWidth, nHeight);
	pOldMapMemory = MemDC.SelectObject(&mapMemory);//加载兼容位图，只有制定了“桌布”尺寸之后，你才能在内存DC上面绘图

	int nCenterX = (rtDC.right + rtDC.left) / 2;
	int nCenterY = (rtDC.top + rtDC.bottom) / 2;

	CPen newPen(0, 2, RGB(255, 0, 0));
	CPen *oldPen = dc.SelectObject(&newPen);
	
	// 可以使用任何DC的绘图函数进行绘图
	RECT rtCorner;			// 决定边角大小的矩形
	rtCorner.left = 0;
	rtCorner.top = 0;
	rtCorner.right = rtCorner.left + 200;
	rtCorner.bottom = rtCorner.top + 150;

	int nWidthCorner = rtCorner.right - rtCorner.left;
	int nHeightCorner = rtCorner.bottom - rtCorner.top;
	
	int nThickCornerV = 10;		// 垂直方向厚度
	int nThickCornerH = 10;		// 水平方向厚度

	int nDistanceV = 300;
	int nDistanceH = 400;
	
	POINT apt[6] = { 0 };
	if (hWnd == pThis->m_pVideoFrame->GetPanelWnd(0))
	{
		int nStartX = rtDC.left + nDistanceH;
		int nStartY = rtDC.top + nDistanceV;
		
		apt[0].x = nStartX;
		apt[0].y = nStartY;
		
		apt[1].x = nStartX + nWidthCorner;
		apt[1].y = nStartY;

		apt[2].x = apt[1].x - nThickCornerH;
		apt[2].y = apt[1].y + nThickCornerH;

		apt[3].x = nStartX + nThickCornerH;
		apt[3].y = nStartY + nThickCornerV;

		apt[4].x = nStartX + nThickCornerV;
		apt[4].y = nStartY + nHeightCorner - nThickCornerV; 

		apt[5].x = nStartX ;
		apt[5].y = nStartY + nHeightCorner;
	}
	else if (hWnd == pThis->m_pVideoFrame->GetPanelWnd(1))
	{
		int nStartX = rtDC.right - nDistanceH - nWidthCorner;
		int nStartY = rtDC.top + nDistanceV;

		apt[0].x = nStartX;
		apt[0].y = nStartY;

		apt[1].x = nStartX + nWidthCorner;
		apt[1].y = nStartY;

		apt[2].x = nStartX + nWidthCorner;
		apt[2].y = nStartY + nHeightCorner;

		apt[3].x = nStartX + nWidthCorner - nThickCornerH;
		apt[3].y = nStartY + nHeightCorner - nThickCornerH;

		apt[4].x = nStartX + nWidthCorner - nThickCornerH;
		apt[4].y = nStartY + nThickCornerV;

		apt[5].x = nStartX + nThickCornerH;
		apt[5].y = nStartY + nThickCornerV;

	}
	else if (hWnd == pThis->m_pVideoFrame->GetPanelWnd(2))
	{
		int nStartX = rtDC.left + nDistanceH;
		int nStartY = rtDC.bottom - nDistanceV - nHeightCorner;

		apt[0].x = nStartX;
		apt[0].y = nStartY;

		apt[1].x = nStartX + nThickCornerH;
		apt[1].y = nStartY + nThickCornerH;

		apt[2].x = nStartX + nThickCornerH;
		apt[2].y = nStartY + nHeightCorner - nThickCornerV;

		apt[3].x = nStartX + nWidthCorner - nThickCornerH;
		apt[3].y = nStartY + nHeightCorner - nThickCornerV;

		apt[4].x = nStartX + nWidthCorner;
		apt[4].y = nStartY + nHeightCorner;

		apt[5].x = nStartX ;
		apt[5].y = nStartY + nHeightCorner;
	}
	else if (hWnd == pThis->m_pVideoFrame->GetPanelWnd(3))
	{
		int nStartX = rtDC.right - nDistanceH - nWidthCorner;
		int nStartY = rtDC.bottom - nDistanceV - nHeightCorner;

		apt[0].x = nStartX + nWidthCorner;
		apt[0].y = nStartY;

		apt[1].x = nStartX + nWidthCorner;
		apt[1].y = nStartY + nHeightCorner;

		apt[2].x = nStartX ;
		apt[2].y = nStartY + nHeightCorner;

		apt[3].x = nStartX + nThickCornerH;
		apt[3].y = nStartY + nHeightCorner - nThickCornerV;

		apt[4].x = nStartX + nWidthCorner - nThickCornerH;
		apt[4].y = nStartY + nHeightCorner - nThickCornerV;

		apt[5].x = nStartX + nWidthCorner - nThickCornerH;
		apt[5].y = nStartY + nThickCornerV;
	}

	CBrush br(RGB(255, 0, 0));
	dc.SelectObject(&br);
	dc.SetPolyFillMode(ALTERNATE);
	dc.Polygon(apt, 6);

// // 以下为网格线示例
// 	int w = 0, h = 0, left_w = 0, left_h = 0;
// 	int nCols = 16, nRows = 16;
// 	w = (rtDC.right - rtDC.left) / nCols;
// 	h = (rtDC.bottom - rtDC.top) / nRows;
// 
// 	left_w = (rtDC.right - rtDC.left) % nCols;
// 	left_h = (rtDC.bottom - rtDC.top) % nRows;
// 
// 	int c = 0, r = 0;
// 	CPen newPen(0, 2, RGB(0, 255, 0));
// 	CPen *oldPen = dc.SelectObject(&newPen);
// 	int nAddX = 1;
// 	for (c = 1; c < nRows; c++)
// 	{
// 		dc.MoveTo(0, h * c);
// 		dc.LineTo(rtDC.right, h * c);
// 	}
// 
// 	for (c = 1; c < nCols; c++)
// 	{
// 		dc.MoveTo(w * c, 0);
// 		dc.LineTo(w * c, rtDC.bottom);
// 	}
// 	dc.SelectObject(oldPen);

	// 以下为绘制文本示例
// 	CFont *pOldFont =(CFont*) dc.SelectObject(&pThis->m_Font);
// 	TCHAR *szText = _T("Mago AvPlayer ActiveX 1.0 Demo");
// 	int nCount = _tcslen(szText);
// 
// 	CSize SizeT = dc.GetTextExtent(szText, nCount);
// 	CSize OutputText;
// 	OutputText.cx = nCenterX - SizeT.cx/2;
// 	OutputText.cy = nCenterY - SizeT.cy/2;
// 	dc.SetTextColor(RGB(255, 0, 0));
// 	//dc.SetBkColor();
// 
// 	dc.TextOut(OutputText.cx, OutputText.cy, szText, nCount);
// 	dc.SelectObject(pOldFont);

	// 以下为贴图示例
// 	bool bStatus = false;
// 	bStatus = ImageMago.CreateCompatibleDC(&dc);
// 	ImageMago.SelectObject(&pThis->m_bitmapMago);
// 
// 	int nInterval = 16;
// 	BITMAP bitmapMago;
// 	pThis->m_bitmapMago.GetBitmap(&bitmapMago);
// 	
// 	CSize Mago;
// 	Mago.cx = nCenterX - bitmapMago.bmWidth/2;
// 	Mago.cy = OutputText.cy - nInterval - bitmapMago.bmHeight;
// 	bStatus = dc.StretchBlt(Mago.cx, Mago.cy, bitmapMago.bmWidth, bitmapMago.bmHeight, &ImageMago, 0, 0, bitmapMago.bmWidth, bitmapMago.bmHeight, SRCPAINT);

// 	bStatus = ImageZpmc.CreateCompatibleDC(&dc);
// 	ImageZpmc.SelectObject(&pThis->m_bitmapZpmc);
// 	BITMAP bitmapZpmc;
// 	pThis->m_bitmapZpmc.GetBitmap(&bitmapZpmc);
// 	CSize Zpmc;
// 	Zpmc.cx = nCenterX - bitmapZpmc.bmWidth;
// 	Zpmc.cy = OutputText.cy + nInterval ;
// 	bStatus = dc.StretchBlt(Zpmc.cx, Zpmc.cy, bitmapZpmc.bmWidth, bitmapZpmc.bmHeight, &ImageZpmc, 0, 0, bitmapZpmc.bmWidth, bitmapZpmc.bmHeight, SRCPAINT);
	
	MemDC.SelectObject(pOldMapMemory);
	ImageMago.DeleteDC();
	ImageZpmc.DeleteDC();
	MemDC.DeleteDC();
	dc.Detach();
	DWORD dwTick2 = timeGetTime();
	TraceMsgA("%s TimeSpan = %dms.\n", __FUNCTION__, dwTick2 - dwTick1);
}

void CSampleCPPDlg::OnBnClickedButtonUpdateassist()
{
	if (IsDlgButtonChecked(IDC_CHECK_DCDRAW))
	{
		if (m_vecDevicePlaying.size() > 0)
		{
			for (auto it = m_vecDevicePlaying.begin(); it != m_vecDevicePlaying.end(); it++)
				m_AvPlayer.EnableOperationAssist((LPCTSTR)it->c_str(), (long)0);
		}
	}
	m_AvPlayer.LoadOpAssistConfigure();
	if (IsDlgButtonChecked(IDC_CHECK_DCDRAW))
	{
		if (m_vecDevicePlaying.size() > 0)
		{
			for (auto it = m_vecDevicePlaying.begin(); it != m_vecDevicePlaying.end(); it++)
				m_AvPlayer.EnableOperationAssist((LPCTSTR)it->c_str(), (long)1);
		}
	}
}


LRESULT CSampleCPPDlg::OnHotkey(WPARAM wParam, LPARAM lParam)
{
	UINT nID = (UINT)wParam;
	if (nID == ID_F12)
	{
		if (!m_bHotkeyActived)
		{
			::RegisterHotKey(m_hWnd, ID_F1, MOD_ALT, VK_F1);
			::RegisterHotKey(m_hWnd, ID_F2, MOD_ALT, VK_F2);
			::RegisterHotKey(m_hWnd, ID_F3, MOD_ALT, VK_F3);
			::RegisterHotKey(m_hWnd, ID_F4, MOD_ALT, VK_F4);

			::RegisterHotKey(m_hWnd, ID_UP, MOD_ALT, VK_UP);
			::RegisterHotKey(m_hWnd, ID_DOWN, MOD_ALT, VK_DOWN);
			::RegisterHotKey(m_hWnd, ID_LEFT, MOD_ALT, VK_LEFT);
			::RegisterHotKey(m_hWnd, ID_RIGHT, MOD_ALT, VK_RIGHT);
			((CStatic*)GetDlgItem(IDC_STATIC_CONFIG))->SetBitmap((HBITMAP)m_bmpActived);
		}
		else
		{
			::UnregisterHotKey(m_hWnd, ID_F1);
			::UnregisterHotKey(m_hWnd, ID_F2);
			::UnregisterHotKey(m_hWnd, ID_F3);
			::UnregisterHotKey(m_hWnd, ID_F4);
			::UnregisterHotKey(m_hWnd, ID_UP);
			::UnregisterHotKey(m_hWnd, ID_DOWN);
			::UnregisterHotKey(m_hWnd, ID_LEFT);
			::UnregisterHotKey(m_hWnd, ID_RIGHT);
			((CStatic*)GetDlgItem(IDC_STATIC_CONFIG))->SetBitmap((HBITMAP)m_bmpUnActived);
		}
	}
	else
	{
		switch (nID)
		{
		case ID_F1:
		{

		}
			break;
		case ID_F2:
		{

		}
			break;
		case ID_F3:
		{

		}
			break;
		case ID_F4:
		{

		}
			break;
		case ID_UP:
		{

		}
			break;
		case ID_DOWN:
		{

		}
			break;
		case ID_LEFT:
		{

		}
			break;
		case  ID_RIGHT:
		{

		}
			break;
		default:
			break;
		}
	}

	return 0L;
}


void CSampleCPPDlg::OnCbnSelendokComboSwitch()
{
	if (!m_bFitFrame)
	{
		GetDlgItem(IDC_STATIC_FRAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FRAME2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FRAME3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FRAME4)->ShowWindow(SW_HIDE);
		CRect rtFrame;
		GetDlgItemRect(IDC_STATIC_VIDEOFRAME, rtFrame);
// 		m_AvPlayer.MoveWindow(&rtFrame, TRUE);
// 		m_AvPlayer.EnableWindow(TRUE);
// 		m_AvPlayer.ShowWindow(SW_SHOW);
		m_bFitFrame = true;
	}

	LONG nMode = (LONG)GetDlgItemInt(IDC_COMBO_SWITCH);
	m_AvPlayer.SwitchScreen(1, nMode, (LONG)GetDlgItem(IDC_STATIC_VIDEOFRAME)->GetSafeHwnd());
}


void CSampleCPPDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (GetDlgItem(IDC_BUTTON_PLAY)->GetSafeHwnd())
	{
		m_WndSizeManager.OnSize(nType, cx, cy);
		RECT rt;
		m_WndSizeManager.GetWndCurrentPostion(IDC_STATIC_FRAME, rt);
		m_pVideoFrame->MoveWindow(&rt, true);
	}
}

#include <windowsx.h>
void CSampleCPPDlg::OnCbnSelendokComboDiv()
{
	int nCurSel = ComboBox_GetCurSel(::GetDlgItem(m_hWnd, IDC_COMBO1));
	// 1分屏;2分屏;3分屏;4分屏;6分屏;9分屏;12分屏;16分屏;20分屏;25分屏;30分屏;5包1;7包1;9包1;
	int nWndCount = 1;
	
	FrameStyle fs = StyleNormal;
	switch (nCurSel)
	{
	case 0:	// 1分屏
	default:
	{
		nWndCount = 1;
	}
	break;
	case 1:	// 2分屏
		nWndCount = 2;
		break;
	case 2:	// 3分屏
		nWndCount = 3;
		fs = Style_2P1;
		break;
	case 3:	// 4分屏
		nWndCount = 4;
		break;
	case 4:	// 6分屏
		nWndCount = 6;
		break;
	case 5:	// 9分屏
		nWndCount = 9;
		break;
	case 6:	// 12分屏
		nWndCount = 12;
		break;
	case 7:	// 16分屏
		nWndCount = 16;
		break;
	case 8:	// 20分屏
		nWndCount = 20;
		break;
	case 9:	// 25分屏
		nWndCount = 25;
		break;
	case 10:// 30分屏
		nWndCount = 30;
		break;
	case 11:// 5包1;
		fs = Style_5P1;
		break;
	case 12:// 7包1;
		fs = Style_7P1;
		break;
	case 13:// 9包1;
		fs = Style_9P1;
		break;
	case 14:
		fs = Style_11P1;
		break;
	}

	if (fs == StyleNormal)
	{
		if (nWndCount != m_pVideoFrame->GetPanelCount())
			m_pVideoFrame->AdjustPanels(nWndCount);
	}
	else
	{
		m_pVideoFrame->AdjustPanels(nWndCount, fs);
	}

}


void CSampleCPPDlg::OnBnClickedButtonQueryrecord()
{
	int nCount = m_ctlDeviceList.GetItemCount();
	TCHAR szDeviceID[32] = { 0 };
	for (int i = 0; i < nCount; i++)
	{
		if (m_ctlDeviceList.GetCheck(i) )
		{
			m_ctlDeviceList.GetItemText(i, 1, szDeviceID, 32);
			break;
		}
	}
	COleDateTime dateTime, StartTime, StopTime;
	m_Date.GetTime(dateTime);
	m_StartTime.GetTime(StartTime);
	m_StopTime.GetTime(StopTime);
	CTime timeStart(dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetDay(), StartTime.GetHour(), StartTime.GetMinute(), StartTime.GetSecond());
	CTime timeStop(dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetDay(), StopTime.GetHour(), StopTime.GetMinute(), StopTime.GetSecond());
	Record_Info RecordArray[24 * 4] = { 0 };	// 假设目前服务器设置每小有四段录像，一天有96段
	LONG nRecordCount = 0;

	m_listRecord.DeleteAllItems();
	if (m_AvPlayer.QueryRecord(szDeviceID, timeStart.GetTime(), timeStop.GetTime(), (long)RecordArray, 96, &nRecordCount) == 0)
	{
		for (int i = 0; i < nRecordCount; i++)
		{
			Record_Info *pRec = new Record_Info;
			memcpy(pRec, &RecordArray[i], sizeof(Record_Info));
			CTime becRec = pRec->startTime;
			CTime endRec = pRec->endTime;
			CString strTimeBeg;
			strTimeBeg.Format(_T("%02d:%02d:%02d"), becRec.GetHour(), becRec.GetMinute(), becRec.GetSecond());
			CString strTimeEnd;
			strTimeEnd.Format(_T("%02d:%02d:%02d"), endRec.GetHour(), endRec.GetMinute(), endRec.GetSecond());
			CString strType;
			strType.Format(_T("%d"), pRec->recordType);
			CString strNum;
			strNum.Format(_T("%d"), i);
			m_listRecord.InsertItem(i, strNum);
			//	m_listRecord.InsertItem(i,strTimeBeg);
			m_listRecord.SetItemText(i, 1, strTimeBeg);
			m_listRecord.SetItemText(i, 2, strTimeEnd);
			m_listRecord.SetItemData(i, (DWORD)pRec);
		}
	}
}


void CSampleCPPDlg::OnBnClickedButtonPlayrecord()
{
	int nCount = m_ctlDeviceList.GetItemCount();
	HWND hVideoWnd = nullptr;
	TCHAR szDeviceID[32] = { 0 };
	for (int i = 0; i < nCount; i++)
	{
		if (m_ctlDeviceList.GetCheck(i) && m_vecDevicePlaying.size() < 4)
		{
			hVideoWnd = m_pVideoFrame->GetPanelWnd(m_pVideoFrame->GetCurPanel());
			m_ctlDeviceList.GetItemText(i, 1, szDeviceID, 32);
			break;

		}
	}
	CString strButtonText;
	if (!m_bPlayRecord)
	{
		
		if (!hVideoWnd)
		{
			AfxMessageBox(_T("Please Select a Window to Play."));
			return;
		}
		int nRecordCount = m_listRecord.GetItemCount();
		int nItemPlay = -1;
		for (int i = 0; i < nRecordCount; i++)
		{
			if (m_listRecord.GetCheck(i))
			{
				nItemPlay = i;
				break;
			}
		}
		if (nItemPlay < 0)
		{
			AfxMessageBox(_T("Please Select a Record to Play."));
			return;
		}
		Record_Info *pRec = (Record_Info *)m_listRecord.GetItemData(nItemPlay);
		
		LONG nSeekFrame = 0;
		if (IsDlgButtonChecked(IDC_CHECK_SEEKFRAME) == BST_CHECKED)
			nSeekFrame = 1;
		//pRec->startTime = pRec->endTime - 30;
		if (m_AvPlayer.PlayBack((long)hVideoWnd, szDeviceID, pRec->startTime , pRec->endTime, nSeekFrame, 5000) == 0)
		{

			HWND hWndArray[16] = { 0 };
			long nArraySize = 16;
			int nErrorCode = m_AvPlayer.GetDeviceWindow(szDeviceID, (long *)hWndArray, &nArraySize);
			m_vecDevicePlaying.push_back(szDeviceID);
			m_strCurPlayBackDevice = szDeviceID;
		}
		// 通过定时器来调整单帧播放的位置
		if (nSeekFrame)
		{
			m_tSeekOffset = pRec->startTime; // 使用起始时间作为第一个播放点，每个100毫秒调整一次，每调整一次，时间增加200毫秒
			m_tSeekOffset *= 1000;			 // 时间放大1000倍
			//m_nTimeEvent = timeSetEvent(100, 1, (LPTIMECALLBACK)MMTIMECALLBACK, (DWORD_PTR)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION);
			m_bThreadSeekRun = true;
			m_hThreadSeek = (HANDLE)_beginthreadex(nullptr, 0, ThreadSeek, this, 0, 0);
		}

		m_bPlayRecord = true;
		strButtonText = _T("&Stop Play");
	}
	else
	{
		strButtonText = _T("&Play Record"); 
		m_AvPlayer.StopPlayBack(szDeviceID);
		m_bPlayRecord = false;
	}
	SetDlgItemText(IDC_BUTTON_PLAYRECORD, strButtonText);


}


void CSampleCPPDlg::OnClose()
{
	m_bThreadSeekRun = false;
	
	WaitForSingleObject(m_hThreadSeek, INFINITE);

	CDialog::OnClose();
}

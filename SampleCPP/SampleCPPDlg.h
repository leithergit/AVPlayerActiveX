
// SampleCPPDlg.h : header file
//

#pragma once
#include "avplayerctrl.h"
#include "DialogPTZ.h"
#include <vector>
#include "draglistctrl.h"
#include "VideoFrame.h"
#include "WndSizeManager.h"
#include "afxdtctl.h"
#include "TimeUtility.h"
#include "Runlog.h"
using namespace std;

#define ID_BASE		WM_USER + 100
#define ID_OEM_3	ID_BASE
#define ID_F1		WM_USER + 101
#define ID_F2		WM_USER + 102
#define ID_F3		WM_USER + 103
#define ID_F4		WM_USER + 104
#define ID_F5		WM_USER + 105
#define ID_F6		WM_USER + 106
#define ID_F7		WM_USER + 107
#define ID_F8		WM_USER + 108
#define ID_F9		WM_USER + 109
#define ID_F10		WM_USER + 110
#define ID_F11		WM_USER + 111
#define ID_F12		WM_USER + 112
#define ID_UP		WM_USER + 113
#define ID_DOWN		WM_USER + 114
#define ID_LEFT		WM_USER + 115
#define ID_RIGHT	WM_USER + 116



// 录像信息结构体
struct Record_Info
{
	unsigned char	source;							//来源类型：1未知，2设备，3中心，7核心节点，8区域节点，9接入节点
	unsigned char	recordType;						//录像类型。1全部，2报警录像，3移动侦测，4手动录像，5卡号录像，6定时录像,VSEnumRecordType
	bool			bDownloadByTime;				//是否按时间下载
	UINT			startTime;						//起始时间
	UINT			endTime;						//结束时间
	char			name[64];		//录像名字（不同厂家对文件的标识不同）
	UINT			length;							//文件长度
	//下面是中心录像所需要的信息
	UINT			planId;							//录像计划ID
	UINT			ssId;							//存储服务ID
	char			diskId[32];			//磁盘ID
	UINT			fileHandle;						//文件句柄

	char			devId[32];			//支持报警录像回放，因为报警源与录像设备是分开的
	int				channelNo;
	char			szCamerID[32];
	bool			bIpc;
};


// CSampleCPPDlg dialog
class CSampleCPPDlg : public CDialog
{
// Construction
public:
	CVideoFrame *m_pVideoFrame;
	CWndSizeManger m_WndSizeManager;
	CSampleCPPDlg(CWnd* pParent = NULL);	// standard constructor
	CRunlog*	m_pRunlog;

// Dialog Data
	enum { IDD = IDD_SAMPLECPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	bool UpdateServer();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CAvplayerctrl m_AvPlayer;
	MMRESULT m_nTimeEvent = 0;
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonLogout();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonStop();
	CDragListCtrl m_ctlDeviceList;
	CListCtrl  m_listRecord;
	vector<wstring>m_vecDevicePlaying;
	vector<int> m_vecIndex;
	afx_msg void OnBnClickedButtonStartswitch();
	afx_msg void OnBnClickedButtonStopswitch();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	LRESULT OnSwitch(WPARAM wParam, LPARAM lParam);
	int m_nNextDevNo;
	// 切换轮次
	int m_nSwitchCount;
	int m_nTimerToSwitch;
	afx_msg void OnFileOutput();
	afx_msg void OnBnClickedButtonSwitch();
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	void RecvTimeoutAvplayerctrl(LPCTSTR strDeviceID, long hWnd);
	afx_msg void OnBnClickedButtonStopextend();
	afx_msg void OnBnClickedButtonStopextend2();
	afx_msg void OnBnClickedButtonStopextend3();
	afx_msg void OnBnClickedButtonShow();
	afx_msg void OnBnClickedButtonHide();
	afx_msg void OnBnClickedButtonPlaycombo();
	bool m_bFrameused[4];
	afx_msg void OnBnClickedButtonPtzpannel();
	CDialogPTZ *m_pDialogPTZ;
	afx_msg void OnDestroy();
	bool	m_bIsLButtonDown;
	CPoint	m_ptMove;
	CPoint	m_ptOffset;
	CImageList	m_imgDrag;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnNMClickListDevice(NMHDR *pNMHDR, LRESULT *pResult);
	CAvplayerctrl m_NewAvPlay;
	afx_msg void OnBnClickedCheckDcdraw();
	static void __stdcall ExternDCDraw(HWND hWnd, HDC hDc, RECT rt, void *pUserPtr);
	CBitmap		m_bitmapMago;
	CBitmap		m_bitmapZpmc;
	CFont		m_Font;
	afx_msg void OnBnClickedButtonUpdateassist();
	bool		m_bHotkeyActived = false;
	afx_msg LRESULT OnHotkey(WPARAM wParam, LPARAM lParam);
	CBitmap		m_bmpActived;
	CBitmap		m_bmpUnActived;
	afx_msg void OnCbnSelendokComboSwitch();
	bool		m_bFitFrame = false;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelendokComboDiv();
	afx_msg void OnBnClickedButtonQueryrecord();
	afx_msg void OnBnClickedButtonPlayrecord();
	bool	m_bPlayRecord = false;
	CDateTimeCtrl m_Date;
	CDateTimeCtrl m_StartTime;
	CDateTimeCtrl m_StopTime;
	time_t	m_tSeekOffset = 0;			// 单帧播放时间点
	CString m_strCurPlayBackDevice;
	bool m_bThreadSeekRun = false;
	HANDLE m_hThreadSeek = nullptr;
	static UINT __stdcall  ThreadSeek(void *p)
	{
		CSampleCPPDlg* pThis = (CSampleCPPDlg *)p;
		double dfLastTime = GetExactTime() - 0.2f;
		while (pThis->m_bThreadSeekRun)
		{
			if (TimeSpanEx(dfLastTime)  >= 5.f)
			{
				if (pThis->m_tSeekOffset)
				{
					int nStatus = pThis->m_AvPlayer.SeekTime(pThis->m_strCurPlayBackDevice, pThis->m_tSeekOffset);
					if (nStatus)
						TraceMsgA("%s SeekTime(%d).\n", __FUNCTION__, nStatus);
					pThis->m_tSeekOffset += 60;
				}
				dfLastTime = GetExactTime();
			}
			Sleep(20);
		}
		return 0;	
	}

	static void  MMTIMECALLBACK(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
	{
		CSampleCPPDlg* pThis = (CSampleCPPDlg *)dwUser;
		if (pThis->m_tSeekOffset)
		{
			int nStatus = pThis->m_AvPlayer.SeekTime(pThis->m_strCurPlayBackDevice, pThis->m_tSeekOffset);
 			if (nStatus)
 				TraceMsgA("%s SeekTime(%d).\n", __FUNCTION__, nStatus);
			pThis->m_tSeekOffset += 200;
		}
	}
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonSeekrecord();
};

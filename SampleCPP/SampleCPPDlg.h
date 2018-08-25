
// SampleCPPDlg.h : header file
//

#pragma once
#include "avplayerctrl.h"
#include "DialogPTZ.h"
#include <vector>
#include "draglistctrl.h"
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

// CSampleCPPDlg dialog
class CSampleCPPDlg : public CDialog
{
// Construction
public:
	CSampleCPPDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SAMPLECPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CAvplayerctrl m_AvPlayer;
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonLogout();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonStop();
	CDragListCtrl m_ctlDeviceList;
	vector<wstring>m_vecDevicePlaying;
	afx_msg void OnBnClickedButtonStartswitch();
	afx_msg void OnBnClickedButtonStopswitch();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	LRESULT OnSwitch(WPARAM wParam, LPARAM lParam);
	int m_nNextDevNo;
	// ÇÐ»»ÂÖ´Î
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
	HWND		m_hLeftTopWnd, m_hRightTopWnd;
	HWND		m_hLeftBottomWnd, m_hRightBottomWnd;
	afx_msg void OnBnClickedButtonUpdateassist();
	bool		m_bHotkeyActived = false;
	afx_msg LRESULT OnHotkey(WPARAM wParam, LPARAM lParam);
	CBitmap		m_bmpActived;
	CBitmap		m_bmpUnActived;
	afx_msg void OnCbnSelendokComboSwitch();
	bool		m_bFitFrame = false;
};

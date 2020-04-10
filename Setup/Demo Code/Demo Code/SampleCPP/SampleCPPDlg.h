
// SampleCPPDlg.h : header file
//

#pragma once
#include "avplayerctrl.h"
#include "DialogPTZ.h"
#include <vector>
using namespace std;

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
	CListCtrl m_ctlDeviceList;
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
};

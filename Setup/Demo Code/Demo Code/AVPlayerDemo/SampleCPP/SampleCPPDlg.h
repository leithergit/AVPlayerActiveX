// SampleCPPDlg.h : header file
//
#pragma once
#include "avplayerctrl.h"
#include <vector>
using namespace std;
enum AVStatus
{
	AvError_Succeed = 0,
	AvError_base = -1024,				
	AvError_InvalidParameters			 = AvError_base - 1,
	AvError_Invalid_ServerAddress		 = AvError_base - 2,
	AvError_Invalid_AccountOrPassword	 = AvError_base - 3,
	AvError_Invliad_ServerPort			 = AvError_base - 4,
	AvError_ConnectServerFailed			 = AvError_base - 5,
	AvError_LoginFailed					 = AvError_base - 6,
	AvError_NotLogintoServer			 = AvError_base - 7,
	AvError_InvalidWindow				 = AvError_base - 8,
	AvError_DeviceNotExist				 = AvError_base - 9,
	AvError_ConnectDeviceFailed			 = AvError_base - 10,
	AvError_DeviceInPlaying				 = AvError_base - 11,
	AvError_DBException					 = AvError_base - 12,
	AvError_InsufficentMemory			 = AvError_base - 254,
	AvError_UnknownException			 = AvError_base - 255,
};

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
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonPlaycombo();
	TCHAR m_szDeviceID[2][32];
	CListCtrl m_ctlDeviceList;
	vector<wstring>m_vecDevicePlaying;
	float m_fHei;
	afx_msg void OnBnClickedButtonMove();
	afx_msg void OnBnClickedButtonMoveex();
	CString m_strDev1;
	CString m_strDev2;
	afx_msg void OnBnClickedButton1();
};

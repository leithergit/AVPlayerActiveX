
// UpdateOCXClassDlg.h : header file
//

#pragma once
#include "avplayerctrl1.h"


// CUpdateOCXClassDlg dialog
class CUpdateOCXClassDlg : public CDialogEx
{
// Construction
public:
	CUpdateOCXClassDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_UPDATEOCXCLASS_DIALOG };

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
	CAvplayerctrl AA;
};

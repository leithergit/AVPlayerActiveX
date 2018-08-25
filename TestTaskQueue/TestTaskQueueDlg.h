
// TestTaskQueueDlg.h : header file
//

#pragma once

#include "SyncTask.h"
#include "Utility.h"

// CTestTaskQueueDlg dialog
class CTestTaskQueueDlg : public CDialogEx
{
// Construction
public:
	CTestTaskQueueDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTTASKQUEUE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CSyncTaskQueue	m_TaskQueue;

	VOID	TestInt(int A)
	{
		TraceMsg(_T("%s A = %d.\n"), __FUNCTION__, A);
		//return 0;
	}

	int	TestString(CString A)
	{
		TraceMsg(_T("%s A = %s.\n"), __FUNCTION__, (LPCTSTR)A);
		return 0;
	}

	int	Testdouble(double A)
	{
		TraceMsg(_T("%s A = %.3f.\n"), __FUNCTION__, A);
		return 0;
	}

	int TestMult1(int A, CString B)
	{
		TraceMsg(_T("%s A = %d\tB = %s.\n"), __FUNCTION__, A, (LPCTSTR)B);
		return 0;
	}

	int TestMult2(int A, CString B, double C)
	{
		TraceMsg(_T("%s A = %d\tB = %s\t%.3f.\n"), __FUNCTION__, A, (LPCTSTR)B,C);
		return 0;
	}

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};

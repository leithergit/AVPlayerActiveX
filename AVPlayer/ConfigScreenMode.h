#pragma once


// CConfigScreenMode dialog

class CConfigScreenMode : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigScreenMode)

public:
	CConfigScreenMode(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigScreenMode();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONFIGUREMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

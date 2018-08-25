#pragma once
#include "VideoFrame.h"

// CModePreview dialog

class CModePreview : public CDialogEx
{
	DECLARE_DYNAMIC(CModePreview)

public:
	CVideoFrame *m_pVideoFrame = nullptr;
	CModePreview(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModePreview();

// Dialog Data
	enum { IDD = IDD_DIALOG_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

#pragma once

// AVPlayerPropPage.h : Declaration of the CAVPlayerPropPage property page class.


// CAVPlayerPropPage : See AVPlayerPropPage.cpp for implementation.

class CAVPlayerPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CAVPlayerPropPage)
	DECLARE_OLECREATE_EX(CAVPlayerPropPage)

// Constructor
public:
	CAVPlayerPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_AVPLAYER };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};


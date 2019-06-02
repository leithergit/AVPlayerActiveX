// AVPlayerPropPage.cpp : Implementation of the CAVPlayerPropPage property page class.

#include "stdafx.h"
#include "AVPlayer.h"
#include "AVPlayerPropPage.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CAVPlayerPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CAVPlayerPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CAVPlayerPropPage, "AVPLAYER.AVPlayerPropPage.1",
	0x750277d8, 0xa0c1, 0x4e4c, 0x99, 0xf1, 0x96, 0xf7, 0x5d, 0x8e, 0x6d, 0x95)



// CAVPlayerPropPage::CAVPlayerPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CAVPlayerPropPage

BOOL CAVPlayerPropPage::CAVPlayerPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_AVPLAYER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CAVPlayerPropPage::CAVPlayerPropPage - Constructor

CAVPlayerPropPage::CAVPlayerPropPage() :
	COlePropertyPage(IDD, IDS_AVPLAYER_PPG_CAPTION)
{
}



// CAVPlayerPropPage::DoDataExchange - Moves data between page and properties

void CAVPlayerPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CAVPlayerPropPage message handlers

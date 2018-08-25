// ModePreview.cpp : implementation file
//

#include "stdafx.h"
#include "AVPlayer.h"
#include "ModePreview.h"
#include "afxdialogex.h"


// CModePreview dialog

IMPLEMENT_DYNAMIC(CModePreview, CDialogEx)

CModePreview::CModePreview(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModePreview::IDD, pParent)
{

}

CModePreview::~CModePreview()
{
}

void CModePreview::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CModePreview, CDialogEx)
END_MESSAGE_MAP()


// CModePreview message handlers


BOOL CModePreview::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

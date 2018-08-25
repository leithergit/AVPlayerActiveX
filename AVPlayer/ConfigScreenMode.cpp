// ConfigScreenMode.cpp : implementation file
//

#include "stdafx.h"
#include "AVPlayer.h"
#include "ConfigScreenMode.h"
#include "afxdialogex.h"


// CConfigScreenMode dialog

IMPLEMENT_DYNAMIC(CConfigScreenMode, CDialogEx)

CConfigScreenMode::CConfigScreenMode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigScreenMode::IDD, pParent)
{

}

CConfigScreenMode::~CConfigScreenMode()
{
}

void CConfigScreenMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigScreenMode, CDialogEx)
END_MESSAGE_MAP()


// CConfigScreenMode message handlers

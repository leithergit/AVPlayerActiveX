#pragma once

#include "avplayerctrl.h"
#include "OleDropTargetEx.h"
// CDialogPTZ dialog

class CDialogPTZ : public CDialog
{
	DECLARE_DYNAMIC(CDialogPTZ)

public:
	CDialogPTZ(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogPTZ();

// Dialog Data
	enum { IDD = IDD_DIALOG_PTZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void CDialogPTZ::OnComandPTZ(UINT nID);
	CAvplayerctrl *m_pAVPlayer;
	 COleDropTarget m_oleTarget;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//COleDataSource	m_OleDataSource;

	// Initialize函数用于注册CDialogPTZ成为拖放接收窗口；
// 	virtual BOOL Initialize()
// 	{
// 		
// 	}

   virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
   {
		  return DROPEFFECT_MOVE;
   }
	/*
	OnDragOver在拖放鼠标进入窗口时被调用。此函数的返回值决定了后续的动作的类型:
	如果返回DROPEFFECT_MOVE，则产生一个剪切动作；如果返回DROPEFFECT_COPY，则产生
	一个复制动作，如果返回DROPEFFECT_NONE，则不会产生拖放动作，因为OnDropEx、OnDrop
	函数将不会被调用（OnDragLeave函数仍会被调用）。
	*/
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
	{
		 return DROPEFFECT_MOVE;
	}

	/*
	OnDropEx函数会在OnDrop函数之前调用，如果OnDropEx函数没有对拖放动作进行处理，
	则应用程序框架会接着调用OnDrop函数进行处理。所以必须要在派生类中重载OnDropEx
	函数——即使什么动作都都没有做——否则我们的OnDrop函数将不会被执行到，因为没
	有重载的话，将会调用基类的OnDropEx函数，而基类的OnDropEx函数对拖放是进行了处
	理的——尽管不是我们所想要的动作。当然你也可以把对拖放进行处理的动作放在OnDropEx
	中——那样就不需要重载OnDrop了
	*/
// 	virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, DROPEFFECT dropList, CPoint point)
// 	{
// 
// 	}

	/*
	OnDragLeave函数会在鼠标离开窗口时被调用，在此可以进行一些简单的清理工作。譬如
	在OnDragEnter或者OnDragOver函数中，我们改变了光标的形态，那么此时我们就应该把
	光标恢复过来
	*/
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
	{
		UINT              nFileCount = 0;
		HDROP           hDropData = NULL;
		HGLOBAL        hMemData = NULL;
		TCHAR			szData[128] = {0};

		if(pDataObject->IsDataAvailable(CF_TEXT))
		{
			hMemData = pDataObject->GetGlobalData(CF_TEXT);
			hDropData = (HDROP)GlobalLock((HGLOBAL)hMemData); //锁定内存块
			if(hDropData != NULL)
			{
				memcpy(szData,hDropData,64);
				SetDlgItemText(IDC_EDIT_CAMERAID,szData);
			}
			GlobalUnlock(hMemData);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	virtual void OnDragLeave(CWnd* pWnd)
	{
		
	}
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCommand();
};

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

	// Initialize��������ע��CDialogPTZ��Ϊ�ϷŽ��մ��ڣ�
// 	virtual BOOL Initialize()
// 	{
// 		
// 	}

   virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
   {
		  return DROPEFFECT_MOVE;
   }
	/*
	OnDragOver���Ϸ������봰��ʱ�����á��˺����ķ���ֵ�����˺����Ķ���������:
	�������DROPEFFECT_MOVE�������һ�����ж������������DROPEFFECT_COPY�������
	һ�����ƶ������������DROPEFFECT_NONE���򲻻�����ϷŶ�������ΪOnDropEx��OnDrop
	���������ᱻ���ã�OnDragLeave�����Իᱻ���ã���
	*/
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
	{
		 return DROPEFFECT_MOVE;
	}

	/*
	OnDropEx��������OnDrop����֮ǰ���ã����OnDropEx����û�ж��ϷŶ������д���
	��Ӧ�ó����ܻ���ŵ���OnDrop�������д������Ա���Ҫ��������������OnDropEx
	����������ʹʲô��������û���������������ǵ�OnDrop���������ᱻִ�е�����Ϊû
	�����صĻ���������û����OnDropEx�������������OnDropEx�������Ϸ��ǽ����˴�
	��ġ������ܲ�����������Ҫ�Ķ�������Ȼ��Ҳ���԰Ѷ��ϷŽ��д���Ķ�������OnDropEx
	�С��������Ͳ���Ҫ����OnDrop��
	*/
// 	virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, DROPEFFECT dropList, CPoint point)
// 	{
// 
// 	}

	/*
	OnDragLeave������������뿪����ʱ�����ã��ڴ˿��Խ���һЩ�򵥵���������Ʃ��
	��OnDragEnter����OnDragOver�����У����Ǹı��˹�����̬����ô��ʱ���Ǿ�Ӧ�ð�
	���ָ�����
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
			hDropData = (HDROP)GlobalLock((HGLOBAL)hMemData); //�����ڴ��
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

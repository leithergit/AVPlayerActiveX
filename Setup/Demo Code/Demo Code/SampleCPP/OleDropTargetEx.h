
#pragma once


// COleDropTargetEx command target

class COleDropTargetEx : public COleDropTarget
{
	DECLARE_DYNAMIC(COleDropTargetEx)

public:
	COleDropTargetEx();
	virtual ~COleDropTargetEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
// 	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
// 	{
// 
// 	}
// 
// 	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
// 	{
// 
// 	}
// 
// 	virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, DROPEFFECT dropList, CPoint point)
// 	{
// 
// 	}

// 	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
// 	{
// 		CDialogPTZ*     pDialog = NULL;
// 		ASSERT_VALID(this);
// 		ASSERT(IsWindow(pWnd->m_hWnd));
// 		if(pWnd->IsKindOf(RUNTIME_CLASS(CListCtrlEx)))
// 		{
// 			pDialog = (CDialogPTZ*)pWnd;
// 			return pDialog->OnDrop(pWnd, pDataObject, dropEffect, point);
// 		}
// 		else
// 		{
// 			return COleDropTarget::OnDrop(pWnd, pDataObject, dropEffect, point);    
// 		}
/*	}*/

	virtual void OnDragLeave(CWnd* pWnd)
	{

	}
};



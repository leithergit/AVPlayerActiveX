// DragListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SampleCPP.h"
#include "DragListCtrl.h"


// CDragListCtrl

IMPLEMENT_DYNAMIC(CDragListCtrl, CListCtrl)

CDragListCtrl::CDragListCtrl()
{
	m_nSelItem = -1;
	m_pDragImageList = NULL;
	m_bDragging = FALSE;
}

CDragListCtrl::~CDragListCtrl()
{
}


BEGIN_MESSAGE_MAP(CDragListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CDragListCtrl::OnLvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CDragListCtrl message handlers



void CDragListCtrl::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

// 	int count = GetSelectedCount();  
// 	if (1 != count)  
// 		return;  
// 
// 	POSITION pos = GetFirstSelectedItemPosition();  
// 	if (NULL == pos)  
// 		return;  
// 
// 	m_nSelItem = GetNextSelectedItem(pos);  
// 
// 	if (-1 == m_nSelItem)  
// 		return;  
// 
// 	CPoint pt = pNMLV->ptAction;  
// 	m_pDragImageList = CreateDragImage(m_nSelItem, &pt);  
// 	if (NULL == m_pDragImageList)  
// 		return;  
// 
// 	m_bDragging = TRUE;  
// 	m_pDragImageList->BeginDrag(0, CPoint(8, 8));  
// 	ClientToScreen(&pt);  
// 	CRect rt;  
// 	GetParent()->GetWindowRect(&rt);  
// 	pt.x -= rt.left;  
// 	pt.y -= rt.top;  
// 	m_pDragImageList->DragEnter(GetParent(), pt);  
// 	SetCapture();  
	char* hgMem= NULL;
	char *htmpMem= NULL;
	CString dataBuf_dataName, dataBuf_dataID;
	int typeIndex, ctrlType, bufSize;
	if (GetSelectedCount() ==0 || GetSelectedCount() > 1)
		return ;
	POSITION pos = GetFirstSelectedItemPosition();
	int nItem = GetNextSelectedItem(pos);
	dataBuf_dataName = GetItemText(nItem,1);
	bufSize = 4 + dataBuf_dataName.GetLength()*2 + 2;
	m_pOleDataSource = new COleDataSource;
	hgMem = (char*) GlobalAlloc(GPTR,bufSize);
	htmpMem = (char *)GlobalLock((HGLOBAL)hgMem);
	ZeroMemory(htmpMem,bufSize);
	memcpy(htmpMem + 4,dataBuf_dataName.GetBuffer(),dataBuf_dataName.GetLength()*2);
	m_pOleDataSource->Empty();
	m_pOleDataSource->CacheGlobalData(CF_TEXT,(HGLOBAL)htmpMem);
	DROPEFFECT dropEffect = m_pOleDataSource->DoDragDrop();
	GlobalUnlock((HGLOBAL)hgMem);
	GlobalFree((HGLOBAL)hgMem);
	delete m_pOleDataSource;
	m_pOleDataSource = NULL;
}

// void CDragListCtrl::OnMouseMove(UINT nFlags, CPoint point)
// {
// // 	if (m_bDragging)  
// // 	{  
// // 		CPoint pt = point;  
// // 		ClientToScreen(&pt);  
// // 
// // 		CRect rt;  
// // 		GetParent()->GetWindowRect(&rt);  
// // 		pt.x -= rt.left;  
// // 		pt.y -= rt.top;  
// // 		m_pDragImageList->DragMove(pt);  
// // 	}  
// 
// 	CListCtrl::OnMouseMove(nFlags, point);
// }
// 
// void CDragListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
// {
// // 	if (m_bDragging)  
// // 	{  
// // 		ReleaseCapture();  
// // 		m_bDragging = FALSE;  
// // 		//m_nSelItem = -1;  
// // 		m_pDragImageList->DragLeave(GetParent());  
// // 		m_pDragImageList->EndDrag();  
// // 		m_pDragImageList->DeleteImageList();  
// // 
// // 		delete m_pDragImageList;  
// // 		m_pDragImageList = NULL;  
// // 	}  
// 
// 	CListCtrl::OnLButtonUp(nFlags, point);
// }

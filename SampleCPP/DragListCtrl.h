#pragma once


// CDragListCtrl

class CDragListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CDragListCtrl)

public:
	CDragListCtrl();
	virtual ~CDragListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
// 	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
// 	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	int m_nSelItem;
	CImageList *m_pDragImageList;
	BOOL m_bDragging;
	COleDataSource *m_pOleDataSource;
};



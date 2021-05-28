// MapToolView.h : CMapToolView ��Ľӿ�
//


#pragma once

#include <MyMapView.h>
#include <vector>
using namespace std;

class CMapToolView : public CView
{
protected: // �������л�����
	CMapToolView();
	DECLARE_DYNCREATE(CMapToolView)

// ����
public:
	CMapToolDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMapToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CMyMapView      m_Doc;
	mapCOORDMAP     m_mapCoordMap; 

	bool            m_bNewData;

	short           m_nLayer;


// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenQm();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnZoomIn();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMapPan();
	afx_msg void OnMeasureDis();
	afx_msg void OnMapCenter();
	afx_msg void OnZoomOut();
	afx_msg void OnShowAllMap();
	afx_msg void OnOpenMif();
	afx_msg void OnLayerMan();
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnFileSave();
	afx_msg void OnGridCreate();
	afx_msg void OnSaveMapFile();
	afx_msg void OnSaveMapFileAs();
	afx_msg void OnSaveGrid();
	afx_msg void OnReadgridindex();
	afx_msg void OnSavegridindexindex();
	afx_msg void OnBuildareaindex();
	afx_msg void OnSaveareaindex();
};

#ifndef _DEBUG  // MapToolView.cpp �еĵ��԰汾
inline CMapToolDoc* CMapToolView::GetDocument() const
   { return reinterpret_cast<CMapToolDoc*>(m_pDocument); }
#endif


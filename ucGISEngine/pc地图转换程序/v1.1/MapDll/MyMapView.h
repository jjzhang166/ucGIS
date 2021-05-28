// MyMapView.h: interface for the CMyMapView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMAPVIEW_H__EC326E72_2151_4554_91C8_09931A2C536E__INCLUDED_)
#define AFX_MYMAPVIEW_H__EC326E72_2151_4554_91C8_09931A2C536E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "mymapdoc.h"
#include "userlayer.h"

class AFX_EXT_CLASS CMyMapView  
{
public:
	CMyMapView();
	virtual ~CMyMapView();

	void Draw(HDC hDC, int nDrawMode,bool bForceDraw,bool bOnlyDrawUserMap);
	void Left();
	void Right();
	void Up();
	void Down();
	// 设置地图中心点
	void SetCenter(float x, float y);
	void SetCenter(int x, int y);

	void SetCoordMap(mapCOORDMAP* mapCoordMap)
	{
		m_mapCoordMap = mapCoordMap;
		m_UserMap.SetCoordMap(mapCoordMap);
		m_FileMap.SetCoordMap(mapCoordMap);
	}	
protected:
	float m_fCenterX;  //绘制图形时的屏幕中心
	float m_fCenterY;  // 绘制图形时的屏幕中心
	int m_nCenterX;  //绘制图形时的屏幕中心
	int m_nCenterY;  // 绘制图形时的屏幕中心

	HDC m_hMapDC;
	HBITMAP hbmNew;
	HDC m_hUserDC;
	HBITMAP m_hUserBmp;

	HDC m_hMemDC;
	HBITMAP m_hMemBmp;

public:
	void ReadUserMap();
	bool AddMyDearPoint(float xx, float yy,ObjectTypeConstants nType,ObjectTypeConstants nSubType,TCHAR* szText);
	//bool m_bMapChanged;  // 指示地图显示是否被改变
	mapCOORDMAP* m_mapCoordMap; // 坐标换算参数
	CMyMapDoc m_UserMap;    // 用户兴趣点，用户自己绘制线段等
	CMyMapDoc m_FileMap;    // 从文件调入的大众地图
	//CUserLayer m_UserLayer; // 用户绘制层
#ifdef UNDER_WINCE
	CCEDraw*   m_ceDraw;         // gapi画布
	unsigned char *      m_pDoubleBuffer;  // Pointer to the head position of back buffer
	LONG                 m_lMemorySize;
#endif
	void OnRButtonDown(UINT nFlags, POINT point,HDC hDC);
	void OnMouseMove(UINT nFlags, POINT point,HDC hDC);
	void OnLButtonUp(UINT nFlags, POINT point,HDC hDC);
	void OnLButtonDown(UINT nFlags, POINT point,HDC hDC);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags,HDC hDC);
	BOOL OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);

	int m_nDrawCurrent;  // 指示当前的地图工具状态
private:
	// 测量距离
	POINT m_PointOne,m_PointTwo;
	float m_fTatolDistance;
	int m_nPointCount;  // 

	//int m_nPointNum; //
	int PushNumb;

	POINT mPointOrign,mPointOrign1,mPointOld;
	bool bMany;

	// 有关地图编辑
	bool bEditDN ;  // 处于地图编辑，左键选中状态吗
	mapMAPPOINTSTRUCT mpt; // 编辑地图数据时候选中的编辑点
	mapMAPPOINTSTRUCT mpt1; // 编辑地图数据时候选中的编辑点的前一个点
	mapMAPPOINTSTRUCT mpt2; // 编辑地图数据时候选中的编辑点的后一个点
	CPline* pLine;
public:
	// 设置工具状态
	void SetTools(int nTools);
	int GetTools();
	// 画比例尺吗
	bool m_bDrawScale;
};

#endif // !defined(AFX_MYMAPVIEW_H__EC326E72_2151_4554_91C8_09931A2C536E__INCLUDED_)

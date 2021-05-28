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
	// ���õ�ͼ���ĵ�
	void SetCenter(float x, float y);
	void SetCenter(int x, int y);

	void SetCoordMap(mapCOORDMAP* mapCoordMap)
	{
		m_mapCoordMap = mapCoordMap;
		m_UserMap.SetCoordMap(mapCoordMap);
		m_FileMap.SetCoordMap(mapCoordMap);
	}	
protected:
	float m_fCenterX;  //����ͼ��ʱ����Ļ����
	float m_fCenterY;  // ����ͼ��ʱ����Ļ����
	int m_nCenterX;  //����ͼ��ʱ����Ļ����
	int m_nCenterY;  // ����ͼ��ʱ����Ļ����

	HDC m_hMapDC;
	HBITMAP hbmNew;
	HDC m_hUserDC;
	HBITMAP m_hUserBmp;

	HDC m_hMemDC;
	HBITMAP m_hMemBmp;

public:
	void ReadUserMap();
	bool AddMyDearPoint(float xx, float yy,ObjectTypeConstants nType,ObjectTypeConstants nSubType,TCHAR* szText);
	//bool m_bMapChanged;  // ָʾ��ͼ��ʾ�Ƿ񱻸ı�
	mapCOORDMAP* m_mapCoordMap; // ���껻�����
	CMyMapDoc m_UserMap;    // �û���Ȥ�㣬�û��Լ������߶ε�
	CMyMapDoc m_FileMap;    // ���ļ�����Ĵ��ڵ�ͼ
	//CUserLayer m_UserLayer; // �û����Ʋ�
#ifdef UNDER_WINCE
	CCEDraw*   m_ceDraw;         // gapi����
	unsigned char *      m_pDoubleBuffer;  // Pointer to the head position of back buffer
	LONG                 m_lMemorySize;
#endif
	void OnRButtonDown(UINT nFlags, POINT point,HDC hDC);
	void OnMouseMove(UINT nFlags, POINT point,HDC hDC);
	void OnLButtonUp(UINT nFlags, POINT point,HDC hDC);
	void OnLButtonDown(UINT nFlags, POINT point,HDC hDC);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags,HDC hDC);
	BOOL OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);

	int m_nDrawCurrent;  // ָʾ��ǰ�ĵ�ͼ����״̬
private:
	// ��������
	POINT m_PointOne,m_PointTwo;
	float m_fTatolDistance;
	int m_nPointCount;  // 

	//int m_nPointNum; //
	int PushNumb;

	POINT mPointOrign,mPointOrign1,mPointOld;
	bool bMany;

	// �йص�ͼ�༭
	bool bEditDN ;  // ���ڵ�ͼ�༭�����ѡ��״̬��
	mapMAPPOINTSTRUCT mpt; // �༭��ͼ����ʱ��ѡ�еı༭��
	mapMAPPOINTSTRUCT mpt1; // �༭��ͼ����ʱ��ѡ�еı༭���ǰһ����
	mapMAPPOINTSTRUCT mpt2; // �༭��ͼ����ʱ��ѡ�еı༭��ĺ�һ����
	CPline* pLine;
public:
	// ���ù���״̬
	void SetTools(int nTools);
	int GetTools();
	// ����������
	bool m_bDrawScale;
};

#endif // !defined(AFX_MYMAPVIEW_H__EC326E72_2151_4554_91C8_09931A2C536E__INCLUDED_)

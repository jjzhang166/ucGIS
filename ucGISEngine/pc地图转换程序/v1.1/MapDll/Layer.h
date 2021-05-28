// Layer.h: interface for the CLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYER_H__8AA60CAE_BB1E_4708_A4E4_8AD6A052A5D3__INCLUDED_)
#define AFX_LAYER_H__8AA60CAE_BB1E_4708_A4E4_8AD6A052A5D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "pline.h"
#include "plineRgn.h"
#include "mypoint.h"
#include "line.h"
#include <vector>
#include "gisgrid.h"
typedef vector<CGisGrid *> GridVector;
typedef vector<CGridIndex *> CGridIndexVector;

#define IDS_PLINE                       32781
#define IDS_POLYGON                     32782
#define IDS_TEXT                        32783
#define IDS_POINT                       32784
#define IDS_LINE                        32785

using namespace std;

typedef vector<CPline *> PLineVector;
typedef vector<CLine *> LineVector;
typedef vector<CPlineRgn *> PlineRgnVector;
typedef vector<CMyPoint *> MyPointVector;

class AFX_EXT_CLASS CLayer  
{
public:
	CLayer();
	virtual ~CLayer();
public:
	void Draw(HDC hDC, int nDrawMode);
	void DrawLabel(HDC hDC);

	void AddMyPoint(CMyPoint* point);
	void AddPlineRgn(CPlineRgn* plinergn);
	void WriteToFile(FILE *fp);
	void ReadFromFile(FILE *fp,int& fileoffset);
	void AddPline(CPline* pline);
	CPlineRgn* AddPLineRgn(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,int Numble, mapMAPPOINTSTRUCT* PointList,mapPENSTRUCT Pen,bool bTransparent,bool bFill,mapBRUSHSTRUCT Brush);
	CMyPoint* AddPoint(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,float X,float Y);
	CPline* AddPLine(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,int Numble, mapMAPPOINTSTRUCT* PointList,mapPENSTRUCT Pen);
	CLine* AddLine(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,float x1,float y1,float x2,float y2,mapPENSTRUCT Pen);

	// ��ȡͼ����α߽�
	bool GetRect(float * m_Xmin, float * m_Ymin, float * m_Xmax, float * m_Ymax);

	// ��ȡ���ξ����ڲ��ĵ�Ԫ��
	void GetPointFromRect(CMyPoint* points,int& nCount);
	bool DelPoint(TCHAR* szLabel);

    void IndexBuid(int layeri,int tcols,GridVector &gridvector,tagDrawRect &griddrawrect,int & fileoffset);
    int GetFileOffSetHeadLayer();
 	virtual int GetFileoffsetLen(){return m_fileoffsetlen;};

	// �Ƿ������Զ���ע
	bool m_bAutoLabel;
	// �Ƿ�����༭�ò�
	bool m_bEditable;
	// �Ƿ���ʾ��ͼ��
	bool m_bVisible;
	// ���б�ע����ʾ��һ����ʾ����վ̨�ı�ע
	bool m_bShowAllLabel;   
	// ͼ������
	TCHAR m_pszName[100];
	// ��Ұ��Χ
	float m_nEyeShotMin;
	float m_nEyeShotMax;

	// ����
	short m_ID;

	mapCOORDMAP* m_mapCoordMap; 
#ifdef UNDER_WINCE
	CCEDraw*   m_ceDraw;         // gapi����
#endif
protected:
	bool m_bDisplay;
    int m_fileoffsetlen;
	PLineVector m_PlineVector;
	LineVector m_LineVector;
	PlineRgnVector m_PlineRgnVector;
	MyPointVector m_MyPointVector;
public:
	void SetDisplay(bool bDsp){m_bDisplay=bDsp;};
	bool GetDisplay(){return m_bDisplay;}
	// ɾ��ָ��Ԫ��
	bool DeleteDraw(CDraw* pDr);
	// ��ȡ�����������������
	bool GetWhereIam(mapMAPPOINTSTRUCT* pt, TCHAR* szWhere);
	// ��Ļ���ڼ���ı༭��������
	bool IsOnTheLine(int x, int y,mapMAPPOINTSTRUCT* mpt,mapMAPPOINTSTRUCT* mpt1,mapMAPPOINTSTRUCT* mpt2, CPline** pLine);
	// ɾ�����ϵ�һ����
	bool DeletePoint(mapMAPPOINTSTRUCT* mpt);
	// ����һ���㵽����
	void AddPointToLine(int x,int y);
	void SetDrawType(bool bEditMode);
};

#endif // !defined(AFX_LAYER_H__8AA60CAE_BB1E_4708_A4E4_8AD6A052A5D3__INCLUDED_)

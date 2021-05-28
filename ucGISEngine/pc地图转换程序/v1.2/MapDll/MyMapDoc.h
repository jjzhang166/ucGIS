// MyMapDoc.h: interface for the CMyMapDoc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMAPDOC_H__D9CF7D13_06E0_41F8_A82A_9F26F4209946__INCLUDED_)
#define AFX_MYMAPDOC_H__D9CF7D13_06E0_41F8_A82A_9F26F4209946__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Layer.h"
#include "UserLayer.h"

typedef vector<CLayer *> LayerVector;

class AFX_EXT_CLASS CMyMapDoc   
{
public:
	CMyPoint* AddPoint(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,float X,float );
	CPlineRgn* AddPLineRgn(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,int nPointCount,mapMAPPOINTSTRUCT* PointList,mapPENSTRUCT Pen,bool bTransparent,bool bFill,mapBRUSHSTRUCT Brush);
	CPline* AddPLine(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,int nPointCount, mapMAPPOINTSTRUCT* PointList,mapPENSTRUCT Pen);
	CLine* AddLine(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,float x1,float y1,float x2,float y2,mapPENSTRUCT Pen);
	short AddLayer(CLayer* pLayer);
	CMyMapDoc();
	virtual ~CMyMapDoc();

	void Draw(HDC hDC, int nDrawMode);

	void WriteToFile(TCHAR* pszFileName);
	bool ReadFromFile(TCHAR* pszFileName);
// 
	void GetPointFromRect(CMyPoint* points,int& nPointCount);
	bool DelPoint(TCHAR* szLabel);
	bool m_bDataChanged;  // ָʾ��ͼ�����Ƿ����ı�
	TCHAR m_pszMapFilePath[200];
	bool m_bShowMapName;         // ��ʾ��ͼ����
	TCHAR m_lpszMapName[50];      // ��ͼ����
	float m_fCenterX;  //����ͼ��ʱ����Ļ����
	float m_fCenterY;  // ����ͼ��ʱ����Ļ����
	float scale;      // ������
	void SetCoordMap(mapCOORDMAP* mapCoordMap)
	{
		//m_mapCoordMap->DPtoVP(m_fCenterX,m_fCenterY,&m_nCenterX,&m_nCenterY);
		m_mapCoordMap = mapCoordMap;
	}
protected:
	CLayer m_layer;
	short m_nCurrentLayer;
	LayerVector m_LayerVector;



	mapCOORDMAP* m_mapCoordMap; // ���껻�����

	HDC m_hMemDC;
	HBITMAP hbmNew;

	CRITICAL_SECTION m_csDraw;		// �������ص��ٽ��
	
	bool WToA(TCHAR* wChar, char* cChar);

	//CTypedPtrArray<CObArray,CLayer*> m_LayerArray;
public:

	// ��ʾȫͼ
	LayerStruct* GetLayersState(int& nLen);
	void SetLayersState(LayerStruct* pState);

	// ��ȡͼ����α߽�
	bool GetRect(float * m_Xmin, float * m_Ymin, float * m_Xmax, float * m_Ymax);

	void ReleaseMap();

public:
	void ShowAllMap();
	CLayer* GetLayer(int nIndex)
	{
		if((nIndex>0)&&(m_LayerVector.size()>nIndex))
		{
			return m_LayerVector[nIndex];
		}else return NULL;
	};
	int GetLayerCount()
	{
		return m_LayerVector.size();
	};
	ToolConstants m_toolConstant;
	//Mapping Modes
	int m_nMappingMode;
	// ���б�ע����ʾ��һ����ʾ����վ̨�ı�ע
	bool m_bShowAllLabel;  

#ifdef UNDER_WINCE
	CCEDraw*   m_ceDraw;         // gapi����
#endif
    void AToW(char *sourceChar,TCHAR *wChar);

	// ɾ��ָ��Ԫ��
	bool DeleteDraw(short nLayer, CDraw* pDr);
	bool DeleteDraw(CDraw* pDr);
	// ��ȡ�����������������
	bool GetWhereIam(mapMAPPOINTSTRUCT* pt, TCHAR* szWhere);
	// ��Ļ����ֱ������ �ھͷ��ط�0������  ��Ļ���꣬��Ļ���꣬ѡ�еĵ㣬��ǰһ���㣬���һ����
	bool IsOnTheLine(int x, int y,mapMAPPOINTSTRUCT* mpt,mapMAPPOINTSTRUCT* mpt1,mapMAPPOINTSTRUCT* mpt2, CPline** pLine); 
	bool DeletePoint(mapMAPPOINTSTRUCT* mpt);
		// ����һ���㵽����
	void AddPointToLine(int x,int y);
	void SetDrawType(bool bEditMode);
	int  GetMapFileHeadOffset();//�õ����и�ʽ��ͼ�ļ���ͷ�ĳ��ȣ�

private:
	   //�����ռ���������
	tagDrawRect GridDrawRect ;//��ͼ�߼�����

	int Tcols;
	int Trows;//�����С�����
	GridVector gridvector;//��������
	int GridHaveBuild;
	bool m_bAreaIndexHaveBuild;
	CGridIndexVector cgridindexvector;
	CAreaIndexVector careaindexvector;
	int Tindexcols;
	int Tindexrows;
public:

	void Oninitgrid();
    void InitGrid(CGridIndex* pcgridindex,CRect *prect,int &irow,int &icol);
	void IndexBuid();
	void BuildAreaIndex();
	void SaveAreaIndex(TCHAR* pszFileName);
	void SaveGridIndexIndex(TCHAR* pszFileName);
	void ReadGridIndex(TCHAR* pszFileName);
	void SaveGridIndex(TCHAR* pszFileName);
    void ReleaseAreaIndex();
    void ReleaseGridIndex();
	void ReleaseGridIndexIndex();

	//void IndexBuid(CLayer *pLayer,int layeri);

};

#endif // !defined(AFX_MYMAPDOC_H__D9CF7D13_06E0_41F8_A82A_9F26F4209946__INCLUDED_)


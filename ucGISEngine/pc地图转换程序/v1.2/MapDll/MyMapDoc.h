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
	bool m_bDataChanged;  // 指示地图数据是否发生改变
	TCHAR m_pszMapFilePath[200];
	bool m_bShowMapName;         // 显示地图名称
	TCHAR m_lpszMapName[50];      // 地图名称
	float m_fCenterX;  //绘制图形时的屏幕中心
	float m_fCenterY;  // 绘制图形时的屏幕中心
	float scale;      // 比例尺
	void SetCoordMap(mapCOORDMAP* mapCoordMap)
	{
		//m_mapCoordMap->DPtoVP(m_fCenterX,m_fCenterY,&m_nCenterX,&m_nCenterY);
		m_mapCoordMap = mapCoordMap;
	}
protected:
	CLayer m_layer;
	short m_nCurrentLayer;
	LayerVector m_LayerVector;



	mapCOORDMAP* m_mapCoordMap; // 坐标换算参数

	HDC m_hMemDC;
	HBITMAP hbmNew;

	CRITICAL_SECTION m_csDraw;		// 与绘制相关的临界段
	
	bool WToA(TCHAR* wChar, char* cChar);

	//CTypedPtrArray<CObArray,CLayer*> m_LayerArray;
public:

	// 显示全图
	LayerStruct* GetLayersState(int& nLen);
	void SetLayersState(LayerStruct* pState);

	// 获取图层矩形边界
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
	// 所有标注都显示吗，一边显示所有站台的标注
	bool m_bShowAllLabel;  

#ifdef UNDER_WINCE
	CCEDraw*   m_ceDraw;         // gapi画布
#endif
    void AToW(char *sourceChar,TCHAR *wChar);

	// 删除指定元素
	bool DeleteDraw(short nLayer, CDraw* pDr);
	bool DeleteDraw(CDraw* pDr);
	// 获取我在哪里的文字描述
	bool GetWhereIam(mapMAPPOINTSTRUCT* pt, TCHAR* szWhere);
	// 屏幕点在直线上吗， 在就返回非0的数据  屏幕坐标，屏幕坐标，选中的点，点前一个点，点后一个点
	bool IsOnTheLine(int x, int y,mapMAPPOINTSTRUCT* mpt,mapMAPPOINTSTRUCT* mpt1,mapMAPPOINTSTRUCT* mpt2, CPline** pLine); 
	bool DeletePoint(mapMAPPOINTSTRUCT* mpt);
		// 增加一个点到线上
	void AddPointToLine(int x,int y);
	void SetDrawType(bool bEditMode);
	int  GetMapFileHeadOffset();//得到自有格式地图文件的头的长度；

private:
	   //建立空间网格索引
	tagDrawRect GridDrawRect ;//地图逻辑网格

	int Tcols;
	int Trows;//网格行、列数
	GridVector gridvector;//格网数组
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


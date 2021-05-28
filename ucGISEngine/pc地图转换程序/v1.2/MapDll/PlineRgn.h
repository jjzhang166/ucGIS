// PlineRgn.h: interface for the CPlineRgn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLINERGN_H__F845178E_4B4E_450E_9BBD_DF1CE6144047__INCLUDED_)
#define AFX_PLINERGN_H__F845178E_4B4E_450E_9BBD_DF1CE6144047__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Pline.h"

class AFX_EXT_CLASS CPlineRgn : public CPline  
{
public:
	CPlineRgn();
	CPlineRgn(TCHAR* pszLabel ,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,
		int nPointCount,mapMAPPOINTSTRUCT* PointList,mapPENSTRUCT stPen,
		bool bTransparent,bool bFill,mapBRUSHSTRUCT stBrush)
		:CPline(pszLabel,nLayer, nType, nSubType,nPointCount,PointList,stPen)
	{
		CPlineRgn();
		GetCenter();
		m_bTransparent = bTransparent;
		m_bFill = bFill;
		m_Brush = stBrush;
	}
	virtual ~CPlineRgn();

	virtual void ReadFromFile(FILE	*fp,int& fileoffset);
	virtual void WriteToFile(FILE *fp);
	virtual void Draw(HDC hDC, int nDrawMode);
	virtual void DrawLabel(HDC hDC);
protected:
	int m_fileoffsetlen;
	bool m_bTransparent;          // 是否透明
	bool m_bFill;                 // 是否需要填充
	mapBRUSHSTRUCT m_Brush;
	float m_fCenterX;            // 中心点地图坐标
	float m_fCenterY;            // 中心点地图坐标

	void GetCenter(void);
public:
	virtual int GetFileoffsetLen(){return m_fileoffsetlen;};

	// 某点在我上面吗
	bool PtInMe(const mapMAPPOINTSTRUCT* pt);
};

#endif // !defined(AFX_PLINERGN_H__F845178E_4B4E_450E_9BBD_DF1CE6144047__INCLUDED_)

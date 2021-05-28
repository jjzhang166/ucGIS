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
	bool m_bTransparent;          // �Ƿ�͸��
	bool m_bFill;                 // �Ƿ���Ҫ���
	mapBRUSHSTRUCT m_Brush;
	float m_fCenterX;            // ���ĵ��ͼ����
	float m_fCenterY;            // ���ĵ��ͼ����

	void GetCenter(void);
public:
	virtual int GetFileoffsetLen(){return m_fileoffsetlen;};

	// ĳ������������
	bool PtInMe(const mapMAPPOINTSTRUCT* pt);
};

#endif // !defined(AFX_PLINERGN_H__F845178E_4B4E_450E_9BBD_DF1CE6144047__INCLUDED_)

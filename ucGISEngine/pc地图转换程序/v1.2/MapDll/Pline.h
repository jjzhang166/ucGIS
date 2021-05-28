// Pline.h: interface for the CPline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLINE_H__295A62E6_F8E4_45AC_A198_183C086B8601__INCLUDED_)
#define AFX_PLINE_H__295A62E6_F8E4_45AC_A198_183C086B8601__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Draw.h"


class AFX_EXT_CLASS CPline : public CDraw  
{
public:
	CPline();

	CPline(TCHAR* pszLabel ,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,
		int nPointCount,mapMAPPOINTSTRUCT* PointList,mapPENSTRUCT stPen)
		:CDraw(pszLabel, nLayer, nType, nSubType)
	{
	m_PointList = NULL;
	m_LableCoord = NULL;
	memset(&m_Pen,0,sizeof(m_Pen));
	m_nPointCount = 0;

		m_Pen = stPen;
		m_nPointCount = nPointCount;
		m_PointList = new mapMAPPOINTSTRUCT[nPointCount];
		if(nPointCount>0)
		{
			for(int i=0;i<nPointCount;i++)
				m_PointList[i] = PointList[i];
		}
		GetRect(&m_Rect.fMinX,&m_Rect.fMinY,&m_Rect.fMaxX,&m_Rect.fMaxY);
	}
	virtual ~CPline();
	virtual void ReadFromFile(FILE	*fp,int& fileoffset);
	virtual void WriteToFile(FILE *fp);
	virtual void Draw(HDC hDC, int m_DrawMode);
	virtual void DrawLabel(HDC hDC);

	virtual void GetRect(float* minX, float* minY, float* maxX, float* maxY);
protected:
	int m_fileoffsetlen;
	int m_nPointCount;               // ����ֱ�ߵĶ�����Ŀ
	mapMAPPOINTSTRUCT* m_PointList;     // �洢�������������ָ��
	mapMAPPOINTSTRUCT* m_LableCoord;  // ��ע������
	mapPENSTRUCT m_Pen;                 // 
private:
	void getCoordFromLine(mapMAPPOINTSTRUCT* pts, int num, int sep, int ser, double &cx, double &cy,double &angle);
	double getAngle(double dx, double dy);
public:
	virtual int GetFileoffsetLen(){return m_fileoffsetlen;};
	// �ж���Ļ���Ƿ������ߵı༭����
	bool IsOnMe(int x, int y,mapMAPPOINTSTRUCT* mpt,mapMAPPOINTSTRUCT* mpt1,mapMAPPOINTSTRUCT* mpt2,CPline** pLine);
	// �޸�ĳ�����ֵ
	void MovePoint(mapMAPPOINTSTRUCT* mptold,mapMAPPOINTSTRUCT* mptnew);
	// ɾ��ĳ��
	bool DeletePoint(mapMAPPOINTSTRUCT* mpt);
	// �ӵ�
	void AddPoint(int x, int y);
};

#endif // !defined(AFX_PLINE_H__295A62E6_F8E4_45AC_A198_183C086B8601__INCLUDED_)

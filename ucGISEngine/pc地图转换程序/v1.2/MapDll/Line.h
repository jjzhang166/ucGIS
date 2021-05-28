// Line.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_H__8B3F7165_E05A_48D6_BD17_321D5BBE5597__INCLUDED_)
#define AFX_LINE_H__8B3F7165_E05A_48D6_BD17_321D5BBE5597__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Draw.h"

class AFX_EXT_CLASS CLine : public CDraw  
{
public:
	CLine();
	CLine(TCHAR* pszLabel, short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,
		float x1,float y1,float x2,float y2,mapPENSTRUCT stPen)
		:CDraw(pszLabel,nLayer, nType, nSubType){
			m_Pen = stPen;
			m_X1 = x1;
			m_X2 = x2;
			m_Y1 = y1;
			m_Y2 = y2;
			GetRect(&m_Rect.fMinX,&m_Rect.fMinY,&m_Rect.fMaxX,&m_Rect.fMaxY);
		}

	virtual ~CLine();
	virtual void ReadFromFile(FILE	*fp,int& fileoffset);
	virtual void WriteToFile(FILE *fp);
	virtual void Draw(HDC hDC, int m_DrawMode);
	virtual void DrawLabel(HDC hDC);
	virtual void GetRect(float* minX, float* minY, float* maxX, float* maxY)
	{
		*minX=min(m_X1,m_X2);  //���㲢������Сxֵ
		*maxX=max(m_X1,m_X2);  //���㲢�������xֵ
		*minY=min(m_Y1,m_Y2);  //���㲢������Сyֵ
		*maxY=max(m_Y1,m_Y2);  //���㲢�������yֵ
	}
	virtual int GetFileoffsetLen(){return m_fileoffsetlen;};

protected:
	int m_fileoffsetlen;
	float m_X1, m_X2, m_Y1, m_Y2; //ֱ�ߵ������յ�
	float m_fLong;                // ֱ�ߵĳ���
	mapPENSTRUCT m_Pen;
};

#endif // !defined(AFX_LINE_H__8B3F7165_E05A_48D6_BD17_321D5BBE5597__INCLUDED_)

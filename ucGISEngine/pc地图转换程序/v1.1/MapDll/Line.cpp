// Line.cpp: implementation of the CLine class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"	
#include "Line.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLine::CLine()
{
  m_fileoffsetlen=0;
}

CLine::~CLine()
{
}
void CLine::ReadFromFile(FILE	*fp,int &fileoffset)
{
    m_fileoffsetlen=fileoffset;
	CDraw::ReadFromFile(fp,fileoffset);
	fread(&m_X1,1,sizeof(float),fp);
	fileoffset+=sizeof(float);
	fread(&m_Y1,1,sizeof(float),fp);
	fileoffset+=sizeof(float);
	fread(&m_X2,1,sizeof(float),fp);
	fileoffset+=sizeof(float);
	fread(&m_Y2,1,sizeof(float),fp);
	fileoffset+=sizeof(float);
}
void CLine::WriteToFile(FILE *fp)
{
	CDraw::WriteToFile(fp);

	fwrite(&m_X1,1,sizeof(float),fp);
	fwrite(&m_Y1,1,sizeof(float),fp);
	fwrite(&m_X2,1,sizeof(float),fp);
	fwrite(&m_Y2,1,sizeof(float),fp);
}
void CLine::Draw(HDC hDC, int nDrawMode)
{

	if(!IsRectCross(m_Rect.fMinX,m_Rect.fMinY,m_Rect.fMaxX,m_Rect.fMaxY))
		return;

	int x1,y1,x2,y2;
	DPtoVP(m_X1,m_Y1,&x1,&y1);
	DPtoVP(m_X2,m_Y2,&x2,&y2);

	HPEN	oldPen,newPen;
	newPen = CreatePen(m_Pen.pattern,m_Pen.width,m_Pen.color);
	oldPen	= (HPEN)SelectObject(hDC,newPen);
	MoveToEx(hDC,x1,y1,NULL);
	LineTo(hDC,x2,y2);
	SelectObject(hDC,oldPen);
	DeleteObject(newPen);

}


void CLine::DrawLabel(HDC hDC)
{
	CDraw::DrawLabel(hDC);
	if(!lstrlen(m_lpszLabel)) return;
	
	if(!IsRectCross(m_Rect.fMinX,m_Rect.fMinY,m_Rect.fMaxX,m_Rect.fMaxY))
		return;

}
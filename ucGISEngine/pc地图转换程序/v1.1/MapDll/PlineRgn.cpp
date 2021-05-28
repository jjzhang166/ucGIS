// PlineRgn.cpp: implementation of the CPlineRgn class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"	
#include "PlineRgn.h"
#include "wingdi.h"
#include <tchar.h>
#include ".\plinergn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlineRgn::CPlineRgn()
{
	m_fileoffsetlen=0;
	m_bFill = m_bTransparent = false;
	memset(&m_Brush,0,sizeof(m_Brush));
	m_fCenterX = m_fCenterY = 0;
}

CPlineRgn::~CPlineRgn()
{

}
void CPlineRgn::ReadFromFile(FILE	*fp,int &fileoffset)
{
	//int offset=0;
    m_fileoffsetlen=fileoffset;
	CPline::ReadFromFile(fp,fileoffset);
  	fread(&m_bFill,sizeof(m_bFill),1,fp);
	fileoffset+=sizeof(m_bFill);
	fread(&m_bTransparent,sizeof(m_bTransparent),1,fp);
	fileoffset+=sizeof(m_bTransparent);
	int x = sizeof(m_Brush);
	fread(&m_Brush,sizeof(m_Brush),1,fp);
	fileoffset+=sizeof(m_Brush);
	fread(&m_fCenterX,sizeof(m_fCenterX),1,fp);
	fileoffset+=sizeof(m_fCenterX);
	fread(&m_fCenterY,sizeof(m_fCenterY),1,fp);
	fileoffset+=sizeof(m_fCenterY);
}
void CPlineRgn::WriteToFile(FILE *fp)
{
	CPline::WriteToFile(fp);
	fwrite(&m_bFill,sizeof(m_bFill),1,fp);
	fwrite(&m_bTransparent,sizeof(m_bTransparent),1,fp);
	fwrite(&m_Brush,sizeof(m_Brush),1,fp);
	fwrite(&m_fCenterX,sizeof(m_fCenterX),1,fp);
	fwrite(&m_fCenterY,sizeof(m_fCenterY),1,fp);
}
void CPlineRgn::Draw(HDC hDC, int m_DrawMode)
{
	if(!IsRectCross(m_Rect.fMinX,m_Rect.fMinY,m_Rect.fMaxX,m_Rect.fMaxY))
		return;

	POINT* ppoint = new POINT[m_nPointCount+1];
	if(ppoint==NULL) return;

	int nDrawPointCount = 0;
	int X1,Y1;
	for(int i=0;i<m_nPointCount;i++)
	{
		DPtoVP(m_PointList[i].x,m_PointList[i].y,&X1,&Y1);
		if((i==0)||(!
			((ppoint[i-1].x==X1)&&
			(ppoint[i-1].y==Y1))
			))
		{
			ppoint[nDrawPointCount].x = X1;
			ppoint[nDrawPointCount].y = Y1;
			nDrawPointCount++;
		}
	}
#ifdef UNDER_WINCE

	CCEPen cePen;
	cePen.CreatePen(m_Pen.pattern,m_Pen.width, m_ceDraw->ConvertColor( m_Pen.color ) ); // Convert RGB color to CE support Colo
	CCEBrush ceBrush;
	ceBrush.CreateBrush( 1, m_ceDraw->ConvertColor(m_Brush.forecolor ) , 1 );

	m_ceDraw->SetGDIObject( &cePen );
	m_ceDraw->SetGDIObject( &ceBrush );
	m_ceDraw->FillPolygon( ppoint, nDrawPointCount, 0, 0  ); // Draw an polygon
	m_ceDraw->DrawPolygon( ppoint, nDrawPointCount  ); // Draw an polygon
#else
	HPEN	oldPen,newPen;
	newPen = CreatePen(m_Pen.pattern,m_Pen.width,m_Pen.color);
	oldPen	= (HPEN)SelectObject(hDC,newPen);

	// 如果需要填充多边形
    if(m_bFill)
	{
		long lColor = m_Brush.forecolor;
		HBRUSH newBrush, oldBrush;
		newBrush = (HBRUSH)CreateSolidBrush(m_Brush.forecolor);
		oldBrush = (HBRUSH)SelectObject(hDC,newBrush);
		Polygon(hDC,ppoint,nDrawPointCount);
		SelectObject(hDC,oldBrush);	
		DeleteObject(newBrush);
	}
	else
		Polygon(hDC,ppoint,nDrawPointCount);
		
	SelectObject(hDC,oldPen);
	DeleteObject(newPen);
#endif
	delete[] ppoint;
	ppoint = NULL;
}

void CPlineRgn::DrawLabel(HDC hDC)
{

	CDraw::DrawLabel(hDC);

	if(!lstrlen(m_lpszLabel)) return;

	if(!IsRectCross(m_Rect.fMinX,m_Rect.fMinY,m_Rect.fMaxX,m_Rect.fMaxY))
		return;

	int x,y;
	DPtoVP(m_fCenterX,m_fCenterY,&x,&y);

	LOGFONT	lf = {0};
	lf.lfHeight = 12;
	lf.lfWidth = 0;
	lf.lfWeight = 0;
	lf.lfCharSet = GB2312_CHARSET;
	lf.lfEscapement=lf.lfOrientation=0;
	memcpy(lf.lfFaceName,TEXT("宋体"),sizeof(lf.lfFaceName));

	// 做防止标注覆盖的运算
	SIZE sz;
	GetTextExtentPoint32(hDC,m_lpszLabel,lstrlen(m_lpszLabel),&sz);

	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x+sz.cx;
	rect.bottom = y+sz.cy;


	if(!m_mapCoordMap->IsLabelRectUsed(rect)) // 如果标注区域没有被绘制
	{
#ifdef UNDER_WINCE

	POINT ptText = { x, y };
	m_ceDraw->DrawText( &ptText,m_lpszLabel,lstrlen(m_lpszLabel), 1 ); // Draw text with pen color
#else
	HFONT newFont, oldFont;
	newFont =  (HFONT)CreateFontIndirect(&lf);

	oldFont = (HFONT)SelectObject(hDC,newFont);

	SetBkMode(hDC,TRANSPARENT);

	ExtTextOut(hDC, x, y, 0, NULL, (LPCTSTR)(m_lpszLabel), 
         lstrlen(m_lpszLabel),0);

	SelectObject(hDC,oldFont);
	DeleteObject(newFont);

#endif
	}
}
void CPlineRgn::GetCenter(void)
{
	float x1,y1,x2,y2;
	GetRect(&x1,&y1,&x2,&y2);
	m_fCenterX = x1+(x2-x1)/2;
	m_fCenterY = y1+(y2-y1)/2;
}
// 某点在我上面吗
bool CPlineRgn::PtInMe(const mapMAPPOINTSTRUCT* pt)
{
	mapRECT* rect = NULL;
	return G_PtInPolygon(m_PointList,m_nPointCount,*pt,rect);
}

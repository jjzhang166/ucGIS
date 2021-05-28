// Pline.cpp: implementation of the CPline class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"	
#include "Pline.h"
#include <math.h>
#include ".\pline.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int linelong =   4;

CPline::CPline()
{
	m_fileoffsetlen=0;
	m_PointList = NULL;
	m_LableCoord = NULL;
	memset(&m_Pen,0,sizeof(m_Pen));
	m_nPointCount = 0;
#ifdef UNDER_WINCE
	m_ceDraw = NULL;
#endif
}

CPline::~CPline()
{
	if (m_nPointCount>0)
	{
		delete[] m_PointList;
		m_PointList = NULL;
	}
	if(m_LableCoord)
	{
		delete[] m_LableCoord;
		m_LableCoord = NULL;
	}
}
void CPline::ReadFromFile(FILE	*fp,int &fileoffset)
{
	m_fileoffsetlen=fileoffset;
	CDraw::ReadFromFile(fp,fileoffset);
	fread(&m_nPointCount,1,sizeof(int),fp);
	fileoffset+=sizeof(int);
	fread(&m_Pen,1,sizeof(mapPENSTRUCT),fp);
	fileoffset+=sizeof(mapPENSTRUCT);
	m_PointList = new mapMAPPOINTSTRUCT[m_nPointCount];
	//fread(m_PointList,sizeof(mapMAPPOINTSTRUCT),m_nPointCount,fp);

	for(int i=0;i<m_nPointCount;i++)
	{
		fread(&m_PointList[i].x,sizeof(float),1,fp);
	    fileoffset+=sizeof(float);
		fread(&m_PointList[i].y,sizeof(float),1,fp);
	    fileoffset+=sizeof(float);
	}
}
void CPline::WriteToFile(FILE *fp)
{
	CDraw::WriteToFile(fp);

	fwrite(&m_nPointCount,1,sizeof(int),fp);
	fwrite(&m_Pen,1,sizeof(mapPENSTRUCT),fp);
	for(int i=0;i<m_nPointCount;i++)
	{
		fwrite(&m_PointList[i].x,sizeof(float),1,fp);
		fwrite(&m_PointList[i].y,sizeof(float),1,fp);
	}
}
void CPline::Draw(HDC hDC, int nDrawMode)
{

	if(!IsRectCross(m_Rect.fMinX,m_Rect.fMinY,m_Rect.fMaxX,m_Rect.fMaxY))
		return;

	//	int nEyeShot = m_mapCoordMap->scale*32*100000;
	//	if((nEyeShot>700)&&(GetWidth(m_Pen.width)<2)) 
	//		return;
#ifdef UNDER_WINCE
	CCEPen cePen;
	cePen.CreatePen( m_Pen.pattern,m_Pen.width, m_ceDraw->ConvertColor( m_Pen.color ) ); // Convert RGB color to CE support Colo
	//cePen.CreatePen( 2, 2, m_ceDraw->ConvertColor( 255, 0, 0 ) ); // Con
	m_ceDraw->SetGDIObject( &cePen );

	POINT* ppoint = new POINT[m_nPointCount+1];
	if(ppoint==NULL) return;
	/*
	int nCount = 0;int x,y;
	for(int i=0;i<m_nPointCount;i++)
	{
	DPtoVP(m_PointList[i].x,m_PointList[i].y,&x,&y);

	if(nCount==0||abs(ppoint[nCount-1].x-x)>0||abs(ppoint[nCount-1].y-y)>0)
	{
	ppoint[nCount].x = x;
	ppoint[nCount].y = y;
	nCount++;
	}
	}
	m_ceDraw->DrawPolyline(ppoint,nCount);
	*/

	int X1,Y1;
	int nDrawPointCount = 0;
	for(int i=0;i<m_nPointCount;i++)
	{
		DPtoVP(m_PointList[i].x,m_PointList[i].y,&X1,&Y1);
		if((i==0)||(!
			((ppoint[nDrawPointCount-1].x==X1)&&
			(ppoint[nDrawPointCount-1].y==Y1))
			))
		{
			ppoint[nDrawPointCount].x = X1;
			ppoint[nDrawPointCount].y = Y1;
			nDrawPointCount++;
		}
	}

	m_ceDraw->DrawPolyline(ppoint,nDrawPointCount);

	delete[] ppoint;
	ppoint = NULL;
#else
	int x,y,xp,yp;
	x = y = xp = yp = 0;
	POINT* ppoints = new POINT[m_nPointCount];
	memset(ppoints,0,sizeof(POINT)*m_nPointCount);
	int nCount = 0;
	if(ppoints)
	{
		for(int i=0;i<m_nPointCount;i++)
		{
			DPtoVP(m_PointList[i].x,m_PointList[i].y,&x,&y);

			if(nCount==0||abs(ppoints[nCount-1].x-x)>0||abs(ppoints[nCount-1].y-y)>0)
			{
				ppoints[nCount].x = x;
				ppoints[nCount].y = y;
				nCount++;
			}

			//DPtoVP(m_PointList[i].x,m_PointList[i].y,&x1,&y1);
			//if(i==0)
			//	MoveToEx(hDC,x1,y1,NULL);
			//else
			//	LineTo(hDC,x1,y1);
		}

		HPEN	oldPen,newPen;
		newPen = CreatePen(m_Pen.pattern,m_Pen.width,m_Pen.color);
		oldPen	= (HPEN)SelectObject(hDC,newPen);
		Polyline(hDC,ppoints,nCount);
		SelectObject(hDC,oldPen);
		DeleteObject(newPen);

		// 绘制激活状态
		if(m_bEditMode)
		{
			HBRUSH newBrush, oldBrush;
			newBrush = (HBRUSH)CreateSolidBrush(RGB(0,0,0));
			oldBrush = (HBRUSH)SelectObject(hDC,newBrush);
			for(int j=0;j<m_nPointCount;j++)
			{
				Rectangle(hDC,ppoints[j].x-linelong,ppoints[j].y-linelong,ppoints[j].x+linelong,ppoints[j].y+linelong);
			}
			SelectObject(hDC,oldBrush);	
			DeleteObject(newBrush);
		}
	}
	delete[] ppoints;
	ppoints = NULL;

#endif
}
#define PI 3.14159265

void CPline::DrawLabel(HDC hDC)
{
	CDraw::DrawLabel(hDC);
	if(!lstrlen(m_lpszLabel)) return;

	if(!IsRectCross(m_Rect.fMinX,m_Rect.fMinY,m_Rect.fMaxX,m_Rect.fMaxY))
		return;

	int nHanziCount = lstrlen(m_lpszLabel);
	// 为避免运算出错，长度不都的加空格，至少保证2个长度
	if(nHanziCount==1)
	{
		lstrcat(m_lpszLabel,_T(" "));
		nHanziCount = lstrlen(m_lpszLabel);
	}

	if (nHanziCount==0) return;
	LOGFONT	lf = {0};
	lf.lfHeight = 12;
	lf.lfWidth = 0;
	lf.lfWeight = 0;
	lf.lfCharSet = GB2312_CHARSET;
	lf.lfEscapement=lf.lfOrientation=0;
	//lf.lfEscapement  = b;
	memcpy(lf.lfFaceName,TEXT("宋体"),sizeof(lf.lfFaceName));

	SetBkMode(hDC,TRANSPARENT);

	// 做防止标注覆盖的运算
	SIZE sz,sz1;
	GetTextExtentPoint32(hDC,m_lpszLabel,lstrlen(m_lpszLabel),&sz);
	GetTextExtentPoint32(hDC,_T("汉"),1,&sz1);
	int x,y;

	if(!m_LableCoord)
	{
		m_LableCoord = new mapMAPPOINTSTRUCT[nHanziCount];
		if(!m_LableCoord) return;

		for(int j=0;j<nHanziCount;j++)
		{
			double	tmpx,tmpy,tmpangle;
			getCoordFromLine(m_PointList,m_nPointCount,nHanziCount+1,j+1,tmpx,tmpy,tmpangle);
			m_LableCoord[j].x = tmpx;
			m_LableCoord[j].y = tmpy;
		}
	}

	POINT* pts = new POINT[nHanziCount];
	for(int i=0;i<nHanziCount;i++)
	{
		DPtoVP(m_LableCoord[i].x,m_LableCoord[i].y,&x,&y);
		pts[i].x = x;
		pts[i].y = y;
	}
	long dw,dh,x1,x2,y1,y2,dwh;
	for(int i=0;i<nHanziCount-1;i++)
	{
		x1 = pts[i].x;
		y1 = pts[i].y;
		x2 = pts[i+1].x;
		y2 = pts[i+1].y;
		if(i==0)
		{
			dw	= abs(x2-x1);	dh	= abs(y2-y1);
		}
		else
		{
			if(dw<abs(x2-x1))	dw	= abs(x2-x1);
			if(dh<abs(y2-y1))	dh	= abs(y2-y1);
		}	
	}
	BOOL		bNeedChangePos = FALSE;
	if(dw>dh)
	{
		dwh	= dw;
		if(m_LableCoord[0].x > m_LableCoord[nHanziCount-1].x)
			bNeedChangePos = TRUE;
	}
	else		
	{
		dwh	= dh;
		if(m_LableCoord[0].y < m_LableCoord[nHanziCount-1].y)
			bNeedChangePos = TRUE;
	}
	if(dwh > sz.cy)
	{
		//转换,使之位置合适
		if(bNeedChangePos)
		{

			long	tmp;
			for(int j=0;j<nHanziCount/2;j++)
			{
				tmp					= pts[nHanziCount-1-j].x;
				pts[nHanziCount-1-j].x	= pts[j].x;
				pts[j].x				= tmp;

				tmp					= pts[nHanziCount-1-j].y;
				pts[nHanziCount-1-j].y	= pts[j].y;
				pts[j].y				= tmp;

			}
		}
#ifdef UNDER_WINCE	
		RECT rect1;
		for(int j=0;j<nHanziCount;j++)
		{

			rect1.left = pts[j].x;
			rect1.top = pts[j].y;
			rect1.right = pts[j].x+sz1.cx;
			rect1.bottom = pts[j].y+sz1.cy;
			if(!m_mapCoordMap->IsLabelRectUsed(rect1))
				m_ceDraw->DrawText( &pts[j], m_lpszLabel+j,1, 1 ); // Draw text with pen color			
		}
#else		
		HFONT newFont, oldFont;
		newFont =  (HFONT)CreateFontIndirect(&lf);
		oldFont = (HFONT)SelectObject(hDC,newFont);	
		RECT rect1;
		for(int j=0;j<nHanziCount;j++)
		{
			rect1.left = pts[j].x;
			rect1.top = pts[j].y;
			rect1.right = pts[j].x+sz1.cx;
			rect1.bottom = pts[j].y+sz1.cy;
			if(!m_mapCoordMap->IsLabelRectUsed(rect1))
				ExtTextOut(hDC, pts[j].x, pts[j].y, 0, NULL, m_lpszLabel+j,1,0);
		}
		SelectObject(hDC,oldFont);
		DeleteObject(newFont);	
#endif				
	}
	else
	{
		x = pts[nHanziCount/2].x;
		y = pts[nHanziCount/2].y;

		x -= sz.cx/2;

		RECT rect;
		rect.left = x;
		rect.top = y;
		rect.right = x+sz.cx;
		rect.bottom = y+sz.cy;
		// 如果线段长度不能显示文字，则不显示
		int xmin,ymin,xmax,ymax;
		DPtoVP(m_Rect.fMinX,m_Rect.fMinY,&xmin,&ymin);
		DPtoVP(m_Rect.fMaxX,m_Rect.fMaxY,&xmax,&ymax);
		int nlinelen = (int)sqrt((float)((xmax-xmin)*(xmax-xmin)+(ymax-ymin)*(ymax-ymin)));
		int labelLen = (int)sqrt((float)(sz.cx*sz.cx+sz.cy*sz.cy));

		if(((nlinelen-labelLen)>-20)&&!m_mapCoordMap->IsLabelRectUsed(rect)) // 如果标注区域没有被绘制
		{
			POINT pt;
			pt.x = x;
			pt.y = y;
#ifdef UNDER_WINCE	
			m_ceDraw->DrawText( &pt, m_lpszLabel,nHanziCount, 1 ); // Draw text with pen color
#else
			HFONT newFont, oldFont;
			newFont =  (HFONT)CreateFontIndirect(&lf);
			oldFont = (HFONT)SelectObject(hDC,newFont);
			ExtTextOut(hDC, x, y, 0, NULL, (LPCTSTR)m_lpszLabel,lstrlen(m_lpszLabel),0);
			SelectObject(hDC,oldFont);
			DeleteObject(newFont);
#endif
		}
	}
	delete[] pts;
	pts = NULL;

}
//函数:CPline::GetRect作用:计算并返回一条连续直线的边界矩形
//连续直线的主控参数在pline1中，坐标在Pointxy中
void CPline::GetRect(float *minX,float *minY,float *maxX,float *maxY)
{
	float x1,y1,x2,y2;
	//用第一个顶点的坐标初试化变量
	x1=m_PointList[0].x;x2=m_PointList[0].x;
	y1=m_PointList[0].y;y2=m_PointList[0].y;
	for(int i=1;i<m_nPointCount;i++)          //从第二个顶点开始循环
	{
		x1=min(x1,m_PointList[i].x);    //得到最小x坐标
		x2=max(x2,m_PointList[i].x);    //得到最大x坐标
		y1=min(y1,m_PointList[i].y);    //得到最小y坐标
		y2=max(y2,m_PointList[i].y);    //得到最大y坐标
	}
	*minX=x1;*maxX=x2;*minY=y1;*maxY=y2; //将值赋给地址返回
}
// 点列表 点数量 汉字个数 ,第几个汉字，
void CPline::getCoordFromLine(mapMAPPOINTSTRUCT* pts, int num, int sep, int ser, double &cx, double &cy,double &angle)
{
	double	dist;
	double	subdist;
	int		i;

	dist	= 0.0;
	for(i=0;i<num-1;i++)
	{
		dist	= dist + sqrt((pts[i].x-pts[i+1].x)*(pts[i].x-pts[i+1].x) + (pts[i].y-pts[i+1].y)*(pts[i].y-pts[i+1].y));
	}

	subdist	= dist/sep * ser;

	for(i=0;i<num-1;i++)
	{
		dist	= sqrt((pts[i].x-pts[i+1].x)*(pts[i].x-pts[i+1].x) + (pts[i].y-pts[i+1].y)*(pts[i].y-pts[i+1].y));

		if(dist>subdist)
		{
			angle	= getAngle(pts[i+1].x-pts[i].x,pts[i+1].y-pts[i].y);

			cx	= pts[i].x + subdist*cos(angle);
			cy	= pts[i].y + subdist*sin(angle);

			break;
		}
		else
		{
			subdist	= subdist - dist;
		}
	}
}

//获取角度
double CPline::getAngle(double dx, double dy)
{
	double	angle;

	if(fabs(dy) < 1e-10)
	{
		if(dx > 0.0)					angle	= 0.0;
		else							angle	= PI;
	}
	else	if(dy > 1e-10)
	{
		if(dx > 1e-10)					angle	= atan2(fabs(dy),fabs(dx));		
		else	if(fabs(dx) < 1e-10)	angle	= PI/2;
		else	if(dx < 1e-10)			angle	= PI-atan2(fabs(dy),fabs(dx));
	}
	else
	{
		if(dx > 1e-10)					angle	= PI*2-atan2(fabs(dy),fabs(dx));
		else	if(fabs(dx) < 1e-10)	angle	= PI*3/2;
		else	if(dx < 1e-10)			angle	= PI+atan2(fabs(dy),fabs(dx));
	}
	return	angle;
}


// 判断屏幕点是否在曲线的编辑点上
bool CPline::IsOnMe(int x, int y,mapMAPPOINTSTRUCT* mpt,mapMAPPOINTSTRUCT* mpt1,mapMAPPOINTSTRUCT* mpt2,CPline** pLine)
{
	if (!m_bEditMode) return false;
	float x1,y1,x2,y2;
	m_mapCoordMap->VPtoDP(x-linelong,y-linelong,&x1,&y1);
	m_mapCoordMap->VPtoDP(x+linelong,y+linelong,&x2,&y2);

	int nDrawPointCount = 0;
	for(int i=0;i<m_nPointCount;i++)
	{
		if((m_PointList[i].x>x1)&&(m_PointList[i].x<x2)&&(m_PointList[i].y<y1)&&(m_PointList[i].y>y2))
		{
			mpt->x = m_PointList[i].x;
			mpt->y = m_PointList[i].y;
			if((i-1)>=0)
			{
				mpt1->x = m_PointList[i-1].x;
				mpt1->y = m_PointList[i-1].y;
			}
			else
			{
				mpt1->x = 0;
				mpt1->y = 0;
			}
			if((i+1)<m_nPointCount)
			{
				mpt2->x = m_PointList[i+1].x;
				mpt2->y = m_PointList[i+1].y;
			}
			else
			{
				mpt2->x = 0;
				mpt2->y = 0;
			}
			*pLine = this;
			return true;
		}
	}	
	return false;
}
void CPline::MovePoint(mapMAPPOINTSTRUCT* mptold,mapMAPPOINTSTRUCT* mptnew)
{
	for(int i=0;i<m_nPointCount;i++)
	{
		if(m_PointList[i].x == mptold->x&&m_PointList[i].y == mptold->y)
		{
			m_PointList[i].x = mptnew->x;
			m_PointList[i].y = mptnew->y;
			break;
		}
	}
}
bool CPline::DeletePoint(mapMAPPOINTSTRUCT* mpt)
{
	int i = 0;
	bool bFindit = false;
	for( i=0;i<m_nPointCount;i++)
	{
		if(m_PointList[i].x == mpt->x&&m_PointList[i].y == mpt->y)
		{
			bFindit = true;
			break;
		}
	}
	if(!bFindit) return false;

	mapMAPPOINTSTRUCT* tmpList = new mapMAPPOINTSTRUCT[m_nPointCount-1];
	mapMAPPOINTSTRUCT* changlist = NULL; //用于交换

	for( int j=0;j<m_nPointCount-1;j++)
	{
		if(j<i)
			tmpList[j] = m_PointList[j];
		else
			tmpList[j] = m_PointList[j+1];
	}
	changlist = m_PointList;
	m_PointList = tmpList;
	delete changlist;;

	m_nPointCount--;
	return true;
}

// 加点
void CPline::AddPoint(int x, int y)
{
	float fx,fy;
	m_mapCoordMap->VPtoDP(x,y,&fx,&fy);
	int nShort1; // 第一最短距离的点，第二最短
	float dismin = 10000;
	int i= 0;
	for(  i=0;i<m_nPointCount;i++)
	{
		float dis =  sqrt(abs(m_PointList[i].x-fx)*abs(m_PointList[i].x-fx) +
			abs(m_PointList[i].y-fy)*abs(m_PointList[i].y-fy));
		if(dismin>dis)
		{
			nShort1 = i;
			dismin = dis;
		}
	}
	float dis1,dis2;
	dis1=dis2=0;
	if (i>0)
		dis1 = PointLine(fx,fy,m_PointList[nShort1].x,m_PointList[nShort1].y,m_PointList[nShort1-1].x,m_PointList[nShort1-1].y);
	if(i<m_nPointCount-1)
		dis2 = PointLine(fx,fy,m_PointList[nShort1].x,m_PointList[nShort1].y,m_PointList[nShort1+1].x,m_PointList[nShort1+1].y);

	mapMAPPOINTSTRUCT* tmpList = new mapMAPPOINTSTRUCT[m_nPointCount+1];
	mapMAPPOINTSTRUCT* changlist = NULL; //用于交换

	for( int j=0;j<m_nPointCount+1;j++)
	{
		if(dis1<dis2)
		{
			if(j<=nShort1)
				tmpList[j] = m_PointList[j];
			else if(j==nShort1)
			{
				tmpList[j].x = fx;tmpList[j].y = fy;
			}
			else if(j>nShort1)
				tmpList[j] = m_PointList[j-1];
		}
		else
		{
			if(j<=nShort1)
				tmpList[j] = m_PointList[j];
			else if(j==nShort1+1)
			{
				tmpList[j].x = fx;tmpList[j].y = fy;
			}
			else if(j>nShort1+1)
				tmpList[j] = m_PointList[j-1];	
		}
	}
	changlist = m_PointList;
	m_PointList = tmpList;
	delete changlist;;

	m_nPointCount++;
}

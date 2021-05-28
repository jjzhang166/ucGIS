#define PLINE_GLOBALES
#include<stdio.h>
#include "GUI.h"
#include "Pline.h"
#include "CoorTrans.h"
#define USEDYNAALLOC 0
//
BOOLEAN Pline_ReadFromFile(FILE *fp,GPLine *lpGpline)
{
	INT32U i,nPointCount;
	GeoPoint geoPoint;
	INT32U nHanziCount=0; 
	GISCHAR *lpszLabel=NULL;

	Lable_ReadFromFile(fp,&lpGpline->baselabel);
/*
    WM_LOCK();
    if(lpGpline->baselabel.text)
	   lpszLabel=(GISCHAR*) GUI_ALLOC_h2p(lpGpline->baselabel.text);
    WM_UNLOCK();
    if(lpszLabel)
	  nHanziCount= GBStrlen(lpszLabel);

	// 为避免运算出错，长度不都的加空格，至少保证2个长度
	if(nHanziCount==1)
	{
		strcat(lpszLabel,(" "));
		nHanziCount = GBStrlen(lpszLabel);
	}
*/

	fread(&nPointCount,1,sizeof(int),fp);
	fread(&lpGpline->plinepen,1,sizeof(GPen),fp);
    //nPointCount=lpGpline->num_pt;
    if(lpGpline->pointarray.InitState== GUI_ARRAY_STATE_NOT_CREATED)
	  GUI_ARRAY_CREATE(&lpGpline->pointarray);
	for(i=0;i<nPointCount;i++)
	{
		fread(&geoPoint.lon,sizeof(FP32),1,fp);
		fread(&geoPoint.lat,sizeof(FP32),1,fp);
        geoPoint.hgt=0;
        WM_LOCK();
		GUI_ARRAY_AddItem(&lpGpline->pointarray, &geoPoint, sizeof(GeoPoint));
        WM_UNLOCK();
	}
	/*
	for(j=0;j<nHanziCount;j++)
	{
		Pline_getCoordFromLine(&lpGpline->pointarray,nPointCount,nHanziCount+1,j+1,&tmpx,&tmpy,&tmpangle);
		geoPoint.lon = tmpx;
		geoPoint.lat = tmpy;
        WM_LOCK();
		GUI_ARRAY_AddItem(&lpGpline->lablecoordarray, &geoPoint, sizeof(GeoPoint));
        WM_UNLOCK();
	}
	*/
    return TRUE;
}
/*
void Pline_WriteToFile(FILE *fp)
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
*/
void Pline_Draw(GPLine *lpGpline, INT32U nDrawMode,BOOLEAN bFilled,GBrush brush)
{
    //INT16U pattern,width;
	//GISLONG color;
	GPen pen;
	GeoPoint* ppoint,*ppointLast;
#if USEDYNAALLOC
	GUI_POINT *pGuiPoint;//[10000]; 
    GUI_HMEM  phmem;
#else
	GUI_POINT pGuiPoint[6000]; 
#endif
	INT32S X0,Y0,X1,Y1,i,nDrawPointCount,nPointCount=0;
	nDrawPointCount = 0;
	if(!IsRectCross(lpGpline->baselabel.pCoordMap,lpGpline->baselabel.m_Rect.fMinX,lpGpline->baselabel.m_Rect.fMinY,lpGpline->baselabel.m_Rect.fMaxX,lpGpline->baselabel.m_Rect.fMaxY))
		return;
    pen.patter=lpGpline->plinepen.patter;
	pen.width=lpGpline->plinepen.width;
	pen.color=lpGpline->plinepen.color;
	GUI_SetPenSize(pen.width);
    GUI_SetPenShape(pen.patter);
    GUI_SetColor(pen.color);
    if(lpGpline->pointarray.haHandle)
      nPointCount=GUI_ARRAY_GetNumItems(&lpGpline->pointarray);
	else
      return;
#if USEDYNAALLOC
    phmem=GUI_ALLOC_AllocZero(sizeof(GUI_POINT)*(nPointCount+1));
	pGuiPoint=(GUI_POINT*)GUI_ALLOC_h2p(phmem);
	if(pGuiPoint==NULL) return;
#endif
	for(i=0;i<nPointCount;i++)
	{
		if(nDrawPointCount==0)
		{
            ppoint=(GeoPoint*)GUI_ARRAY_GetpItem(&lpGpline->pointarray,i);
		    DPtoVP(lpGpline->baselabel.pCoordMap,ppoint->lon,ppoint->lat,&X1,&Y1);
			pGuiPoint[nDrawPointCount].x = X1;
			pGuiPoint[nDrawPointCount].y = Y1;
			nDrawPointCount++;
		}
		else
		{
		    ppointLast=(GeoPoint*)GUI_ARRAY_GetpItem(&lpGpline->pointarray,i-1);
            ppoint=(GeoPoint*)GUI_ARRAY_GetpItem(&lpGpline->pointarray,i);
		    DPtoVP(lpGpline->baselabel.pCoordMap,ppoint->lon,ppoint->lat,&X1,&Y1);
		    DPtoVP(lpGpline->baselabel.pCoordMap,ppointLast->lon,ppointLast->lat,&X0,&Y0);
		    if(abs(X0-X1)>0||abs(Y0-Y1)>0)
			{
			   pGuiPoint[nDrawPointCount].x = X1;
			   pGuiPoint[nDrawPointCount].y = Y1;
			   nDrawPointCount++;
			}
		}
	}
	
	//GUI_SetDrawMode(nDrawMode);
	/*
	if(bFilled)
	{
      GUI_SetColor(brush.forcolor);
      GUI_SetBkColor(brush.backcolor);
	  GUI_SetPenShape(brush.patter);
      GUI_FillPolygon(pGuiPoint,nDrawPointCount,0,0);
	}
	else
	*/
	GUI_DrawPolygon(pGuiPoint,nDrawPointCount,0,0);

#if USEDYNAALLOC

	GUI_ALLOC_Free(phmem);
    phmem=0;
    pGuiPoint = NULL;
#endif
}

void Pline_DrawLabel(GPLine *lpGpline)
{
	GISCHAR  *lpszLabel;
	GISCHAR tempText[3]={0};
	//GeoPoint *ppoint,*ppoint0,*ppointend;
	FP64 tmpx,tmpy,tmpangle;
#if USEDYNAALLOC
    GUI_HMEM  phmem,phmempts;
	GeoPoint *geoPoint;//[1000];
#else
	GeoPoint geoPoint[1000];
#endif
	GISLONG dw,dh,x1,x2,y1,y2,dwh,tmp;
	GIS_RECT rect,rect1;
	GUI_RECT rect2;
	INT32S cx,cy,cx1,cy1;
	INT32S i,j,nHanziCount,nPointCount;
	INT32S x,y;
	INT32S xmin,ymin,xmax,ymax;
	INT32S labelLen , nlinelen; 
#if USEDYNAALLOC
	GUI_POINT *pts;//[1000];
#else
	GUI_POINT pts[1000];
#endif
	BOOLEAN		bNeedChangePos = FALSE;
	if(!lpGpline->baselabel.text) return;
	WM_LOCK();
	lpszLabel=(GISCHAR*) GUI_ALLOC_h2p(lpGpline->baselabel.text);
    WM_UNLOCK();
	if(!lpszLabel) return;
	if(!IsRectCross(lpGpline->baselabel.pCoordMap,lpGpline->baselabel.m_Rect.fMinX,lpGpline->baselabel.m_Rect.fMinY,lpGpline->baselabel.m_Rect.fMaxX,lpGpline->baselabel.m_Rect.fMaxY))
		return;
	nHanziCount= GBStrlen(lpszLabel);

	// 为避免运算出错，长度不都的加空格，至少保证2个长度
	if(nHanziCount==1)
	{
		strcat(lpszLabel,(" "));
		nHanziCount = GBStrlen(lpszLabel);
	}

	if (nHanziCount==0) return;
    GUI_SetFont(gGisFont);
	GUI_SetColor(GUI_BLACK);
	GUI_SetTextMode(GUI_TM_TRANS);
	// 做防止标注覆盖的运算
    GisGetTextExtentPoint(lpszLabel,&cx,&cy);
    GisGetTextExtentPoint("汉",&cx1,&cy1);
    
	//if(!lpGpline->lablecoordarray.haHandle)
	//  GUI_ARRAY_CREATE(&lpGpline->lablecoordarray);
    if(lpGpline->pointarray.haHandle)
      nPointCount=GUI_ARRAY_GetNumItems(&lpGpline->pointarray);
	else
      return;
#if USEDYNAALLOC
	phmem=GUI_ALLOC_AllocZero(sizeof(GeoPoint)*(nPointCount+1));
	geoPoint=(GeoPoint*)GUI_ALLOC_h2p(phmem);
	if(geoPoint==NULL) return;

	phmempts=GUI_ALLOC_AllocZero(sizeof(GUI_POINT)*(nPointCount+1));
	pts=(GUI_POINT*)GUI_ALLOC_h2p(phmempts);
	if(pts==NULL) goto FreeDraw;
#endif
	for(j=0;j<nHanziCount;j++)
	{
		Pline_getCoordFromLine(&lpGpline->pointarray,nPointCount,nHanziCount+1,j+1,&tmpx,&tmpy,&tmpangle);
		geoPoint[j].lon = tmpx;
		geoPoint[j].lat = tmpy;
       // WM_LOCK();
	//	GUI_ARRAY_AddItem(&lpGpline->lablecoordarray, &geoPoint, sizeof(GeoPoint));
    //    WM_UNLOCK();
	}
	
	for(i=0;i<nHanziCount;i++)
	{
       // ppoint=(GeoPoint*)GUI_ARRAY_GetpItem(&lpGpline->lablecoordarray,i);
		DPtoVP(lpGpline->baselabel.pCoordMap,geoPoint[i].lon,geoPoint[i].lat,&x,&y);
		pts[i].x = x;
		pts[i].y = y;
	}

	//ppoint0=(GeoPoint*)GUI_ARRAY_GetpItem(&lpGpline->lablecoordarray,0);
	//ppointend=(GeoPoint*)GUI_ARRAY_GetpItem(&lpGpline->lablecoordarray,nHanziCount-1);
	for(i=0;i<(nHanziCount-1);i++)
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
			if(dw<(GISLONG)abs(x2-x1))	dw	= abs(x2-x1);
			if(dh<(GISLONG)abs(y2-y1))	dh	= abs(y2-y1);
		}	
	}
	if(dw>dh)
	{
		dwh	= dw;
		
		if(geoPoint[0].lon > geoPoint[nHanziCount-1].lon)
			bNeedChangePos = TRUE;
	}
	else		
	{
		dwh	= dh;
		if(geoPoint[0].lat < geoPoint[nHanziCount-1].lat)
			bNeedChangePos = TRUE;
	}
	if(dwh > cy)
	{
		//转换,使之位置合适
		if(bNeedChangePos)
		{
			
			for(j=0;j<nHanziCount/2;j++)
			{
				tmp					= pts[nHanziCount-1-j].x;
				pts[nHanziCount-1-j].x	= pts[j].x;
				pts[j].x				= tmp;
				
				tmp					= pts[nHanziCount-1-j].y;
				pts[nHanziCount-1-j].y	= pts[j].y;
				pts[j].y				= tmp;
				
			}
		}
		for(j=0;j<nHanziCount;j++)
		{
 		   rect2.x0=rect1.left = pts[j].x;
		   rect2.y0=rect1.top = pts[j].y;
		   rect2.x1=rect1.right = pts[j].x+cx1;
		   rect2.y1=rect1.bottom = pts[j].y+cy1;
		  if(!IsLabelRectUsed(lpGpline->baselabel.pCoordMap,rect1))
 		  {
              lpszLabel=GISDispCharAt(lpszLabel,pts[j].x,pts[j].y);
			 // GUI_DispCharAt(lpszLabel+j,pts[j].x,pts[j].y);
			  //GUI_DispStringAt(lpszLabel+j,&rect2,GUI_TA_CENTER);
		  }
		}
	}
	else
	{
		x = pts[nHanziCount/2].x;
		y = pts[nHanziCount/2].y;
		
		x -= cx/2;

		rect.left = x;
		rect.top = y;
		rect.right = x+cx;
		rect.bottom = y+cy;
		// 如果线段长度不能显示文字，则不显示
		DPtoVP(lpGpline->baselabel.pCoordMap,lpGpline->baselabel.m_Rect.fMinX,lpGpline->baselabel.m_Rect.fMinY,&xmin,&ymin);
		DPtoVP(lpGpline->baselabel.pCoordMap,lpGpline->baselabel.m_Rect.fMaxX,lpGpline->baselabel.m_Rect.fMaxY,&xmax,&ymax);
		nlinelen = (INT32U)sqrt((FP32)((xmax-xmin)*(xmax-xmin)+(ymax-ymin)*(ymax-ymin)));
		labelLen = (INT32U)sqrt((FP32)(cx*cx+cy*cy));
		
		if(((nlinelen-labelLen)>-20)&&!IsLabelRectUsed(lpGpline->baselabel.pCoordMap,rect)) // 如果标注区域没有被绘制
		{
			//GUI_DispCharAt(lpszLabel+j,x,y);
			GUI_DispStringAt(lpszLabel,x,y);
		    //GUI_SetFont(&GUI_FontHZ_SimSun_13);
		    //GUI_DispStringAt ("目标:", x, y);

		}
	}
#if USEDYNAALLOC
FreeDraw:
	GUI_ALLOC_Free(phmem);
	GUI_ALLOC_Free(phmempts);
	phmem=0;
	phmempts=0;
	pts = NULL;
    geoPoint=NULL;
#endif
}
//函数:CPline::GetRect作用:计算并返回一条连续直线的边界矩形
//连续直线的主控参数在pline1中，坐标在Pointxy中
void Pline_GetRect(GPLine *lpGpline,FP32 *minX,FP32 *minY,FP32 *maxX,FP32 *maxY)
{
	INT32U i,nPointCount;
	FP32 x1,y1,x2,y2;
	GeoPoint * ppoint;
	//用第一个顶点的坐标初试化变量
    nPointCount=GUI_ARRAY_GetNumItems(&lpGpline->pointarray);
	ppoint=(GeoPoint*)GUI_ARRAY_GetpItem(&lpGpline->pointarray,0);
	
	x1=ppoint->lon;x2=ppoint->lon;
	y1=ppoint->lat;y2=ppoint->lat;
	for( i=1;i<nPointCount;i++)          //从第二个顶点开始循环
	{
	    ppoint=(GeoPoint*)GUI_ARRAY_GetpItem(&lpGpline->pointarray,i);
		x1=GISMIN(x1,ppoint->lon);    //得到最小x坐标
		x2=GISMAX(x2,ppoint->lon);    //得到最大x坐标
		y1=GISMIN(y1,ppoint->lat);    //得到最小y坐标
		y2=GISMAX(y2,ppoint->lat);    //得到最大y坐标
	}
	*minX=x1;*maxX=x2;*minY=y1;*maxY=y2; //将值赋给地址返回
}
// 点列表 点数量 汉字个数 ,第几个汉字，
void Pline_getCoordFromLine(GUI_ARRAY *ptsdata, INT32U num, INT32S sep, INT32S ser, FP64 *cx, FP64 *cy,FP64 *angle)
{
	FP64	dist;
	FP64	subdist;
	INT32U		i;
    GeoPoint *pgeoPoint,*pgeoPointNext;
	dist	= 0.0;
	for(i=0;i<num-1;i++)
	{
		pgeoPoint=GUI_ARRAY_GetpItem(ptsdata,i);
        pgeoPointNext=GUI_ARRAY_GetpItem(ptsdata,i+1);
		dist	= dist + sqrt((pgeoPoint->lon-pgeoPointNext->lon)*(pgeoPoint->lon-pgeoPointNext->lon) + (pgeoPoint->lat-pgeoPointNext->lat)*(pgeoPoint->lat-pgeoPointNext->lat));
	}

	subdist	= dist/sep * ser;
	
	for(i=0;i<num-1;i++)
	{
		pgeoPoint=GUI_ARRAY_GetpItem(ptsdata,i);
        pgeoPointNext=GUI_ARRAY_GetpItem(ptsdata,i+1);

		dist	= sqrt((pgeoPoint->lon-pgeoPointNext->lon)*(pgeoPoint->lon-pgeoPointNext->lon) + (pgeoPoint->lat-pgeoPointNext->lat)*(pgeoPoint->lat-pgeoPointNext->lat));

		if(dist>subdist)
		{
			*angle	= Pline_getAngle(pgeoPointNext->lon-pgeoPoint->lon,pgeoPointNext->lat-pgeoPoint->lat);
			
			*cx	= pgeoPoint->lon + subdist*cos(*angle);
			*cy	= pgeoPoint->lat + subdist*sin(*angle);

			break;
		}
		else
		{
			subdist	= subdist - dist;
		}
	}
}
//#define PI 3.14159265
//获取角度
FP64 Pline_getAngle(FP64 dx, FP64 dy)
{
	FP64	angle;

	if(fabs(dy) < 1e-10)
	{
		if(dx > 0.0)					angle	= 0.0;
		else							angle	= GEO_PI;
	}
	else	if(dy > 1e-10)
	{
		if(dx > 1e-10)					angle	= atan2(fabs(dy),fabs(dx));		
		else	if(fabs(dx) < 1e-10)	angle	= GEO_PI/2;
		else	if(dx < 1e-10)			angle	= GEO_PI-atan2(fabs(dy),fabs(dx));
	}
	else
	{
		if(dx > 1e-10)					angle	= GEO_PI*2-atan2(fabs(dy),fabs(dx));
		else	if(fabs(dx) < 1e-10)	angle	= GEO_PI*3/2;
		else	if(dx < 1e-10)			angle	= GEO_PI+atan2(fabs(dy),fabs(dx));
	}
	return	angle;
}
void GPLine_Free(GPLine* pgPline)
{
    Label_Free(&pgPline->baselabel);
    if(pgPline->pointarray.haHandle)
	 GUI_ARRAY_Delete(&pgPline->pointarray);
    if(pgPline->lablecoordarray.haHandle)
	 GUI_ARRAY_Delete(&pgPline->lablecoordarray);
	memset(pgPline,0,sizeof(GPLine));
	//pgPline->baselabel
}
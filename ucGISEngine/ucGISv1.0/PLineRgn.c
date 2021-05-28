#define PLINERGN_GLOBALES
#include "CoorTrans.h"
#include "Pline.h"
#include "PlineRgn.h"

void PlineRgn_ReadFromFile(FILE	*fp,GRegion *lpgRegion)
{
	Pline_ReadFromFile(fp,&lpgRegion->gpline);
	fread(&lpgRegion->m_bFill,sizeof(lpgRegion->m_bFill),1,fp);
	fread(&lpgRegion->m_bTransparent,sizeof(lpgRegion->m_bTransparent),1,fp);
	fread(&lpgRegion->regionbrush,sizeof(lpgRegion->regionbrush),1,fp);
	fread(&lpgRegion->centerPoint.lon,sizeof(lpgRegion->centerPoint.lon),1,fp);
	fread(&lpgRegion->centerPoint.lat,sizeof(lpgRegion->centerPoint.lat),1,fp);
}
void PlineRgn_WriteToFile(FILE *fp,GRegion *lpgRegion)
{
	/*
	CPline::WriteToFile(fp);
	fwrite(&m_bFill,sizeof(m_bFill),1,fp);
	fwrite(&m_bTransparent,sizeof(m_bTransparent),1,fp);
	fwrite(&m_Brush,sizeof(m_Brush),1,fp);
	fwrite(&m_fCenterX,sizeof(m_fCenterX),1,fp);
	fwrite(&m_fCenterY,sizeof(m_fCenterY),1,fp);
	*/
}
void PlineRgn_Draw(GRegion *lpgRegion, int m_DrawMode)
{
/*
	GUI_DRAWMODE dm;
	if(lpgRegion->m_bTransparent)
	  dm=GUI_DRAWMODE_TRANS;
	else
      dm=m_DrawMode;
*/
	  //INT16U pattern,width;
	//GISLONG color;
	GBrush brush;
	GPen pen;
	GeoPoint* ppoint;
    GUI_POINT pGuiPoint[10000]; 
//	GUI_HMEM  phmem;
	INT32U X1,Y1,i,nDrawPointCount,nPointCount=0;
	nDrawPointCount = 0;
	if(!IsRectCross(lpgRegion->gpline.baselabel.pCoordMap,lpgRegion->gpline.baselabel.m_Rect.fMinX,lpgRegion->gpline.baselabel.m_Rect.fMinY,lpgRegion->gpline.baselabel.m_Rect.fMaxX,lpgRegion->gpline.baselabel.m_Rect.fMaxY))
		return;
    pen.patter=lpgRegion->gpline.plinepen.patter;
	pen.width=lpgRegion->gpline.plinepen.width;
	pen.color=lpgRegion->gpline.plinepen.color;
	brush.backcolor=lpgRegion->regionbrush.backcolor;
	brush.forcolor=lpgRegion->regionbrush.forcolor;
	brush.patter=lpgRegion->regionbrush.patter;
	GUI_SetPenSize(pen.width);
    GUI_SetPenShape(pen.patter);
    GUI_SetColor(pen.color);
    if(lpgRegion->gpline.pointarray.haHandle)
      nPointCount=GUI_ARRAY_GetNumItems(&lpgRegion->gpline.pointarray);
	else
      return;
  //  phmem=GUI_ALLOC_AllocZero(sizeof(GUI_POINT)*(nPointCount+1));
//	pGuiPoint=(GUI_POINT*)phmem;
//	if(pGuiPoint==NULL) return;
	for(i=0;i<nPointCount;i++)
	{
        ppoint=(GeoPoint*)GUI_ARRAY_GetpItem(&lpgRegion->gpline.pointarray,i);
		DPtoVP(lpgRegion->gpline.baselabel.pCoordMap,ppoint->lon,ppoint->lat,&X1,&Y1);
		if((i==0)||(!
			((ppoint->lon==X1)&&
			(ppoint->lat==Y1))
			))
		{
			pGuiPoint[i].x = X1;
			pGuiPoint[i].y = Y1;
			nDrawPointCount++;
		}
	}
	
	//GUI_SetDrawMode(nDrawMode);
	if(lpgRegion->m_bFill)
	{
      GUI_SetColor(brush.forcolor);
      GUI_SetBkColor(brush.backcolor);
	  GUI_SetPenShape(brush.patter);
      GUI_FillPolygon(pGuiPoint,nDrawPointCount,0,0);
	}
	else
       GUI_DrawPolygon(pGuiPoint,nDrawPointCount,0,0);

	//Pline_Draw(&lpgRegion->gpline,dm,lpgRegion->m_bFill,lpgRegion->regionbrush);
}

void PlineRgn_DrawLabel(GRegion *lpgRegion)
{
	INT32S x,y,cx,cy,nHanziCount;
    GISCHAR *lpszLabel;
	GIS_RECT rect;
	if(!lpgRegion->gpline.baselabel.text) return;
	WM_LOCK();
	  lpszLabel=(GISCHAR*) GUI_ALLOC_h2p(lpgRegion->gpline.baselabel.text);
	WM_UNLOCK();
	nHanziCount= GBStrlen(lpszLabel);
	if(!strlen(lpszLabel)) return;

	if(!IsRectCross(lpgRegion->gpline.baselabel.pCoordMap,lpgRegion->gpline.baselabel.m_Rect.fMinX,lpgRegion->gpline.baselabel.m_Rect.fMinY,lpgRegion->gpline.baselabel.m_Rect.fMaxX,lpgRegion->gpline.baselabel.m_Rect.fMaxY))
		return;

	DPtoVP(lpgRegion->gpline.baselabel.pCoordMap,lpgRegion->centerPoint.lon,lpgRegion->centerPoint.lat,&x,&y);

	// 做防止标注覆盖的运算
	
    GUI_SetFont(gGisFont);
	GisGetTextExtentPoint(lpszLabel,&cx,&cy);

	rect.left= x;
	rect.top= y;
	rect.right= x+cx;
	rect.bottom= y+cy;


	if(!IsLabelRectUsed(lpgRegion->gpline.baselabel.pCoordMap,rect)) // 如果标注区域没有被绘制
	{
		GUI_SetColor(GUI_BLACK);
		GUI_SetTextMode(GUI_TM_TRANS);

	    //GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt (lpszLabel, x,y);

		//GUI_DispStringAt ("目标:", 5, 23);
		//GUI_SetPenShape(lpgRegion->regionbrush.patter);
		//POINT ptText = { x, y };
		//m_ceDraw->DrawText( &ptText,m_lpszLabel,lstrlen(m_lpszLabel), 1 ); // Draw text with pen color
	}
}
void PlineRgn_GetCenter(GRegion *lpgRegion)
{
	FP32 x1,y1,x2,y2;
	Pline_GetRect(&lpgRegion->gpline,&x1,&y1,&x2,&y2);
	lpgRegion->centerPoint.lat= x1+(x2-x1)/2;
	lpgRegion->centerPoint.lon = y1+(y2-y1)/2;
}

void GRegion_Free(GRegion* pgRegion)
{
	GPLine_Free(&pgRegion->gpline);
	memset(pgRegion,0,sizeof(GRegion));
}

/*
// 某点在我上面吗
BOOLEAN PlineRgn_PtInMe(GRegion* pgRegion,const GeoPoint* pt)
{
	mapRECT* rect = NULL;
	return G_PtInPolygon(m_PointList,m_nPointCount,*pt,rect);
}
*/
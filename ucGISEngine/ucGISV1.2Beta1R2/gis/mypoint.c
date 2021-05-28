#define MYPOINT_GLOBALES
#include <windows.h>
#include "mypoint.h"
extern TCHAR g_szAppPath[MAX_PATH];

typedef struct{
 ObjectTypeConstants otCode;
 GUI_BITMAP   *pbmphandle;
}MyICONIndex;
extern  GUI_BITMAP bm1[];
extern  GUI_BITMAP bm1000[];
extern  GUI_BITMAP bm1100[];
extern  GUI_BITMAP bm1200[];
extern  GUI_BITMAP bm1300[];
extern  GUI_BITMAP bm1400[];
extern  GUI_BITMAP bm1401[];
extern  GUI_BITMAP bm1500[];
extern  GUI_BITMAP bm1600[];
extern  GUI_BITMAP bm1700[];
extern  GUI_BITMAP bm1800[];
extern  GUI_BITMAP bm1900[];
extern  GUI_BITMAP bm2000[];
extern  GUI_BITMAP bm2201[];
extern  GUI_BITMAP bm2202[];
extern  GUI_BITMAP bm2300[];
//extern  GUI_BITMAP bm2400[];
extern  GUI_BITMAP bm2401[];
extern  GUI_BITMAP bm2402[];
extern  GUI_BITMAP bm2403[];
extern  GUI_BITMAP bm2404[];
extern  GUI_BITMAP bm2405[];
extern  GUI_BITMAP bm2406[];
extern  GUI_BITMAP bm2700[];

static MyICONIndex myICONIdex[]={
	{1,bm1},
	{1000,bm1000},
	{1100,bm1100},
	{1200,bm1200},
	{1300,bm1300},
	{1400,bm1400},
	{1401,bm1401},
	{1500,bm1500},
	{1600,bm1600},
	{1700,bm1700},
	{1800,bm1800},
	{1900,bm1900},
	{2000,bm2000},
	{2201,bm2201},
	{2202,bm2202},
	{2300,bm2300},
	{2401,bm2401},
	{2402,bm2402},
	{2403,bm2403},
	{2404,bm2404},
	{2405,bm2405},
	{2406,bm2406},
	{2700,bm2700},
	{0,NULL}
};
GUI_BITMAP *MyPoint_GetMyGisICON(ObjectTypeConstants otCode)
{
    MyICONIndex *pmyiconidex;
	pmyiconidex=&myICONIdex[0];
    do{
      if(pmyiconidex->otCode==otCode)
		  return pmyiconidex->pbmphandle;
	}while((pmyiconidex++)->pbmphandle!=NULL);
	return NULL;
}

void MyPoint_Draw(MyPoint *pMypoint,INT32U nDrawMode)
{
	int x,y;
	GUI_BITMAP  * pbmp=NULL;

	if(!IsRectCross(pMypoint->baselabel.pCoordMap,pMypoint->baselabel.m_Rect.fMinX,pMypoint->baselabel.m_Rect.fMinY,pMypoint->baselabel.m_Rect.fMaxX,pMypoint->baselabel.m_Rect.fMaxY))
		return;

	DPtoVP(pMypoint->baselabel.pCoordMap,pMypoint->m_X,pMypoint->m_Y,&x,&y);


	pbmp = MyPoint_GetMyGisICON(pMypoint->baselabel.m_nType);
	if(NULL==pbmp)
		pbmp = MyPoint_GetMyGisICON(pMypoint->baselabel.m_nSubType);
	if(NULL==pbmp)
		pbmp = myICONIdex[0].pbmphandle;

	if(NULL!=pbmp)
	{
		//GUI_SetDrawMode(nDrawMode);
		GUI_SetAlpha(0x80);
		GUI_DrawBitmap(pbmp,x,y);
        GUI_SetAlpha(0);
		VPtoDP(pMypoint->baselabel.pCoordMap,x-pbmp->XSize/2,y-pbmp->YSize/2,&pMypoint->baselabel.m_Rect.fMinX,&pMypoint->baselabel.m_Rect.fMinY);
		VPtoDP(pMypoint->baselabel.pCoordMap,x+pbmp->XSize/2,y+pbmp->YSize/2,&pMypoint->baselabel.m_Rect.fMaxX,&pMypoint->baselabel.m_Rect.fMaxY);
	}

}
void MyPoint_DrawLabel(MyPoint *pMypoint)
{
	GISCHAR *lpszLabel;
	INT32S x,y,cx,cy;
	GIS_RECT rect;
	if(!pMypoint->baselabel.text) return;
	WM_LOCK();
	lpszLabel=(GISCHAR*) GUI_ALLOC_h2p(pMypoint->baselabel.text);
    WM_UNLOCK();
	if(!lstrlen(lpszLabel)) return;

	if(!IsRectCross(pMypoint->baselabel.pCoordMap,pMypoint->baselabel.m_Rect.fMinX,pMypoint->baselabel.m_Rect.fMinY,pMypoint->baselabel.m_Rect.fMaxX,pMypoint->baselabel.m_Rect.fMaxY))
		return;

	DPtoVP(pMypoint->baselabel.pCoordMap,pMypoint->m_X,pMypoint->m_Y,&x,&y);
	// 做防止标注覆盖的运算
	//GetTextExtentPoint32(hDC,m_lpszLabel,lstrlen(m_lpszLabel),&sz);
    GUI_SetFont(gGisFont);
	GUI_SetColor(GUI_BLACK);
    //GUI_SetPenSize(lpGpline->plinepen.width);
    //GUI_SetPenShape(lpGpline->plinepen.patter);
	//GUI_SetTextMode(GUI_TEXTMODE_TRANS);
	GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(gGisFont);
    GisGetTextExtentPoint(lpszLabel,&cx,&cy);

	rect.left = x;
	rect.top = y;
	rect.right = x+cx;
	rect.bottom = y+cy;

	if(!IsLabelRectUsed(pMypoint->baselabel.pCoordMap,rect)) // 如果标注区域没有被绘制
	{
	//POINT ptText = { x+15, y-sz.cy/2-2 };
	    GUI_DispStringAt(lpszLabel,x+15, y-cy/2-2 );
	    //GUI_SetFont(&GUI_FontHZ_SimSun_13);
		//    GUI_DispStringAt ("目标:", x+15, y-cy/2-2);

		//m_ceDraw->DrawText( &ptText, (LPCTSTR)m_lpszLabel,lstrlen(m_lpszLabel), 1 ); // Draw text with pen color

	}
}
void MyPoint_ReadFromFile(FILE *fp,MyPoint *pMypoint)
{
	Lable_ReadFromFile(fp,&pMypoint->baselabel);
	fread(&pMypoint->m_X,1,sizeof(pMypoint->m_X),fp);
	fread(&pMypoint->m_Y,1,sizeof(pMypoint->m_Y),fp);
//	fread(&m_Symbol,sizeof(m_Symbol),1,fp);
}
void MyPoint_WriteToFile(FILE *fp,MyPoint *pMypoint)
{
	/*
	CDraw::WriteToFile(fp);
	fwrite(&m_X,1,sizeof(m_X),fp);
	fwrite(&m_Y,1,sizeof(m_Y),fp);
	*/
	  //fwrite(&m_Symbol,sizeof(m_Symbol),1,fp);
}

MYPOINT_EXT void GetRect(MyPoint *pMypoint,FP32* minX, FP32* minY, FP32* maxX, FP32* maxY)
{
		*minX = *maxX = pMypoint->m_X;
		*minY = *maxY = pMypoint->m_Y;
}
MYPOINT_EXT FP32 getx(MyPoint *pMypoint)
	{
		return pMypoint->m_X;
	}
MYPOINT_EXT FP32 gety(MyPoint *pMypoint)
	{
		return pMypoint->m_Y;
	}

MYPOINT_EXT void MyPoint_Free(MyPoint *pMypoint)
{
	
    Label_Free(&pMypoint->baselabel);
    memset(pMypoint,0,sizeof(MyPoint));
}

MYPOINT_EXT void MyPointSetValue(MyPoint *pMypointDes,MyPoint *pMypointSrc)
{
	GUI_MEMCPY(pMypointDes,pMypointSrc,sizeof(MyPoint));
}
MYPOINT_EXT void MyPoint_GetRect(MyPoint *pMypoint,float* minX, float* minY, float* maxX, float* maxY)
{
	*minX = *maxX = pMypoint->m_X;
	*minY = *maxY = pMypoint->m_Y;
}

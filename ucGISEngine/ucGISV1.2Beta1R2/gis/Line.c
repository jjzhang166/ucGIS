#define LINE_GLOBALES
#include "Line.h"
void GLine_GetRect(GLine *pGline,FP32* minX, FP32* minY, FP32* maxX, FP32* maxY)
{
		*minX=min(pGline->m_X1,pGline->m_X2);  //计算并返回最小x值
		*maxX=max(pGline->m_X1,pGline->m_X2);  //计算并返回最大x值
		*minY=min(pGline->m_Y1,pGline->m_Y2);  //计算并返回最小y值
		*maxY=max(pGline->m_Y1,pGline->m_Y2);  //计算并返回最大y值
}
LINE_EXT void GLine_ReadFromFile(FILE *fp,GLine *pGLine)
{
       Lable_ReadFromFile(fp,&pGLine->baselabel);
	   fread(&pGLine->m_X1,1,sizeof(pGLine->m_X1),fp);
	   fread(&pGLine->m_X2,1,sizeof(pGLine->m_X2),fp);
	   fread(&pGLine->m_Y1,1,sizeof(pGLine->m_Y1),fp);
	   fread(&pGLine->m_Y2,1,sizeof(pGLine->m_Y2),fp);
   	   fread(&pGLine->linepen,1,sizeof(pGLine->linepen),fp);
   	   fread(&pGLine->m_fLong,1,sizeof(pGLine->m_fLong),fp);
}
LINE_EXT void GLine_Draw(GLine *pGLine,INT32U nDrawMode)
{
	GPen pen;
	INT32S x,y;
	if(!IsRectCross(pGLine->baselabel.pCoordMap,pGLine->baselabel.m_Rect.fMinX,pGLine->baselabel.m_Rect.fMinY,pGLine->baselabel.m_Rect.fMaxX,pGLine->baselabel.m_Rect.fMaxY))
		return;
    pen.patter=pGLine->linepen.patter;
	pen.width=pGLine->linepen.width;
	pen.color=pGLine->linepen.color;
	GUI_SetPenSize(pen.width);
    GUI_SetPenShape(pen.patter);
    GUI_SetColor(pen.color);
	GUI_SetDrawMode(nDrawMode);
	DPtoVP(pGLine->baselabel.pCoordMap,pGLine->m_X1,pGLine->m_Y1,&x,&y);
    GUI_MoveTo(x,y);
	DPtoVP(pGLine->baselabel.pCoordMap,pGLine->m_X2,pGLine->m_Y2,&x,&y);
    GUI_DrawLineTo(x,y); 
}
LINE_EXT void GLine_DrawLabel(GLine *pGLine)
{

}
LINE_EXT void GLine_Free(GLine *pGLine)
{
    Label_Free(&pGLine->baselabel);
    memset(pGLine,0,sizeof(GLine));
}

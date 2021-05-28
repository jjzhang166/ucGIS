#ifndef _LINE_H_INCLUDED
#define _LINE_H_INCLUDED
#ifdef LINE_GLOBALES
#define LINE_EXT 

#else 
#define LINE_EXT extern
#endif
#include "CoorTrans.h"
//������Ŀ��ṹ //line
typedef struct 
{
	FP32 m_X1, m_X2, m_Y1, m_Y2; //ֱ�ߵ������յ�
	FP32 m_fLong;                // ֱ�ߵĳ���
    GPen  linepen;
	GBaseLabel baselabel;
}GLine;
LINE_EXT void GLine_GetRect(GLine *pGline,FP32* minX, FP32* minY, FP32* maxX, FP32* maxY);
LINE_EXT void GLine_ReadFromFile(FILE *fp,GLine *pGLine);
LINE_EXT void GLine_Draw(GLine *pGLine,INT32U nDrawMode);
LINE_EXT void GLine_DrawLabel(GLine *pGLine);
LINE_EXT void GLine_Free(GLine *pGLine);
#endif
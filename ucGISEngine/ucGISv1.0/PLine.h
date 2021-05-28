#ifndef _PLINE_H_INCLUDED
#define _PLINE_H_INCLUDED
#ifdef PLINE_GLOBALES
#define PLINE_EXT 

#else 
#define PLINE_EXT extern
#endif

//#include "gisbase.h"
#include "CoorTrans.h"
//������Ŀ��ṹ //polyline
typedef struct
{
//	char name[16];				//����
	INT32U index;					//
	GUI_ARRAY pointarray;       //�洢�������������ָ��
	GUI_ARRAY lablecoordarray;
	GBaseLabel baselabel;		//�ߵĻ�����ע��Ϣ
	GPen plinepen;
	//MapCoorTrans *pcoordtrans;
}GPLine;

PLINE_EXT BOOLEAN Pline_ReadFromFile(FILE *fp,GPLine *lpGpline);
PLINE_EXT void Pline_Draw(GPLine *lpGpline, INT32U nDrawMode,BOOLEAN bFilled,GBrush brush);
PLINE_EXT void Pline_DrawLabel(GPLine *lpGpline);
PLINE_EXT void Pline_GetRect(GPLine *lpGpline,FP32 *minX,FP32 *minY,FP32 *maxX,FP32 *maxY);
PLINE_EXT void  Pline_getCoordFromLine(GUI_ARRAY *ptsdata, INT32U num, INT32S sep, INT32S ser, FP64 *cx, FP64 *cy,FP64 *angle);
PLINE_EXT FP64 Pline_getAngle(FP64 dx, FP64 dy);
PLINE_EXT  void GPLine_Free(GPLine* pgPline);
#endif
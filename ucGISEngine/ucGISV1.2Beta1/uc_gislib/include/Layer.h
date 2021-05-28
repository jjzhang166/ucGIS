#ifndef _LAYER_H_INCLUDE
#define _LAYER_H_INCLUDE
#ifdef  LAYER_GLOBALES
#define LAYER_EXT 
#else 
#define LAYER_EXT extern
#endif
#include "MyPoint.h"
#include "PLineRgn.h"
#include "Line.h"
#include "PLine.h"
//��ͼͼ��ṹ��
typedef struct 
{
    // �Ƿ������Զ���ע
    BOOLEAN m_bAutoLabel;
    // �Ƿ�����༭�ò�
    BOOLEAN m_bEditable;
	// �Ƿ���ʾ��ͼ��
    BOOLEAN m_bVisible;
	// ͼ������
	GISCHAR m_pszName[100];
	// ��Ұ��Χ
	FP32 m_nEyeShotMin;
	FP32 m_nEyeShotMax;
	MapCoorTrans* pCoordMap; 

	GUI_ARRAY     mypointdata;
    GUI_ARRAY     linedata;
    GUI_ARRAY     plinedata;
    GUI_ARRAY     regiondata;
    GUI_ARRAY     speedpointdata;
}GMapLayer;

LAYER_EXT BOOLEAN Layer_GetRect(GMapLayer *pLayer,FP32 * m_Xmin, FP32 * m_Ymin, FP32 * m_Xmax, FP32 * m_Ymax);
LAYER_EXT void Layer_DrawLabel(GMapLayer *pLayer);
LAYER_EXT void Layer_Draw(GMapLayer *pLayer, INT32U nDrawMode);
LAYER_EXT void Layer_ReadFromFile(FILE *fp,GMapLayer *pLayer);
LAYER_EXT void Layer_AddMyPoint(GMapLayer *pLayer,MyPoint *pMypoint);
LAYER_EXT void Layer_AddPlineRgn(GMapLayer *pLayer,GRegion *pPlinergn);
LAYER_EXT void Layer_AddLine(GMapLayer *pLayer,GLine* pGLine);
LAYER_EXT void Layer_AddPline(GMapLayer *pLayer,GPLine* pGPline);
LAYER_EXT BOOLEAN Layer_SearchMap(GMapLayer *pLayer,GISCHAR* szKey, MyPoint *ppoints, int  *nCount);
LAYER_EXT void Layer_Free(GMapLayer *player);
#endif
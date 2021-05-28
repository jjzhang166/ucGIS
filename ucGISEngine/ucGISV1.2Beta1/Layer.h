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
#include "gridindex.h"
//地图图层结构体
typedef struct 
{
	INT32U  iLayer;
    // 是否允许自动标注
    BOOLEAN m_bAutoLabel;
    // 是否允许编辑该层
    BOOLEAN m_bEditable;
	// 是否显示该图层
    BOOLEAN m_bVisible;
	// 图层名字
	GISCHAR m_pszName[100];
	// 视野范围
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
LAYER_EXT void Layer_ReadFileWithGridIndex(FILE *fp,GridIndex* pgrid,GUI_ARRAY *pLayerArray,INT32U nEyeShot);  
LAYER_EXT void Layer_AddMyPoint(GMapLayer *pLayer,MyPoint *pMypoint);
LAYER_EXT void Layer_AddPlineRgn(GMapLayer *pLayer,GRegion *pPlinergn);
LAYER_EXT void Layer_AddLine(GMapLayer *pLayer,GLine* pGLine);
LAYER_EXT void Layer_AddPline(GMapLayer *pLayer,GPLine* pGPline);
LAYER_EXT BOOLEAN Layer_SearchMap(GMapLayer *pLayer,GISCHAR* szKey, MyPoint *ppoints, int  *nCount);
LAYER_EXT void Layer_Free(GMapLayer *player);
LAYER_EXT void Layer_FreeGridMap(GridIndex* pgrid,GUI_ARRAY *pLayerArray);
LAYER_EXT void  MYPOINT_FreeIndex(GMapLayer *pLayer,int objectindex);
LAYER_EXT void  LINE_FreeIndex(GMapLayer *pLayer,int objectindex);
LAYER_EXT void  RGN_FreeIndex(GMapLayer *pLayer,int objectindex);
LAYER_EXT void  PLINE_FreeIndex(GMapLayer *pLayer,int objectindex);
LAYER_EXT void RGN_AddIndex(FILE *fp,GMapLayer *pLayer,int objectindex);
LAYER_EXT void LINE_AddIndex(FILE *fp,GMapLayer *pLayer,int objectindex);
LAYER_EXT void PLINE_AddIndex(FILE *fp,GMapLayer *pLayer,int objectindex);
LAYER_EXT void MYPOINT_AddIndex(FILE *fp,GMapLayer *pLayer,int objectindex);
LAYER_EXT void Layer_ReadFileWithAreaIndex(FILE *fp,AreaIndex* pMyAreaIndex,GRegion *prgn);  


#endif
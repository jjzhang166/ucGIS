
#ifndef _GISAPI_H_INCLUDED
#define _GISAPI_H_INCLUDED
#ifdef GISAPI_GLOBALES
#define GISAPI_EXT 

#else 
#define GISAPI_EXT extern
#endif
#define VERSION  1000
#include "Layer.h"
#include "userlayer.h"
#include "gridindex.h"
typedef struct{
	GUI_ARRAY layerarray;
	GUI_ARRAY gridarray;
	GUI_ARRAY currentGridsArray;
    GISCHAR map_lpszMapName[50];// 地图名称
	BOOLEAN m_bDataChanged;  // 指示地图数据是否发生改变
//	TCHAR m_pszMapFilePath[200];
	BOOLEAN m_bShowMapName;         // 显示地图名称
	FP32 map_fCenterX;  //绘制图形时的屏幕中心
	FP32 map_fCenterY;  // 绘制图形时的屏幕中心
	FP32 map_blc;      // 比例尺
	BOOLEAN m_bDrawScale;
	//Mapping Modes
	INT32U m_nMappingMode;
	INT16U m_nCurrentLayer;
	INT32U Trows;
	INT32U Tcols;
	INT32U gridNum;
	BOOLEAN bGridHave;
	MapCoorTrans* pmapCoorMap; // 坐标换算参数
	lpUserLayer puserlayer;

}UcMap;
//无索引读取地图文件
GISAPI_EXT BOOLEAN Map_ReadFromFile(UcMap *pucmap,GISCHAR *pszFileName);
GISAPI_EXT BOOLEAN Map_ReadFileIndex(UcMap *pucmap,GISCHAR *pszMapFileName,GISCHAR *pszIndexFileName);
GISAPI_EXT void Map_ReadMapWithGridIndex(UcMap *pucmap,GISCHAR *pszFileName);

GISAPI_EXT void RealeaseGridIndex(UcMap *pucmap);
GISAPI_EXT void Map_ReadGridIndex(UcMap *pucmap,GISCHAR *pszFileName);
GISAPI_EXT void Map_Draw(UcMap *pucmap, INT32U nDrawMode);
GISAPI_EXT void DrawBlc(UcMap *pucmap,BOOLEAN bFilled);
GISAPI_EXT BOOLEAN SearchMap(UcMap *pucmap,GISCHAR* szKey, MyPoint* ppoints, int * nCount);
GISAPI_EXT void Map_GetRect(UcMap *pucmap,FP32 * m_Xmin, FP32 * m_Ymin, FP32 * m_Xmax, FP32 * m_Ymax);
GISAPI_EXT void MapRelease(UcMap *pucmap);
GISAPI_EXT void MapReleaseGrid(UcMap *pucmap);


#endif

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
#define PRESIDIALFILENAME "ʡ��.inx"//ȫ��ʡһ���������
#define PRESIDIALMAPFILENAME "ʡ��.qm2"//ȫ��ʡһ���������
#define AREAINDEXFILESAMENAME "�Ĵ�" //���//ʡ����������������ļ����Ʋ���XXX���.inx��ʶ
typedef struct{
	GUI_ARRAY layerarray;//ͼ��
	GUI_ARRAY gridarray;//��������
	GUI_ARRAY indexgridarray;//��������������
	GUI_ARRAY currentGridsArray;//��ǰ�Ӵ�����������
    GUI_ARRAY areaindexArray;
    GISCHAR map_lpszMapName[50];// ��ͼ����
	BOOLEAN m_bDataChanged;  // ָʾ��ͼ�����Ƿ����ı�
//	TCHAR m_pszMapFilePath[200];
	BOOLEAN m_bShowMapName;         // ��ʾ��ͼ����
	FP32 map_fCenterX;  //����ͼ��ʱ����Ļ����
	FP32 map_fCenterY;  // ����ͼ��ʱ����Ļ����
	FP32 map_blc;      // ������
	BOOLEAN m_bDrawScale;
	//Mapping Modes
	INT32U m_nMappingMode;
	INT16U m_nCurrentLayer;
	INT32U Trows;
	INT32U Tcols;
	INT32U gridNum;
	BOOLEAN bGridHave;
	INT32U TIndexrows;
	INT32U TIndexcols;
	//INT32U IndexgridNum;
	MapCoorTrans* pmapCoorMap; // ���껻�����
	lpUserLayer puserlayer;

}UcMap;
//��������ȡ��ͼ�ļ�
GISAPI_EXT BOOLEAN Map_ReadFromFile(UcMap *pucmap,GISCHAR *pszFileName);
GISAPI_EXT BOOLEAN Map_ReadFileIndex(UcMap *pucmap,GISCHAR *pszMapFileName);
GISAPI_EXT void Map_ReadMapWithGridIndex(UcMap *pucmap,GISCHAR *pszFileName);
GISAPI_EXT BOOLEAN Map_ReadFileIndexIndex(UcMap *pucmap,GISCHAR *pszIndexIndexFileName);
GISAPI_EXT BOOLEAN Map_ReadGridIndexWithIndex(UcMap *pucmap,GISCHAR *pszFileName);

GISAPI_EXT void RealeaseGridIndexIndex(UcMap *pucmap);
GISAPI_EXT void RealeaseGridIndex(UcMap *pucmap);
GISAPI_EXT BOOLEAN Map_ReadGridIndex(UcMap *pucmap,GISCHAR *pszFileName);
GISAPI_EXT void Map_Draw(UcMap *pucmap, INT32U nDrawMode);
GISAPI_EXT void DrawBlc(UcMap *pucmap,BOOLEAN bFilled);
GISAPI_EXT BOOLEAN SearchMap(UcMap *pucmap,GISCHAR* szKey, MyPoint* ppoints, int * nCount);
GISAPI_EXT void Map_GetRect(UcMap *pucmap,FP32 * m_Xmin, FP32 * m_Ymin, FP32 * m_Xmax, FP32 * m_Ymax);
GISAPI_EXT void MapRelease(UcMap *pucmap);
GISAPI_EXT void MapReleaseGrid(UcMap *pucmap);

GISAPI_EXT BOOLEAN ReadAreaIndex(UcMap *pucmap,GISCHAR* pszFileName);
GISAPI_EXT void ReleaseAreaIndexArray(UcMap *pucmap);
GISAPI_EXT BOOLEAN AutoSwitchMap(UcMap *pucmap,GISCHAR *areaIndexpath,float x,float y);
GISAPI_EXT BOOLEAN PointInArea(UcMap *pucmap,float x,float y,AreaIndex *pMyAreaIndex,GISCHAR* pareaname);


#endif
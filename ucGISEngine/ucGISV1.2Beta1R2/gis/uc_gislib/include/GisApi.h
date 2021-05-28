#ifndef _GISAPI_H_INCLUDED
#define _GISAPI_H_INCLUDED
#ifdef GISAPI_GLOBALES
#define GISAPI_EXT 

#else 
#define GISAPI_EXT extern
#endif

#include "Layer.h"
#include "userlayer.h"
typedef struct{
	GUI_ARRAY layerarray;
	
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
	MapCoorTrans* pmapCoorMap; // ���껻�����
	lpUserLayer puserlayer;

}UcMap;
GISAPI_EXT BOOLEAN Map_ReadFromFile(UcMap *pucmap,GISCHAR *pszFileName);
GISAPI_EXT void Map_Draw(UcMap *pucmap, INT32U nDrawMode);
GISAPI_EXT void DrawBlc(UcMap *pucmap,BOOLEAN bFilled);
GISAPI_EXT BOOLEAN SearchMap(UcMap *pucmap,GISCHAR* szKey, MyPoint* ppoints, int * nCount);

#endif
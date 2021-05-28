#ifndef _PLINERGN_H_INCLUDED
#define _PLINERGN_H_INCLUDED
#ifdef PLINERGN_GLOBALES
#define PLINERGN_EXT 

#else 
#define PLINERGN_EXT extern
#endif

//#include "gisbase.h"
#include "Pline.h"
//地理区域目标结构 region
typedef struct
{
	INT32U index;					//
	GPLine gpline;       //线数组
    GBrush regionbrush;
    GeoPoint centerPoint;       //中心点坐标
	BOOLEAN m_bTransparent;          // 是否透明
	BOOLEAN m_bFill;                 // 是否需要填充

}GRegion;

PLINERGN_EXT  void PlineRgn_ReadFromFile(FILE *fp,GRegion *lpgRegion);
PLINERGN_EXT  void PlineRgn_WriteToFile(FILE *fp,GRegion *lpgRegion);
PLINERGN_EXT  void PlineRgn_Draw(GRegion *lpgRegion, int m_DrawMode);
PLINERGN_EXT  void PlineRgn_DrawLabel(GRegion *lpgRegion);
PLINERGN_EXT  void PlineRgn_GetCenter(GRegion *lpgRegion);
PLINERGN_EXT  void GRegion_Free(GRegion* pgRegion);

#endif
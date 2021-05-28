#ifndef _PLINERGN_H_INCLUDED
#define _PLINERGN_H_INCLUDED
#ifdef PLINERGN_GLOBALES
#define PLINERGN_EXT 

#else 
#define PLINERGN_EXT extern
#endif

//#include "gisbase.h"
#include "Pline.h"
//��������Ŀ��ṹ region
typedef struct
{
	INT32U index;					//
	GPLine gpline;       //������
    GBrush regionbrush;
    GeoPoint centerPoint;       //���ĵ�����
	BOOLEAN m_bTransparent;          // �Ƿ�͸��
	BOOLEAN m_bFill;                 // �Ƿ���Ҫ���

}GRegion;

PLINERGN_EXT  void PlineRgn_ReadFromFile(FILE *fp,GRegion *lpgRegion);
PLINERGN_EXT  void PlineRgn_WriteToFile(FILE *fp,GRegion *lpgRegion);
PLINERGN_EXT  void PlineRgn_Draw(GRegion *lpgRegion, int m_DrawMode);
PLINERGN_EXT  void PlineRgn_DrawLabel(GRegion *lpgRegion);
PLINERGN_EXT  void PlineRgn_GetCenter(GRegion *lpgRegion);
PLINERGN_EXT  void GRegion_Free(GRegion* pgRegion);

#endif
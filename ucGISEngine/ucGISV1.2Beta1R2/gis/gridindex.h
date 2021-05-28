#ifndef _GRIDINDEX_H_INCLUDED
#define _GRIDINDEX_H_INCLUDED
#ifdef  GRIDINDEX_GLOBALES
#define GRIDINDEX_EXT 
#else 
#define GRIDINDEX_EXT  extern
#endif
#include "CoorTrans.h"
#define INDEXFILEID      0x51525354
#define AREAINDEXFILEID  0x53545556
#define INDEXINDEXFILEID 0x52535455
#define SEARCHINDEXFILEID 0x53545556
typedef struct
{
	INT8U  type;
	INT32U offset;
    char name[60];
}MAPSearchIndex;
typedef struct tagINDEX 
{
//	int ATTKey;
	INT32S arrayIndex;
    INT32S layeroffset;
	INT32S ClassCode;
	INT32S ilayer;
	INT32S offset;
}INDEX;
typedef struct  
{
	int arrayIndex;
	int offset;//文件中偏移量
}INDEXINDEX;

typedef struct tagDrawRect 
{ 
  INT32S left;
  INT32S right;
  INT32S top;
  INT32S bottom;

}GRIDDrawRect;

typedef struct{
   mapMAPRECTSTRCT mapExtentRect;
   GUI_ARRAY   indexvector;
   INT32U indexNums;
   INT32U nGridNo;
}GridIndex;

typedef struct{
   mapMAPRECTSTRCT mapExtentRect;
   GUI_ARRAY   indexindexvector;
   INT32U nGridindexNums;
   INT32U nGridIndexNo;
}GridIndexIndex;
typedef struct{
   mapMAPRECTSTRCT mapExtentRect;
   int offset;
}AreaIndex;
GRIDINDEX_EXT void GridIndex_Realease(GridIndex *pgridIndex);
GRIDINDEX_EXT BOOLEAN GridIsInView(MapCoorTrans *pcoortrans,GridIndex* pgridIndex);
GRIDINDEX_EXT void GridIndex_ReadFromFile(FILE *fp,GridIndex *pgridIndex);
GRIDINDEX_EXT void GridIndex_ReadFromFileOffSet(FILE *fp,GridIndex *pgridIndex,int offset);
GRIDINDEX_EXT int GridIndex_GetGridCode(int Tcols,int row,int col);
GRIDINDEX_EXT void GridIndexIndex_Realease(GridIndexIndex *pgridIndexIndex);
GRIDINDEX_EXT BOOLEAN GridIndexIsInView(MapCoorTrans *pcoortrans,GridIndexIndex* pgridIndexIndex);
GRIDINDEX_EXT void GridIndexIndex_ReadFromFile(FILE *fp,GridIndexIndex *pgridIndexIndex);
GRIDINDEX_EXT void AreaIndexRead(FILE *fp,GUI_ARRAY* pareaindexarray);
GRIDINDEX_EXT BOOLEAN SwitchPointtoArea(float lon,float lat,GUI_ARRAY* pareaindexarray);


//	void Draw(HDC hDC);
#endif
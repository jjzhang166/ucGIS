#ifndef _GRIDINDEX_H_INCLUDED
#define _GRIDINDEX_H_INCLUDED
#ifdef  GRIDINDEX_GLOBALES
#define GRIDINDEX_EXT 
#else 
#define GRIDINDEX_EXT  extern
#endif
#include "CoorTrans.h"

typedef struct tagINDEX 
{
//	int ATTKey;
	INT32S arrayIndex;
    INT32S layeroffset;
	INT32S ClassCode;
	INT32S ilayer;
	INT32S offset;
}INDEX;

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

GRIDINDEX_EXT void GridIndex_Realease(GridIndex *pgridIndex);
//void  CGisGrid(GRIDDrawRect *prect,int irow,int icol);
GRIDINDEX_EXT BOOLEAN GridIsInView(MapCoorTrans *pcoortrans,GridIndex* pgridIndex);
GRIDINDEX_EXT void GridIndex_ReadFromFile(FILE *fp,GridIndex *pgridIndex);
GRIDINDEX_EXT int GridIndex_GetGridCode(int Tcols,int row,int col);
//	void Draw(HDC hDC);
#endif
#ifndef _MYPOINT_H_INCLUDED
#define _MYPOINT_H_INCLUDED
#ifdef MYPOINT_GLOBALES
#define MYPOINT_EXT 

#else 
#define MYPOINT_EXT extern
#endif
#include "GUI.h"
#include "coortrans.h"



typedef struct{
    INT32U       index;					//
	float        m_X;         // 点横坐标
	float        m_Y;         // 点纵坐标
	GBaseLabel   baselabel;
}MyPoint;
MYPOINT_EXT GUI_BITMAP *MyPoint_GetMyGisICON(ObjectTypeConstants otCode);
MYPOINT_EXT void MyPoint_Draw(MyPoint *pMypoint,INT32U nDrawMode);
MYPOINT_EXT void MyPoint_DrawLabel(MyPoint *pMypoint);
MYPOINT_EXT void MyPoint_ReadFromFile(FILE *fp,MyPoint *pMypoint);
MYPOINT_EXT void GetRect(MyPoint *pMypoint,FP32* minX, FP32* minY, FP32* maxX, FP32* maxY);
MYPOINT_EXT FP32 getx(MyPoint *pMypoint);
MYPOINT_EXT FP32 gety(MyPoint *pMypoint);
MYPOINT_EXT void MyPoint_WriteToFile(FILE *fp,MyPoint *pMypoint);
MYPOINT_EXT void MyPoint_Free(MyPoint *pMypoint);
MYPOINT_EXT void MyPointSetValue(MyPoint *pMypointDes,MyPoint *pMypointSrc);
MYPOINT_EXT void MyPoint_GetRect(MyPoint *pMypoint,float* minX, float* minY, float* maxX, float* maxY);

	//mapSYMBOLSTRUCT m_Symbol;     // 点符号类型



#endif
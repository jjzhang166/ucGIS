#pragma once
#include "Draw.h"
#include <vector>
#define INDEXFILEID      0x51525354
#define AREAINDEXFILEID  0x53545556
#define INDEXINDEXFILEID 0x52535455

//class CDraw;
using namespace std;
/////**************INDEXt****************************
typedef struct tagINDEX 
{
//	int ATTKey;
	int arrayIndex;
    int layeroffset;
	int ClassCode;
	int ilayer;
	int offset;//�ļ���ƫ����
}INDEX;
typedef struct  
{
	int arrayIndex;
	int offset;//�ļ���ƫ����
}GRIDINDEX;
typedef struct tagDrawRect 
{ int left;
  int right;
  int top;
  int bottom;

}GRIDDrawRect;
typedef vector<INDEX *> INDEXVector;
typedef vector<GRIDINDEX *> GRIDINDEXVector;
class CGridIndex
{
private:
	int m_fileoffset;
public:
    CRect ExtentRect;
    GRIDINDEXVector gridindexvector;
    mapMAPRECTSTRCT mapExtentRect;
public:
	CGridIndex(void);
	~CGridIndex(void);
    CGridIndex(GRIDDrawRect *prect,int irow,int icol,int nheight=240*4,int nwidth=320*4);
  //  bool GridIsInView(mapCOORDMAP *pDraw,CGisGrid* grid,long m_nViewWidth,long m_nVeiwHeight);
    void ReadFromFile(FILE *fp);
    void WriteToFile(FILE *fp);
    //int  GetCol(GRIDDrawRect* prect,double x);
    //int GetRow(GRIDDrawRect *prect,double y);
    //int GetGridCode(int Tcols,int row,int col);
	void Draw(HDC hDC,mapCOORDMAP* pmapCoordMap);
	int GetFileOffset(){return m_fileoffset;};

};
class CGisGrid
{
private:
	int m_fileoffset;
public:
    CRect ExtentRect;
    INDEXVector indexvector;
    mapMAPRECTSTRCT mapExtentRect;
    //mapMAPRECTSTRCT mapExtentRectIndex;
    static long GridWidth,GridHeigth;
public:
	CGisGrid(void);
	~CGisGrid(void);
    CGisGrid(CRect *prect,int irow,int icol);
    bool GridIsInView(mapCOORDMAP *pDraw,CGisGrid* grid,long m_nViewWidth,long m_nVeiwHeight);
    void ReadFromFile(FILE *fp);
    void WriteToFile(FILE *fp,int &offset);
    int  GetCol(GRIDDrawRect* prect,double x);
    int GetRow(GRIDDrawRect *prect,double y);
    int GetGridCode(int Tcols,int row,int col);
	void Draw(HDC hDC);
	int GetFileOffset(){return m_fileoffset;};
};

class CAreaIndex
{
private:
     mapMAPRECTSTRCT mapExtentRect;
     int m_offset;
	 char *pAreaName;
public:
	CAreaIndex();
	CAreaIndex(float fminx,float fminy,float fmaxx,float fmaxy,int offset);
	~CAreaIndex();
	void WriteToFile(FILE *fp);
};
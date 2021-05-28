#pragma once
#include "Draw.h"
#include <vector>
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
	int offset;//文件中偏移量
}INDEX;
typedef struct  
{
	int arrayIndex;
	int offset;//文件中偏移量
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


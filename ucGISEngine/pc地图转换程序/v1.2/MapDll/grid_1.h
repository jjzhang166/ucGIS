#ifndef __GRIG_H
#define __GRIG_H
#include "stdafx.h"
#include <vector>
typedef vector<INDEX *> INDEXVector;
class CDraw;
using namespace std;
/////**************INDEXt****************************
typedef struct tagINDEX 
{
//	int ATTKey;
	int arrayIndex;
	int ClassCode;
	int ilayer;
}INDEX;

typedef struct tagDrawRect 
{ long left;
  long right;
  long top;
  long bottom;

}DrawRect;

/////**************Grid****************************

class AFX_EXT_CLASS Grid 
{
public:
   CRect ExtentRect;
   INDEXVector indexvector;
    static long GridWidth,GridHeigth;
public:
	Grid(){};
    Grid(DrawRect prect,int irow,int icol);
    bool GridIsInView(CDraw *pDraw,Grid* grid,long m_nViewWidth,long m_nVeiwHeight);
    void ReadFromFile(FILE *fp);
    void WriteToFile(FILE *fp);
    int GetRow(DrawRect prect,double y);
    int GetGridCode(int Tcols,int row,int col);
	~Grid(){};
};

#endif
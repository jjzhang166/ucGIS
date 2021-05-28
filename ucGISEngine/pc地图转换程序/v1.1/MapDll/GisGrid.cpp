#include "stdafx.h"
#include ".\gisgrid.h"

CGisGrid::CGisGrid(void)
{
	m_fileoffset=0;
}

CGisGrid::~CGisGrid(void)
{
	INDEX* pIndex = NULL;
	int nCount = (int)indexvector.size();
	for(int i=0;i<nCount;i++)
	{
		pIndex = (INDEX*)indexvector[i];
		if(pIndex)
			delete pIndex;
		pIndex = NULL;
	}
	indexvector.clear();
}
long CGisGrid::GridWidth=640;
long CGisGrid::GridHeigth=480;
//long CGisGrid::GridWidth=960;//320;
//long CGisGrid::GridHeigth=720;//240;
//long CGisGrid::GridWidth=1024;//320;
//long CGisGrid::GridHeigth=960;//240;
int CGisGrid :: GetGridCode(int Tcols,int row,int col)
{
	int gridCode=(row-1)*Tcols+col;
	CString str;
    //str.Format("%d",N);
    //AfxMessageBox(str);
	return gridCode;


}

int CGisGrid ::GetRow(GRIDDrawRect* prect,double y)
{   
	int m;
    m=int(abs(y-(prect->top))/GridHeigth);
	return m;

}
int CGisGrid ::GetCol(GRIDDrawRect* prect,double x)
{
	int n;
	n=int((x-(prect->left))/GridWidth);
	return n;

}
CGisGrid ::CGisGrid(CRect * prect,int irow,int icol)
{
	m_fileoffset=0;
	ExtentRect.left=prect->left+icol*GridWidth;
	ExtentRect.top=prect->top+irow*GridHeigth;
	ExtentRect.bottom=prect->top+(irow+1)*GridHeigth;
	ExtentRect.right=prect->left+(icol+1)*GridWidth;

}
void CGisGrid::Draw(HDC hDC)
{
		HPEN	oldPen,newPen;
        int x1=ExtentRect.left;
        int y2=ExtentRect.bottom;
        int x2=ExtentRect.right;
        int y1=ExtentRect.top;
		newPen = CreatePen(PS_SOLID, 3,RGB(255,0,0));
		oldPen	= (HPEN)SelectObject(hDC,newPen);
        MoveToEx(hDC,x1,y1,NULL);
        LineTo(hDC,x1,y2);
        LineTo(hDC,x2,y2);
        LineTo(hDC,x2,y1);
        LineTo(hDC,x1,y1);
		SelectObject(hDC,oldPen);

}
void CGisGrid ::WriteToFile(FILE *fp,int &offset)
{
	//float fleft,ftop,fright,fbottom;
    m_fileoffset=offset;
	fwrite(&mapExtentRect.fMinX,sizeof(float),1,fp);
	offset+=sizeof(float);
	fwrite(&mapExtentRect.fMinY,sizeof(float),1,fp);
	offset+=sizeof(float);
	fwrite(&mapExtentRect.fMaxX,sizeof(float),1,fp);
	offset+=sizeof(float);
	fwrite(&mapExtentRect.fMaxY,sizeof(float),1,fp);
	offset+=sizeof(float);

	int indexNums=indexvector.size();
	fwrite(&indexNums,sizeof(int),1,fp);
	offset+=sizeof(int);
    for (int k=0;k<indexNums;k++)
	{
		INDEX* pindex;
		pindex=(INDEX*)indexvector[k];
		fwrite(&pindex->arrayIndex,sizeof(int),1,fp);
	    offset+=sizeof(int);
		fwrite(&pindex->layeroffset,sizeof(int),1,fp);
	    offset+=sizeof(int);
		fwrite(&pindex->ClassCode,sizeof(int),1,fp);
	    offset+=sizeof(int);
		fwrite(&pindex->ilayer,sizeof(int),1,fp);
	    offset+=sizeof(int);
		fwrite(&pindex->offset,sizeof(int),1,fp);
	    offset+=sizeof(int);
	}
}
void CGisGrid ::ReadFromFile(FILE *fp)
{
	fread(&mapExtentRect.fMinX,sizeof(float),1,fp);
	fread(&mapExtentRect.fMinY,sizeof(float),1,fp);
	fread(&mapExtentRect.fMaxX,sizeof(float),1,fp);
	fread(&mapExtentRect.fMaxY,sizeof(float),1,fp);

	//fread(&ExtentRect.left,sizeof(int),1,fp);
	//fread(&ExtentRect.top,sizeof(int),1,fp);
	//fread(&ExtentRect.right,sizeof(int),1,fp);
	//fread(&ExtentRect.bottom,sizeof(int),1,fp);

	int indexNums;
	fread(&indexNums,sizeof(int),1,fp);
    for (int k=0;k<indexNums;k++)
	{
		INDEX *pindex=new INDEX;;
		fread(&pindex->arrayIndex,sizeof(int),1,fp);
		fread(&pindex->layeroffset,sizeof(int),1,fp);
		fread(&pindex->ClassCode,sizeof(int),1,fp);
		fread(&pindex->ilayer,sizeof(int),1,fp);
		fread(&pindex->offset,sizeof(int),1,fp);
		indexvector.push_back(pindex);
	}
}

bool CGisGrid ::GridIsInView(mapCOORDMAP *pDraw,CGisGrid* grid,long m_nViewWidth,long m_nVeiwHeight)
{
	int left,top,right,bottom;

	pDraw->DPtoVP(grid->ExtentRect.left,grid->ExtentRect.top,&left,&top);//×ª»»ÎªÂß¼­×ø±ê
	pDraw->DPtoVP(grid->ExtentRect.right,grid->ExtentRect.bottom,&right,&bottom);

	CRect gridRect(left,top,right,bottom);

	
	long x1, y1, x2, y2;
	
	x1 = 0, y1 = 0;
	x2 = m_nViewWidth, y2 = m_nVeiwHeight;
	CRect viewRect(x1, y1, x2, y2);

	
	CRect rect;
	return rect.IntersectRect(viewRect,gridRect);
}

CGridIndex::CGridIndex(void)
{
}
CGridIndex::~CGridIndex(void)
{
	GRIDINDEX* pIndexIndex = NULL;
	int nCount = (int)gridindexvector.size();
	for(int i=0;i<nCount;i++)
	{
		pIndexIndex = (GRIDINDEX*)gridindexvector[i];
		if(pIndexIndex)
			delete pIndexIndex;
		pIndexIndex = NULL;
	}
	gridindexvector.clear();
}
CGridIndex::CGridIndex(GRIDDrawRect *prect,int irow,int icol,int nheight,int nwidth)
{
	ExtentRect.left=prect->left+icol*nwidth;
	ExtentRect.top=prect->bottom+irow*nheight;
	ExtentRect.bottom=prect->bottom+(irow+1)*nheight;
	ExtentRect.right=prect->left+(icol+1)*nwidth;
}
  //  bool GridIsInView(mapCOORDMAP *pDraw,CGisGrid* grid,long m_nViewWidth,long m_nVeiwHeight);
void CGridIndex::ReadFromFile(FILE *fp)
{
}
void CGridIndex::WriteToFile(FILE *fp)
{
	//float fleft,ftop,fright,fbottom;
	fwrite(&mapExtentRect.fMinX,sizeof(float),1,fp);
	fwrite(&mapExtentRect.fMinY,sizeof(float),1,fp);
	fwrite(&mapExtentRect.fMaxX,sizeof(float),1,fp);
	fwrite(&mapExtentRect.fMaxY,sizeof(float),1,fp);
	int indexNums=gridindexvector.size();
	fwrite(&indexNums,sizeof(int),1,fp);
    for (int k=0;k<indexNums;k++)
	{
		GRIDINDEX* pgridindex;
		pgridindex=(GRIDINDEX*)gridindexvector[k];
		fwrite(&pgridindex->arrayIndex,sizeof(int),1,fp);
		fwrite(&pgridindex->offset,sizeof(int),1,fp);
	}
}
void CGridIndex::Draw(HDC hDC,mapCOORDMAP* pmapCoordMap)
{
		HPEN	oldPen,newPen;
        //int x1,y2,x2,y1;
		//pmapCoordMap->DPtoVP(mapExtentRect.fMinX,mapExtentRect.fMinY,&x1,&y1);
		//pmapCoordMap->DPtoVP(mapExtentRect.fMaxX,mapExtentRect.fMaxY,&x2,&y2);
        int x1=ExtentRect.left;
        int y2=ExtentRect.bottom;
        int x2=ExtentRect.right;
        int y1=ExtentRect.top;
	
		newPen = CreatePen(PS_SOLID, 3,RGB(255,255,0));
		oldPen	= (HPEN)SelectObject(hDC,newPen);
        MoveToEx(hDC,x1,y1,NULL);
        LineTo(hDC,x1,y2);
        LineTo(hDC,x2,y2);
        LineTo(hDC,x2,y1);
        LineTo(hDC,x1,y1);
		SelectObject(hDC,oldPen);

}

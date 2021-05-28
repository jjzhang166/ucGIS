#include "grid.h"
//*******************************************
//*********** Grid ********************
//*******************************************

long Grid::GridWidth=32;
long Grid::GridHeigth=16;
int Grid :: GetGridCode(int Tcols,int row,int col)
{
	m_pDoc=pDoc;
	int gridCode=(row-1)*Tcols+col;
	CString str;
    //str.Format("%d",N);
    //AfxMessageBox(str);
	return gridCode;


}

int Grid ::GetRow(DrawRect prect,double y)
{   
	int m;
    m=int(abs(y-(prect->top))/GridHeigth);
	return m;

}
int Grid ::GetCol(DrawRect prect,double x)
{
	int n;
	n=int((x-(prect->left))/GridWidth);
	return n;

}
Grid ::Grid(DrawRect prect,int irow,int icol)
{
	ExtentRect.left=prect->left+icol*GridWidth;
	ExtentRect.bottom=prect->bottom+irow*GridHeigth;
	ExtentRect.top=prect->bottom+(irow+1)*GridHeigth;
	ExtentRect.right=prect->left+(icol+1)*GridWidth;	
}

void Grid ::WriteToFile(FILE *fp)
{
	
	fwrite(&ExtentRect.left,sizeof(int),1,fp);
	fwrite(&ExtentRect.top,sizeof(int),1,fp);
	fwrite(&ExtentRect.right,sizeof(int),1,fp);
	fwrite(&ExtentRect.bottom,sizeof(int),1,fp);

	int indexNums=indexvector.size();
	fwrite(&indexNums,sizeof(int),1,fp);
    for (int k=0;k<indexNums;k++)
	{
		INDEX index;
		index=indexvector[k];
		fwrite(&index.arrayIndex,sizeof(int),1,fp);
		fwrite(&index.ClassCode,sizeof(int),1,fp);
		fwrite(&index.ilayer,sizeof(int),1,fp);
	}
}
void Grid ::ReadFromFile(FILE *fp)
{
	fread(&ExtentRect.left,sizeof(int),1,fp);
	fread(&ExtentRect.top,sizeof(int),1,fp);
	fread(&ExtentRect.right,sizeof(int),1,fp);
	fread(&ExtentRect.bottom,sizeof(int),1,fp);

	int indexNums£»
	fread(&indexNums,sizeof(int),1,fp);
    for (int k=0;k<indexNums;k++)
	{
		INDEX *index=new INDEX;;
		fread(&index->arrayIndex,sizeof(int),1,fp);
		fread(&index->ClassCode,sizeof(int),1,fp);
		fread(&index->ilayer,sizeof(int),1,fp);
		indexvector.push_back(index);
	}
}

bool Grid :: GridIsInView(CDraw *pDraw,Grid* grid,long m_nViewWidth,long m_nVeiwHeight)
{
	long left,top,right,bottom;

	pDraw->DPtoVP(grid->ExtentRect.left,grid->ExtentRect.top,left,top);//×ª»»ÎªÂß¼­×ø±ê
	pDraw->DPtoVP(grid->ExtentRect.right,grid->ExtentRect.bottom,right,bottom);
	CRect gridRect(left,top,right,bottom);

	
	long x1, y1, x2, y2;
	
	x1 = 0, y1 = 0;
	x2 = m_nViewWidth, y2 = m_nVeiwHeight;
	CRect viewRect(x1, y1, x2, y2);

	
	CRect rect;
	return rect.IntersectRect(viewRect,gridRect);
}
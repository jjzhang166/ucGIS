#define GRIDINDE_GLOBALES
#include "GridIndex.h"
GRIDINDEX_EXT void GridIndex_Realease(GridIndex *pgridIndex)
{
	if(pgridIndex->indexvector.haHandle)
	{
		GUI_ARRAY_Delete(&pgridIndex->indexvector);
	}
}
GRIDINDEX_EXT void GridIndex_ReadFromFile(FILE *fp,GridIndex *pgridIndex)
{
	INDEX index;
	int k=0;
    if(pgridIndex->indexvector.InitState== GUI_ARRAY_STATE_NOT_CREATED)
	  GUI_ARRAY_CREATE(&pgridIndex->indexvector);

	fread(&pgridIndex->mapExtentRect.fMinX,sizeof(float),1,fp);
	fread(&pgridIndex->mapExtentRect.fMinY,sizeof(float),1,fp);
	fread(&pgridIndex->mapExtentRect.fMaxX,sizeof(float),1,fp);
	fread(&pgridIndex->mapExtentRect.fMaxY,sizeof(float),1,fp);

	fread(&pgridIndex->indexNums,sizeof(int),1,fp);
    for ( k=0;k<pgridIndex->indexNums;k++)
	{
		fread(&index.arrayIndex,sizeof(int),1,fp);
		fread(&index.layeroffset,sizeof(int),1,fp);
		fread(&index.ClassCode,sizeof(int),1,fp);
		fread(&index.ilayer,sizeof(int),1,fp);
		fread(&index.offset,sizeof(int),1,fp);
		GUI_ARRAY_AddItem(&pgridIndex->indexvector,&index,sizeof(INDEX));
	}

}
GRIDINDEX_EXT int GridIndex_GetGridCode(int Tcols,int row,int col)
{
  	int gridCode=(row-1)*Tcols+col;
	return gridCode;

}
GRIDINDEX_EXT BOOLEAN GridIsInView(MapCoorTrans *pcoortrans,GridIndex* grid)
{
	int left,top,right,bottom;
    GIS_RECT gridRect,viewRect,xRect; 
	
	DPtoVP(pcoortrans,grid->mapExtentRect.fMinX,grid->mapExtentRect.fMinY,&left,&top);//×ª»»ÎªÂß¼­×ø±ê
	DPtoVP(pcoortrans,grid->mapExtentRect.fMaxX,grid->mapExtentRect.fMaxY,&right,&bottom);
	//CRect gridRect(left,top,right,bottom);
    gridRect.left=left;
    gridRect.top=top;
    gridRect.right=right;
    gridRect.bottom=bottom;
	
    viewRect.left=0;
	viewRect.top=0;
	viewRect.right=pcoortrans->wScreen;
	viewRect.bottom=pcoortrans->hScreen;
	return (GIS_IntersectRects(&xRect,&viewRect,&gridRect));
}

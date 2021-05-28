#define GRIDINDE_GLOBALES
#include "GridIndex.h"
GRIDINDEX_EXT void GridIndex_Realease(GridIndex *pgridIndex)
{
	if(pgridIndex->indexvector.haHandle)
	{
		GUI_ARRAY_Delete(&pgridIndex->indexvector);
		memset(&pgridIndex->indexvector,0,sizeof(GUI_ARRAY));
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
void GridIndex_ReadFromFileOffSet(FILE *fp,GridIndex *pgridIndex,int offset)
{
	INDEX index;
	int k=0;
    if(pgridIndex->indexvector.InitState== GUI_ARRAY_STATE_NOT_CREATED)
	  GUI_ARRAY_CREATE(&pgridIndex->indexvector);

	fseek(fp,offset,SEEK_SET);
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
	
	DPtoVP(pcoortrans,grid->mapExtentRect.fMinX,grid->mapExtentRect.fMinY,&left,&top);//转换为逻辑坐标
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

//网格索引的索引
//gridindexindex
GRIDINDEX_EXT void GridIndexIndex_ReadFromFile(FILE *fp,GridIndexIndex *pgridIndexIndex)
{
	INDEXINDEX index;
	int k=0;
	if(pgridIndexIndex->indexindexvector.InitState== GUI_ARRAY_STATE_NOT_CREATED)
	  GUI_ARRAY_CREATE(&pgridIndexIndex->indexindexvector);

	fread(&pgridIndexIndex->mapExtentRect.fMinX,sizeof(float),1,fp);
	fread(&pgridIndexIndex->mapExtentRect.fMinY,sizeof(float),1,fp);
	fread(&pgridIndexIndex->mapExtentRect.fMaxX,sizeof(float),1,fp);
	fread(&pgridIndexIndex->mapExtentRect.fMaxY,sizeof(float),1,fp);

	fread(&pgridIndexIndex->nGridindexNums,sizeof(int),1,fp);
    for ( k=0;k<pgridIndexIndex->nGridindexNums;k++)
	{
		fread(&index.arrayIndex,sizeof(int),1,fp);
		fread(&index.offset,sizeof(int),1,fp);
		GUI_ARRAY_AddItem(&pgridIndexIndex->indexindexvector,&index,sizeof(INDEXINDEX));
	}

}
GRIDINDEX_EXT BOOLEAN GridIndexIsInView(MapCoorTrans *pcoortrans,GridIndexIndex* gridindexindex)
{
	int left,top,right,bottom;
    GIS_RECT gridRect,viewRect,xRect; 
	
	DPtoVP(pcoortrans,gridindexindex->mapExtentRect.fMinX,gridindexindex->mapExtentRect.fMinY,&left,&top);//转换为逻辑坐标
	DPtoVP(pcoortrans,gridindexindex->mapExtentRect.fMaxX,gridindexindex->mapExtentRect.fMaxY,&right,&bottom);
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
GRIDINDEX_EXT void GridIndexIndex_Realease(GridIndexIndex *pgridIndexIndex)
{
	if(pgridIndexIndex->indexindexvector.haHandle)
	{
		GUI_ARRAY_Delete(&pgridIndexIndex->indexindexvector);
		memset(&pgridIndexIndex->indexindexvector,0,sizeof(GUI_ARRAY));
	}
}

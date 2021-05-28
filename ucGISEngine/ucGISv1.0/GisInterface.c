#define GISAPI_GLOBALES

#include "GUI.h"
#include "GisApi.h"
#include "userlayer.h"
//#include "gridindex.h"

// 从文件读取数据
extern INT32U  WToA(GISTCHAR *unistr,GISCHAR *gbstr,INT32U msg_len);
BOOLEAN Map_ReadFromFile(UcMap *pucmap,GISCHAR *pszFileName)
{
	// 显示忙光标
	FILE	*fp;
	INT32U  nCount,i;
	GISTCHAR szMapName[50];
	GMapLayer  layer;
	//GUI_SetCursor (LoadCursor (NULL, IDC_WAIT));
    GUI_CURSOR_Select(&GUI_CursorHeaderMI);
	fp = fopen(pszFileName,"rb");
	if(!fp)	return FALSE;
	fread(&pucmap->map_fCenterX,sizeof(FP32),1,fp);  // 
	fread(&pucmap->map_fCenterY,sizeof(FP32),1,fp);  // 
	fread(&pucmap->map_blc,sizeof(FP32),1,fp);  // 
	fread(&szMapName,sizeof(szMapName),1,fp);  // 
    WToA(szMapName,pucmap->map_lpszMapName,50);
		//fread(&pucmap->map_lpszMapName,sizeof(pucmap->map_lpszMapName),1,fp);  // 
    
	fread(&nCount,sizeof(INT32U),1,fp);  // 读层的数量
//	if(layarray
	if(pucmap->layerarray.InitState== GUI_ARRAY_STATE_NOT_CREATED)
	  GUI_ARRAY_CREATE(&pucmap->layerarray);
	//if(nCount>1) nCount=1;
	for(i=0;i<nCount;i++)
	{
	    memset(&layer,0,sizeof(GMapLayer));
		layer.iLayer=i;
		GUI_LOCK();
        Layer_ReadFromFile(fp,&layer);
        GUI_ARRAY_AddItem(&pucmap->layerarray,&layer,sizeof(GMapLayer));
		GUI_UNLOCK();
	}
	fclose(fp);
	pucmap->pmapCoorMap->m_bMapChanged = TRUE;
	GUI_CURSOR_Select(&GUI_CursorArrowM);
	return TRUE;
}
void RealeaseGridIndex(UcMap *pucmap)
{
	GridIndex *pGrid;
	int nCount,i; 
	if(!pucmap->gridarray.haHandle) return;
	nCount= GUI_ARRAY_GetNumItems(&pucmap->gridarray);
	for(i=0;i<nCount;i++)
	{
      pGrid=(GridIndex*)GUI_ARRAY_GetpItem(&pucmap->gridarray,i);
	  if(pGrid)
	  {
		  GridIndex_Realease(pGrid);
	  }
	  pGrid=NULL;
	}
    GUI_ARRAY_Delete(&pucmap->gridarray);
}
GISAPI_EXT BOOLEAN Map_ReadFileIndex(UcMap *pucmap,GISCHAR *pszMapFileName,GISCHAR *pszIndexFileName)
{
	// 显示忙光标
	FILE	*fp;
	//INT32U  nCount,i;
	GISTCHAR szMapName[50];
	//GUI_SetCursor (LoadCursor (NULL, IDC_WAIT));
    GUI_CURSOR_Select(&GUI_CursorHeaderMI);
	fp = fopen(pszMapFileName,"rb");
	if(!fp)	return FALSE;
	fread(&pucmap->map_fCenterX,sizeof(FP32),1,fp);  // 
	fread(&pucmap->map_fCenterY,sizeof(FP32),1,fp);  // 
	fread(&pucmap->map_blc,sizeof(FP32),1,fp);  // 
	fread(&szMapName,sizeof(szMapName),1,fp);  // 
    WToA(szMapName,pucmap->map_lpszMapName,50);
	fclose(fp);
	//Map_ReadGridIndex(pucmap,pszIndexFileName);
    //Map_ReadMapWithGridIndex(pucmap,pszMapFileName);
	GUI_CURSOR_Select(&GUI_CursorArrowM);
	return TRUE;
}

#define CURRENTGRIDINDEX 0
//通过读取地图索引来获取地图数据
void Map_ReadMapWithGridIndex(UcMap *pucmap,GISCHAR *pszFileName)
{
	
	FILE	*fp;
	INT32U  nCount,i,j,nGridCount;
	INT32U *pgridno;
//	GISTCHAR szMapName[50];
    INT32U nEyeShot;
	GridIndex *pGrid;
    BOOLEAN bInCurrentGrid=FALSE;
	BOOLEAN bGridChange=FALSE;
	if(!pucmap->gridarray.haHandle) return;
	fp = fopen(pszFileName,"rb");
	if(!fp)	return;
	if(!pucmap->gridarray.haHandle) return;
#if  CURRENTGRIDINDEX

	if(pucmap->currentGridsArray.InitState== GUI_ARRAY_STATE_NOT_CREATED)
	   GUI_ARRAY_Create(&pucmap->currentGridsArray);
#endif

	nCount= GUI_ARRAY_GetNumItems(&pucmap->gridarray);
	//GUI_ARRAY_Delete(&pucmap->currentGridsArray);
   // MapReleaseGrid(pucmap);
	nEyeShot = pucmap->pmapCoorMap->blc*32*100000;

	for(i=0;i<nCount;i++)
	{
      pGrid=(GridIndex*)GUI_ARRAY_GetpItem(&pucmap->gridarray,i);
	  if(pGrid)
	  {

        bInCurrentGrid=FALSE;
#if  CURRENTGRIDINDEX

		nGridCount=GUI_ARRAY_GetNumItems(&pucmap->currentGridsArray);
		for(j=0;j<nGridCount;j++)
		{
			pgridno=(INT32U*)GUI_ARRAY_GetpItem(&pucmap->currentGridsArray,j);
			if(pGrid->nGridNo==*pgridno)
			{
				bInCurrentGrid=TRUE;
			    break;
			}
		}
#endif
		// if(bGridChange 
        if(GridIsInView(pucmap->pmapCoorMap,pGrid))
		{
          //在当前视窗里，则需读取格网内的的图元
		    if(!bInCurrentGrid)
			{
				if(!bGridChange)
				{//第一个格网到视窗时清空所有数据，重新添加
                    MapReleaseGrid(pucmap);
                    bGridChange=TRUE;  
#if  CURRENTGRIDINDEX
					GUI_ARRAY_Delete(&pucmap->currentGridsArray);
                    memset(&pucmap->currentGridsArray,0,sizeof(pucmap->currentGridsArray)); 
#endif
				}
			   Layer_ReadFileWithGridIndex(fp,pGrid,&pucmap->layerarray,nEyeShot);  
#if  CURRENTGRIDINDEX

			   if(pucmap->currentGridsArray.InitState== GUI_ARRAY_STATE_NOT_CREATED)
	              GUI_ARRAY_Create(&pucmap->currentGridsArray);
				GUI_ARRAY_AddItem(&pucmap->currentGridsArray,&pGrid->nGridNo,sizeof(pGrid->nGridNo));
#endif
			}
 		}
		else
		{
#if  CURRENTGRIDINDEX
	      if(bInCurrentGrid)
          {
			  //Layer_FreeGridMap(pGrid,&pucmap->layerarray);
		      //GUI_ARRAY_DeleteItem(&pucmap->currentGridsArray,j);
		  }
#endif
		}
	  }
	}
    fclose(fp);
}
void Map_ReadGridIndex(UcMap *pucmap,GISCHAR *pszFileName)
{
	FILE	*fp;
	int i;//,nminx,nminy,nmaxx,nmaxy;
	int gridNum=0;
	fp = fopen(pszFileName,"rb");
	if(!fp)	return;
	fread(&pucmap->Trows,sizeof(int),1,fp);  // 
	fread(&pucmap->Tcols,sizeof(int),1,fp);  //
	fread(&gridNum,sizeof(int),1,fp);  //
	pucmap->gridNum=gridNum;
	RealeaseGridIndex(pucmap);
    if(pucmap->gridarray.InitState== GUI_ARRAY_STATE_NOT_CREATED)
	  GUI_ARRAY_CREATE(&pucmap->gridarray);
	for (i=0;i<gridNum;i++)
	{
		GridIndex gridindex;
		memset(&gridindex,0,sizeof(GridIndex));
		gridindex.nGridNo=i;
		GridIndex_ReadFromFile(fp,&gridindex);
        GUI_ARRAY_AddItem(&pucmap->gridarray,&gridindex,sizeof(GridIndex));
	}
    pucmap->bGridHave=TRUE;
    fclose(fp);
}

// 绘制地图
void Map_Draw(UcMap *pucmap, INT32U nDrawMode)
{
	
	// 计算比例尺，单位为 公里
	//int nEyeShot = max(m_mapCoordMap->wScreen,m_mapCoordMap->hScreen)*m_mapCoordMap->blc*100;
	INT32U nEyeShot,nCount,i; 
	GMapLayer* player ;
    
	if(pucmap->pmapCoorMap->m_bMapChanged)
	{
		//nEyeShot = pucmap->pmapCoorMap->blc*32*100000;
		nEyeShot = pucmap->pmapCoorMap->blc*32*100000;
		SetStart(pucmap->pmapCoorMap);
		ClearLabelRect(pucmap->pmapCoorMap);
		SetScreenXY(pucmap->pmapCoorMap);

		//	if(m_mapCoordMap->m_bMapChanged||bForceDraw)
		if(pucmap->layerarray.haHandle)
		nCount = GUI_ARRAY_GetNumItems(&pucmap->layerarray);
		else
		return;
		//nCount = 5;
		for(i=0;i<nCount;i++)
		{
			player = (GMapLayer*)GUI_ARRAY_GetpItem(&pucmap->layerarray,i);
			player->pCoordMap = pucmap->pmapCoorMap;
			if (player->m_nEyeShotMax==0||nEyeShot>=player->m_nEyeShotMin&&nEyeShot<=player->m_nEyeShotMax)
				Layer_Draw(player,nDrawMode);
			
		}		
		
		// 画标注
		for(i=0;i<nCount;i++)
		{
			player = (GMapLayer*)GUI_ARRAY_GetpItem(&pucmap->layerarray,i);
			player->pCoordMap = pucmap->pmapCoorMap;
			if (player->m_nEyeShotMax==0||nEyeShot>=player->m_nEyeShotMin&&nEyeShot<=player->m_nEyeShotMax)
				Layer_DrawLabel(player);
		}
		
		if(pucmap->m_bShowMapName) 
		{
			GUI_DispStringAt( pucmap->map_lpszMapName,0,60 ); // Draw text with pen color
		}
		//pucmap->pmapCoorMap->m_bMapChanged=FALSE;
    //userlayer
	pucmap->puserlayer->m_mapCoordMap=pucmap->pmapCoorMap;
	UserLayer_Draw(pucmap->puserlayer);
	}
  	//DrawBlc(pucmap,FALSE);



}
void DrawBlc(UcMap *pucmap,BOOLEAN bFilled)
{
	// 绘制比例尺
	GUI_RECT rect;
	int nDistance,cx,cy;
	GISCHAR lpszState[300] = {0};
	if(pucmap->m_bDrawScale)
	{
        GUI_SetFont(&GUI_Font13_1);
		GUI_SetColor(GUI_RED);
	    GUI_SetBkColor(GUI_BLACK);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetTextAlign(GUI_TA_LEFT);

		rect.x0 = pucmap->pmapCoorMap->wScreen-55;
		rect.y0 = pucmap->pmapCoorMap->hScreen-20;
		rect.y1 = rect.y0+17;
		rect.x1= rect.x0+39;
		// 如果需要填充多边形

		GUI_DrawRectEx(&rect);
		if(bFilled)
		{
		  GUI_SetColor(GUI_GREEN);
		  GUI_SetBkColor(GUI_BLACK);
		  //GUI_SetPenShape(brush.patter);
		  GUI_FillRectEx(&rect);
		}

		// 绘制比例尺
		GUI_MoveTo(rect.x0+2,rect.y0+13);
		GUI_DrawLineTo(rect.x0+35,rect.y0+13);

		GUI_MoveTo(rect.x0+2,rect.y0+9);
		GUI_DrawLineTo(rect.x0+2,rect.y0+153);
		
		GUI_MoveTo(rect.x0+35,rect.y0+9);
		GUI_DrawLineTo(rect.x0+35,rect.y0+153);

		// 计算比例尺
		nDistance = pucmap->pmapCoorMap->blc*33*100000;

/*		if(nDistance>999)
			sprintf(lpszState,"%dKM",nDistance/1000);
		else
			sprintf(lpszState,"%dM",nDistance);
*/
		if(nDistance>999)
			sprintf(lpszState,"%d千米",nDistance/1000);
		else
			sprintf(lpszState,"%d米",nDistance);
        
		GUI_SetFont(gGisFont);
		
        GisGetTextExtentPoint(lpszState,&cx,&cy);
		GUI_DispStringAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+1);
	}
}

void MapReleaseGrid(UcMap *pucmap)
{
	
	GMapLayer  *player;
    int i,nCount=0;
	if(pucmap->layerarray.haHandle)
	{

		nCount = GUI_ARRAY_GetNumItems(&pucmap->layerarray);
		for(i=0;i<nCount;i++)
		{
			GUI_LOCK();
		    player = (GMapLayer*)GUI_ARRAY_GetpItem(&pucmap->layerarray,i);
        	Layer_Free(player);
			GUI_UNLOCK();
		}
       GUI_ARRAY_Delete(&pucmap->layerarray);
      memset(&pucmap->layerarray,0,sizeof(pucmap->layerarray));
	}
    //memset(pucmap,0,sizeof(UcMap));
}

void MapRelease(UcMap *pucmap)
{
	
	GMapLayer  *player;
    int i,nCount=0;
	if(pucmap->layerarray.haHandle)
	{

		nCount = GUI_ARRAY_GetNumItems(&pucmap->layerarray);
		for(i=0;i<nCount;i++)
		{
			GUI_LOCK();
		    player = (GMapLayer*)GUI_ARRAY_GetpItem(&pucmap->layerarray,i);
        	Layer_Free(player);
			GUI_UNLOCK();
		}
       GUI_ARRAY_Delete(&pucmap->layerarray);
	}
    memset(pucmap,0,sizeof(UcMap));
}
/*
// 添加一个层，返回该层的索引号
short CMyMapDoc::AddLayer(CLayer *pLayer)
{
	m_LayerVector.push_back(pLayer);
	// 设置为当前图层
	m_nCurrentLayer = m_LayerVector.size()-1;
	return m_nCurrentLayer;
}
*/


// 查询点目标
BOOLEAN SearchMap(UcMap *pucmap,GISCHAR* szKey, MyPoint* ppoints, int * nCount)
{
	GMapLayer  *player;
	MyPoint layerpoints[100];
    int i,j,npoint=0,nCountLayer=0;
	*nCount=0;
	if(pucmap->layerarray.haHandle)
	{

		nCountLayer = GUI_ARRAY_GetNumItems(&pucmap->layerarray);
		for(i=0;i<nCountLayer;i++)
		{
			GUI_LOCK();
		    player = (GMapLayer*)GUI_ARRAY_GetpItem(&pucmap->layerarray,i);
			GUI_UNLOCK();
            Layer_SearchMap(player,szKey,layerpoints,&npoint);
			if(npoint>0)
			{
				for(j=0;j<npoint;j++)
				{
					MyPointSetValue(ppoints++,&layerpoints[j]);
					(*nCount)++;
				}
			}
		}

	}
	if(*nCount>0)
		return TRUE;
	else
		return FALSE;
}
void Map_GetRect(UcMap *pucmap,FP32 * m_Xmin, FP32 * m_Ymin, FP32 * m_Xmax, FP32 * m_Ymax)
{
	
	GMapLayer  *player;
    int i,nCount=0;
	float m_minX,m_minY,m_maxX,m_maxY;
	m_minX=(float)1E20;m_minY=(float)1E20;m_maxX=-(float)1E20;m_maxY=-(float)1E20;
	if(pucmap->layerarray.haHandle)
	{

		nCount = GUI_ARRAY_GetNumItems(&pucmap->layerarray);
		for(i=0;i<nCount;i++)
		{
			player = (GMapLayer*)GUI_ARRAY_GetpItem(&pucmap->layerarray,i);
			Layer_GetRect(player,&m_minX,&m_minY,&m_maxX,&m_maxY);
			*m_Xmin = GISMIN(*m_Xmin,m_minX);
			*m_Xmax = GISMAX(*m_Xmax,m_maxX);
			*m_Ymin = GISMIN(*m_Ymin,m_minY);
			*m_Ymax = GISMAX(*m_Ymax,m_maxY);
		}
	}
}
/*
// 获取我在哪里的文字描述
BOOLEAN GetWhereIam(UcMap *pucmap,GeoPoint* pt, GISCHAR* szWhere)
{
	GMapLayer* pLayer = NULL;
	int nCount ;
	if(pucmap->layerarray.haHandle)

	for(int i=0;i<nCount;i++)
	{
		pLayer = (CLayer*)m_LayerVector[i];
		if(pLayer)
		{
			if(lstrcmp(pLayer->m_pszName,_T("中国省级区域"))==0)
			{
				pLayer->GetWhereIam(pt,szWhere);
			}
			else if(lstrcmp(pLayer->m_pszName,_T("中国-四川省-地区区域"))==0)
			{
				pLayer->GetWhereIam(pt,szWhere);
			}
			else if(lstrcmp(pLayer->m_pszName,_T("中国-四川省-县级区域"))==0)
			{
				pLayer->GetWhereIam(pt,szWhere);
			}
			//if(pLayer->m_pszName
		}
	}
	return false;
}
*/
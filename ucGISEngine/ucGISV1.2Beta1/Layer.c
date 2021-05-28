#define LAYER_GLOBALES
#include "Layer.h"

void Layer_AddPline(GMapLayer *pLayer,GPLine* pGPline)
{
	if(pLayer->plinedata.InitState== GUI_ARRAY_STATE_NOT_CREATED)
	   GUI_ARRAY_CREATE(&pLayer->plinedata);
    WM_LOCK();
	GUI_ARRAY_AddItem(&pLayer->plinedata, pGPline, sizeof(GPLine));
    WM_UNLOCK();

}
void Layer_AddLine(GMapLayer *pLayer,GLine* pGLine)
{
	if(pLayer->linedata.InitState== GUI_ARRAY_STATE_NOT_CREATED)
	   GUI_ARRAY_CREATE(&pLayer->linedata);
    WM_LOCK();
	GUI_ARRAY_AddItem(&pLayer->linedata, pGLine, sizeof(GLine));
    WM_UNLOCK();

}

void Layer_AddPlineRgn(GMapLayer *pLayer,GRegion *pPlinergn)
{
	if(pLayer->regiondata.InitState== GUI_ARRAY_STATE_NOT_CREATED)
	   GUI_ARRAY_CREATE(&pLayer->regiondata);
    WM_LOCK();
	GUI_ARRAY_AddItem(&pLayer->regiondata, pPlinergn, sizeof(GRegion));
    WM_UNLOCK();

}

void Layer_AddMyPoint(GMapLayer *pLayer,MyPoint *pMypoint)
{
	if(pLayer->mypointdata.InitState== GUI_ARRAY_STATE_NOT_CREATED)
	   GUI_ARRAY_CREATE(&pLayer->mypointdata);
    WM_LOCK();
	GUI_ARRAY_AddItem(&pLayer->mypointdata, pMypoint, sizeof(MyPoint));
    WM_UNLOCK();
}
void Layer_ReadFromFile(FILE *fp,GMapLayer *pLayer)
{
	int i;
	GRegion rgn;
	GPLine gPline;
	GLine gline;
    MyPoint mypoint;
    GISTCHAR szName[100];
	int nCountRgn = 0;
	int nCountPline = 0;
	int nCountLine = 0;
	int nCountPoint = 0;

	fread(&pLayer->m_bAutoLabel,sizeof(BOOLEAN),1,fp);
	fread(&pLayer->m_bVisible,sizeof(BOOLEAN),1,fp);
	fread(&pLayer->m_nEyeShotMin,sizeof(pLayer->m_nEyeShotMin),1,fp);
	fread(&pLayer->m_nEyeShotMax,sizeof(pLayer->m_nEyeShotMax),1,fp);
	//fread(pLayer->m_pszName,sizeof(pLayer->m_pszName),1,fp);
    fread(&szName,sizeof(szName),1,fp); 
    WToA(szName,pLayer->m_pszName,100);
	// 多边形
	fread(&nCountRgn,sizeof(int),1,fp);
	for(i=0;i<nCountRgn;i++)
	{
		if(feof(fp)) return;
 	    memset(&rgn,0,sizeof(GRegion));
		PlineRgn_ReadFromFile(fp,&rgn);
		Layer_AddPlineRgn(pLayer,&rgn);
	}
	// 曲线
	fread(&nCountPline,sizeof(int),1,fp);
	for(i=0;i<nCountPline;i++)
	{
		if(feof(fp)) return;
        memset(&gPline,0,sizeof(GPLine));
		Pline_ReadFromFile(fp,&gPline);
		Layer_AddPline(pLayer,&gPline);
	}
	// 直线
	fread(&nCountLine,sizeof(int),1,fp);
	for(i=0;i<nCountLine;i++)
	{
		if(feof(fp)) return;
        memset(&gline,0,sizeof(GLine));
		GLine_ReadFromFile(fp,&gline);
		Layer_AddLine(pLayer,&gline);
	}
	// 点
	fread(&nCountPoint,sizeof(int),1,fp);
	for(i=0;i<nCountPoint;i++)
	{
		if(feof(fp)) return;
        memset(&mypoint,0,sizeof(MyPoint));
		MyPoint_ReadFromFile(fp,&mypoint);
		Layer_AddMyPoint(pLayer,&mypoint);
	}
}
void  PLINE_FreeIndex(GMapLayer *pLayer,int objectindex)
{
	GPLine* pPline;
	int i,nCount=0;
	if(!pLayer->plinedata.haHandle) return;
    nCount=GUI_ARRAY_GetNumItems(&pLayer->plinedata);
	for(i=0;i<nCount;i++)
	{
       pPline=(GPLine *)GUI_ARRAY_GetpItem(&pLayer->plinedata,i);
	   if(pPline->index==objectindex)
	   {
           GUI_ARRAY_DeleteItem(&pLayer->plinedata,i);
           // nCount=GUI_ARRAY_GetNumItems(&pLayer->plinedata);
		   break;
	   }
	}
}
void  RGN_FreeIndex(GMapLayer *pLayer,int objectindex)
{
	GRegion *prgn;
	int i,nCount=0;
	if(!pLayer->regiondata.haHandle) return;
    nCount=GUI_ARRAY_GetNumItems(&pLayer->regiondata);
	for(i=0;i<nCount;i++)
	{
       prgn=(GRegion *)GUI_ARRAY_GetpItem(&pLayer->regiondata,i);
   
	   if(prgn->index==objectindex)
	   {
           GUI_ARRAY_DeleteItem(&pLayer->regiondata,i);
           //nCount=GUI_ARRAY_GetNumItems(&pLayer->regiondata);
		   break;
	   }
	}
}
void  LINE_FreeIndex(GMapLayer *pLayer,int objectindex)
{
	GLine* pgline;
	int i,nCount=0;
	if(!pLayer->linedata.haHandle) return;
    nCount=GUI_ARRAY_GetNumItems(&pLayer->linedata);
	for(i=0;i<nCount;i++)
	{
       pgline=(GLine *)GUI_ARRAY_GetpItem(&pLayer->linedata,i);
	   if(pgline->index==objectindex)
	   {
           GUI_ARRAY_DeleteItem(&pLayer->linedata,i);
		   break;
	   }
	}
}
void  MYPOINT_FreeIndex(GMapLayer *pLayer,int objectindex)
{
    MyPoint* pmypoint;
	int i,nCount=0;
	if(!pLayer->mypointdata.haHandle) return;
    nCount=GUI_ARRAY_GetNumItems(&pLayer->mypointdata);
	for(i=0;i<nCount;i++)
	{
       pmypoint=(MyPoint *)GUI_ARRAY_GetpItem(&pLayer->mypointdata,i);
	   if(pmypoint->index==objectindex)
	   {
           GUI_ARRAY_DeleteItem(&pLayer->mypointdata,i);
           //nCount=GUI_ARRAY_GetNumItems(&pLayer->mypointdata);
		   break;
	   }
	}
}

void Layer_FreeGridMap(GridIndex* pgrid,GUI_ARRAY *pLayerArray)
{
	int i,j,nCount,nlayerCount,ilayer=-1;
	GMapLayer* pLayer;
	INDEX *pIndex;
    if(!pLayerArray->haHandle) return;
	if(!pgrid->indexvector.haHandle) return;
    nCount=GUI_ARRAY_GetNumItems(&pgrid->indexvector);
	for(i=0;i<nCount;i++)
	{
		ilayer=-1;
		pIndex=GUI_ARRAY_GetpItem(&pgrid->indexvector,i);
		nlayerCount=GUI_ARRAY_GetNumItems(pLayerArray);
		for(j=0;j<nlayerCount;j++)
		{
            pLayer=(GMapLayer*)GUI_ARRAY_GetpItem(pLayerArray,j);
			if(pIndex->ilayer==pLayer->iLayer)
			{
				ilayer=j;
				break;
			}
		}
        //没找到继续
		if(ilayer==-1)
		{
           continue;
		}
        switch(pIndex->ClassCode)
		{
		case IDS_PLINE:
			PLINE_FreeIndex(pLayer,pIndex->arrayIndex);
			break;
		case IDS_POLYGON:
			RGN_FreeIndex(pLayer,pIndex->arrayIndex);
			break;
		case IDS_TEXT:
			break;
		case IDS_POINT:
			MYPOINT_FreeIndex(pLayer,pIndex->arrayIndex);
			break;
		case IDS_LINE:
			LINE_FreeIndex(pLayer,pIndex->arrayIndex);
			break;
        default:
			break;
		}
	}

}
#define NOTREADD  1

void RGN_AddIndex(FILE *fp,GMapLayer *pLayer,int objectindex)
{
	GRegion rgn;
	GRegion* prgn;
	int i,nCount=0;
	if(feof(fp)) return;
	if(pLayer->regiondata.InitState==GUI_ARRAY_STATE_NOT_CREATED)
       GUI_ARRAY_Create(&pLayer->regiondata);

#if  NOTREADD
 
	nCount=GUI_ARRAY_GetNumItems(&pLayer->regiondata);
	for(i=0;i<nCount;i++)
	{
       prgn=(GRegion *)GUI_ARRAY_GetpItem(&pLayer->regiondata,i);
	   if(prgn->index==objectindex)
	   {
		   //原先有的话不用再加入
		   return;
	   }
	}
#endif 
    memset(&rgn,0,sizeof(GRegion));
	rgn.index=objectindex;
	PlineRgn_ReadFromFile(fp,&rgn);
	Layer_AddPlineRgn(pLayer,&rgn);
}
void LINE_AddIndex(FILE *fp,GMapLayer *pLayer,int objectindex)
{
	GLine gline;
	GLine *pgline;
	int i,nCount=0;
	if(feof(fp)) return;
	if(pLayer->linedata.InitState== GUI_ARRAY_STATE_NOT_CREATED)
		GUI_ARRAY_Create(&pLayer->linedata);
#if  NOTREADD
	nCount=GUI_ARRAY_GetNumItems(&pLayer->linedata);
	for(i=0;i<nCount;i++)
	{
	   pgline=(GLine *)GUI_ARRAY_GetpItem(&pLayer->linedata,i);
	   if(pgline->index==objectindex)
	   {
		   //原先有的话不用再加入
		   return;
	   }
	}
#endif
	memset(&gline,0,sizeof(GLine));
	gline.index=objectindex;
	GLine_ReadFromFile(fp,&gline);
	Layer_AddLine(pLayer,&gline);
}
void PLINE_AddIndex(FILE *fp,GMapLayer *pLayer,int objectindex)
{
	GPLine gPline;
	GPLine *pgPline;
	int i,nCount=0;
	if(feof(fp)) return;
	if(pLayer->plinedata.InitState== GUI_ARRAY_STATE_NOT_CREATED)
		GUI_ARRAY_Create(&pLayer->plinedata);
#if  NOTREADD
	nCount=GUI_ARRAY_GetNumItems(&pLayer->plinedata);
	for(i=0;i<nCount;i++)
	{
	   pgPline=(GPLine *)GUI_ARRAY_GetpItem(&pLayer->plinedata,i);
	   if(pgPline->index==objectindex)
	   {
		   //原先有的话不用再加入
		   return;
	   }
	}
#endif
	memset(&gPline,0,sizeof(GPLine));
	gPline.index=objectindex;
	Pline_ReadFromFile(fp,&gPline);
	Layer_AddPline(pLayer,&gPline);
}
void MYPOINT_AddIndex(FILE *fp,GMapLayer *pLayer,int objectindex)
{
	MyPoint mypoint;
	MyPoint *pmypoint;
	int i,nCount=0;
	if(feof(fp)) return;
	if(pLayer->mypointdata.InitState== GUI_ARRAY_STATE_NOT_CREATED)
		GUI_ARRAY_Create(&pLayer->mypointdata);
#if  NOTREADD
	nCount=GUI_ARRAY_GetNumItems(&pLayer->mypointdata);
	for(i=0;i<nCount;i++)
	{
		pmypoint=(MyPoint *)GUI_ARRAY_GetpItem(&pLayer->mypointdata,i);
	   if(pmypoint->index==objectindex)
	   {
		   //原先有的话不用再加入
		   return;
	   }
	}
#endif
	memset(&mypoint,0,sizeof(MyPoint));
	mypoint.index=objectindex;
	MyPoint_ReadFromFile(fp,&mypoint);
	Layer_AddMyPoint(pLayer,&mypoint);
}


void Layer_ReadFileWithAreaIndex(FILE *fp,AreaIndex* pMyAreaIndex,GRegion *prgn)  
{
	fseek(fp, pMyAreaIndex->offset, SEEK_SET);
	PlineRgn_ReadFromFile(fp,prgn);
}
void Layer_ReadFileWithGridIndex(FILE *fp,GridIndex* pgrid,GUI_ARRAY *pLayerArray,INT32U nEyeShot)  
{
	int i,j,nCount,nlayerCount,ilayer=-1;
	GMapLayer layer;
	GMapLayer* pLayer;
	INDEX *pIndex;
    GISTCHAR szName[100];
	if(!pgrid->indexvector.haHandle) return;
	if(pLayerArray->InitState==GUI_ARRAY_STATE_NOT_CREATED)
		GUI_ARRAY_Create(pLayerArray);
    nCount=GUI_ARRAY_GetNumItems(&pgrid->indexvector);
	for(i=0;i<nCount;i++)
	{
		//pLayer->iLayer
		ilayer=-1;
		pIndex=GUI_ARRAY_GetpItem(&pgrid->indexvector,i);
		nlayerCount=GUI_ARRAY_GetNumItems(pLayerArray);
		for(j=0;j<nlayerCount;j++)
		{
            pLayer=(GMapLayer*)GUI_ARRAY_GetpItem(pLayerArray,j);
			if(pIndex->ilayer==pLayer->iLayer)
			{
				ilayer=j;
				break;
			}
		}
        if(ilayer==-1)
		{
			//以前没读过该图层，则获取本图层的头
		  memset(&layer,0,sizeof(GMapLayer));
		  layer.iLayer=pIndex->ilayer;
		  pLayer=&layer;
 		  fseek(fp, pIndex->layeroffset, SEEK_SET);
		  fread(&pLayer->m_bAutoLabel,sizeof(BOOLEAN),1,fp);
		  fread(&pLayer->m_bVisible,sizeof(BOOLEAN),1,fp);
		  fread(&pLayer->m_nEyeShotMin,sizeof(pLayer->m_nEyeShotMin),1,fp);
		  fread(&pLayer->m_nEyeShotMax,sizeof(pLayer->m_nEyeShotMax),1,fp);
		  memset(szName,0,sizeof(szName));
		  fread(&szName,sizeof(szName),1,fp); 
		  WToA(szName,pLayer->m_pszName,100);

		}
	    if (!(pLayer->m_nEyeShotMax==0||nEyeShot>=pLayer->m_nEyeShotMin&&nEyeShot<=pLayer->m_nEyeShotMax))
			continue;
             

        //pIndex->ilayer
	    fseek(fp, pIndex->offset, SEEK_SET);
        switch(pIndex->ClassCode)
		{
		case IDS_PLINE:
			PLINE_AddIndex(fp,pLayer,pIndex->arrayIndex);
			break;
		case IDS_POLYGON:
			RGN_AddIndex(fp,pLayer,pIndex->arrayIndex);

			break;
		case IDS_TEXT:
			break;
		case IDS_POINT:
			MYPOINT_AddIndex(fp,pLayer,pIndex->arrayIndex);
			break;
		case IDS_LINE:
			LINE_AddIndex(fp,pLayer,pIndex->arrayIndex);
			break;
        default:
			break;
		}
        if(ilayer==-1)
			GUI_ARRAY_AddItem(pLayerArray,pLayer,sizeof(GMapLayer));
	}
 }
void Layer_Free(GMapLayer *player)
{
	INT32U i,nCount;
	MyPoint* pmyPoint=NULL;
	GPLine* pgPline=NULL;
	GLine*  pgLine=NULL;
    GRegion* pgRegion=NULL;
	WM_LOCK();
	if(player->mypointdata.haHandle)
	{
	nCount=GUI_ARRAY_GetNumItems(&player->mypointdata);
	for(i=0;i<nCount;i++)
	{	
		pmyPoint=(MyPoint*)GUI_ARRAY_GetpItem(&player->mypointdata,i);
		MyPoint_Free(pmyPoint);
	}
	GUI_ARRAY_Delete(&player->mypointdata);
	}
	if(player->linedata.haHandle)
	{
	nCount=GUI_ARRAY_GetNumItems(&player->linedata);
	for(i=0;i<nCount;i++)
	{	
        pgLine=(GLine*)GUI_ARRAY_GetpItem(&player->linedata,i);
		GLine_Free(pgLine);
	}
	GUI_ARRAY_Delete(&player->linedata);
	}
	if(player->plinedata.haHandle)
	{
	nCount=GUI_ARRAY_GetNumItems(&player->plinedata);
	for(i=0;i<nCount;i++)
	{	
		pgPline=(GPLine*)GUI_ARRAY_GetpItem(&player->plinedata,i);
		GPLine_Free(pgPline);
	}
	GUI_ARRAY_Delete(&player->plinedata);
	}
	if(player->regiondata.haHandle)
	{
	nCount=GUI_ARRAY_GetNumItems(&player->regiondata);
	for(i=0;i<nCount;i++)
	{	
		pgRegion=(GRegion*)GUI_ARRAY_GetpItem(&player->regiondata,i);
		GRegion_Free(pgRegion);
	}
	GUI_ARRAY_Delete(&player->regiondata);
	memset(player,0,sizeof(GMapLayer));
	}
    WM_UNLOCK();
}

/*
void Layer_GetPointFromRect(CMyPoint* points,int& nPointCount)
{
	CDraw* pDraw = NULL;
	int nCount = m_MyPointVector.size();
	for(int l=0;l<nCount;l++)
	{
		pDraw = (CDraw*)m_MyPointVector[l];
		if(pDraw->IsRectCross())
		{
			if (nPointCount>19) break;
			points[nPointCount] = *((CMyPoint*)pDraw); 
			nPointCount++;
			
		}
	}
}
bool Layer_DelPoint(TCHAR* szLabel)
{
	CDraw* pDraw = NULL;
	for(int l=0;l<m_MyPointVector.size();l++)
	{
		pDraw = (CDraw*)m_MyPointVector[l];
		if(pDraw->IsRectCross()&&
			!lstrcmp(pDraw->GetLabel(),szLabel))
		{
			MyPointVector::iterator pos = m_MyPointVector.begin();
			pos+=l;
			delete pDraw;
			m_MyPointVector.erase(pos);
			l--;
		//	m_MyPointVector
			return true;
		}
	}
	return false;
}

BOOLEAN Layer_SearchMap(GISCHAR* szKey, MyPoint* pmypoints, int* nCount)
{
	*nCount = 0;
	MyPoint* pDraw = NULL;
	for(int l=0;l<m_MyPointVector.size();l++)
	{
		//pDraw = (CMyPoint*)m_MyPointVector[l];
		//CString s1(pDraw->GetLabel());
		//CString s2(szKey);
		//if(!lstrcmp(pDraw->GetLabel(),szKey))
		////if(BlurFindStr(s1,s2));
		//{
		//	if(nCount>100)
		//		break;
		//	points[nCount] = *pDraw;
		//	nCount++;
		//}
	}
	return true;
}
void Layer_WriteToFile(FILE *fp)
{
	
	fwrite(&m_bAutoLabel,sizeof(bool),1,fp);
	fwrite(&m_bVisible,sizeof(bool),1,fp);
	fwrite(&m_nEyeShotMin,sizeof(m_nEyeShotMin),1,fp);
	fwrite(&m_nEyeShotMax,sizeof(m_nEyeShotMax),1,fp);
	fwrite(m_pszName,sizeof(m_pszName),1,fp);
	
	// 画多边形
	CDraw* pDraw = NULL;
	int nCount = m_PlineRgnVector.size();
	fwrite(&nCount,sizeof(int),1,fp);
	for(int i=0;i<nCount;i++)
	{
		pDraw = (CDraw*)m_PlineRgnVector[i];
		if(pDraw)
			pDraw->WriteToFile(fp);
	}

	// 画曲线
	pDraw = NULL;
	nCount = m_PlineVector.size();
	fwrite(&nCount,sizeof(int),1,fp);
	for(int j=0;j<nCount;j++)
	{
		pDraw = (CDraw*)m_PlineVector[j];
		if(pDraw)
			pDraw->WriteToFile(fp);
	}
	// 画直线
	pDraw = NULL;
	nCount = m_LineVector.size();
	fwrite(&nCount,sizeof(int),1,fp);
	for(int k=0;k<nCount;k++)
	{
		pDraw = (CDraw*)m_LineVector[k];
		if(pDraw)
			pDraw->WriteToFile(fp);
	}
	// 画点
	pDraw = NULL;
	nCount = m_MyPointVector.size();
	fwrite(&nCount,sizeof(int),1,fp);
	for(int l=0;l<nCount;l++)
	{
		pDraw = (CDraw*)m_MyPointVector[l];
		if(pDraw)
			pDraw->WriteToFile(fp);
	}
	
}
*/
void Layer_Draw(GMapLayer *pLayer, INT32U nDrawMode)
{

	// 画多边形
	GRegion *pGRegion;
	GPLine  *pGPline;
	GLine   *pGline;
    MyPoint *pMyPoint;
	GBrush  brush;
	INT32U i;
	int nCount=0;
	memset(&brush,0,sizeof(GBrush));
	if(pLayer->regiondata.haHandle)
	{
	    nCount= GUI_ARRAY_GetNumItems(&pLayer->regiondata);//m_PlineRgnVector.size();
		for(i=0;i<nCount;i++)
		{
			pGRegion=(GRegion *)GUI_ARRAY_GetpItem(&pLayer->regiondata,i);
			if(pGRegion)
			{
				pGRegion->gpline.baselabel.pCoordMap = pLayer->pCoordMap;
				PlineRgn_Draw(pGRegion,nDrawMode);
			}
		}
	}
	
	// 画曲线
	if(pLayer->plinedata.haHandle)
	{
		
		nCount = GUI_ARRAY_GetNumItems(&pLayer->plinedata);
		for(i=0;i<nCount;i++)
		{
			pGPline=(GPLine *)GUI_ARRAY_GetpItem(&pLayer->plinedata,i);
			if(pGPline)
			{
				pGPline->baselabel.pCoordMap = pLayer->pCoordMap;
				
				Pline_Draw(pGPline,nDrawMode,FALSE,brush);
			}
		}
	}
	
	if(pLayer->linedata.haHandle)
	{
		// 画直线
		nCount = GUI_ARRAY_GetNumItems(&pLayer->linedata);
		for(i=0;i<nCount;i++)
		{
			pGline=(GLine *)GUI_ARRAY_GetpItem(&pLayer->linedata,i);
			if(pGline)
			{
				pGline->baselabel.pCoordMap = pLayer->pCoordMap;
				GLine_Draw(pGline,nDrawMode);
			}
		}
	}
	if(pLayer->mypointdata.haHandle)
	{
		// 画点
		nCount = GUI_ARRAY_GetNumItems(&pLayer->mypointdata);
		for(i=0;i<nCount;i++)
		{
			pMyPoint=(MyPoint *)GUI_ARRAY_GetpItem(&pLayer->mypointdata,i);
			if(pMyPoint)
			{
				pMyPoint->baselabel.pCoordMap = pLayer->pCoordMap;
				MyPoint_Draw(pMyPoint,nDrawMode);
			}
		}
	}
}
void Layer_DrawLabel(GMapLayer *pLayer)
{

	// 画多边形
	GRegion *pGRegion;
	GPLine  *pGPline;
	GLine   *pGline;
    MyPoint *pMyPoint;
	INT32U i;
	int nCount=0;
	if(pLayer->regiondata.haHandle)
	{
		nCount= GUI_ARRAY_GetNumItems(&pLayer->regiondata);//m_PlineRgnVector.size();

		for(i=0;i<nCount;i++)
		{
			pGRegion=(GRegion *)GUI_ARRAY_GetpItem(&pLayer->regiondata,i);
			if(pGRegion)
			{
				pGRegion->gpline.baselabel.pCoordMap = pLayer->pCoordMap;
				PlineRgn_DrawLabel(pGRegion);
			}
		}
	}
	if(pLayer->plinedata.haHandle)
	{
		// 画曲线
		nCount = GUI_ARRAY_GetNumItems(&pLayer->plinedata);
		for(i=0;i<nCount;i++)
		{
			pGPline=(GPLine *)GUI_ARRAY_GetpItem(&pLayer->plinedata,i);
			if(pGPline)
			{
				pGPline->baselabel.pCoordMap = pLayer->pCoordMap;
				Pline_DrawLabel(pGPline);
			}
		}
	}
	
	// 画直线
	if(pLayer->linedata.haHandle)
	{
		nCount = GUI_ARRAY_GetNumItems(&pLayer->linedata);
		for(i=0;i<nCount;i++)
		{
			pGline=(GLine *)GUI_ARRAY_GetpItem(&pLayer->linedata,i);
			if(pGline)
			{
				pGline->baselabel.pCoordMap = pLayer->pCoordMap;
				GLine_DrawLabel(pGline);
			}
		}
	}
	// 画点
	if(pLayer->mypointdata.haHandle)
	{
		nCount = GUI_ARRAY_GetNumItems(&pLayer->mypointdata);
		for(i=0;i<nCount;i++)
		{
			pMyPoint=(MyPoint *)GUI_ARRAY_GetpItem(&pLayer->mypointdata,i);
			if(pMyPoint)
			{
				pMyPoint->baselabel.pCoordMap = pLayer->pCoordMap;
				MyPoint_DrawLabel(pMyPoint);
			}
		}
	}
}
/*
void CLayer::AddPLineRgn(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,int Numble, mapMAPPOINTSTRUCT* PointList,mapPENSTRUCT Pen,bool bTransparent,bool bFill,mapBRUSHSTRUCT Brush)
{
	CPlineRgn* p_Plinergn = new CPlineRgn(pszLabel,nLayer, nType, nSubType,Numble,PointList,Pen,bTransparent,bFill,Brush);
	if(p_Plinergn)
		m_PlineRgnVector.push_back(p_Plinergn);
}

void CLayer::AddPoint(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,FP32 X,FP32 Y,mapSYMBOLSTRUCT Symbol)
{
	CMyPoint* p_Point = new CMyPoint( pszLabel, nLayer,  nType, nSubType,X, Y, Symbol);
	if(p_Point)
		m_MyPointVector.push_back(p_Point);
}
void CLayer::AddSpeedPoint(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,FP32 X,FP32 Y,mapSYMBOLSTRUCT Symbol,int nMaxSpeed,int nMinSpeed)
{
	CSpeedPoint* pPoint = new CSpeedPoint();
	if(pPoint)
		m_SpeedPointVector.push_back(pPoint);
}
void CLayer::AddPLine(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,int Numble, mapMAPPOINTSTRUCT* PointList,mapPENSTRUCT Pen)
{
	CPline* p_Pline = new CPline( pszLabel, nLayer,  nType, nSubType,Numble,  PointList, Pen);
	if(p_Pline)
		m_PlineVector.push_back(p_Pline);
}
	float m_minX,m_minY,m_maxX,m_maxY;
	m_minX=(float)1E20;m_minY=(float)1E20;m_maxX=-(float)1E20;m_maxY=-(float)1E20;
	// 画多边形
	CDraw* pDraw = NULL;
	int nCount =(int) m_PlineRgnVector.size();
	for(int i=0;i<nCount;i++)
	{
		pDraw = (CDraw*)m_PlineRgnVector[i];
		if(pDraw)
		{
			pDraw->GetRect(&m_minX,&m_minY,&m_maxX,&m_maxY);
			*m_Xmin = min(*m_Xmin,m_minX);
			*m_Xmax = max(*m_Xmax,m_maxX);
			*m_Ymin = min(*m_Ymin,m_minY);
			*m_Ymax = max(*m_Ymax,m_maxY);
		}
	}

	// 画曲线
	pDraw = NULL;
	nCount = (int)m_PlineVector.size();

	for(int j=0;j<nCount;j++)
	{
		pDraw = (CDraw*)m_PlineVector[j];
		if(pDraw)
		{
			pDraw->GetRect(&m_minX,&m_minY,&m_maxX,&m_maxY);
			*m_Xmin = min(*m_Xmin,m_minX);
			*m_Xmax = max(*m_Xmax,m_maxX);
			*m_Ymin = min(*m_Ymin,m_minY);
			*m_Ymax = max(*m_Ymax,m_maxY);
		}
	}
	// 画直线
	pDraw = NULL;
	nCount = (int)m_LineVector.size();
	for(int k=0;k<nCount;k++)
	{
		pDraw = (CDraw*)m_LineVector[k];
		if(pDraw)
		{
			pDraw->GetRect(&m_minX,&m_minY,&m_maxX,&m_maxY);
			*m_Xmin = min(*m_Xmin,m_minX);
			*m_Xmax = max(*m_Xmax,m_maxX);
			*m_Ymin = min(*m_Ymin,m_minY);
			*m_Ymax = max(*m_Ymax,m_maxY);;
		}
	}
	// 画点
	pDraw = NULL;
	nCount = (int)m_MyPointVector.size();
	for(int l=0;l<nCount;l++)
	{
		pDraw = (CDraw*)m_MyPointVector[l];
		if(pDraw)
		{
			pDraw->GetRect(&m_minX,&m_minY,&m_maxX,&m_maxY);
			*m_Xmin = min(*m_Xmin,m_minX);
			*m_Xmax = max(*m_Xmax,m_maxX);
			*m_Ymin = min(*m_Ymin,m_minY);
			*m_Ymax = max(*m_Ymax,m_maxY);
		}
	}
	return true;

*/
// 获取图层矩形边界
BOOLEAN Layer_GetRect(GMapLayer *pLayer,FP32 * m_Xmin, FP32 * m_Ymin, FP32 * m_Xmax, FP32 * m_Ymax)
{

	FP32 m_minX,m_minY,m_maxX,m_maxY;
	// 画多边形
	GRegion *pGRegion;
	GPLine  *pGPline;
	GLine   *pGline;
    MyPoint *pMyPoint;
	INT32U i;
	int nCount = GUI_ARRAY_GetNumItems(&pLayer->regiondata);//m_PlineRgnVector.size();
	m_minX=(FP32)1E20;m_minY=(FP32)1E20;m_maxX=-(FP32)1E20;m_maxY=-(FP32)1E20;

	for(i=0;i<nCount;i++)
	{
        pGRegion=(GRegion *)GUI_ARRAY_GetpItem(&pLayer->regiondata,i);
		if(pGRegion)
		{
			Pline_GetRect(&pGRegion->gpline,&m_minX,&m_minY,&m_maxX,&m_maxY);
			*m_Xmin = GISMIN(*m_Xmin,m_minX);
			*m_Xmax = GISMAX(*m_Xmax,m_maxX);
			*m_Ymin = GISMIN(*m_Ymin,m_minY);
			*m_Ymax = GISMAX(*m_Ymax,m_maxY);
		}
	}

	// 画曲线
	nCount = GUI_ARRAY_GetNumItems(&pLayer->plinedata);
	for(i=0;i<nCount;i++)
	{
        pGPline=(GPLine *)GUI_ARRAY_GetpItem(&pLayer->plinedata,i);
		if(pGPline)
		{
			Pline_GetRect(pGPline,&m_minX,&m_minY,&m_maxX,&m_maxY);
			*m_Xmin = GISMIN(*m_Xmin,m_minX);
			*m_Xmax = GISMAX(*m_Xmax,m_maxX);
			*m_Ymin = GISMIN(*m_Ymin,m_minY);
			*m_Ymax = GISMAX(*m_Ymax,m_maxY);
		}
	}
	
	
	// 画直线
	nCount = GUI_ARRAY_GetNumItems(&pLayer->linedata);
	for(i=0;i<nCount;i++)
	{
        pGline=(GLine *)GUI_ARRAY_GetpItem(&pLayer->linedata,i);
		if(pGline)
		{
   			GLine_GetRect(pGline,&m_minX,&m_minY,&m_maxX,&m_maxY);
			*m_Xmin = GISMIN(*m_Xmin,m_minX);
			*m_Xmax = GISMAX(*m_Xmax,m_maxX);
			*m_Ymin = GISMIN(*m_Ymin,m_minY);
			*m_Ymax = GISMAX(*m_Ymax,m_maxY);
		}
	}
	
	// 画点
	nCount = GUI_ARRAY_GetNumItems(&pLayer->mypointdata);
	for(i=0;i<nCount;i++)
	{
        pMyPoint=(MyPoint *)GUI_ARRAY_GetpItem(&pLayer->mypointdata,i);
		if(pMyPoint)
		{
			MyPoint_GetRect(pMyPoint,&m_minX,&m_minY,&m_maxX,&m_maxY);
			*m_Xmin = GISMIN(*m_Xmin,m_minX);
			*m_Xmax = GISMAX(*m_Xmax,m_maxX);
			*m_Ymin = GISMIN(*m_Ymin,m_minY);
			*m_Ymax = GISMAX(*m_Ymax,m_maxY);
		}
	}

	return TRUE;
}

BOOLEAN Layer_SearchMap(GMapLayer *pLayer,GISCHAR* szKey, MyPoint *ppoints, int  *nCount)
{
	int l,count=0,npointnum=0;
   	GISCHAR *lpszLabel;
	//MyPoint myPoint;
	MyPoint *pMyPoint,*pMyPointDes;
	pMyPointDes=ppoints;    
	*nCount = 0;
	if(!pLayer->mypointdata.haHandle) return FALSE;
    npointnum=GUI_ARRAY_GetNumItems(&pLayer->mypointdata);
	for(l=0;l<npointnum;l++)
	{
		pMyPoint=GUI_ARRAY_GetpItem(&pLayer->mypointdata,l);
        if(pMyPoint->baselabel.text)
		{
	       WM_LOCK();
	       lpszLabel=(GISCHAR*) GUI_ALLOC_h2p(pMyPoint->baselabel.text);
           WM_UNLOCK();
	       if(strlen(lpszLabel))
		   {
			   if(strstr(lpszLabel,szKey)!=NULL)
			 {
                 if(count>=100)
		     		break;
				 MyPointSetValue(pMyPointDes++,pMyPoint);

		        // memcpy(ppoints[nCount],pMyPoint,sizeof(MyPoint));
				 count++;
			 }
		   }
		}
	}
	*nCount =count;
	if(*nCount>0)

	   return TRUE;
	else
       return FALSE;
}

/*
// 获取我在哪里的文字描述
BOOLEAN Layer_GetWhereIam(mapMAPPOINTSTRUCT* pt, TCHAR* szWhere)
{
	PlineRgn* pDraw = NULL;
	int nCount = (int)m_PlineRgnVector.size();
	
	for(int i=0;i<nCount;i++)
	{
		pDraw = (CPlineRgn*)m_PlineRgnVector[i];
		if(pDraw)
		{
			if(pDraw->PtInMe(pt))
			{
				if(lstrlen(szWhere)>0)
				{
					lstrcat(szWhere,_T(";"));
				}
				lstrcat(szWhere,pDraw->GetLabel());
				return true;
			}
		}
	}
	return false;
}
*/
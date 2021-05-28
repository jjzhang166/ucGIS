// MyMapDoc.cpp: implementation of the CMyMapDoc class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"	
#include "MyMapDoc.h"
#include "wingdi.h"
#include "TCHAR.h"
#include ".\mymapdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyMapDoc::CMyMapDoc()
{
	//memset(m_lpszMapName,0,sizeof(m_lpszMapName));
	lstrcpy(m_lpszMapName , _T("δ����"));
	memset(m_pszMapFilePath,0,sizeof(m_pszMapFilePath));
    GridHaveBuild=0;
	m_nMappingMode = 1;
	m_nCurrentLayer = 0;
	m_bShowAllLabel = false;
    
	m_mapCoordMap = NULL;
	//m_fCenterX = (float)104.1878;
	//m_fCenterY = (float)30.587;
	m_toolConstant = miPanTool;
	m_bShowMapName = false;
	m_bDataChanged = false;
#ifdef UNDER_WINCE
	m_ceDraw = NULL;
#endif

	m_hMemDC = CreateCompatibleDC(NULL);
	hbmNew = NULL;


	//m_LayerVector.
	InitializeCriticalSection(&m_csDraw);

}

CMyMapDoc::~CMyMapDoc()
{   

	DeleteDC(m_hMemDC);
	DeleteObject(hbmNew);
	RealseMap();
	DeleteCriticalSection(&m_csDraw);
}
void CMyMapDoc::RealseMap()
{
    int i=0;
	//if(m_bDataChanged)
	//	WriteToFile();

	int nCount = m_LayerVector.size();

	CLayer* pLayer = NULL;
	for(i=0;i<nCount;i++)
	{
		pLayer = ((CLayer*)m_LayerVector[i]);
		if(pLayer)
			delete pLayer;
		pLayer = NULL;
	}
	m_LayerVector.clear();
	RealeaseGridIndex();
	RealeaseGridIndexIndex();
}
void CMyMapDoc::RealeaseGridIndex()
{
	int nCount = gridvector.size();
	CGisGrid *pGrid;
	for(int i=0;i<nCount;i++)
	{
      pGrid=(CGisGrid*)gridvector[i];
	  if(pGrid)
	  {
		  pGrid->~CGisGrid();
	      delete pGrid;
	  }
	  pGrid=NULL;
	}
	gridvector.clear();

}
void CMyMapDoc::RealeaseGridIndexIndex()
{
	int nCount = cgridindexvector.size();
	CGridIndex *pGridIndex;
	for(int i=0;i<nCount;i++)
	{
      pGridIndex=(CGridIndex*)cgridindexvector[i];
	  if(pGridIndex)
	  {
		  pGridIndex->~CGridIndex();
	      delete pGridIndex;
	  }
	  pGridIndex=NULL;
	}
	cgridindexvector.clear();

}

// ��Unicodeת����Ansi
bool CMyMapDoc::WToA(TCHAR* wChar, char* cChar)
{

	int multibytelen=WideCharToMultiByte(  //�����Unicodeת����Ansi����Ҫ���ֽ���
		CP_ACP,  //����ANSI code pageת��
		WC_COMPOSITECHECK | WC_DEFAULTCHAR, //ת��������ȱʡ�ַ�����
		wChar,  //Ҫת�����ַ�����ַ
		lstrlen(wChar),  //Ҫת���ĸ���
		0,  //ת�����ַ������õĵ�ַ
		0,  //���ת���ַ��ĸ�����Ϊ0��ʾ����ת��Unicode����Ҫ���ٸ��ֽ�
		0,  //ȱʡ���ַ�:"\0"
		0   //ȱʡ������
		);
	if(multibytelen > 0)
	{
		WideCharToMultiByte(  //ת��Unicode��Ansi
			CP_ACP,
			WC_COMPOSITECHECK | WC_DEFAULTCHAR,
			wChar,
			lstrlen(wChar),
			(char *)cChar,  //ת������������
			128,  //���128���ֽ�
			0,
			0);
	}
	cChar[multibytelen] = '\0';
	return true;
}
// ���ļ���ȡ����
bool CMyMapDoc::ReadFromFile(TCHAR* pszFileName)
{
	lstrcpy(m_pszMapFilePath,pszFileName);
	char path[255];
	int object_fileoffset=0;
	WToA(pszFileName,path);
	// ��ʾæ���
	SetCursor (LoadCursor (NULL, IDC_WAIT));

	FILE	*fp;
	fp = fopen(path,"rb");
	if(!fp)	return false;
	fread(&m_fCenterX,sizeof(float),1,fp);  // 
	object_fileoffset+=sizeof(float);
	fread(&m_fCenterY,sizeof(float),1,fp);  // 
	object_fileoffset+=sizeof(float);
	fread(&scale,sizeof(float),1,fp);  // 
	object_fileoffset+=sizeof(float);
	fread(&m_lpszMapName,sizeof(m_lpszMapName),1,fp);  // 
	object_fileoffset+=sizeof(m_lpszMapName);

	int nCount ;
	fread(&nCount,sizeof(int),1,fp);  // д�������
	object_fileoffset+=sizeof(int);
	for(int i=0;i<nCount;i++)
	{
		CLayer* layer = new CLayer;
		m_LayerVector.push_back(layer);
		m_LayerVector[i]->ReadFromFile(fp,object_fileoffset);
		m_nCurrentLayer = m_LayerVector.size()-1;
		layer->m_ID = m_nCurrentLayer;
	}
	fclose(fp);

	//m_mapCoordMap->m_bMapChanged = true;
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	return true;
}
// �������ݵ��ļ�
void CMyMapDoc::WriteToFile(TCHAR* pszFileName)
{
	SetCursor (LoadCursor (NULL, IDC_WAIT));
	lstrcpy(m_pszMapFilePath,pszFileName);
	char path[255];
	WToA(pszFileName,path);
	FILE	*fp;
	fp = fopen(path,"wb");
	if(!fp)	return;
	fwrite(&m_mapCoordMap->m_fCenterX,sizeof(float),1,fp);  // 
	fwrite(&m_mapCoordMap->m_fCenterY,sizeof(float),1,fp);  // 
	fwrite(&m_mapCoordMap->scale,sizeof(float),1,fp);  // 
	fwrite(&m_lpszMapName,sizeof(m_lpszMapName),1,fp);  // 

	int nCount = m_LayerVector.size();

	fwrite(&nCount,sizeof(int),1,fp);  // д�������
	for(int i=0;i<nCount;i++)
	{
		((CLayer*)m_LayerVector[i])->WriteToFile(fp);
	}
	fclose(fp);
	m_bDataChanged = false;
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}
void CMyMapDoc::GetPointFromRect(CMyPoint* points,int& nPointCount)
{
	nPointCount = 0;
	int nEyeShot = m_mapCoordMap->scale*32*100000;
	int nCount = m_LayerVector.size();
	for(int i=0;i<nCount;i++)
	{
		CLayer* layer = ((CLayer*)m_LayerVector[i]);
		layer->m_mapCoordMap = m_mapCoordMap;

		if (m_LayerVector[i]->m_nEyeShotMax==0||nEyeShot>=m_LayerVector[i]->m_nEyeShotMin&&nEyeShot<=m_LayerVector[i]->m_nEyeShotMax)
			layer->GetPointFromRect(points,nPointCount);

	}
}
bool CMyMapDoc::DelPoint(TCHAR* szLabel)
{
	int nEyeShot = m_mapCoordMap->scale*32*100000;
	int nCount = m_LayerVector.size();
	for(int i=0;i<nCount;i++)
	{
		CLayer* layer = ((CLayer*)m_LayerVector[i]);
		layer->m_mapCoordMap = m_mapCoordMap;

		if (m_LayerVector[i]->m_nEyeShotMax==0||nEyeShot>=m_LayerVector[i]->m_nEyeShotMin&&nEyeShot<=m_LayerVector[i]->m_nEyeShotMax)
			return layer->DelPoint(szLabel);

	}
	return false;
}
// ���Ƶ�ͼ
void CMyMapDoc::Draw(HDC hDC, int nDrawMode)
{
	bool bDsp=true;
	EnterCriticalSection(&m_csDraw);

	// ��������ߣ���λΪ ����
	//int nEyeShot = max(m_mapCoordMap->wScreen,m_mapCoordMap->hScreen)*m_mapCoordMap->scale*100;
	int nEyeShot = m_mapCoordMap->scale*32*100000;
	int nCount = m_LayerVector.size();
	for(int i=0;i<nCount;i++)
	{
		CLayer* layer = ((CLayer*)m_LayerVector[i]);
		layer->m_mapCoordMap = m_mapCoordMap;
#ifdef UNDER_WINCE
		layer->m_ceDraw = m_ceDraw;
#endif
		//if(i==6)
		//	layer->SetDisplay(bDsp);
		if (m_LayerVector[i]->m_nEyeShotMax==0||nEyeShot>=m_LayerVector[i]->m_nEyeShotMin&&nEyeShot<=m_LayerVector[i]->m_nEyeShotMax)
			layer->Draw(hDC,nDrawMode);

	}		

	// ����ע
	for(int j=0;j<nCount;j++)
	{
		CLayer* layer = ((CLayer*)m_LayerVector[j]);
		layer->m_mapCoordMap = m_mapCoordMap;
		layer->m_bShowAllLabel = m_bShowAllLabel;
#ifdef UNDER_WINCE
		layer->m_ceDraw = m_ceDraw;
#endif
		//	if (m_LayerVector[j]->m_nEyeShotMax==0||nEyeShot>=m_LayerVector[j]->m_nEyeShotMin&&nEyeShot<=m_LayerVector[j]->m_nEyeShotMax)
		if (m_LayerVector[j]->m_nEyeShotMax==0||
			nEyeShot>=m_LayerVector[j]->m_nEyeShotMin&&
			nEyeShot<=m_LayerVector[j]->m_nEyeShotMax)
			layer->DrawLabel(hDC);
	}
    //������
	if(GridHaveBuild)
	{
		nCount=(int)gridvector.size();
	    for(int k=0;k<nCount;k++)
		{
			CGisGrid *pgisGrid=(CGisGrid *)gridvector[k];
			if(pgisGrid)
			{
              pgisGrid->Draw(hDC); 
			}
		}
		nCount=(int)cgridindexvector.size();
	    for(int l=0;l<nCount;l++)
		{
			CGridIndex *pcGridindex=(CGridIndex *)cgridindexvector[l];
			if(pcGridindex)
			{
              pcGridindex->Draw(hDC,m_mapCoordMap); 
			}
		}



	}

#ifdef UNDER_WINCE
	/*	// ��ʾ��ͼ����
	if(m_bShowMapName) 
	{
	POINT ptText = { 0, 60 };
	m_ceDraw->DrawText( &ptText, (LPCTSTR)lpszMapName,lstrlen(lpszMapName), 1 ); // Draw text with pen color
	}
	*/
	// End Draw
	m_ceDraw->EndDraw();

#endif

	LeaveCriticalSection(&m_csDraw);


}
// ���һ���㣬���ظò��������
short CMyMapDoc::AddLayer(CLayer *pLayer)
{
	m_LayerVector.push_back(pLayer);
	// ����Ϊ��ǰͼ��
	m_nCurrentLayer = m_LayerVector.size()-1;
	pLayer->m_ID = m_nCurrentLayer;
	return m_nCurrentLayer;
}

CPline* CMyMapDoc::AddPLine(TCHAR *pszLabel, short nLayer, ObjectTypeConstants nType,ObjectTypeConstants nSubType,int nPointCount, mapMAPPOINTSTRUCT *PointList, mapPENSTRUCT Pen)
{

	return ((CLayer*)m_LayerVector[m_nCurrentLayer])->AddPLine( pszLabel,  nLayer,  nType, nSubType,nPointCount,  PointList, Pen);
}

CPlineRgn* CMyMapDoc::AddPLineRgn(TCHAR *pszLabel, short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType, int nPointCount, mapMAPPOINTSTRUCT *PointList, mapPENSTRUCT Pen, bool bTransparent, bool bFill, mapBRUSHSTRUCT Brush)
{

	return ((CLayer*)m_LayerVector[m_nCurrentLayer])->AddPLineRgn(pszLabel,nLayer, nType, nSubType,nPointCount,PointList, Pen,bTransparent,bFill,Brush);
}

CMyPoint* CMyMapDoc::AddPoint(TCHAR *pszLabel, short nLayer, ObjectTypeConstants nType,ObjectTypeConstants nSubType,float X, float Y)
{

	return ((CLayer*)m_LayerVector[m_nCurrentLayer])->AddPoint(pszLabel, nLayer,  nType, nSubType,X, Y);
}
CLine* CMyMapDoc::AddLine(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,float x1,float y1,float x2,float y2,mapPENSTRUCT Pen)
{

	return ((CLayer*)m_LayerVector[m_nCurrentLayer])->AddLine(pszLabel, nLayer,  nType, nSubType,x1,y1,x2,y2, Pen);
}
// ��ȡ����״̬���Դ��ݸ�ͼ�����öԻ���
LayerStruct* CMyMapDoc::GetLayersState(int& nLen)
{
	int nCount = m_LayerVector.size();
	LayerStruct* pState = new LayerStruct[nCount];
	nLen = nCount;
	for(int i=0;i<nCount;i++)
	{
		pState[i].AutoLabel = m_LayerVector[i]->m_bAutoLabel;
		pState[i].Editable = m_LayerVector[i]->m_bEditable;
		lstrcpy(pState[i].Name , m_LayerVector[i]->m_pszName);
		pState[i].Visible = m_LayerVector[i]->m_bVisible;
		pState[i].nEyeShotMin = m_LayerVector[i]->m_nEyeShotMin;
		pState[i].nEyeShotMax = m_LayerVector[i]->m_nEyeShotMax;

		pState[i].id = m_LayerVector[i]->m_ID;
		pState[i].l = m_LayerVector[i];
	}
	return pState;
}

// ͨ������ͼ�����öԻ��򴫵ݵĲ���������ͼ�������
void CMyMapDoc::SetLayersState(LayerStruct* pState)
{
	int nCount = m_LayerVector.size();
	for(int i=0;i<nCount;i++)
	{
		m_LayerVector[i] = pState[i].l;

	}
	for(int i=0;i<nCount;i++)
	{
		if(m_LayerVector[i]->m_ID == pState[i].id)
			//if(lstrcmp(m_LayerVector[i]->m_pszName , pState[i].Name)==0)
		{
			m_LayerVector[i]->m_bAutoLabel  = pState[i].AutoLabel;
			m_LayerVector[i]->m_bEditable   = pState[i].Editable;
			m_LayerVector[i]->m_bVisible    = pState[i].Visible;
			m_LayerVector[i]->m_nEyeShotMin = pState[i].nEyeShotMin;
			m_LayerVector[i]->m_nEyeShotMax = pState[i].nEyeShotMax;
		}

	}
	m_bDataChanged = true;
}

// ��ȡͼ����α߽�
bool CMyMapDoc::GetRect(float * m_Xmin, float * m_Ymin, float * m_Xmax, float * m_Ymax)
{
	float m_minX,m_minY,m_maxX,m_maxY;
	m_minX=(float)1E20;m_minY=(float)1E20;m_maxX=-(float)1E20;m_maxY=-(float)1E20;
	// �������
	CLayer* pLayer = NULL;
	int nCount = m_LayerVector.size();
	for(int i=0;i<nCount;i++)
	{
		pLayer = (CLayer*)m_LayerVector[i];
		if(pLayer)
		{
			pLayer->GetRect(&m_minX,&m_minY,&m_maxX,&m_maxY);

			*m_Xmin = min(*m_Xmin,m_minX);
			*m_Xmax = max(*m_Xmax,m_maxX);
			*m_Ymin = min(*m_Ymin,m_minY);
			*m_Ymax = max(*m_Ymax,m_maxY);
		}
	}
	return true;
}
//void CMyMapDoc::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	switch(m_toolConstant)
//	{
//	case miArrowTool:
//			 break;
//	case miCenterTool:
//			 break;
//	case miZoomInTool:
//			 break;
//	case miZoomOutTool:
//			 break;
//	case miPanTool:
//			 break;
//	default:
//		;
//	}
//}
//void CMyMapDoc::OnMouseMove(UINT nFlags, CPoint point)
//{
//	switch(m_toolConstant)
//	{
//	case miArrowTool:
//			 break;
//	case miCenterTool:
//			 break;
//	case miZoomInTool:
//			 break;
//	case miZoomOutTool:
//			 break;
//	case miPanTool:
//			 break;
//	default:
//		;
//	}
//}
//
//void CMyMapDoc::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	switch(m_toolConstant)
//	{
//	case miArrowTool:
//			 break;
//	case miCenterTool:
//			 break;
//	case miZoomInTool:
//			 break;
//	case miZoomOutTool:
//			 break;
//	case miPanTool:
//			 break;
//	default:
//		;
//	}
//}
//




void CMyMapDoc::ShowAllMap()
{
	float minx,miny,maxx,maxy,bl1;

	//����Сֵ���ǳ��󣬰����ֵ���ǳ�С��Ϊ�˶Ա������г��Ի�
	minx=(float)1E20;miny=(float)1E20;maxx=-(float)1E20;maxy=-(float)1E20;

	GetRect(&minx,&miny,&maxx,&maxy);

	//if(!pb)		//û��ͼ��Ԫ��,���򷵻�
	//	return;
	bl1=(maxx-minx)/(m_mapCoordMap->wScreen);//����ı���
	m_mapCoordMap->scale=(maxy-miny)/(m_mapCoordMap->hScreen);//ʵ���������
	if(bl1>m_mapCoordMap->scale) m_mapCoordMap->scale=bl1;  //ȡ�ϴ��һ����Ϊ������
	/*����ȷ����Ļ���½ǵ�ʵ�����꣬���������²඼����10���������
	��minx,miny)����Ļ�����½�����10����ͬʱ����10���󴦡�*/
	m_mapCoordMap->xStart=minx-10*m_mapCoordMap->scale;
	m_mapCoordMap->yStart=miny-10*m_mapCoordMap->scale;
	//��ȫͼ����ʾ��������������������Ļ��Ϊ����


	//// �����ĵ����ĵ�ͼ����
	float fCenterX = m_mapCoordMap->xStart + m_mapCoordMap->VLtoDL(m_mapCoordMap->wScreen/2);
	float fCenterY = m_mapCoordMap->yStart + m_mapCoordMap->VLtoDL(m_mapCoordMap->hScreen/2);
	m_mapCoordMap->SetCenter(fCenterX,fCenterY);
}

// ɾ��ָ��Ԫ��
bool CMyMapDoc::DeleteDraw(short nLayer, CDraw* pDr)
{
	int nCount = m_LayerVector.size();
	if((nLayer>=nCount)||(nLayer<0)) return false;
	CLayer* pLayer = NULL;
	pLayer = (CLayer*)m_LayerVector[nLayer];
	if(pLayer)
	{
		return pLayer->DeleteDraw(pDr);
	}
	return false;
}
// ɾ��ָ��Ԫ��
bool CMyMapDoc::DeleteDraw( CDraw* pDr)
{
	int nCount = m_LayerVector.size();
	CLayer* pLayer = NULL;
	for(int i=0;i<nCount;i++)
	{
		pLayer = ((CLayer*)m_LayerVector[i]);
		if(pLayer)
		{
			if(pLayer->DeleteDraw(pDr))
			{
				return true;
			}	
		}
	}
	return false;
}

// ��ȡ�����������������
bool CMyMapDoc::GetWhereIam(mapMAPPOINTSTRUCT* pt, TCHAR* szWhere)
{
	CLayer* pLayer = NULL;
	int nCount = m_LayerVector.size();

	for(int i=0;i<nCount;i++)
	{
		pLayer = (CLayer*)m_LayerVector[i];
		if(pLayer)
		{
			if(lstrcmp(pLayer->m_pszName,_T("�й�ʡ������"))==0)
			{
				pLayer->GetWhereIam(pt,szWhere);
			}
			else if(lstrcmp(pLayer->m_pszName,_T("�й�-�Ĵ�ʡ-��������"))==0)
			{
				pLayer->GetWhereIam(pt,szWhere);
			}
			else if(lstrcmp(pLayer->m_pszName,_T("�й�-�Ĵ�ʡ-�ؼ�����"))==0)
			{
				pLayer->GetWhereIam(pt,szWhere);
			}
			//if(pLayer->m_pszName
		}
	}
	return false;
}

// ��Ļ����ֱ������ �ھͷ��ط�0������
// ��Ļ���꣬��Ļ���꣬ѡ�еĵ㣬��ǰһ���㣬���һ����
bool CMyMapDoc::IsOnTheLine(int x, int y,mapMAPPOINTSTRUCT* mpt,mapMAPPOINTSTRUCT* mpt1,mapMAPPOINTSTRUCT* mpt2, CPline** pLine)
{
	int nCount = m_LayerVector.size();

	for(int i=0;i<nCount;i++)
	{
		CLayer* pLayer = 0;
		pLayer = ((CLayer*)m_LayerVector[i]);
		if(pLayer)
		{
			bool r = pLayer->IsOnTheLine(x,y,mpt,mpt1,mpt2,pLine);
			if(r)
			{
				return true;
			}
		}
	}
	return false;
}
bool CMyMapDoc::DeletePoint(mapMAPPOINTSTRUCT* mpt)
{
	int nCount = m_LayerVector.size();

	for(int i=0;i<nCount;i++)
	{
		CLayer* pLayer = 0;
		pLayer = ((CLayer*)m_LayerVector[i]);
		if(pLayer)
		{
			bool r = pLayer->DeletePoint(mpt);
			if(r)
			{
				return true;
			}
		}
	}
	return false;
}
// ����һ���㵽����
void CMyMapDoc::AddPointToLine(int x,int y)
{
	int nCount = m_LayerVector.size();

	for(int i=0;i<nCount;i++)
	{
		CLayer* pLayer = 0;
		pLayer = ((CLayer*)m_LayerVector[i]);
		if(pLayer)
		{
			pLayer->AddPointToLine(x,y);
		}
	}
}
void CMyMapDoc::SetDrawType(bool bEditMode)
{
	int nCount = m_LayerVector.size();

	for(int i=0;i<nCount;i++)
	{
		CLayer* pLayer = 0;
		pLayer = ((CLayer*)m_LayerVector[i]);
		if(pLayer)
		{
			pLayer->SetDrawType(bEditMode);
		}
	}
}
/*
#define GRIDINDEXNUM 4

void CMyMapDoc::Oninitgrid() 
{
	int m=0;//�����к�
	int n=0;//�����к�
	long x,y;
	int nwidth,nheight;
	float minx,miny,maxx,maxy;

	//����Сֵ���ǳ��󣬰����ֵ���ǳ�С��Ϊ�˶Ա������г��Ի�
	minx=(float)1E20;miny=(float)1E20;maxx=-(float)1E20;maxy=-(float)1E20;
    GetRect(&minx,&miny,&maxx,&maxy);   
	m_mapCoordMap->DPtoVP(minx,miny,&GridDrawRect.left,&GridDrawRect.top);
	m_mapCoordMap->DPtoVP(maxx,maxy,&GridDrawRect.right,&GridDrawRect.bottom);
	nwidth=GridDrawRect.right-GridDrawRect.left;
	nheight=GridDrawRect.bottom-GridDrawRect.top;
	//CGridIndex::GridWidth=abs(nwidth)/12;
	//CGrid::GridHeigth=abs(nheight)/12;
	char ss[100];
	TCHAR swChar[100];
	sprintf(ss,"������=%d ������=%d",CGisGrid::GridWidth,CGisGrid::GridHeigth);
	AToW(ss,swChar);
	AfxMessageBox(swChar);
    int gridindexnum=0,indexcol,indexrow,currgridnum;
	Tindexcols=0;
	Tindexrows=0;
	for (y=GridDrawRect.bottom;y<GridDrawRect.top;y=y+(CGisGrid::GridHeigth))
	{
		for (long x=GridDrawRect.left;x<GridDrawRect.right;x=x+(CGisGrid::GridWidth))
		{
			CGisGrid* pgrid=new CGisGrid(&GridDrawRect,m,n);
		    m_mapCoordMap->VPtoDP(pgrid->ExtentRect.left,pgrid->ExtentRect.top,&pgrid->mapExtentRect.fMinX,&pgrid->mapExtentRect.fMinY);
		    m_mapCoordMap->VPtoDP(pgrid->ExtentRect.right,pgrid->ExtentRect.bottom,&pgrid->mapExtentRect.fMaxX,&pgrid->mapExtentRect.fMaxY);
			indexcol=n/(GRIDINDEXNUM);
            Tindexrows=m/(GRIDINDEXNUM);
           // Tindexrpws=
			currgridnum=Tindexcols*Tindexrows+indexcol;
			if(gridindexnum<currgridnum||gridindexnum==currgridnum)
			{
			  CGridIndex* pcgridindex=new CGridIndex ;
		      pcgridindex->mapExtentRect.fMinX=pgrid->mapExtentRect.fMinX;
		      pcgridindex->mapExtentRect.fMinY=pgrid->mapExtentRect.fMinY;
			  cgridindexvector.push_back(pcgridindex);
			  gridindexnum++;
			}
		    CGridIndex* pcgridindex=cgridindexvector[currgridnum];
            GRIDINDEX* pgridindex=new GRIDINDEX;
			pgridindex->arrayIndex=m*Tcols+n;
			pcgridindex->gridindexvector.push_back(pgridindex);
			if(((m+1)%GRIDINDEXNUM==0)&&((n+1)%GRIDINDEXNUM==0))
			{
			  CGridIndex* pcgridindex=cgridindexvector[currgridnum] ;
			  pcgridindex->mapExtentRect.fMaxX=pgrid->mapExtentRect.fMaxX;
			  pcgridindex->mapExtentRect.fMaxY=pgrid->mapExtentRect.fMaxY;
			  //gridindexvector.push_back(pgridindex);
			}

			n++;
			gridvector.push_back(pgrid);
		  
			 
		}
		if(n%GRIDINDEXNUM!=0)
		{
			if(((m+1)%GRIDINDEXNUM)==0)
			{
			  //indexcol=(n-1)/(GRIDINDEXNUM);
              //indexrow=m/(GRIDINDEXNUM);
		      CGridIndex* pcgridindex=cgridindexvector[currgridnum];
			  CGisGrid* pgrid=gridvector[m*Tcols+n-1];
			  pcgridindex->mapExtentRect.fMaxX=pgrid->mapExtentRect.fMaxX;
			  pcgridindex->mapExtentRect.fMaxY=pgrid->mapExtentRect.fMaxY;
			  //gridindexvector.push_back(pgridindex);
			}
	   }
		Tindexcols=indexcol;
		Tcols=n;//���ظ�������
		n=0;
		m++;

	}
	Trows=m;
	
	char s[100];
	TCHAR wChar[100];
	sprintf(s,"��������=%d ��������=%d",Trows,Tcols);
	AToW(s,wChar);
	AfxMessageBox(wChar);
	
//	str.Format("��������=%d ��������=%d",Trows,Tcols);
	
//	AfxMessageBox(str);
	return;
	//::MessageBox(NULL,str,"����",MB_OK);

}
*/
#define GRIDINDEXNUM 4

void CMyMapDoc::Oninitgrid() 
{
	int m=0;//�����к�
	int n=0;//�����к�
	int mm=0;//�����к�
	int nn=0;//�����к�

	long x,y;
	int nwidth,nheight;
	float minx,miny,maxx,maxy;
    Tcols=0;
    Trows=0;
	//����Сֵ���ǳ��󣬰����ֵ���ǳ�С��Ϊ�˶Ա������г��Ի�
	minx=(float)1E20;miny=(float)1E20;maxx=-(float)1E20;maxy=-(float)1E20;
    GetRect(&minx,&miny,&maxx,&maxy);   
	m_mapCoordMap->DPtoVP(minx,miny,&GridDrawRect.left,&GridDrawRect.top);
	m_mapCoordMap->DPtoVP(maxx,maxy,&GridDrawRect.right,&GridDrawRect.bottom);
	//nwidth=//GridDrawRect.right-GridDrawRect.left;
	//nheight=GridDrawRect.bottom-GridDrawRect.top;
	int GridIndexWidth=CGisGrid::GridWidth*GRIDINDEXNUM;
	int GridIndexHeigth=CGisGrid::GridHeigth*GRIDINDEXNUM;
	char ss[100];
	TCHAR swChar[100];
	sprintf(ss,"������=%d ������=%d",GridIndexWidth,GridIndexHeigth);
	AToW(ss,swChar);
	AfxMessageBox(swChar);
    int gridindexnum=0,indexcol,indexrow,currgridnum;
	Tindexcols=0;
	Tindexrows=0;
	for (y=GridDrawRect.bottom;y<GridDrawRect.top;y=y+GridIndexHeigth)
	{
		Tcols=0;
		for ( x=GridDrawRect.left;x<GridDrawRect.right;x=x+GridIndexWidth)
       {
			  CGridIndex* pcgridindex=new CGridIndex(&GridDrawRect,m,n,GridIndexHeigth,GridIndexWidth);
		      m_mapCoordMap->VPtoDP(pcgridindex->ExtentRect.left,pcgridindex->ExtentRect.top,&pcgridindex->mapExtentRect.fMinX,&pcgridindex->mapExtentRect.fMinY);
		      m_mapCoordMap->VPtoDP(pcgridindex->ExtentRect.right,pcgridindex->ExtentRect.bottom,&pcgridindex->mapExtentRect.fMaxX,&pcgridindex->mapExtentRect.fMaxY);
			  cgridindexvector.push_back(pcgridindex);
			  n++;
			  mm=0;
			  nn=0;
			  InitGrid(pcgridindex,&pcgridindex->ExtentRect,mm,nn);
			  Tcols+=nn;
		}
	    Trows+=mm;
		Tindexcols=n;//���ظ�������
		n=0;
		m++;

	}
	Tindexrows=m;
}

void CMyMapDoc::InitGrid(CGridIndex* pcgridindex,CRect *prect,int &irow,int &icol)
{
	int n=0,m=0;
	long x,y;
	static int gridnum=0;
	for (y=prect->top;y<prect->bottom;y=y+(CGisGrid::GridHeigth))
	{
		for ( x=prect->left;x<prect->right;x=x+(CGisGrid::GridWidth))
		{

            GRIDINDEX* pgridindex=new GRIDINDEX;
			CGisGrid* pgrid=new CGisGrid(prect,m,n);
		    m_mapCoordMap->VPtoDP(pgrid->ExtentRect.left,pgrid->ExtentRect.top,&pgrid->mapExtentRect.fMinX,&pgrid->mapExtentRect.fMinY);
		    m_mapCoordMap->VPtoDP(pgrid->ExtentRect.right,pgrid->ExtentRect.bottom,&pgrid->mapExtentRect.fMaxX,&pgrid->mapExtentRect.fMaxY);
			gridvector.push_back(pgrid);
			pgridindex->arrayIndex=gridnum++;//m*icol+n+Trows*Tcols;
			pcgridindex->gridindexvector.push_back(pgridindex);
			n++;

		}
		icol=n;//���ظ�������
		n=0;
		m++;

	}
	irow=m;
}

void CMyMapDoc::AToW(char *sourceChar,TCHAR *wChar)
{
	char	*p = sourceChar;
	int widecharlen=MultiByteToWideChar( //�����Ansiת����Unicode����Ҫ���ֽ���
		CP_ACP,
		MB_COMPOSITE,
		(char*)p, //Ҫת����Ansi�ַ���
		-1, //�Զ����㳤��
		0,
		0
        );

	MultiByteToWideChar( //��Ansiת����Unicode�ַ�
		CP_ACP,
		MB_COMPOSITE,
		(char*)p,
		-1,
		wChar,	// ת���� strReceive
		widecharlen // ���ת��widecharlen��Unicode�ַ�
        );

}
int  CMyMapDoc::GetMapFileHeadOffset()
{
	int fileoffset=sizeof(float)+sizeof(float)+sizeof(float)+sizeof(m_lpszMapName)+sizeof(int);
	return fileoffset;
	/*
	fwrite(&m_mapCoordMap->m_fCenterX,sizeof(float),1,fp);  // 
	fwrite(&m_mapCoordMap->m_fCenterY,sizeof(float),1,fp);  // 
	fwrite(&m_mapCoordMap->scale,sizeof(float),1,fp);  // 
	fwrite(&m_lpszMapName,sizeof(m_lpszMapName),1,fp);  // 
	fwrite(&nCount,sizeof(int),1,fp);  // д�������
	*/

}
void CMyMapDoc::IndexBuid()
{
	int k;
    int gridnuns;
	gridnuns=(int)gridvector.size();
	char s[100];
	TCHAR wChar[100];
	sprintf(s,"%d",gridnuns);
	AToW(s,wChar);
	AfxMessageBox(wChar);
	SetCursor (LoadCursor (NULL, IDC_WAIT));

	int nFileOffSet=0;//��ͼ�ļ����߼�ƫ�������������������
	nFileOffSet+=GetMapFileHeadOffset();
	/*
	fwrite(&m_mapCoordMap->m_fCenterX,sizeof(float),1,fp);  // 
	fwrite(&m_mapCoordMap->m_fCenterY,sizeof(float),1,fp);  // 
	fwrite(&m_mapCoordMap->scale,sizeof(float),1,fp);  // 
	fwrite(&m_lpszMapName,sizeof(m_lpszMapName),1,fp);  // 
	fwrite(&nCount,sizeof(int),1,fp);  // д�������
	*/
	int j=m_LayerVector.size();
	for (k=0;k<j;k++)
	{
		CLayer *player;
		player=m_LayerVector[k];
		player->IndexBuid(k,Tcols,gridvector,GridDrawRect,nFileOffSet);
	
	}
    GridHaveBuild=1;
	SetCursor(LoadCursor(NULL, IDC_ARROW));
    //Invalidate();
}

void CMyMapDoc::SaveGridIndex(TCHAR* pszFileName)
{

	if (GridHaveBuild!=0)
	{
	SetCursor (LoadCursor (NULL, IDC_WAIT));
	lstrcpy(m_pszMapFilePath,pszFileName);
	char path[255];
	WToA(pszFileName,path);
	FILE	*fp;
	fp = fopen(path,"wb");
	if(!fp)	return;
	int i=0,offset=0;
	fwrite(&Trows,sizeof(int),1,fp);  // 
	offset+=sizeof(int);
	fwrite(&Tcols,sizeof(int),1,fp);  //
	offset+=sizeof(int);
	int gridNum=(int)gridvector.size();
	fwrite(&gridNum,sizeof(int),1,fp);  //
	offset+=sizeof(int);
	for ( i=0;i<gridNum;i++)
	{

		CGisGrid* pgrid;
		pgrid=gridvector[i];
		pgrid->WriteToFile(fp,offset);
		
	}
	fclose(fp);
	/*
#define GRIDINDEXNUM 8
//	int gridindexnum=gridNum/GRIDINDEXNUM;
	CGridIndex gridindex;
	int cols,rows,gridindexnum,grid_index=0;
	//cols=Tcols/GRIDINDEXNUM;
    //rows=Trows/GRIDINDEXNUM;
    //gridindexnum=cols*rows;
	for ( i=0;i<Trows;i++)
	{
		for(j=0;j<Tcols;j++)
		{
		  CGridIndex* pgridindex=new CGridIndex ;
		  CGisGrid* pgrid;
		  grid_index=i*j+j;
		  pgrid=gridvector[i*j+j];
		  pgridindex->mapExtentRect.fMinX=pgrid->mapExtentRect.fMinX;
		  pgridindex->mapExtentRect.fMinX=pgrid->mapExtentRect.fMinY;
		  pgrid=gridvector[i*j+j];


		pgrid->WriteToFile(fp,offset);
		}
	}
	*/
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
}

void CMyMapDoc::ReadGridIndex(TCHAR* pszFileName)
{
    
	SetCursor (LoadCursor (NULL, IDC_WAIT));
	lstrcpy(m_pszMapFilePath,pszFileName);
	char path[255];
	WToA(pszFileName,path);
	FILE	*fp;
	fp = fopen(path,"rb");
	if(!fp)	return;
	fread(&Trows,sizeof(int),1,fp);  // 
	fread(&Tcols,sizeof(int),1,fp);  //
	int gridNum=0;
	fread(&gridNum,sizeof(int),1,fp);  //
	RealeaseGridIndex();
	for (int i=0;i<gridNum;i++)
	{
		CGisGrid* pgrid=new CGisGrid;
		pgrid->ReadFromFile(fp);
		int nminx,nminy,nmaxx,nmaxy;

	    m_mapCoordMap->DPtoVP(pgrid->mapExtentRect.fMinX,pgrid->mapExtentRect.fMinY,&nminx,&nminy);
	    m_mapCoordMap->DPtoVP(pgrid->mapExtentRect.fMaxX,pgrid->mapExtentRect.fMaxY,&nmaxx,&nmaxy);
        pgrid->ExtentRect.left=nminx;
		pgrid->ExtentRect.top=nminy;
		pgrid->ExtentRect.right=nmaxx;
		pgrid->ExtentRect.bottom=nmaxy;
		gridvector.push_back(pgrid);
	}
    GridHaveBuild=0;
    fclose(fp);
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	//Invalidate();
}
void CMyMapDoc::SaveGridIndexIndex(TCHAR* pszFileName)
{
	if (GridHaveBuild!=0)
	{
	SetCursor (LoadCursor (NULL, IDC_WAIT));
	lstrcpy(m_pszMapFilePath,pszFileName);
	char path[255];
	WToA(pszFileName,path);
	FILE	*fp;
	fp = fopen(path,"wb");
	if(!fp)	return;
	fwrite(&Tindexrows,sizeof(int),1,fp);  // 
	fwrite(&Tindexcols,sizeof(int),1,fp);  //
	int gridNum=(int)cgridindexvector.size();
	fwrite(&gridNum,sizeof(int),1,fp);  //
	for ( int i=0;i<gridNum;i++)
	{

		CGridIndex* pcgridindex;
		pcgridindex=cgridindexvector[i];
		int count=pcgridindex->gridindexvector.size();
		for(int j=0;j<count;j++)
		{
		    CGisGrid* pcgisgrid;
		    GRIDINDEX *pgridindex;
			pgridindex=pcgridindex->gridindexvector[j];
			pcgisgrid=gridvector[pgridindex->arrayIndex];
			pgridindex->offset=pcgisgrid->GetFileOffset();
		}
		pcgridindex->WriteToFile(fp);
		
	}
	fclose(fp);
	}
}

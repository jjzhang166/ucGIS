// Layer.cpp: implementation of the CLayer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"	
#include "Layer.h"
#include ".\layer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern bool mapPtInRect(const mapRECT* r,mapMAPPOINTSTRUCT* point);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLayer::CLayer()
{
	m_bDisplay=true;
	m_fileoffsetlen=0;
	m_ID = 0;
	// 是否允许自动标注
	m_bAutoLabel =  true;
	// 是否允许编辑该层
	m_bEditable =  false;
	// 是否显示该图层
	m_bVisible = true;;
	// 图层名字
	memset(m_pszName,0,sizeof(m_pszName));
	// 视野范围
	m_nEyeShotMin = 0;
	m_nEyeShotMax = 999999;
	m_bShowAllLabel = false;
#ifdef UNDER_WINCE
	m_ceDraw = NULL;
#endif
}

CLayer::~CLayer()
{
	// 画多边形
	CDraw* pDraw = NULL;
	int nCount = (int)m_PlineRgnVector.size();
	for(int i=0;i<nCount;i++)
	{
		pDraw = (CDraw*)m_PlineRgnVector[i];
		if(pDraw)
			delete pDraw;
		pDraw = NULL;
	}
	m_PlineRgnVector.clear();

	// 画曲线
	pDraw = NULL;
	nCount = (int)m_PlineVector.size();
	for(int i=0;i<nCount;i++)
	{
		pDraw = (CDraw*)m_PlineVector[i];
		if(pDraw)
			delete pDraw;
		pDraw = NULL;
	}
	m_PlineVector.clear();
	// 画直线
	pDraw = NULL;
	nCount = (int)m_LineVector.size();
	for(int i=0;i<nCount;i++)
	{
		pDraw = (CDraw*)m_LineVector[i];
		if(pDraw)
			delete pDraw;
		pDraw = NULL;
	}
	m_LineVector.clear();
	// 画点
	pDraw = NULL;
	nCount = (int)m_MyPointVector.size();
	for(int i=0;i<nCount;i++)
	{
		pDraw = (CDraw*)m_MyPointVector[i];
		if(pDraw)
			delete pDraw;
		pDraw = NULL;
	}
	m_MyPointVector.clear();
}

void CLayer::AddPline(CPline* pline)
{
	m_PlineVector.push_back(pline);
}

void CLayer::AddPlineRgn(CPlineRgn *plinergn)
{
	m_PlineRgnVector.push_back(plinergn);
}

void CLayer::AddMyPoint(CMyPoint *point)
{
	m_MyPointVector.push_back(point);
}
void CLayer::ReadFromFile(FILE *fp,int &fileoffset)
{
	m_fileoffsetlen=fileoffset;
	fread(&m_bAutoLabel,sizeof(bool),1,fp);
	fileoffset+=sizeof(bool);
	fread(&m_bVisible,sizeof(bool),1,fp);
	fileoffset+=sizeof(bool);
	fread(&m_nEyeShotMin,sizeof(m_nEyeShotMin),1,fp);
	fileoffset+=sizeof(m_nEyeShotMin);
	fread(&m_nEyeShotMax,sizeof(m_nEyeShotMax),1,fp);
	fileoffset+=sizeof(m_nEyeShotMax);
	fread(m_pszName,sizeof(m_pszName),1,fp);
	fileoffset+=sizeof(m_pszName);

	int nCountRgn = 0;
	int nCountPline = 0;
	int nCountLine = 0;
	int nCountPoint = 0;

	// 多边形
	fread(&nCountRgn,sizeof(int),1,fp);
	fileoffset+=sizeof(int);
	for(int i=0;i<nCountRgn;i++)
	{
		if(feof(fp)) return ;
		CPlineRgn* rgn = new CPlineRgn;
		rgn->ReadFromFile(fp,fileoffset);
		m_PlineRgnVector.push_back(rgn);
	}
	// 曲线
	fread(&nCountPline,sizeof(int),1,fp);
	fileoffset+=sizeof(int);
	for(int j=0;j<nCountPline;j++)
	{
		if(feof(fp)) return ;
		CPline* pline = new CPline;
		pline->ReadFromFile(fp,fileoffset);
		m_PlineVector.push_back(pline);
	}
	// 直线
	fread(&nCountLine,sizeof(int),1,fp);
	fileoffset+=sizeof(int);
	for(int k=0;k<nCountLine;k++)
	{
		if(feof(fp)) return ;
		CLine* line = new CLine;
		line->ReadFromFile(fp,fileoffset);
		m_LineVector.push_back(line);
	}
	// 点
	fread(&nCountPoint,sizeof(int),1,fp);
	fileoffset+=sizeof(int);
	for(int n=0;n<nCountPoint;n++)
	{
		if(feof(fp)) return;
		CMyPoint* point = new CMyPoint;
		point->ReadFromFile(fp,fileoffset);
		m_MyPointVector.push_back(point);
	}
}
void CLayer::GetPointFromRect(CMyPoint* points,int& nPointCount)
{
	CDraw* pDraw = NULL;
	int nCount = (int)m_MyPointVector.size();
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
bool CLayer::DelPoint(TCHAR* szLabel)
{
	CDraw* pDraw = NULL;
	for(int l=0;l<(int)m_MyPointVector.size();l++)
	{
		pDraw = (CDraw*)m_MyPointVector[l];
		if(pDraw->IsRectCross()&&
			!lstrcmp(pDraw->GetLabel(),szLabel))
		{
			MyPointVector::iterator pos = m_MyPointVector.begin();
			pos+=l;
			delete pDraw;
			pDraw = NULL;
			m_MyPointVector.erase(pos);
			l--;
		//	m_MyPointVector
			return true;
		}
	}
	return false;
}
void CLayer::WriteToFile(FILE *fp)
{
	fwrite(&m_bAutoLabel,sizeof(bool),1,fp);
	fwrite(&m_bVisible,sizeof(bool),1,fp);
	fwrite(&m_nEyeShotMin,sizeof(m_nEyeShotMin),1,fp);
	fwrite(&m_nEyeShotMax,sizeof(m_nEyeShotMax),1,fp);
	fwrite(m_pszName,sizeof(m_pszName),1,fp);
	
	// 画多边形
	CDraw* pDraw = NULL;
	int nCount = (int)m_PlineRgnVector.size();
	fwrite(&nCount,sizeof(int),1,fp);
	for(int i=0;i<nCount;i++)
	{
		pDraw = (CDraw*)m_PlineRgnVector[i];
		if(pDraw)
			pDraw->WriteToFile(fp);
	}

	// 画曲线
	pDraw = NULL;
	nCount = (int)m_PlineVector.size();
	fwrite(&nCount,sizeof(int),1,fp);
	for(int j=0;j<nCount;j++)
	{
		pDraw = (CDraw*)m_PlineVector[j];
		if(pDraw)
			pDraw->WriteToFile(fp);
	}
	// 画直线
	pDraw = NULL;
	nCount = (int)m_LineVector.size();
	fwrite(&nCount,sizeof(int),1,fp);
	for(int k=0;k<nCount;k++)
	{
		pDraw = (CDraw*)m_LineVector[k];
		if(pDraw)
			pDraw->WriteToFile(fp);
	}
	// 画点
	pDraw = NULL;
	nCount = (int)m_MyPointVector.size();
	fwrite(&nCount,sizeof(int),1,fp);
	for(int l=0;l<nCount;l++)
	{
		pDraw = (CDraw*)m_MyPointVector[l];
		if(pDraw)
			pDraw->WriteToFile(fp);
	}
}

void CLayer::Draw(HDC hDC, int nDrawMode)
{

	if(!m_bDisplay) return;
	// 画多边形
	CDraw* pDraw = NULL;
	int nCount = (int)m_PlineRgnVector.size();

	for(int i=0;i<nCount;i++)
	{
		pDraw = (CDraw*)m_PlineRgnVector[i];
		if(pDraw)
		{
			pDraw->m_mapCoordMap = m_mapCoordMap;
#ifdef UNDER_WINCE
			pDraw->m_ceDraw = m_ceDraw;
#endif
			pDraw->Draw(hDC,nDrawMode);
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
			pDraw->m_mapCoordMap = m_mapCoordMap;
#ifdef UNDER_WINCE
			pDraw->m_ceDraw = m_ceDraw;
#endif
			pDraw->Draw(hDC,nDrawMode);
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
			pDraw->m_mapCoordMap = m_mapCoordMap;
#ifdef UNDER_WINCE
			pDraw->m_ceDraw = m_ceDraw;
#endif
			pDraw->Draw(hDC,nDrawMode);
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
			pDraw->m_mapCoordMap = m_mapCoordMap;
#ifdef UNDER_WINCE
			pDraw->m_ceDraw = m_ceDraw;
#endif
			pDraw->Draw(hDC,nDrawMode);
		}
	}

}
void CLayer::DrawLabel(HDC hDC)
{
	if(!m_bDisplay) return;

// 画多边形
	CDraw* pDraw = NULL;
	int nCount = (int)m_PlineRgnVector.size();
	
	for(int i=0;i<nCount;i++)
	{
		pDraw = (CDraw*)m_PlineRgnVector[i];
		if(pDraw)
		{
			pDraw->m_bShowAllLabel = m_bShowAllLabel;
			pDraw->m_mapCoordMap = m_mapCoordMap;
			pDraw->DrawLabel(hDC);
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
			pDraw->m_bShowAllLabel = m_bShowAllLabel;
			pDraw->m_mapCoordMap = m_mapCoordMap;
			pDraw->DrawLabel(hDC);
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
			pDraw->m_bShowAllLabel = m_bShowAllLabel;
			pDraw->m_mapCoordMap = m_mapCoordMap;
			pDraw->DrawLabel(hDC);
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
			pDraw->m_bShowAllLabel = m_bShowAllLabel;
			pDraw->m_mapCoordMap = m_mapCoordMap;
			pDraw->DrawLabel(hDC);
		}
	}
}
// 删除指定元素
bool CLayer::DeleteDraw(CDraw* pDr)
{
// 画多边形
	CDraw* pDraw = NULL;
	int nCount = (int)m_PlineRgnVector.size();
	
	for(int i=0;i<nCount;i++)
	{
		pDraw = (CDraw*)m_PlineRgnVector[i];
		if(pDraw==pDr)
		{
		    PlineRgnVector::iterator pos = m_PlineRgnVector.begin();
			pos+=i;
			delete pDraw;
			pDraw = NULL;
			m_PlineRgnVector.erase(pos);
			i--;
			return true;
		}
	}

	// 画曲线
	pDraw = NULL;
	nCount = (int)m_PlineVector.size();
	for(int j=0;j<nCount;j++)
	{
		pDraw = (CDraw*)m_PlineVector[j];
		if(pDraw==pDr)
		{
		    PLineVector::iterator pos = m_PlineVector.begin();
			pos+=j;
			delete pDraw;
			pDraw = NULL;
			m_PlineVector.erase(pos);
			j--;
			return true;
		}
	}

	// 画直线
	pDraw = NULL;
	nCount = (int)m_LineVector.size();
	for(int k=0;k<nCount;k++)
	{
		pDraw = (CDraw*)m_LineVector[k];
		if(pDraw==pDr)
		{
		    LineVector::iterator pos = m_LineVector.begin();
			pos+=k;
			delete pDraw;
			pDraw = NULL;
			m_LineVector.erase(pos);
			k--;
			return true;
		}
	}
	// 画点
	pDraw = NULL;
	nCount = (int)m_MyPointVector.size();
	for(int l=0;l<nCount;l++)
	{
		pDraw = (CDraw*)m_MyPointVector[l];
		if(pDraw==pDr)
		{
			MyPointVector::iterator pos = m_MyPointVector.begin();
			pos+=l;
			delete pDraw;
			pDraw = NULL;
			m_MyPointVector.erase(pos);
			l--;
			return true;
		}
	}
	return false;
}
CPlineRgn* CLayer::AddPLineRgn(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,int Numble, mapMAPPOINTSTRUCT* PointList,mapPENSTRUCT Pen,bool bTransparent,bool bFill,mapBRUSHSTRUCT Brush)
{
	CPlineRgn* p_Plinergn = new CPlineRgn(pszLabel,nLayer, nType, nSubType,Numble,PointList,Pen,bTransparent,bFill,Brush);
	if(p_Plinergn)
		m_PlineRgnVector.push_back(p_Plinergn);
	return p_Plinergn;
}

CMyPoint* CLayer::AddPoint(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,float X,float Y)
{
	CMyPoint* p_Point = new CMyPoint( pszLabel, nLayer,  nType, nSubType,X, Y);
	if(p_Point)
		m_MyPointVector.push_back(p_Point);
	return p_Point;
}
CPline* CLayer::AddPLine(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,int Numble, mapMAPPOINTSTRUCT* PointList,mapPENSTRUCT Pen)
{
	CPline* p_Pline = new CPline( pszLabel, nLayer,  nType, nSubType,Numble,  PointList, Pen);
	if(p_Pline)
		m_PlineVector.push_back(p_Pline);
	return p_Pline;
}
CLine* CLayer::AddLine(TCHAR* pszLabel,short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,float x1,float y1,float x2,float y2,mapPENSTRUCT Pen)
{
	CLine* pLine = new CLine( pszLabel, nLayer,  nType, nSubType,x1,y1,x2,y2, Pen);
	if(pLine)
		m_LineVector.push_back(pLine);
	return pLine;
}
// 获取图层矩形边界
bool CLayer::GetRect(float * m_Xmin, float * m_Ymin, float * m_Xmax, float * m_Ymax)
{
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
}
// 获取我在哪里的文字描述
bool CLayer::GetWhereIam(mapMAPPOINTSTRUCT* pt, TCHAR* szWhere)
{
	CPlineRgn* pDraw = NULL;
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

// 屏幕点在激活的编辑曲线上吗
bool CLayer::IsOnTheLine(int x, int y,mapMAPPOINTSTRUCT* mpt,mapMAPPOINTSTRUCT* mpt1,mapMAPPOINTSTRUCT* mpt2, CPline** pLine)
{
	CPline* pDraw = NULL;
	int nCount =  (int)m_PlineVector.size();

	for(int i=0;i<nCount;i++)
	{
		pDraw = (CPline*)m_PlineVector[i];
		if(pDraw)
		{
			bool r = pDraw->IsOnMe(x,y,mpt,mpt1,mpt2,pLine);
			if(r)
			{
				return true;
			}

		}
	}
	return false;
}
bool CLayer::DeletePoint(mapMAPPOINTSTRUCT* mpt)
{
	CPline* pDraw = NULL;
	int nCount =  (int)m_PlineVector.size();

	for(int i=0;i<nCount;i++)
	{
		pDraw = (CPline*)m_PlineVector[i];
		if(pDraw)
		{
			bool r = pDraw->DeletePoint(mpt);
			if(r)
			{
				return true;
			}

		}
	}
	return false;
}
void CLayer::AddPointToLine(int x,int y)
{
	CPline* pDraw = NULL;
	int nCount =  (int)m_PlineVector.size();

	for(int i=0;i<nCount;i++)
	{
		pDraw = (CPline*)m_PlineVector[i];
		if(pDraw)
		{
			pDraw->AddPoint(x,y);
		}
	}
}
void CLayer::SetDrawType(bool bEditMode)
{
	CPline* pDraw = NULL;
	int nCount =  (int)m_PlineVector.size();

	for(int i=0;i<nCount;i++)
	{
		pDraw = (CPline*)m_PlineVector[i];
		if(pDraw)
		{
			pDraw->SetDrawType(bEditMode);
		}
	}
}
int CLayer::GetFileOffSetHeadLayer()
{
	int fileoffset=sizeof(bool)+sizeof(m_nEyeShotMin)+sizeof(m_nEyeShotMax)+sizeof(m_pszName);
    return fileoffset;
	/*
	fread(&m_bAutoLabel,sizeof(bool),1,fp);
	fread(&m_bVisible,sizeof(bool),1,fp);
	fread(&m_nEyeShotMin,sizeof(m_nEyeShotMin),1,fp);
	fread(&m_nEyeShotMax,sizeof(m_nEyeShotMax),1,fp);
	fread(m_pszName,sizeof(m_pszName),1,fp);
	fread(&nCountRgn,sizeof(int),1,fp);

*/

}
void CLayer::IndexBuid(int layeri,int tcols,GridVector &gridvector,tagDrawRect &griddrawrect,int & fileoffset)
{
	// 画多边形
	CDraw* pDraw = NULL;
	//计算数据在文件中的偏移量，以便更快的索引
	int nCount =(int) m_PlineRgnVector.size();
	for(int i=0;i<nCount;i++)
	{
		pDraw = (CDraw*)m_PlineRgnVector[i];
		if(pDraw)
		{
			//pDraw->
		   //通过求图元外接矩形与网格是否相交，建立TEXT目标索引/线目标索引/面目标索引

			float fminx,fminy,fmaxx,fmaxy;
			int minx,miny,maxx,maxy;
			pDraw->GetRect(&fminx,&fminy,&fmaxx,&fmaxy);
			//转换为逻辑坐标
			m_mapCoordMap->DPtoVP(fminx,fmaxy , &minx,&maxy);
			m_mapCoordMap->DPtoVP(fmaxx,fminy , &maxx,&miny);
            CRect objectMBR(minx,maxy,maxx,miny);
			int gridnum=(int)gridvector.size();
			for (int k=0;k<gridnum;k++)
			{
				CGisGrid* pgrid=(CGisGrid*)gridvector[k];
				//转换为逻辑坐标
				int x1,y1,x2,y2;
				//m_mapCoordMap->(grid->ExtentRect.left,grid->ExtentRect.top,&x1,&y1);
				//m_mapCoordMap->DPtoVP(grid->ExtentRect.right,grid->ExtentRect.bottom,&x2,&y2);
				x1=pgrid->ExtentRect.left;
				y1=pgrid->ExtentRect.top;
				x2=pgrid->ExtentRect.right;
				y2=pgrid->ExtentRect.bottom;
				CRect gridRect(x1,y1,x2,y2);
		
                CRect rect;
				BOOL objectMBRIsInGrid=false;
	            objectMBRIsInGrid=rect.IntersectRect(gridRect,objectMBR);
                if (objectMBRIsInGrid)
				{
					INDEX *pindex=new INDEX;
                    pindex->arrayIndex=i;
					pindex->layeroffset=GetFileoffsetLen();
			        pindex->ClassCode=IDS_POLYGON;
			        pindex->ilayer=layeri;
					pindex->offset=pDraw->GetFileoffsetLen();
			        pgrid->indexvector.push_back(pindex);
				}
			}
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
			float fminx,fminy,fmaxx,fmaxy;
			int minx,miny,maxx,maxy;
			pDraw->GetRect(&fminx,&fminy,&fmaxx,&fmaxy);
			//转换为逻辑坐标
			m_mapCoordMap->DPtoVP(fminx,fmaxy , &minx,&maxy);
			m_mapCoordMap->DPtoVP(fmaxx,fminy , &maxx,&miny);
            CRect objectMBR(minx,maxy,maxx,miny);

			//通过求图元外接矩形与网格是否相交，建立TEXT目标索引/线目标索引/面目标索引
			int gridnum=(int)gridvector.size();
			for (int k=0;k<gridnum;k++)
			{
				CGisGrid* pgrid=gridvector[k];
				//转换为逻辑坐标
				int x1,y1,x2,y2;
				x1=pgrid->ExtentRect.left;
				y1=pgrid->ExtentRect.top;
				x2=pgrid->ExtentRect.right;
				y2=pgrid->ExtentRect.bottom;
				CRect gridRect(x1,y1,x2,y2);
		
                CRect rect;
				BOOL objectMBRIsInGrid=false;
	            objectMBRIsInGrid=rect.IntersectRect(gridRect,objectMBR);

				

                if (objectMBRIsInGrid)
				{
					INDEX *pindex=new INDEX;
                    pindex->arrayIndex=j;
			        pindex->layeroffset=GetFileoffsetLen();
			        pindex->ClassCode=IDS_PLINE;
			        pindex->ilayer=layeri;
                    pindex->offset=pDraw->GetFileoffsetLen();
			        pgrid->indexvector.push_back(pindex);
				}
			}
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
			float fminx,fminy,fmaxx,fmaxy;
			int minx,miny,maxx,maxy;
			pDraw->GetRect(&fminx,&fminy,&fmaxx,&fmaxy);
			//转换为逻辑坐标
			m_mapCoordMap->DPtoVP(fminx,fmaxy , &minx,&maxy);
			m_mapCoordMap->DPtoVP(fmaxx,fminy , &maxx,&miny);
            CRect objectMBR(minx,maxy,maxx,miny);
		   //通过求图元外接矩形与网格是否相交，建立TEXT目标索引/线目标索引/面目标索引
			int gridnum=(int)gridvector.size();
			for (int k=0;k<gridnum;k++)
			{
				CGisGrid* pgrid=gridvector[k];
				//转换为逻辑坐标
				int x1,y1,x2,y2;
				x1=pgrid->ExtentRect.left;
				y1=pgrid->ExtentRect.top;
				x2=pgrid->ExtentRect.right;
				y2=pgrid->ExtentRect.bottom;
				CRect gridRect(x1,y1,x2,y2);
		
                CRect rect;
				BOOL objectMBRIsInGrid=false;
	            objectMBRIsInGrid=rect.IntersectRect(gridRect,objectMBR);
                if (objectMBRIsInGrid)
				{
					INDEX *pindex=new INDEX;
                    pindex->arrayIndex=k;
			        pindex->layeroffset=GetFileoffsetLen();
			        pindex->ClassCode=IDS_LINE;
			        pindex->ilayer=layeri;
                    pindex->offset=pDraw->GetFileoffsetLen();
			        pgrid->indexvector.push_back(pindex);
				}
			}
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
		//建立点目标索引
			float fminx,fminy,fmaxx,fmaxy;
			mapRECT mapRect;
			//int minx,miny;
			mapMAPPOINTSTRUCT point;
			pDraw->GetRect(&fminx,&fminy,&fmaxx,&fmaxy);
			//m_mapCoordMap->DPtoVP(fminx,fminy,&minx,&miny);
			point.x=fminx;
			point.y=fminy;
			int m,n;
			int gridnum=(int)gridvector.size();
			for (int k=0;k<gridnum;k++)
			{
				CGisGrid* pgrid=gridvector[k];
				//转换为逻辑坐标
				mapRect.left=pgrid->mapExtentRect.fMinX;
				mapRect.top=pgrid->mapExtentRect.fMinY;
				mapRect.right=pgrid->mapExtentRect.fMaxX;
				mapRect.bottom=pgrid->mapExtentRect.fMaxY;

				if(mapPtInRect(&mapRect,&point))
				{
					INDEX* pindex=new INDEX;
					pindex->arrayIndex=l;
					pindex->layeroffset=GetFileoffsetLen();
					pindex->ClassCode=IDS_POINT;
					pindex->ilayer=layeri;
					pindex->offset=pDraw->GetFileoffsetLen();
					pgrid->indexvector.push_back(pindex);
                    break;
				}
			}
		}
	}
}

void CLayer::BuildAreaIndex(CAreaIndexVector &careaindexvector)
{
	CDraw* pDraw = NULL;
	//计算数据在文件中的偏移量，以便更快的索引
	int nCount =(int) m_PlineRgnVector.size();
	for(int i=0;i<nCount;i++)
	{
		pDraw = (CDraw*)m_PlineRgnVector[i];
		if(pDraw)
		{
		   //通过求图元外接矩形与网格是否相交，建立TEXT目标索引/线目标索引/面目标索引
			float fminx,fminy,fmaxx,fmaxy;
			pDraw->GetRect(&fminx,&fminy,&fmaxx,&fmaxy);
			int offset=pDraw->GetFileoffsetLen();
			CAreaIndex *careaIndex=new CAreaIndex(fminx,fminy,fmaxx,fmaxy,offset);//,pDraw->GetLabel());
			careaindexvector.push_back(careaIndex);
		}
	}
}
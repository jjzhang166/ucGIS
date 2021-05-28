// MyPoint.cpp: implementation of the CMyPoint class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"	
#include "MyPoint.h"
#include "resource.h"
#include ".\mypoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyPoint::CMyPoint()
: m_bShowTrace(false)
{
	m_X = m_Y = 0;
	m_nCarID = 0;
    m_fileoffsetlen=0;	
}

CMyPoint::~CMyPoint()
{
	vector<mapGPSPOINT *>::iterator p = m_vectorGpsPoints.begin();
	//while(p!==m_vectorGpsPoints.end())
	for( p=m_vectorGpsPoints.begin(); p!=m_vectorGpsPoints.end(); ++p)
	{
		delete *p;
		*p = NULL;
	}
	

	//vector<mapGPSPOINT *>::iterator pos = m_vectorGpsPoints.begin();
	//int nCount = m_vectorGpsPoints.size();
	//	for(int i=0;i<nCount;i++)
	//	{
	//		pos = m_vectorGpsPoints.begin();
	//		delete[] m_vectorGpsPoints[i];
	//	}
	//EnterCriticalSection(&m_csDraw);
/*	mapGPSPOINT* point = NULL;
	int nCount = m_vectorGpsPoints.size();
	for(int i=0;i<nCount;i++)
	{
		point = NULL;
		point = m_vectorGpsPoints[i];
		if(point)
			delete point;
	}	*/	
	m_vectorGpsPoints.clear();
	//LeaveCriticalSection(&m_csDraw);
	//DeleteCriticalSection(&m_csDraw);
}
void CMyPoint::IntiBMPPath()
{
	if(myIcoMap.size()>0) return;

		TCHAR szFileName[MAX_PATH] = {0};
		GetModuleFileName(NULL,szFileName,MAX_PATH);
		int len = lstrlen(szFileName);
		int nIndex = 0;
		while(len)
		{
			if(szFileName[len-1]==_T('\\'))
			{
				nIndex = len;
				break;
			}
			len--;
		}
		TCHAR szPicPath[MAX_PATH] = {0};
		memcpy(szPicPath,szFileName,len*2);
			  TCHAR   szDirect[MAX_PATH];   
			  WIN32_FIND_DATA   winfd;   
			  //_stprintf(szDirect,_T("%s\\maps\\*.map"),g_szAppPath);   
			  _stprintf(szDirect,_T("%sres\\a\\*.bmp"),szPicPath);   

			HANDLE   hFind   =   FindFirstFile(szDirect,&winfd); 
			TCHAR szMapName[100];
			_stprintf(szMapName,_T("%s"),winfd.cFileName);   
			
			int id = myatoi(szMapName);
			TCHAR szFilePath[MAX_PATH] = {0};
			_stprintf(szFilePath,_T("%s\\res\\a\\%s"),szPicPath,szMapName);
			if(id>0)
			{
#ifdef UNDER_WINCE
				CCEBitmap* pCebmp1 = new CCEBitmap;
				if(NULL!=pCebmp1)
				{
					if(pCebmp1->LoadBitmap( m_ceDraw,szFilePath))//szFilePath))
						myIcoMap.insert(pair<int,CCEBitmap*>(id,pCebmp1));
				}
#else
				
				HBITMAP  hBitmap = NULL;
				
				hBitmap = (HBITMAP)::LoadImage(NULL,szFilePath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
				if(NULL!=hBitmap)
					myIcoMap.insert(pair<int,HBITMAP>(id,hBitmap));
				
#endif	
			}
		
			while(FindNextFile(hFind,&winfd))   
			{  
				_stprintf(szMapName,_T("%s"),winfd.cFileName);   
				int id = myatoi(szMapName);
				TCHAR szFilePath[MAX_PATH] = {0};
				_stprintf(szFilePath,_T("%s\\res\\a\\%s"),szPicPath,szMapName);
				if(id>0)
				{
#ifdef UNDER_WINCE
					CCEBitmap* pCebmp = new CCEBitmap;
					if(NULL!=pCebmp)
					{
						if(pCebmp->LoadBitmap(m_ceDraw,szFilePath))
							myIcoMap.insert(pair<int,CCEBitmap*>(id,pCebmp));
					}
#else
					
					HBITMAP  hBitmap = NULL;
					
					hBitmap = (HBITMAP)::LoadImage(NULL,szFilePath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
					if(NULL!=hBitmap)
						myIcoMap.insert(pair<int,HBITMAP>(id,hBitmap));
					
#endif	
				}			
			}   
			FindClose(hFind);

}
/////////////////////////////////////////////////////////
// Draw a transparent bitmap
BOOL BitBltTrans(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight,
HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop, COLORREF crColorTrans)
{
	try{
		CDC dcMask;
		CBitmap btMask;
		dcMask.CreateCompatibleDC(CDC::FromHandle(hdcDest));
		btMask.CreateBitmap(nWidth, nHeight, 1, 1,NULL);
		if (hdcSrc==NULL) return FALSE;
		if (hdcDest==NULL) return FALSE;
		if (hdcSrc==NULL) return FALSE;
		CBitmap* pbtOldMaskdcMask = dcMask.SelectObject(&btMask);
		COLORREF crOldSrcBack = SetBkColor(hdcSrc, crColorTrans);
		BitBlt(dcMask.m_hDC, 0, 0, nWidth, nHeight, hdcSrc, 0, 0, SRCCOPY);
		BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, SRCINVERT);
		BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, dcMask.m_hDC, nXSrc, nXSrc, SRCAND);
		BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, SRCINVERT); 
		
		dcMask.SelectObject(pbtOldMaskdcMask);
		SetBkColor(hdcSrc, crOldSrcBack);
		dcMask.DeleteDC();
		btMask.DeleteObject();
		pbtOldMaskdcMask->DeleteObject();
	}catch(CException* pe){

		return FALSE;
	}
	return TRUE;
}
extern TCHAR g_szAppPath[MAX_PATH];
void CMyPoint::Draw(HDC hDC,int nDrawMode)
{
	if(!IsRectCross(m_Rect.fMinX,m_Rect.fMinY,m_Rect.fMaxX,m_Rect.fMaxY))
		return;
	int x,y;
	DPtoVP(m_X,m_Y,&x,&y);

	if(m_bShowTrace) DrawTrace(hDC);  // 画轨迹

	IntiBMPPath();
#ifdef UNDER_WINCE

	CCEBitmap* pCebmp = NULL;
	pCebmp = myIcoMap[this->m_nType];
	if(NULL==pCebmp)
		pCebmp = myIcoMap[this->m_nSubType];
	if(NULL==pCebmp)
		pCebmp = myIcoMap[1];

	if(NULL!=pCebmp)
		pCebmp->BitBlt( m_ceDraw, x, y-5, 0, 0, 0, 0, SRCCOLORKEY, 0.6f );  // Draw bitmap with alpha blend
	
/*
CCEBitmap bmp;
TCHAR szTmp[255] = {0},szNull[255] = {0};
_stprintf(szNull,_T("%s\\res\\a\\%d"),g_szAppPath,1);
if(otNull!=m_nSubType)
_stprintf(szTmp,_T("%s\\res\\a\\%d"),g_szAppPath,m_nSubType);
else
_stprintf(szTmp,_T("%s\\res\\a\\%d"),g_szAppPath,m_nType);
if(bmp.LoadBitmap( m_ceDraw,szTmp))
bmp.BitBlt( m_ceDraw, x, y-5, 0, 0, 0, 0, SRCCOLORKEY, 0.6f );  // Draw bitmap with alpha blend
else if(bmp.LoadBitmap( m_ceDraw,szNull))
bmp.BitBlt( m_ceDraw, x, y-5, 0, 0, 0, 0, SRCCOLORKEY, 0.6f );  // Draw bitmap with alpha blend

*/
	/*
	// 绘制
	CCEPen cePen;
	cePen.CreatePen(1,1, m_ceDraw->ConvertColor(0,0,0 ) ); //
	CCEBrush ceBrush;
	ceBrush.CreateBrush( 1, m_ceDraw->ConvertColor(0,0,255) , 1 );
	m_ceDraw->SetGDIObject( &ceBrush );
	m_ceDraw->SetGDIObject( &cePen );
	m_ceDraw->FillRect(x-2,y-2,x+2,y+2,1);
	*/

#else
	 HDC hdcMem = CreateCompatibleDC (hDC) ;
	 if(!hdcMem) return;
	 HBITMAP  hBitmap = NULL;

	 hBitmap = myIcoMap[this->m_nType];
	  if(NULL==hBitmap)
		   hBitmap = myIcoMap[this->m_nSubType];
	 if(NULL==hBitmap)
		 hBitmap = myIcoMap[1];

	SelectObject (hdcMem, hBitmap) ;

	BITMAP bmp;
	GetObject(hBitmap,sizeof(BITMAP),&bmp);

	BitBltTrans(hDC,x-bmp.bmWidth/2,y-bmp.bmHeight/2,bmp.bmWidth,bmp.bmHeight,hdcMem, 0, 0, SRCCOPY, RGB(255,255,255));

	DeleteDC (hdcMem) ;

	m_mapCoordMap->VPtoDP(x-bmp.bmWidth/2,y-bmp.bmHeight/2,&m_Rect.fMinX,&m_Rect.fMinY);
	m_mapCoordMap->VPtoDP(x+bmp.bmWidth/2,y+bmp.bmHeight/2,&m_Rect.fMaxX,&m_Rect.fMaxY);
#endif

}
void CMyPoint::DrawTrace(HDC hDC)
{
	if(m_vectorGpsPoints.size()<2) return;
#ifdef UNDER_WINCE
	CCEPen cePen;
	cePen.CreatePen(1,2, m_ceDraw->ConvertColor( 0,0,255 ) ); // Convert RGB color to CE support Colo
	m_ceDraw->SetGDIObject(&cePen);
	int nCount = m_vectorPoints.size();

	POINT* ppoint = new POINT[nCount+1];
	if(ppoint==NULL) return;
	
	EnterCriticalSection(&m_csDraw);
	for(int i=0;i<nCount;i++)
	{
		m_mapCoordMap->DPtoVP(m_vectorPoints[i]->xx,m_vectorPoints[i]->yy,(int*)&ppoint[i].x,(int*)&ppoint[i].y);	
	}
	LeaveCriticalSection(&m_csDraw);
	m_mapCoordMap->DPtoVP(m_gpsDataInfo.m_xx,m_gpsDataInfo.m_yy,(int*)&ppoint[nCount].x,(int*)&ppoint[nCount].y);	
	m_ceDraw->DrawPolyline(ppoint,nCount+1);
	delete[] ppoint;
	ppoint = NULL;
#else
	HPEN	oldPen,newPen;
	newPen = CreatePen(PS_SOLID,2,RGB(0,255,255));
	oldPen	= (HPEN)SelectObject(hDC,newPen);

	int nCount = (int)m_vectorGpsPoints.size();
	POINT* ppoint = new POINT[nCount+1];
	if(ppoint==NULL) return;
	
	EnterCriticalSection(&m_csDraw);
	for(int i=0;i<nCount;i++)
	{
		m_mapCoordMap->DPtoVP(m_vectorGpsPoints[i]->x,m_vectorGpsPoints[i]->y,(int*)&ppoint[i].x,(int*)&ppoint[i].y);	
	}
	LeaveCriticalSection(&m_csDraw);	
	Polyline(hDC,ppoint,nCount);
	delete[] ppoint;
	ppoint = NULL;
	//EnterCriticalSection(&m_csDraw);
	//for(int i=0;i<nCount;i++)
	//{
	//	m_mapCoordMap->DPtoVP(m_vectorGpsPoints[i]->x,m_vectorGpsPoints[i]->y,&x,&y);

	//	{
	//		if((i==0))
	//			MoveToEx(hDC,x,y,NULL);
	//		else
	//			LineTo(hDC,x,y);
	//	}

	//}
	//LeaveCriticalSection(&m_csDraw);
	SelectObject(hDC,oldPen);
	DeleteObject(newPen);
#endif
	
}
void CMyPoint::DrawLabel(HDC hDC)
{
	CDraw::DrawLabel(hDC);
	if(!lstrlen(m_lpszLabel)) return;

	if(!IsRectCross(m_Rect.fMinX,m_Rect.fMinY,m_Rect.fMaxX,m_Rect.fMaxY))
		return;

	LOGFONT	lf = {0};
	lf.lfHeight = 12;
	lf.lfWidth = 0;
	lf.lfWeight = 0;
	lf.lfCharSet = GB2312_CHARSET;
	lf.lfEscapement=lf.lfOrientation=0;
	memcpy(lf.lfFaceName,TEXT("宋体"),sizeof(lf.lfFaceName));
	if(m_bShowAllLabel)  // 粗体
	{
		lf.lfWeight=FW_BOLD;
	}



	int x,y;
	DPtoVP(m_X,m_Y,&x,&y);
	// 做防止标注覆盖的运算
	SIZE sz;
	GetTextExtentPoint32(hDC,m_lpszLabel,lstrlen(m_lpszLabel),&sz);

	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x+sz.cx;
	rect.bottom = y+sz.cy;

	if(m_bShowAllLabel||(!m_mapCoordMap->IsLabelRectUsed(rect))) // 如果标注区域没有被绘制  m_bShowAllLabel是否强制标注
	{
#ifdef UNDER_WINCE

	POINT ptText = { x+15, y-sz.cy/2-2 };
	m_ceDraw->DrawText( &ptText, (LPCTSTR)m_lpszLabel,lstrlen(m_lpszLabel), 1 ); // Draw text with pen color

#else

	HFONT newFont, oldFont;

	newFont =  (HFONT)CreateFontIndirect(&lf);

	oldFont = (HFONT)SelectObject(hDC,newFont);

	COLORREF oldFontColor;
	if(m_bShowAllLabel)
	  oldFontColor = SetTextColor(hDC,RGB(153,32,32));

	SetBkMode(hDC,TRANSPARENT);
	ExtTextOut(hDC, x+12, y, 0, NULL, (LPCTSTR)m_lpszLabel, 
         lstrlen(m_lpszLabel),0);
	if(m_bShowAllLabel)
	SetTextColor(hDC,oldFontColor);
	SelectObject(hDC,oldFont);
	DeleteObject(newFont);
#endif
	}
}
void CMyPoint::ReadFromFile(FILE *fp,int &fileoffset)
{
	m_fileoffsetlen=fileoffset;
	CDraw::ReadFromFile(fp,fileoffset);
	fread(&m_X,1,sizeof(m_X),fp);
    fileoffset+=sizeof(m_X);
	fread(&m_Y,1,sizeof(m_Y),fp);
    fileoffset+=sizeof(m_Y);
//	fread(&m_Symbol,sizeof(m_Symbol),1,fp);
}
void CMyPoint::WriteToFile(FILE *fp)
{
	CDraw::WriteToFile(fp);
	fwrite(&m_X,1,sizeof(m_X),fp);
	fwrite(&m_Y,1,sizeof(m_Y),fp);
	//fwrite(&m_Symbol,sizeof(m_Symbol),1,fp);
}
// 设置目标点的经纬度
void CMyPoint::SetXY(float x, float y)
{
	m_X = x;
	m_Y = y;
}

// 某点在不在我这图形上
bool CMyPoint::IsOnMe(mapMAPPOINTSTRUCT* point)
{
	return (point->x>m_Rect.fMinX)&&(point->y<m_Rect.fMinY)&&(point->x<m_Rect.fMaxX)&&(point->y>m_Rect.fMaxY);
}

// 添加一个轨迹点到车车
void CMyPoint::SetXY(mapGPSPOINT* mapPt)
{
	if(mapPt==NULL) return;

	SetXY(mapPt->x,mapPt->y);
	m_vectorGpsPoints.push_back(mapPt);
	int nCount = m_vectorGpsPoints.size();
	if((nCount>2)&&m_bShowTrace)
		GetTraceRect(&m_Rect.fMinX,&m_Rect.fMinY,&m_Rect.fMaxX,&m_Rect.fMaxY);
	else
		GetRect(&m_Rect.fMinX,&m_Rect.fMinY,&m_Rect.fMaxX,&m_Rect.fMaxY);

	int nPointCountLimit = 300;  // 每条轨迹最大的点数量
	int  nSmallCount = 100;      // 每次清除点后保留点的数量
 //   if(nCount>nPointCountLimit)
	//{
	//	EnterCriticalSection(&m_csDraw);
	//	for(int i=0;i<nSmallCount;i++)
	//	{
	//		mapGPSPOINT* pt = m_vectorGpsPoints[i];
	//		if(pt)
	//		{
	//			delete pt;
	//			pt = NULL;
	//		}
	//	}
	//	vector<mapGPSPOINT *>::iterator p = m_vectorGpsPoints.begin();
	//	m_vectorGpsPoints.erase(p,p+nSmallCount);
	//	LeaveCriticalSection(&m_csDraw);
	//}	
}
//函数:CPline::GetRect作用:计算并返回一条连续直线的边界矩形
//连续直线的主控参数在pline1中，坐标在Pointxy中
void CMyPoint::GetTraceRect(float* minX, float* minY, float* maxX, float* maxY)
{
	int nCount = m_vectorGpsPoints.size();
	if (nCount<2) return;
	float x1,y1,x2,y2;
	//用第一个顶点的坐标初试化变量
	x1=m_vectorGpsPoints[0]->x;x2=m_vectorGpsPoints[0]->x;
	y1=m_vectorGpsPoints[0]->y;y2=m_vectorGpsPoints[0]->y;

	for(int i=1;i<nCount;i++)          
	{
		x1=min(x1,m_vectorGpsPoints[i]->x);    //得到最小x坐标
		x2=max(x2,m_vectorGpsPoints[i]->x);    //得到最大x坐标
		y1=min(y1,m_vectorGpsPoints[i]->y);    //得到最小y坐标
		y2=max(y2,m_vectorGpsPoints[i]->y);    //得到最大y坐标
	}
	*minX=x1;*maxX=x2;*minY=y1;*maxY=y2; //将值赋给地址返回
}


void CMyPoint::SetShowTrace(bool bShowTrace)
{
	m_bShowTrace = bShowTrace;
	if(bShowTrace)
		GetTraceRect(&m_Rect.fMinX,&m_Rect.fMinY,&m_Rect.fMaxX,&m_Rect.fMaxY);
	else
		GetRect(&m_Rect.fMinX,&m_Rect.fMinY,&m_Rect.fMaxX,&m_Rect.fMaxY);
	
}

bool CMyPoint::GetShowTrace(void)
{
	return m_bShowTrace;
}

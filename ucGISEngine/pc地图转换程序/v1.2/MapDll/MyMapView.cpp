// MyMapView.cpp: implementation of the CMyMapView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyMapView.h"
#include ".\mymapview.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMyMapView::CMyMapView()
: m_bDrawScale(false)
{
	pLine = NULL;
	bEditDN = false;
	mpt.x = mpt.y = 0;
	m_nDrawCurrent = 0;
	m_nPointCount = 0;
	bMany = false;

#ifdef UNDER_WINCE
	m_ceDraw = NULL;
	m_pDoubleBuffer = NULL;
#endif
	m_hMapDC = NULL;
	m_hMapDC = CreateCompatibleDC(NULL);
	hbmNew = NULL;

	m_hUserDC = NULL;
	m_hUserDC = CreateCompatibleDC(NULL);
	m_hUserBmp = NULL;

	m_hMemDC = NULL;
	m_hMemDC = CreateCompatibleDC(NULL);
	m_hMemBmp = NULL;

	if(m_UserMap.GetLayerCount()==0)
	{
		CLayer* pLayer = new CLayer();
		ASSERT(pLayer!=NULL);
		pLayer->m_bAutoLabel = true;
		lstrcpy(pLayer->m_pszName, _T("用户点"));
		m_UserMap.AddLayer(pLayer);
	}
}

CMyMapView::~CMyMapView()
{
#ifdef UNDER_WINCE
	if( NULL != m_pDoubleBuffer ) 
	{
		free( m_pDoubleBuffer );
		m_pDoubleBuffer = NULL;
	}
#endif

	if(m_hMapDC)
		DeleteDC(m_hMapDC);
	if(hbmNew)
		DeleteObject(hbmNew);
	if(m_hMapDC)
		DeleteDC(m_hMapDC);
	if(m_hUserBmp)
		DeleteObject(m_hUserBmp);
	if(m_hMapDC)
		DeleteDC(m_hMemDC);
	if(m_hUserBmp)
		DeleteObject(m_hMemBmp);

}
/////////////////////////////////////////////////////////
// Draw a transparent bitmap
extern BOOL BitBltTrans(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight,
						HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop, COLORREF crColorTrans);

// 绘制地图
// bForceDraw 是否强制重绘

void CMyMapView::Draw(HDC hDC, int nDrawMode,bool bForceDraw,bool bOnlyDrawUserMap)
{
		
	m_mapCoordMap->nMapMode = 1;
#ifdef UNDER_NT
	SetMapMode(hDC,m_mapCoordMap->nMapMode);
#endif

#ifdef UNDER_WINCE
	if( NULL == m_pDoubleBuffer) 
	{
		m_lMemorySize = m_ceDraw->m_lMemorySize;
		m_pDoubleBuffer = (unsigned char*)malloc( m_lMemorySize );
		if( NULL == m_pDoubleBuffer ) 
		{
			return;
		}
	}
	m_FileMap.m_ceDraw = m_ceDraw;
	m_UserMap.m_ceDraw = m_ceDraw;
#else
	// 实现内存 HDC的操作
	if(!hbmNew)
	{
		hbmNew = CreateCompatibleBitmap(hDC,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
		(HBITMAP)SelectObject(m_hMapDC, hbmNew);
	}
	if(!m_hUserBmp)
	{
		m_hUserBmp = CreateCompatibleBitmap(hDC,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
		(HBITMAP)SelectObject(m_hUserDC, m_hUserBmp);
	}
	if(!m_hMemBmp)
	{
		m_hMemBmp = CreateCompatibleBitmap(hDC,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
		(HBITMAP)SelectObject(m_hMemDC, m_hMemBmp);
	}

#endif

	if(m_mapCoordMap->m_bMapChanged||bForceDraw||bOnlyDrawUserMap)
	{
#ifdef UNDER_WINCE
		m_ceDraw->Clear( (DWORD)255 );  // Clear screen use white color
#else
		(HBITMAP)SelectObject(m_hMapDC, hbmNew);

		if(m_mapCoordMap->m_bMapChanged||bForceDraw)
		{
			// 画背景色
			HBRUSH newBrush, oldBrush;
			newBrush = (HBRUSH)CreateSolidBrush(RGB(255,255,255));
			oldBrush = (HBRUSH)SelectObject(m_hMapDC,newBrush);
			PatBlt(m_hMapDC,0,0,m_mapCoordMap->wScreen,m_mapCoordMap->hScreen,PATCOPY);
			SelectObject(m_hMapDC,oldBrush);	
			DeleteObject(newBrush);
		}
		//if(m_mapCoordMap->m_bMapChanged||bOnlyDrawUserMap){
		HBRUSH newBrush, oldBrush;
		newBrush = (HBRUSH)CreateSolidBrush(RGB(255,255,255));
		oldBrush = (HBRUSH)SelectObject(m_hUserDC,newBrush);
		PatBlt(m_hUserDC,0,0,m_mapCoordMap->wScreen,m_mapCoordMap->hScreen,PATCOPY);
		SelectObject(m_hMapDC,oldBrush);	
		DeleteObject(newBrush);
		//}
#endif
		m_mapCoordMap->SetStart();
		m_mapCoordMap->ClearLabelRect();
		m_mapCoordMap->SetScreenXY();

		if(m_mapCoordMap->m_bMapChanged||bForceDraw)
			m_FileMap.Draw(m_hMapDC,nDrawMode);
		m_UserMap.Draw(m_hUserDC,nDrawMode);
		m_mapCoordMap->m_bMapChanged = false;
#ifdef UNDER_WINCE
		memcpy( m_pDoubleBuffer,m_ceDraw->GetBuffer(),  m_lMemorySize );
#endif
	}
	else
	{
#ifdef UNDER_WINCE
		memcpy(m_ceDraw->GetBuffer(),m_pDoubleBuffer, m_lMemorySize );
#endif
	}
#ifdef UNDER_WINCE
	/*
	MEMORYSTATUS st;
	GlobalMemoryStatus(&st);
	TCHAR lpszState[300] = {0};
	_sntprintf(lpszState,300,_T("内存:%.2fMb"),st.dwAvailPhys/1024.0/1024.0);
	POINT ptText = { 0, 60 };
	m_ceDraw->DrawText( &ptText, (LPCTSTR)lpszState,lstrlen(lpszState), 1 ); // Draw text with pen color
	*/
#else
	BOOL c = BitBlt(m_hMemDC,0,0,m_mapCoordMap->wScreen,m_mapCoordMap->hScreen,m_hMapDC,0,0,SRCCOPY);
	c = BitBltTrans(m_hMemDC, 0,0,m_mapCoordMap->wScreen,m_mapCoordMap->hScreen, m_hUserDC, 0, 0, SRCCOPY, RGB(255,255,255));
	c = BitBlt(hDC,0,0,m_mapCoordMap->wScreen,m_mapCoordMap->hScreen,m_hMemDC,0,0,SRCCOPY);

#endif			
	// 绘制比例尺
	if(m_bDrawScale)
	{
		HPEN	oldPen,newPen;
		newPen = CreatePen(0,1,RGB(0,0,0));
		oldPen	= (HPEN)SelectObject(hDC,newPen);

		HFONT newFont, oldFont;

		LOGFONT	lf = {0};
		lf.lfHeight = 12;
		lf.lfWidth = 0;
		lf.lfWeight = 0;
		lf.lfCharSet = GB2312_CHARSET;
		lf.lfEscapement=lf.lfOrientation=0;
		memcpy(lf.lfFaceName,TEXT("宋体"),sizeof(lf.lfFaceName));

		newFont =  (HFONT)CreateFontIndirect(&lf);

		oldFont = (HFONT)SelectObject(hDC,newFont);


		SetBkMode(hDC,TRANSPARENT);

		RECT rect;
		rect.left = m_mapCoordMap->wScreen-55;
		rect.top = m_mapCoordMap->hScreen-20;
		rect.bottom = rect.top+17;
		rect.right = rect.left+39;
		// 如果需要填充多边形

		HBRUSH newBrush, oldBrush;
		newBrush = (HBRUSH)CreateSolidBrush(RGB(0,255,255));
		oldBrush = (HBRUSH)SelectObject(hDC,newBrush);
		//FillRect(hDC,&rect,newBrush);
		RoundRect(hDC,rect.left,rect.top,rect.right,rect.bottom,3,3);

		// 绘制比例尺
		MoveToEx(hDC,rect.left+2,rect.top+13,NULL);
		LineTo(hDC,rect.left+35,rect.top+13);

		MoveToEx(hDC,rect.left+2,rect.top+9,NULL);
		LineTo(hDC,rect.left+2,rect.top+15);

		MoveToEx(hDC,rect.left+35,rect.top+9,NULL);
		LineTo(hDC,rect.left+35,rect.top+15);

		// 计算比例尺
		int nDistance = m_mapCoordMap->scale*33*100000;

		TCHAR lpszState[300] = {0};
		if(nDistance>999)
			_sntprintf(lpszState,300,_T("%dKM"),
			nDistance/1000
			);
		else
			_sntprintf(lpszState,300,_T("%dM"),
			nDistance
			);


		SIZE sz;
		GetTextExtentPoint32(hDC,lpszState,lstrlen(lpszState),&sz);

		ExtTextOut(hDC, rect.left+((rect.right-rect.left)- sz.cx)/2, rect.top+1, 0, NULL,(LPCTSTR)(lpszState), 
			lstrlen(lpszState),0);
		SelectObject(hDC,oldBrush);	
		DeleteObject(newBrush);
		SelectObject(hDC,oldPen);
		DeleteObject(newPen);
		SelectObject(hDC,oldFont);
		DeleteObject(newFont);
	}
}
// 设置地图在屏幕上的中心位置
void CMyMapView::SetCenter(float x, float y)
{
	m_mapCoordMap->SetCenter(x, y);
}
// 设置地图在屏幕上的中心位置
void CMyMapView::SetCenter(int x, int y)
{
	m_mapCoordMap->SetCenter(x, y);
}
void CMyMapView::Left()
{
	m_mapCoordMap->Left();
}
void CMyMapView::Right()
{
	m_mapCoordMap->Right();
}
void CMyMapView::Up()
{
	m_mapCoordMap->Up();
}
void CMyMapView::Down()
{
	m_mapCoordMap->Down();
}


bool CMyMapView::AddMyDearPoint(float xx, float yy,ObjectTypeConstants nType,ObjectTypeConstants nSubType,TCHAR* szText)
{
	m_UserMap.AddPoint(szText,0,nType,nSubType,xx,yy);//->Draw(&ddd,0,0,m_bColor);
	m_UserMap.m_bDataChanged = true;
	return false;
}
//extern TCHAR g_szAppPath[MAX_PATH];
void CMyMapView::ReadUserMap()
{

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

	TCHAR szMapPath[MAX_PATH] = {0};
	_stprintf(szMapPath,_T("%s\\user.map"),szPicPath);

	m_UserMap.SetCoordMap(m_mapCoordMap);
	m_UserMap.ReadFromFile(szMapPath);


}

void CMyMapView::OnRButtonDown(UINT nFlags, POINT point,HDC hDC)
{
	if(m_nDrawCurrent==16)
	{
		SetTools(16);
		//Invalidate();
		Draw(hDC,1,false,false);
	}	
}

void CMyMapView::OnMouseMove(UINT nFlags, POINT point,HDC hDC)
{
	SetROP2(hDC,R2_NOT);
	if(((m_nDrawCurrent==16)||(m_nDrawCurrent==11)||(m_nDrawCurrent==13))&&PushNumb==1)
	{
		if((point.x!=mPointOld.x)||(point.y!=mPointOld.y))
		{
			HPEN	oldPen,newPen;
			newPen = CreatePen(PS_DOT,1, RGB(0, 128, 128));
			oldPen	= (HPEN)SelectObject(hDC,newPen);
			//CPen pen(PS_DOT,1, RGB(128, 128, 128));
			if(m_nDrawCurrent==11||m_nDrawCurrent==13)
			{
				HBRUSH newBrush, oldBrush;
				newBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				oldBrush = (HBRUSH)SelectObject(hDC,newBrush);
				Rectangle(hDC,mPointOrign.x,mPointOrign.y,mPointOld.x,mPointOld.y);
				Rectangle(hDC,mPointOrign.x,mPointOrign.y,point.x,point.y);
				SelectObject(hDC,oldBrush);	
				DeleteObject(newBrush);
			}
			else
			{	
				MoveToEx(hDC,mPointOrign.x,mPointOrign.y,NULL);LineTo(hDC,mPointOld.x,mPointOld.y);
				MoveToEx(hDC,mPointOrign.x,mPointOrign.y,NULL);LineTo(hDC,point.x,point.y);
			}
			SelectObject(hDC,oldPen);
			DeleteObject(newPen);

			mPointOld = point;
		}
	}else if(m_nDrawCurrent==2)
	{
		if(bMany)
		{			
			//FillSolidRect(0,0,m_mapCoordMap.wScreen,m_mapCoordMap.hScreen,RGB(255,255,255));
			// 画背景色

			HBRUSH newBrush;
			newBrush = (HBRUSH)CreateSolidBrush(RGB(255,255,255));

			RECT rect;
			if((point.x-mPointOld.x)>0)
			{
				rect.top = 0;
				rect.left = 0;
				rect.bottom = m_mapCoordMap->hScreen;
				rect.right = point.x-mPointOld.x;
				FillRect(hDC,&rect,newBrush);
			}
			else
			{
				rect.top = 0;
				rect.left = m_mapCoordMap->wScreen+point.x-mPointOld.x;
				rect.bottom = m_mapCoordMap->hScreen;
				rect.right = m_mapCoordMap->wScreen;
				FillRect(hDC,&rect,newBrush);
			}

			if((point.y-mPointOld.y)>0)
			{
				rect.top = 0;
				rect.left = 0;
				rect.bottom = point.y-mPointOld.y;
				rect.right = m_mapCoordMap->wScreen;
				FillRect(hDC,&rect,newBrush);
			}
			else
			{
				rect.top = m_mapCoordMap->hScreen+point.y-mPointOld.y;
				rect.left = 0;
				rect.bottom =  m_mapCoordMap->hScreen;
				rect.right = m_mapCoordMap->wScreen;
				FillRect(hDC,&rect,newBrush);
			}



			DeleteObject(newBrush);


			BitBlt(hDC,point.x-mPointOld.x,point.y-mPointOld.y,m_mapCoordMap->wScreen,m_mapCoordMap->hScreen,m_hMemDC,0,0,SRCCOPY);	
			mPointOrign = point;
		}
	}
	else if(m_nDrawCurrent==100)
	{
		if(bEditDN)    // 绘制编辑地图的橡皮经过
		{
			HPEN	oldPen,newPen;
			newPen = CreatePen(PS_SOLID,1, RGB(255, 0, 0));
			oldPen	= (HPEN)SelectObject(hDC,newPen);

			int x1,y1,x2,y2;
			m_mapCoordMap->DPtoVP(mpt1.x,mpt1.y,&x1,&y1);
			m_mapCoordMap->DPtoVP(mpt2.x,mpt2.y,&x2,&y2);

			if(mpt1.x!=0)
			{
				MoveToEx(hDC,x1,y1,NULL);LineTo(hDC,mPointOld.x,mPointOld.y);			
				MoveToEx(hDC,x1,y1,NULL);LineTo(hDC,point.x,point.y);	
			}
			if(mpt2.x!=0)
			{
				MoveToEx(hDC,x2,y2,NULL);LineTo(hDC,mPointOld.x,mPointOld.y);
				MoveToEx(hDC,x2,y2,NULL);LineTo(hDC,point.x,point.y);
			}
			SelectObject(hDC,oldPen);
			DeleteObject(newPen);
			mPointOld = point;
		}
	}
}

void CMyMapView::OnLButtonUp(UINT nFlags, POINT point,HDC hDC)
{
	int x1,x2,y1,y2;
	if(m_nDrawCurrent==11||(m_nDrawCurrent==13))
	{

		x1 = min(mPointOrign.x,point.x);
		y1 = max(mPointOrign.y,point.y);

		x2 = max(mPointOrign.x,point.x);
		y2 = min(mPointOrign.y,point.y);

		// 如果鼠标是单击放大或者缩小
		if(((mPointOrign.x==point.x)&&(mPointOrign.y==point.y))||(x2-x1)<50)  // 如果是单击屏幕，或者圈选的范围很小
		{
			SetCenter(point.x,point.y);
			if(m_nDrawCurrent==11)
				m_mapCoordMap->ZoomIn();
			else
				m_mapCoordMap->ZoomOut();
		}
		else
		{
			m_mapCoordMap->ZoomTo(x1,y1,x2,y2);
		}
		//Invalidate();
		Draw(hDC,1,false,false);
		PushNumb = 0;
	}
	else if(m_nDrawCurrent==2)
	{
		m_mapCoordMap->SetCenterOffset(point.x-mPointOld.x,point.y-mPointOld.y);
		//Invalidate();
		Draw(hDC,1,false,false);
		bMany = false;
	}
	else if(m_nDrawCurrent==100)
	{
		mapMAPPOINTSTRUCT pt;
		m_mapCoordMap->VPtoDP(point.x,point.y,&pt.x,&pt.y);
		if(pLine!=NULL)
		{
			pLine->MovePoint(&mpt,&pt);
			Draw(hDC,1,true,false);
		}

		bEditDN = false;
		pLine = NULL;
	}
	else if((m_nDrawCurrent==101))   // 增加点到线路
	{
		m_UserMap.AddPointToLine(point.x,point.y);
		Draw(hDC,1,true,false);
	}

}
BOOL CMyMapView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{


	return false;
}
 
void CMyMapView::OnLButtonDown(UINT nFlags, POINT point,HDC hDC)
{
	if(m_nDrawCurrent==2)
	{
		// 实现内存 HDC的操作
		//BitBlt(m_hMemDC,0,0,m_mapCoordMap->wScreen,m_mapCoordMap->hScreen,hDC,0,0,SRCCOPY);		
		bMany = true;
		mPointOrign = point;
		mPointOld = point;
	}
	else if((m_nDrawCurrent==11)||(m_nDrawCurrent==12)||(m_nDrawCurrent==13))
	{
		if(PushNumb==0)
		{
			mPointOrign = point;
			mPointOld = point;
			PushNumb++;
			//SetCapture();
		}		
	}
	else if((m_nDrawCurrent==16))   // 测量距离
	{
		LOGFONT	lf = {0};
		lf.lfHeight = 30;
		lf.lfWidth = 0;
		lf.lfWeight = 0;
		lf.lfCharSet = GB2312_CHARSET;
		lf.lfEscapement=lf.lfOrientation=0;
		memcpy(lf.lfFaceName,TEXT("宋体"),sizeof(lf.lfFaceName));

		HFONT newFont, oldFont;

		newFont =  (HFONT)CreateFontIndirect(&lf);

		oldFont = (HFONT)SelectObject(hDC,newFont);

		SetBkMode(hDC,OPAQUE);

		TCHAR lpszLabel[200];
		_stprintf( lpszLabel, _T("总长度：%3.5f公里"), m_fTatolDistance );  //转换成字符型

		if(m_nPointCount==0)
		{
			mPointOrign = point;
			mPointOld = point;

			// 距离车辆点信息 暂存
			m_PointOne = point;
			m_PointTwo = point;
			m_nPointCount++;
			ExtTextOut(hDC, 30, 30, 0, NULL, (LPCTSTR)lpszLabel, 
				lstrlen(lpszLabel),0);
		}
		else
		{
			mPointOrign = mPointOld;
			mPointOld = point;

			m_nPointCount++;
			m_PointTwo = point;
			float fx = abs(m_PointTwo.x-m_PointOne.x)*abs(m_PointTwo.x-m_PointOne.x);
			float fy = abs(m_PointTwo.y-m_PointOne.y)*abs(m_PointTwo.y-m_PointOne.y);
			m_fTatolDistance += sqrt(fx+fy) *m_mapCoordMap->scale*100;

			_stprintf( lpszLabel, _T("总长度：%3.5f公里"), m_fTatolDistance );  //转换成字符型

			ExtTextOut(hDC, 30, 30, 0, NULL, (LPCTSTR)lpszLabel, 
				lstrlen(lpszLabel),0);

			m_PointOne = point;
		}
		PushNumb = 1;

		SelectObject(hDC,oldFont);
		DeleteObject(newFont);

	}
	else if((m_nDrawCurrent==100))   // 编辑图形
	{
		if(m_UserMap.IsOnTheLine(point.x,point.y,&mpt,&mpt1,&mpt2,&pLine))
		{
			int x,y;
			m_mapCoordMap->DPtoVP(mpt.x,mpt.y,&x,&y);
			HBRUSH newBrush, oldBrush;
			newBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			oldBrush = (HBRUSH)SelectObject(hDC,newBrush);
			HPEN	oldPen,newPen;
			newPen = CreatePen(PS_SOLID,1, RGB(255, 0, 0));
			oldPen	= (HPEN)SelectObject(hDC,newPen);
			Rectangle(hDC,x-5,y-5,x+5,y+5);

			SelectObject(hDC,oldBrush);	
			DeleteObject(newBrush);
			SelectObject(hDC,oldPen);	
			DeleteObject(newPen);

			mPointOld = point;
			bEditDN = true;
		}
	}
	else if((m_nDrawCurrent==101))   // 增加点到线路
	{

			HBRUSH newBrush, oldBrush;
			newBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			oldBrush = (HBRUSH)SelectObject(hDC,newBrush);
			HPEN	oldPen,newPen;
			newPen = CreatePen(PS_SOLID,1, RGB(255, 0, 0));
			oldPen	= (HPEN)SelectObject(hDC,newPen);
			Rectangle(hDC,point.x-5,point.y-5,point.x+5,point.y+5);

			SelectObject(hDC,oldBrush);	
			DeleteObject(newBrush);
			SelectObject(hDC,oldPen);	
			DeleteObject(newPen);

			mPointOld = point;
	}
	else
	{
		SetCenter(point.x,point.y);
		Draw(hDC,1,false,false);
		//Invalidate();
	}
}

// 设置工具状态
void CMyMapView::SetTools(int nTools)
{
	m_nDrawCurrent = nTools;
	switch(m_nDrawCurrent)
	{
	case 11:
		PushNumb =0;
		break;
	case 13:
		PushNumb =0;
		break;
	case 16:
		PushNumb =0;
		m_fTatolDistance = 0;
		m_nPointCount = 0;
		break;
	default:
		break;
	}
}
// 获取工具状态
int CMyMapView::GetTools()
{
	return m_nDrawCurrent;
}
void CMyMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags,HDC hDC)
{
	switch (nChar)
	{
		case   VK_DELETE: 
			{
				// 删除编辑的点目标
				if(bEditDN)
				{
					pLine->DeletePoint(&mpt);
					Draw(hDC,1,true,false);
				}
			}
	}
}



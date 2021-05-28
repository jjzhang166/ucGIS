// Car.cpp: implementation of the CCar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "gps.h"
#include "Car.h"
#include "math.h"
#include "Mmsystem.h"
#include "log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCar::CCar()
{
	hGPSFile = NULL;
	hMifFile = NULL;
	hMidFile = NULL;
	m_mapCoordMap = NULL;
	m_pSound = NULL;

	m_bSaveTrace = false;   // 保存轨迹吗？
	m_bShowTrace = true;   // 显示轨迹吗？
	nLastSpeedAlterTime = GetTickCount();

	m_bCalledDraw  = false;

	m_fDistance = 0;
	m_fArea = 0;
	m_averSpeed = 0;
	GetSystemTime(&m_stBeginTime);

	memset(m_szNiceName,0,sizeof(m_szNiceName));
	memset(m_szTraceFilePath,0,sizeof(m_szTraceFilePath));
#ifdef UNDER_WINCE
	m_ceDraw = NULL;
#endif	

	m_dwLastValueGpsDataTime = 0 ;  // 收到最后一个有效GPS数据的时间
	m_dwLastComDataTime = 0;  // 收到最后一个串口数据的时间

	InitializeCriticalSection(&m_csDraw);

}

CCar::~CCar()
{
	// 画多边形
	GPSPOINT* point = NULL;
	int nCount = m_vectorPoints.size();
	for(int i=0;i<nCount;i++)
	{
		point = NULL;
		point = m_vectorPoints[i];
		if(point)
		{
			delete point;
			point = NULL;
		}
	}		//CloseFile(hGPSFile);
	m_vectorPoints.clear();
	DeleteCriticalSection(&m_csDraw);
}
void CCar::ClearTrack()
{
		int nCount = m_vectorPoints.size();

		for(int i=0;i<nCount;i++)
		{
			delete m_vectorPoints[i];
			m_vectorPoints[i] = NULL;
		}
		m_vectorPoints.clear();  // 由于没有做线程间的访问的锁定，这里有可能引起内存泄漏

}

void CCar::Draw(HDC hDC)
{
	
	if(m_bShowTrace) DrawTrace(hDC);
	DrawCar(hDC);
	m_bCalledDraw = true;
}
//
//  画箭头
//
void DrawArrow(HDC hDC,POINT m_One, POINT m_Two)
{
  double slopy , cosy , siny;
  double Par = 10.0;  //length of Arrow (>)
  slopy = atan2( (double)( m_One.y - m_Two.y ),
    (double)( m_One.x - m_Two.x ) );
  cosy = cos( slopy );
  siny = sin( slopy ); //need math.h for these functions
 
  /*/-------------similarly the the other end-------------/*/
  MoveToEx(hDC, m_Two.x,m_Two.y,NULL);
  LineTo(hDC, m_Two.x + int( Par * cosy - ( Par / 2.0 * siny ) ),
    m_Two.y + int( Par * siny + ( Par / 2.0 * cosy ) ) );
  LineTo(hDC, m_Two.x + int( Par * cosy + Par / 2.0 * siny ),
    m_Two.y - int( Par / 2.0 * cosy - Par * siny ) );
  LineTo(hDC, m_Two.x,m_Two.y );
}
//
//  画箭头
//
void DrawArrow(HDC hDC,int x,int y,int angle)
{
	
}
#define PI 3.14159
void CCar::AddPoint(CGpsDataInfo* pGpsData, HDC hDC)
{
	if(!pGpsData) return;

	m_dwLastComDataTime = GetTickCount();;  // 收到最后一个串口数据的时间

	// 做数据合法检测
	// 如果是GPS刚启动，则放弃数据
	if (pGpsData->m_nState == -1)
	{
		delete pGpsData;
		pGpsData = NULL;
		return;
	}

	// 如果数据不是有效数据，或者不是GPS类型的数据，则放弃，不做
	// 但要保持状态显示
	if(pGpsData->nDataType!=1||pGpsData->m_nState<2)
	{
		m_gpsDataInfo.m_nState   = pGpsData->m_nState;
		m_gpsDataInfo.m_nStCount = pGpsData->m_nStCount;
		m_gpsDataInfo.m_dbHeigth = pGpsData->m_dbHeigth;
		delete pGpsData;
		pGpsData = NULL;
		return;
	}
	// 如果速度很慢，则方向保持上一方向
	if((pGpsData->m_nSpeed<5))
		pGpsData->m_fAngle = m_gpsDataInfo.m_fAngle;


	// 如果数据有效，更新当前点数据，但处于2D定位状态,则只更新CAR中的当前数据点信息
	// 而不把点数据添加道轨迹中
	if(pGpsData->m_nState<3)
	{
		m_gpsDataInfo = *pGpsData;
		delete pGpsData;
		pGpsData = NULL;
		return;
	}


	if(m_gpsDataInfo.m_nState>2&&m_gpsDataInfo.m_xx!=-1&&pGpsData->m_nSpeed>1)
	{
		// 计算里程累加
		float fx = (m_gpsDataInfo.m_xx-pGpsData->m_xx)*(m_gpsDataInfo.m_xx-pGpsData->m_xx);
		float fy = (m_gpsDataInfo.m_yy-pGpsData->m_yy)*(m_gpsDataInfo.m_yy-pGpsData->m_yy);
		m_fDistance += sqrt(fx+fy)*100;

		// 计算平均速度 
		DWORD noww = GetTickCount();	
		DWORD dbTime = noww-m_dwDistanceBeginDataTime;
		if(dbTime>0)
			m_averSpeed = m_fDistance*1000/(dbTime/60/60);
		
		// 做面积累加
		if(m_vectorPoints.size()>1)
		{
		m_fArea +=( ((m_gpsDataInfo.m_xx - m_vectorPoints[0]->xx)*(pGpsData->m_yy-m_gpsDataInfo.m_yy)
			-(m_gpsDataInfo.m_yy-m_vectorPoints[0]->yy)*(pGpsData->m_xx-m_gpsDataInfo.m_xx))/2)*10000000000;
		}
	}
	m_gpsDataInfo = *pGpsData;

	// 判断是否该下车了
/*
	float xx1 = 104.187431;
	float xx2 = 30.578262;
	float yy1 = 222;
	float yy2 = 222;
	if(pGpsData->m_xx>xx1&&pGpsData->m_xx<xx2&&pGpsData->m_yy>yy1pGpsData->m_yy<yy2)
		m_pSound->Home();
*/

	m_dwLastValueGpsDataTime = GetTickCount(); // 收到最后一个有效GPS数据的时间	
	// 把点添加到轨迹，保存在文件
	// 如果两个点直接距离过小，则不记录在轨迹中，这样可以防止轨迹重复累赘
	int nCount = m_vectorPoints.size();
	// 如果点太多，则删除部分
	int nPointCountLimit = 2000;   //  最大点数量
	int nSmallCount = nPointCountLimit/10;  // 释放内存和保存点的最小点个数
	// 把点写成MIF文件
	//if(nCount>nPointCountLimit)
	//{
	//	for(int i=0;i<nSmallCount;i++)
	//	{
	//		delete m_vectorPoints[i];
	//		m_vectorPoints[i] = NULL;
	//	}
	//	vector<GPSPOINT *>::iterator p = m_vectorPoints.begin();
	//	EnterCriticalSection(&m_csDraw);
	//	m_vectorPoints.erase(p,p+nSmallCount);
	//	LeaveCriticalSection(&m_csDraw);
	//}
	// 删除太多的点目标，以免内存被耗尽
	if(nCount>nPointCountLimit)
	{
		for(int i=0;i<nSmallCount;i++)
		{
			delete m_vectorPoints[i];
			m_vectorPoints[i] = NULL;
		}
		vector<GPSPOINT *>::iterator p = m_vectorPoints.begin();
		EnterCriticalSection(&m_csDraw);
		m_vectorPoints.erase(p,p+nSmallCount);
		LeaveCriticalSection(&m_csDraw);
	}

	// 速度语音提示
	//if(((pGpsData->m_nSpeed-m_gpsDataInfo.m_nSpeed)>1)||((pGpsData->m_nSpeed-m_gpsDataInfo.m_nSpeed)<-1))
	DWORD now = GetTickCount();
	if((now-nLastSpeedAlterTime)>10*1000)
	{
		if(m_pSound)
			m_pSound->Speed((int)pGpsData->m_nSpeed);
		nLastSpeedAlterTime = GetTickCount();
	}

	// 添加点信息到列表中
	if((nCount==0)||
		((fabs(m_vectorPoints[nCount-1]->xx - pGpsData->m_xx)>0.00002)&&
		(fabs(m_vectorPoints[nCount-1]->yy - pGpsData->m_yy)>0.00002)
		))
	{
		GPSPOINT* point  = new GPSPOINT;
		if(point==NULL)
		{
			delete pGpsData;
			pGpsData = NULL;
			return;
		}
		point->xx = pGpsData->m_xx;
		point->yy = pGpsData->m_yy;

		EnterCriticalSection(&m_csDraw);
		m_vectorPoints.push_back(point);
		LeaveCriticalSection(&m_csDraw);
		if(m_bSaveTrace)
		{
			SaveTrace(point);
		}
	}

	delete pGpsData;
	pGpsData = NULL;

	m_bCalledDraw = false;
}
void CCar::GetAngleXY(const float fmyAngle,const int x, const int y, const int nLineLen,int& x1,int& y1)
{
	float  fAngle = 0;
	//if(fmyAngle>359)
		fAngle = fmod(fmyAngle,360)/180;
	//else
	//	fAngle = fmyAngle/180;

	if((fAngle>=0)&(fAngle<=0.5))
	{
		x1 = x+sin(fAngle*PI)*nLineLen;
		y1 = y-cos(fAngle*PI)*nLineLen;
	}
	else if((fAngle>0.5)&(fAngle<=1))
	{
		x1 = x+cos((fAngle-0.5)*PI)*nLineLen;
		y1 = y+sin((fAngle-0.5)*PI)*nLineLen;
	}
	else if((fAngle>1)&(fAngle<=1.5))
	{
		x1 = x-sin((fAngle-1)*PI)*nLineLen;
		y1 = y+cos((fAngle-1)*PI)*nLineLen;
	}
	else if((fAngle>1.5)&(fAngle<=2))
	{
		x1 = x-cos((fAngle-1.5)*PI)*nLineLen;
		y1 = y-sin((fAngle-1.5)*PI)*nLineLen;
	}
}
void CCar::DrawCar(HDC hDC)
{
	
	if (m_gpsDataInfo.m_nState==-1) return;
	
	int x,y;
	m_mapCoordMap->DPtoVP(m_gpsDataInfo.m_xx,m_gpsDataInfo.m_yy,&x,&y);
	int n = 18;
	int nLineLen = 100;

	n = 13;
	// 画车箭头
	int xb1=0,yb1=0;
	int xb2=0,yb2=0;
	int xb3=0,yb3=0;
	int xb4=0,yb4=0;
	GetAngleXY(m_gpsDataInfo.m_fAngle,x,y,n-1,xb1,yb1);
	GetAngleXY(m_gpsDataInfo.m_fAngle+135,x,y,n-1,xb2,yb2);
	GetAngleXY(m_gpsDataInfo.m_fAngle+225,x,y,n-1,xb3,yb3);
	GetAngleXY(m_gpsDataInfo.m_fAngle+180,x,y,n/2-1,xb4,yb4);

	POINT pts1[4],pts2[4],pts3[4];
	pts1[0].x = xb1;
	pts1[0].y = yb1;
	pts1[1].x = xb2;
	pts1[1].y = yb2;
	pts1[2].x = x;
	pts1[2].y = y;

	pts2[0].x = xb1;
	pts2[0].y = yb1;
	pts2[1].x = xb3;
	pts2[1].y = yb3;
	pts2[2].x = x;
	pts2[2].y = y;

	pts3[0].x = x;
	pts3[0].y = y;
	pts3[1].x = xb2;
	pts3[1].y = yb2;
	pts3[2].x = xb4;
	pts3[2].y = yb4;
	pts3[3].x = xb3;
	pts3[3].y = yb3;

#ifdef UNDER_WINCE
	CCEPen cePen;
	CCEBrush ceBrush;
	ceBrush.CreateBrush( 1, m_ceDraw->ConvertColor(255,0,0 ) , 1 );

	m_ceDraw->SetGDIObject( &ceBrush );
	m_ceDraw->SetGDIObject( &cePen );
	cePen.CreatePen(1,1, m_ceDraw->ConvertColor(255,0,0 ) ); // Convert RGB color to CE support Colo
	m_ceDraw->FillPolygon( pts1, 3, 0, 0  ); // Draw an polygon
	m_ceDraw->DrawPolygon( pts1, 3); // Draw an polygon
	
	ceBrush.CreateBrush( 1, m_ceDraw->ConvertColor(0,255,0 ) , 1 );
	cePen.CreatePen(1,1, m_ceDraw->ConvertColor(0,255,0 ) ); // Convert RGB color to CE support Colo
	m_ceDraw->FillPolygon( pts2, 3, 0, 0  ); // Draw an polygon
	m_ceDraw->DrawPolygon(pts2,3);

	ceBrush.CreateBrush( 1, m_ceDraw->ConvertColor(0,0,255 ) , 1 );
	cePen.CreatePen(1,1, m_ceDraw->ConvertColor(0,0,255 ) ); // Convert RGB color to CE support Colo
	m_ceDraw->FillPolygon( pts3, 4, 0, 0  ); // Draw an polygon
	m_ceDraw->DrawPolygon(pts3,4);
#else
	HPEN	oldPen,newPen,newPen1,newPen2;
	// 虚线的外圆

	newPen = CreatePen(PS_DOT,1,RGB(255,0,0));  //PS_DASH

	oldPen	= (HPEN)SelectObject(hDC,newPen);

	HBRUSH newBrush, oldBrush,newBrush1,newBrush2,newBrush3;
	newBrush = (HBRUSH)GetStockObject (NULL_BRUSH);//CreateSolidBrush(RGB(255,0,0));NULL_PEN
	oldBrush = (HBRUSH)SelectObject(hDC,newBrush);
	Ellipse(hDC,x-n,y-n,x+n,y+n);

	newPen1 = CreatePen(PS_SOLID,1,RGB(255,0,0));  //PS_DASH
	oldPen	= (HPEN)SelectObject(hDC,newPen1);

	// 画 方向符号
	if(m_gpsDataInfo.m_nSpeed>0)
	{
		int x1=0,y1=0;
		nLineLen  = m_gpsDataInfo.m_nSpeed;
		MoveToEx(hDC,x,y,NULL);
		GetAngleXY(m_gpsDataInfo.m_fAngle,x,y,nLineLen,x1,y1);
		LineTo(hDC,x1,y1);
		/*
		MoveToEx(hDC,xb1,yb1,NULL);
		int x1=0,y1=0;
		int x2=0,y2=0;
		GetAngleXY(m_gpsDataInfo.m_fAngle+2+360,x,y,nLineLen,x1,y1);
		GetAngleXY(m_gpsDataInfo.m_fAngle-2+360,x,y,nLineLen,x2,y2);

		POINT pts[3];

		pts[0].x = x;
		pts[0].y = y;
		pts[1].x = x1;
		pts[1].y = y1;
		pts[2].x = x2;
		pts[2].y = y2;

		Polygon(hDC,pts,3);
		*/
	}
	newBrush1 = (HBRUSH)CreateSolidBrush(RGB(255,23,0));
	newPen2 = (HPEN)GetStockObject (NULL_PEN);
	oldBrush = (HBRUSH)SelectObject(hDC,newBrush1);
	oldPen = (HPEN)SelectObject(hDC,newPen2);
	Polygon(hDC,pts1,3);

	newBrush2 = (HBRUSH)CreateSolidBrush(RGB(0,245,56));
	oldBrush = (HBRUSH)SelectObject(hDC,newBrush2);

	Polygon(hDC,pts2,3);

	newBrush3 = (HBRUSH)CreateSolidBrush(RGB(0,23,255));
	oldBrush = (HBRUSH)SelectObject(hDC,newBrush3);
	Polygon(hDC,pts3,4);
	// 画目标点的标注
	HFONT newFont, oldFont;

	LOGFONT	lf = {0};
	lf.lfHeight = 14;
	lf.lfWidth = 0;
	lf.lfWeight = 0;
	lf.lfCharSet = GB2312_CHARSET;
	lf.lfEscapement=lf.lfOrientation=0;
	memcpy(lf.lfFaceName,TEXT("宋体"),sizeof(lf.lfFaceName));

	newFont =  (HFONT)CreateFontIndirect(&lf);
	oldFont = (HFONT)SelectObject(hDC,newFont);
	SetBkMode(hDC,TRANSPARENT);
	COLORREF oldFontColor = SetTextColor(hDC,RGB(0,255,0));
	ExtTextOut(hDC, x+15,y-5, 0, NULL, (LPCTSTR)(m_szNiceName), 
         lstrlen(m_szNiceName),0);

	SetTextColor(hDC,RGB(0,0,0));
	ExtTextOut(hDC, x+16,y-6, 0, NULL, (LPCTSTR)(m_szNiceName), 
         lstrlen(m_szNiceName),0);

	SelectObject(hDC,oldFont);
	DeleteObject(newFont);
	SetTextColor(hDC,oldFontColor);
	// 
	SelectObject(hDC,oldBrush);	
	DeleteObject(newBrush);
	SelectObject(hDC,oldPen);
	DeleteObject(newPen);
	DeleteObject(newPen1);
	DeleteObject(newPen2);
	DeleteObject(newBrush1);
	DeleteObject(newBrush2);
	DeleteObject(newBrush3);
#endif
}
void CCar::DrawTrace(HDC hDC)
{
	if(m_vectorPoints.size()<2) return;
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
	newPen = CreatePen(PS_SOLID,2,RGB(0,255,0));
	oldPen	= (HPEN)SelectObject(hDC,newPen);

	int x,y;
	int nCount = m_vectorPoints.size();
	bool bBreak = false;
	bool bFirstOutPoint = false;
	for(int i=0;i<nCount;i++)
	{
		m_mapCoordMap->DPtoVP(m_vectorPoints[i]->xx,m_vectorPoints[i]->yy,&x,&y);
		// 如果点在屏幕内
		if(
			(x<m_mapCoordMap->wScreen)&&
			(x>0)&&
			(y<m_mapCoordMap->hScreen)&&
			(y>0)
			)
		{
			bFirstOutPoint = true;
			if((i==0)|bBreak)
				MoveToEx(hDC,x,y,NULL);
			else
				LineTo(hDC,x,y);
			bBreak = false;
		}
		else
		{
			if(bFirstOutPoint)
			{
				LineTo(hDC,x,y);
				bFirstOutPoint = false;
			}
		
			bBreak = true;				
		}
	}
	SelectObject(hDC,oldPen);
	DeleteObject(newPen);
#endif
}
void CCar::SaveTrace(GPSPOINT* point)
{
	DWORD dwRet = 0;
	if(hGPSFile==NULL)
	{


//		TCHAR lpszFileName[512] = L"\\Storage Card\\GPS\\trace.txt";
//		_sntprintf(lpszFileName,512,
//			TEXT("%s\\%s"),m_lpszFilePath,L"轨迹.tr");

		hGPSFile = CreateFileW(L"trace.txt",//(LPCWSTR)lpszFileName,   // create file
					GENERIC_WRITE,                // open for writing 
					0,                            // do not share 
					NULL,                         // no security 
					OPEN_ALWAYS,                  // overwrite existing 
					FILE_ATTRIBUTE_NORMAL,        // normal file 
					NULL);                        // no attr. template 
		SetFilePointer(hGPSFile,0,0,FILE_END);
		char* line = "ANOTHER LINE\r\n";
		WriteFile(hGPSFile,line,strlen(line),&dwRet,NULL);
	}
	
	char ch[200] = {0};
	sprintf(ch,"%3.6f %3.5f\r\n",point->xx,point->yy);

	// 如果点和点直接的距离太大则认为是另条轨迹
	if((m_vectorPoints.size()>0
		&&((abs(m_gpsDataInfo.m_xx - point->xx)>0.1)
		|(abs(m_gpsDataInfo.m_yy - point->yy)>0.1))))
	{
		char* line = "ANOTHER LINE\r\n";
		WriteFile(hGPSFile,line,strlen(line),&dwRet,NULL);
	}

	WriteFile(hGPSFile,ch,strlen(ch),&dwRet,NULL);
}
void CCar::SaveTrackToMif()  // 把轨迹保存成mif文件
{
	DWORD dwRet = 0;
	if(hMifFile==NULL)
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
		TCHAR szMifFilePath[MAX_PATH] = {0};
		TCHAR szMidFilePath[MAX_PATH] = {0};
		TCHAR szNow[MAX_PATH] = {0};
		memcpy(szPicPath,szFileName,len*2);

		SYSTEMTIME time;
		GetSystemTime(&time);
		_stprintf(szNow,_T("%.2d-%.2d-%.2d %.2d_%.2d_%.2d"),
			time.wYear,
			time.wMonth,
			time.wDay,
			time.wHour,
			time.wMinute,
			time.wSecond);

		_stprintf(szMifFilePath,_T("%s\\Mif\\%s.mif"),szPicPath,szNow);
		_stprintf(szMidFilePath,_T("%s\\Mif\\%s.mid"),szPicPath,szNow);

		hMifFile = CreateFile(szMifFilePath,//(LPCWSTR)lpszFileName,   // create file
					GENERIC_WRITE,                // open for writing 
					0,                            // do not share 
					NULL,                         // no security 
					OPEN_ALWAYS,                  // overwrite existing 
					FILE_ATTRIBUTE_NORMAL,        // normal file 
					NULL);   
		hMidFile = CreateFile(szMidFilePath,//(LPCWSTR)lpszFileName,   // create file
					GENERIC_WRITE,                // open for writing 
					0,                            // do not share 
					NULL,                         // no security 
					OPEN_ALWAYS,                  // overwrite existing 
					FILE_ATTRIBUTE_NORMAL,        // normal file 
					NULL);  		// no attr. template 
		if (hMifFile != INVALID_HANDLE_VALUE)
		{
		char* szHeader = "Version 450\r\n\
Charset \"WindowsSimpChinese\"\r\n\
Delimiter \",\"\r\n\
Index 1\r\\
nCoordSys Earth Projection 1, 104\r\n\
Columns 1\r\n\
  name Char(50)\r\nData";

		WriteFile(hMifFile,szHeader,strlen(szHeader),&dwRet,NULL);
		}
		/*
		SetFilePointer(hGPSFile,0,0,FILE_END);
		SetFilePointer(hGPSFile,0,0,FILE_END);
		char* line = "ANOTHER LINE\r\n";
		WriteFile(hGPSFile,line,strlen(line),&dwRet,NULL);
		*/
	}
	if (hMifFile == INVALID_HANDLE_VALUE) return;
	if (hMidFile == INVALID_HANDLE_VALUE) return;
}
void CCar::SaveLiCheng()
{
	// 保存里程信息到文件
	HANDLE	hFile = CreateFile(TEXT("\\Storage Card\\GPS\\里程.txt"),//(LPCWSTR)lpszFileName,   // create file
					GENERIC_WRITE,                // open for writing 
					0,                            // do not share 
					NULL,                         // no security 
					OPEN_ALWAYS,                  // overwrite existing 
					FILE_ATTRIBUTE_NORMAL,        // normal file 
					NULL);                        // no attr. template 
	if (hFile != INVALID_HANDLE_VALUE) 
	{
		SetFilePointer(hFile,0,0,FILE_END);
		char szBeginTime[50]={0},szNow[50]={0},szMsg[200]={0};
		sprintf(szBeginTime,"%.2d-%.2d-%.2d %.2d:%.2d:%.2d",
			m_stBeginTime.wYear,
			m_stBeginTime.wMonth,
			m_stBeginTime.wDay,
			m_stBeginTime.wHour,
			m_stBeginTime.wMinute,
			m_stBeginTime.wSecond);

		SYSTEMTIME time;
		GetSystemTime(&time);

		sprintf(szNow,"%.2d-%.2d-%.2d %.2d:%.2d:%.2d",
			time.wYear,
			time.wMonth,
			time.wDay,
			time.wHour,
			time.wMinute,
			time.wSecond);

		sprintf(szMsg,"%s TO %s %f\r\n",szBeginTime,szNow,m_fDistance);
		DWORD dwRet = 0;
		WriteFile(hFile,szMsg,strlen(szMsg),&dwRet,NULL);
	}
//	else
//		MessageBox(NULL,TEXT("write licheng err！"),TEXT(""),MB_OK);
}

void CCar::GetPoints(mapMAPPOINTSTRUCT *points, int &nLen)
{
	nLen = 0;
	int nCount = m_vectorPoints.size();
	if(nCount==0) return;
	if(points==NULL) return;
	
	for(int i=0;i<nCount;i++)
	{
		(points)[i].x = m_vectorPoints[i]->xx;
		(points)[i].y = m_vectorPoints[i]->yy;
	}
	nLen = nCount;
	ClearTrack();
}

void CCar::SetDistance()
{
	m_dwDistanceBeginDataTime = GetTickCount();
	m_fDistance = 0.0;
	m_averSpeed = 0.0;
}

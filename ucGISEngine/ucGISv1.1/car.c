#define CAR_GLOBALES
#include "gpsdatainfo.h"
#include "coortrans.h"
#include "car.h"

void Car_ClearTrack(lpCar pcar)
{
		if(pcar->guiArrayPoints.haHandle)
		{
			GUI_ARRAY_Delete(&pcar->guiArrayPoints);
		   memset(&pcar->guiArrayPoints,0,sizeof(GUI_ARRAY));
		}
		else 
			return;

}
void Car_Draw(lpCar pcar)
{
	
	if(pcar->m_bShowTrace) DrawTrace(pcar);
	DrawCar(pcar);
	pcar->m_bCalledDraw = TRUE;
}
//
//  画箭头
//
void DrawArrow(GUI_POINT m_One, GUI_POINT m_Two)
{
  double slopy , cosy , siny;
  double Par = 10.0;  //length of Arrow (>)
  slopy = atan2( (double)( m_One.y - m_Two.y ),
    (double)( m_One.x - m_Two.x ) );
  cosy = cos( slopy );
  siny = sin( slopy ); //need math.h for these functions
 
  /*/-------------similarly the the other end-------------/*/
  GUI_MoveTo( m_Two.x,m_Two.y);
  GUI_DrawLineTo( m_Two.x + (int)( Par * cosy - ( Par / 2.0 * siny )),m_Two.y + (int)( Par * siny + ( Par / 2.0 * cosy ) ) );
  GUI_DrawLineTo( m_Two.x + (int)( Par * cosy + Par / 2.0 * siny ), m_Two.y - (int)( Par / 2.0 * cosy - Par * siny ) );
  GUI_DrawLineTo( m_Two.x,m_Two.y );
}
//
//  画箭头
//
#define PI 3.14159
void Car_AddPoint(GpsDataInfo* pGpsData,lpCar pcar)
{
	float fx,fy;
	GPSPOINT *pgpspoint;
	GISLONG noww ,dbTime,now ;
	int i=0;
	int nPointCount=0;
	int nPointCountLimit = 2000;   //  最大点数量
	int nSmallCount = nPointCountLimit/10;  // 释放内存和保存点的最小点个数

	if(!pGpsData) return;

	//m_dwLastComDataTime = GetTickCount();;  // 收到最后一个串口数据的时间

	// 做数据合法检测
	// 如果是GPS刚启动，则放弃数据
	if (pGpsData->m_nState == -1)
	{
		//delete pGpsData;
		//pGpsData = NULL;
		return;
	}

	// 如果数据不是有效数据，或者不是GPS类型的数据，则放弃，不做
	// 但要保持状态显示
	if(pGpsData->nDataType!=1||pGpsData->m_nState<2)
	{
		pcar->m_gpsDataInfo.m_nState   = pGpsData->m_nState;
		pcar->m_gpsDataInfo.m_nStCount = pGpsData->m_nStCount;
		pcar->m_gpsDataInfo.m_dbHeigth = pGpsData->m_dbHeigth;
		//delete pGpsData;
		//pGpsData = NULL;
		return;
	}
	// 如果速度很慢，则方向保持上一方向
	if((pGpsData->m_nSpeed<5))
		pGpsData->m_fAngle = pcar->m_gpsDataInfo.m_fAngle;


	// 如果数据有效，更新当前点数据，但处于2D定位状态,则只更新CAR中的当前数据点信息
	// 而不把点数据添加道轨迹中
	if(pGpsData->m_nState<3)
	{
		GpsDataInfoSetpValue(&pcar->m_gpsDataInfo ,pGpsData);
		return;
	}

	if(pcar->guiArrayPoints.haHandle)
		nPointCount=GUI_ARRAY_GetNumItems(&pcar->guiArrayPoints);
	else
		GUI_ARRAY_Create(&pcar->guiArrayPoints);

	if(pcar->m_gpsDataInfo.m_nState>2&&pcar->m_gpsDataInfo.m_xx!=-1&&pGpsData->m_nSpeed>1)
	{
		// 计算里程累加
		fx = (pcar->m_gpsDataInfo.m_xx-pGpsData->m_xx)*(pcar->m_gpsDataInfo.m_xx-pGpsData->m_xx);
		fy = (pcar->m_gpsDataInfo.m_yy-pGpsData->m_yy)*(pcar->m_gpsDataInfo.m_yy-pGpsData->m_yy);
		pcar->m_fDistance += sqrt(fx+fy)*100;

		// 计算平均速度 
		noww = GetTickCount();	
		dbTime = noww-pcar->m_dwDistanceBeginDataTime;
		if(dbTime>0)
			pcar->m_averSpeed = pcar->m_fDistance*1000/(dbTime/60/60);
		
		// 做面积累加
		if(nPointCount>1)
		{
           pgpspoint=GUI_ARRAY_GetpItem(&pcar->guiArrayPoints,0);
		   pcar->m_fArea +=( ((pcar->m_gpsDataInfo.m_xx - pgpspoint->xx)*(pGpsData->m_yy-pcar->m_gpsDataInfo.m_yy)
			-(pcar->m_gpsDataInfo.m_yy-pgpspoint->yy)*(pGpsData->m_xx-pcar->m_gpsDataInfo.m_xx))/2)*10000000000;
		}
	}
	GpsDataInfoSetpValue(&pcar->m_gpsDataInfo ,pGpsData);


	// 判断是否该下车了
/*
	float xx1 = 104.187431;
	float xx2 = 30.578262;
	float yy1 = 222;
	float yy2 = 222;
	if(pGpsData->m_xx>xx1&&pGpsData->m_xx<xx2&&pGpsData->m_yy>yy1pGpsData->m_yy<yy2)
		m_pSound->Home();
*/

	pcar->m_dwLastValueGpsDataTime = GetTickCount(); // 收到最后一个有效GPS数据的时间	
	// 把点添加到轨迹，保存在文件
	// 如果两个点直接距离过小，则不记录在轨迹中，这样可以防止轨迹重复累赘
	//int nCount = m_vectorPoints.size();
	// 如果点太多，则删除部分
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
	if(nPointCount>nPointCountLimit)
	{
		for(i=0;i<nSmallCount;i++)
		{
            GUI_ARRAY_DeleteItem(&pcar->guiArrayPoints,i);
		}
	}

	// 速度语音提示
	//if(((pGpsData->m_nSpeed-m_gpsDataInfo.m_nSpeed)>1)||((pGpsData->m_nSpeed-m_gpsDataInfo.m_nSpeed)<-1))
	now = GetTickCount();
	if((now-pcar->nLastSpeedAlterTime)>10*1000)
	{
		if(pcar->pSoundFunc)
			pcar->pSoundFunc((int)pGpsData->m_nSpeed);
		pcar->nLastSpeedAlterTime = GetTickCount();
	}
    if(nPointCount>0)
	{
       pgpspoint=GUI_ARRAY_GetpItem(&pcar->guiArrayPoints,nPointCount-1);
       
	}
	// 添加点信息到列表中
	if((nPointCount==0)||
		((fabs(pgpspoint->xx - pGpsData->m_xx)>0.00002)&&
		(fabs(pgpspoint->yy - pGpsData->m_yy)>0.00002)
		))
	{
        GUI_ARRAY_AddItem(&pcar->guiArrayPoints,pGpsData,sizeof(GPSPOINT));
		if(pcar->m_bSaveTrace)
		{
			//SaveTrace(pGpsData);
		}
	}

	pcar->m_bCalledDraw = FALSE;
}
void GetAngleXY(const float fmyAngle,const int x, const int y, const int nLineLen,int* x1,int* y1)
{
	float  fAngle = 0;
	//if(fmyAngle>359)
		fAngle = fmod(fmyAngle,360)/180;
	//else
	//	fAngle = fmyAngle/180;

	if((fAngle>=0)&(fAngle<=0.5))
	{
		*x1 = x+sin(fAngle*PI)*nLineLen;
		*y1 = y-cos(fAngle*PI)*nLineLen;
	}
	else if((fAngle>0.5)&(fAngle<=1))
	{
		*x1 = x+cos((fAngle-0.5)*PI)*nLineLen;
		*y1 = y+sin((fAngle-0.5)*PI)*nLineLen;
	}
	else if((fAngle>1)&(fAngle<=1.5))
	{
		*x1 = x-sin((fAngle-1)*PI)*nLineLen;
		*y1 = y+cos((fAngle-1)*PI)*nLineLen;
	}
	else if((fAngle>1.5)&(fAngle<=2))
	{
		*x1 = x-cos((fAngle-1.5)*PI)*nLineLen;
		*y1 = y-sin((fAngle-1.5)*PI)*nLineLen;
	}
}
//

void DrawCar(lpCar pcar)
{
	
    int x,y;
    FP32  lon,lat;
	int n = 18;
	int nLineLen = 100;
	int xb1=0,yb1=0;
	int xb2=0,yb2=0;
	int xb3=0,yb3=0;
	int xb4=0,yb4=0;
	GUI_POINT pts1[4],pts2[4],pts3[4];

	if (pcar->m_gpsDataInfo.m_nState==-1) return;
	
	lon=pcar->m_gpsDataInfo.m_xx;
	lat=pcar->m_gpsDataInfo.m_yy;

	DPtoVP(pcar->m_mapCoordMap,lon,lat,&x,&y);

	n = 13;
	// 画车箭头
	GetAngleXY(pcar->m_gpsDataInfo.m_fAngle,x,y,n-1,&xb1,&yb1);
	GetAngleXY(pcar->m_gpsDataInfo.m_fAngle+135,x,y,n-1,&xb2,&yb2);
	GetAngleXY(pcar->m_gpsDataInfo.m_fAngle+225,x,y,n-1,&xb3,&yb3);
	GetAngleXY(pcar->m_gpsDataInfo.m_fAngle+180,x,y,n/2-1,&xb4,&yb4);

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

	GUI_SetAlpha(0x80);
	GUI_SetColor(GIS_RGB(255,0,0));
	GUI_FillPolygon( pts1, 3, 0, 0  ); // Draw an polygon
	GUI_DrawPolygon( pts1, 3,0,0); // Draw an polygon
	
	GUI_SetColor(GIS_RGB(0,255,0));
	GUI_FillPolygon( pts2, 3, 0, 0  ); // Draw an polygon
	GUI_DrawPolygon(pts2,3,0,0);

	GUI_SetColor(GIS_RGB(0,0,255));
	GUI_FillPolygon( pts3, 4, 0, 0  ); // Draw an polygon
	GUI_DrawPolygon(pts3,4,0,0);
    GUI_SetAlpha(0);
}
void DrawTrace(lpCar pcar)
{
	int nCount,i;
	GPSPOINT *pgpspoint;
#if USEDYNAALLOC
    GUI_HMEM  phmem;
	GUI_POINT *point;
#else
	GUI_POINT point[1000];
#endif
	nCount=0;
	if(pcar->guiArrayPoints.haHandle)
	  nCount= GUI_ARRAY_GetNumItems(&pcar->guiArrayPoints);
	if(nCount<2) return;
#if USEDYNAALLOC
	phmem=GUI_ALLOC_AllocZero(sizeof(GUI_POINT)*(nCount+1));
	point=(GeoPoint*)GUI_ALLOC_h2p(phmem);
	if(point==NULL) return;
#endif

	for(i=0;i<nCount;i++)
	{
		pgpspoint=GUI_ARRAY_GetpItem(&pcar->guiArrayPoints,i);
		DPtoVP(pcar->m_mapCoordMap,pgpspoint->xx,pgpspoint->yy,(int*)&point[i].x,(int*)&point[i].y);	
	}
	DPtoVP(pcar->m_mapCoordMap,pcar->m_gpsDataInfo.m_xx,pcar->m_gpsDataInfo.m_yy,(int*)&point[nCount].x,(int*)&point[nCount].y);	
	GUI_SetColor(GIS_RGB(0,0,255));
	GUI_DrawPolyLine(point,nCount+1,0,0);
#if USEDYNAALLOC
	GUI_ALLOC_Free(phmem);
	phmem=0;
	point = NULL;
#endif
}
/*
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
*/
void SetDistance(lpCar pcar)
{
	pcar->m_dwDistanceBeginDataTime = GetTickCount();
	pcar->m_fDistance = 0.0;
	pcar->m_averSpeed = 0.0;
}

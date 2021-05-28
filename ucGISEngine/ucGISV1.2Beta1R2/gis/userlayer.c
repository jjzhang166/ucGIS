#define USERLAYER_GLOBALES
#include "gui.h"
#include "userlayer.h"
#include "coortrans.h"
#include "car.h"
void UserLayerInitial(lpUserLayer puserlayer)
{
	puserlayer->m_mapCoordMap = NULL;
	puserlayer->m_pNavCar = NULL;
	puserlayer->m_pSoundFunc= NULL;
	puserlayer->m_bShowState = TRUE;
	puserlayer->m_bShowMeasureLen = FALSE;
	puserlayer->m_bShowMeasureArea = FALSE;
	puserlayer->m_nNetState = 0;
	puserlayer->m_pBmpCenter = NULL;
	if(puserlayer->carArray.haHandle)
	{
       GUI_ARRAY_Delete(&puserlayer->carArray);
	}

}
void UserLayerFree(lpUserLayer puserlayer)
{
	if(puserlayer->carArray.haHandle)
	{
       GUI_ARRAY_Delete(&puserlayer->carArray);
	}
    memset(puserlayer,0,sizeof(UserLayer));
}

void UserLayer_Draw(lpUserLayer puserlayer)
{
	int i,nCount=0;
	lpCar pcar;
	if(puserlayer->m_bShowState)
		GisDrawState(puserlayer);
	if(puserlayer->carArray.InitState== GUI_ARRAY_STATE_NOT_CREATED) return;
    nCount=GUI_ARRAY_GetNumItems(&puserlayer->carArray);
	for(i=0;i<nCount;i++)
	{
        pcar=GUI_ARRAY_GetpItem(&puserlayer->carArray,i);
		DrawCar(pcar);//lpUserLayer->m_pNavCar);
	}
	DrawXX(puserlayer);  // 画中心图标
}
void GisDrawState(lpUserLayer puserlayer)
{
	GUI_RECT rect;
	GISLONG now;
	int nDistance;
	int cx,cy;
	//GUI_POINT pt;
    GUI_COLOR TextColor,RecColor,FillColor,LineColor;
    GISCHAR lpszState[30] = {0};
    GUI_SetFont(gGisFont);
    //GUI_SetColor(GUI_RED);
    //GUI_SetBkColor(GUI_BLACK);
	GUI_SetTextMode(GUI_TM_TRANS);
	//GUI_SetTextMode(GUI_TM_TRANS);
	//GUI_SetTextAlign(GUI_TA_LEFT);
    GUI_SetAlpha(0x80);
	rect.x0 = 0;
	rect.y0 = 0;//puserlayer->m_mapCoordMap->hScreen-20;
	rect.y1 = rect.y0+17;
	rect.x1= rect.x0+39;

    FillColor=RecColor=GIS_RGB(255,230,255);
    TextColor=GUI_TRANSPARENT;
	LineColor=GUI_RED;
	// 绘制比例尺
	
	GUI_SetColor(RecColor);
	GUI_FillRectEx(&rect);

	GUI_SetColor(LineColor);
	GUI_DrawLine(2,14,34,14);
	GUI_DrawLine(2,9,2,14);
	GUI_DrawLine(35,9,35,14);

	now = GetTickCount();
	if(puserlayer->m_pNavCar!=NULL)
	{
		// 如果没有收到有效的GPS数据，显示黄色边框
		// 如果根本没有收到串口数据，显示红色色边框
		 if((now-puserlayer->m_pNavCar->m_dwLastComDataTime)>3*1000)
			RecColor=GIS_RGB(255,255,232 ); // 浅色
		else if((now-puserlayer->m_pNavCar->m_dwLastValueGpsDataTime)<5*1000)
			RecColor=GIS_RGB(255,0,0 ) ; // 红色边框
		else
			RecColor=GIS_RGB(255,255,0 ); // 黄色
	}else
	{
		RecColor=GIS_RGB(255,0,0 ) ; // 红色边框//
	}
	GUI_DrawRectEx(&rect);
		// 计算比例尺
	nDistance = puserlayer->m_mapCoordMap->blc*32*100000;

	if(nDistance>999)
		sprintf(lpszState,"%dKm",nDistance/1000);
	else
		sprintf(lpszState,"%dM",nDistance);
    GisGetTextExtentPoint(lpszState,&cx,&cy);
	GUI_SetColor(TextColor);
	GUI_DispStringAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);
	// 速度
	rect.x0 = rect.x1;
	rect.x1 += 46;
    FillColor=RecColor=GIS_RGB(255,230,255);
	if(puserlayer->m_pNavCar!=NULL)
	{

		GUI_SetColor(FillColor); // 浅色
			GUI_FillRectEx(&rect);
		GUI_SetColor(RecColor); // 浅色
			GUI_DrawRectEx(&rect);

		if(puserlayer->m_pNavCar->m_gpsDataInfo.m_nSpeed==-1)
		sprintf(lpszState,"速%s","**");
		else
		sprintf(lpszState,"速%3.0f",puserlayer->m_pNavCar->m_gpsDataInfo.m_nSpeed);

		GisGetTextExtentPoint(lpszState,&cx,&cy);
		GUI_SetColor(TextColor);
		GUI_DispStringAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);
	}

		// 海拔高度
	   rect.x0 = rect.x1;
	   rect.x1 += 42;

		GUI_SetColor(FillColor); // 浅色
			GUI_FillRectEx(&rect);
		GUI_SetColor(RecColor); // 浅色
			GUI_DrawRectEx(&rect);
		
		if(puserlayer->m_pNavCar!=NULL)
		{

		if(puserlayer->m_pNavCar->m_gpsDataInfo.m_dbHeigth==-1)
		sprintf(lpszState,"高%s","**");
		else
		sprintf(lpszState,"高%4.0f",puserlayer->m_pNavCar->m_gpsDataInfo.m_dbHeigth);

		GisGetTextExtentPoint(lpszState,&cx,&cy);
		GUI_SetColor(TextColor);
		GUI_DispStringAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);
		}

		// 卫星数量
		rect.x0 = rect.x1;
		rect.x1 += 26;

		GUI_SetColor(FillColor); // 浅色
			GUI_FillRectEx(&rect);
		GUI_SetColor(RecColor); // 浅色
			GUI_DrawRectEx(&rect);
		if(puserlayer->m_pNavCar!=NULL)
		{
			if(puserlayer->m_pNavCar->m_gpsDataInfo.m_nStCount==-1)
			sprintf(lpszState,"星%s","**");
			else
			sprintf(lpszState,"星%d",puserlayer->m_pNavCar->m_gpsDataInfo.m_nStCount);

		GisGetTextExtentPoint(lpszState,&cx,&cy);
		GUI_SetColor(TextColor);
		GUI_DispStringAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);
		}
	// 网络状态
		rect.x0 = rect.x1;
		rect.x1 += 29;
		GUI_SetColor(FillColor); // 浅色
			GUI_FillRectEx(&rect);
		GUI_SetColor(RecColor); // 浅色
			GUI_DrawRectEx(&rect);

		switch(puserlayer->m_nNetState)
		{
		case NET_BREAK:
			sprintf(lpszState,"%s","网断");
			break;
		case NET_CONNECTING:
			sprintf(lpszState,"%s","连...");
			break;
		case NET_CONNECTED:
			sprintf(lpszState,"%s","登...");
			break;
		case NET_LOGINED:
			sprintf(lpszState,"%s","在线");
			break;
		case NET_LOGINFEILD:
			sprintf(lpszState,"%s","密错");
			break;
		}

    GisGetTextExtentPoint(lpszState,&cx,&cy);
	GUI_SetColor(TextColor);
	GUI_DispStringAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);

		 //
		 // 显示长度测量情况
 		 //
		 if(puserlayer->m_bShowMeasureLen)
		 {
			 
			rect.x0 = 0;
			rect.y0 = puserlayer->m_mapCoordMap->hScreen-18; 	// 当前视图的高度和宽度;
			rect.y1 = 	rect.y0+17;
			rect.x1 = puserlayer->m_mapCoordMap->wScreen/2;
		GUI_SetColor(FillColor); // 浅色
			GUI_FillRectEx(&rect);
		GUI_SetColor(RecColor); // 浅色
			GUI_DrawRectEx(&rect);

			if(puserlayer->m_pNavCar)
			{
			    sprintf(lpszState,"里程:%0.3fKm",puserlayer->m_pNavCar->m_fDistance);
			}
			else
			    sprintf(lpszState,"里程:%sKm","****");

    GisGetTextExtentPoint(lpszState,&cx,&cy);
	GUI_SetColor(TextColor);
	GUI_DispStringAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);
			rect.x0 = puserlayer->m_mapCoordMap->wScreen/2;
			rect.y0 = puserlayer->m_mapCoordMap->hScreen-18; 	// 当前视图的高度和宽度;
			rect.y1 = 	rect.y0+17;
			rect.x1 = puserlayer->m_mapCoordMap->wScreen-1;
		GUI_SetColor(FillColor); // 浅色
			GUI_FillRectEx(&rect);
		GUI_SetColor(RecColor); // 浅色
			GUI_DrawRectEx(&rect);

			if(puserlayer->m_pNavCar)
			{

			   sprintf(lpszState,"均速:%0.1fKm/h",puserlayer->m_pNavCar->m_averSpeed);
			}
			else
			   sprintf(lpszState,"均速:%sKm/h","****");
				

    GisGetTextExtentPoint(lpszState,&cx,&cy);
	GUI_SetColor(TextColor);
	GUI_DispStringHCenterAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);


		 }
		 //
		 // 测量面积
		 // 
		 if(puserlayer->m_bShowMeasureArea)
		 {
			rect.x0 = puserlayer->m_mapCoordMap->wScreen/2;
			rect.y0 = puserlayer->m_mapCoordMap->hScreen-18; 	// 当前视图的高度和宽度;
			rect.y1 = 	rect.y0+17;
			rect.x1 = puserlayer->m_mapCoordMap->wScreen-1;
		GUI_SetColor(FillColor); // 浅色
			GUI_FillRectEx(&rect);
		GUI_SetColor(RecColor); // 浅色
			GUI_DrawRectEx(&rect);

			if(puserlayer->m_pNavCar)
			{
				// 如果测量面积大于1平方公里，面积则改为平方公里作为单位
				if(fabs(puserlayer->m_pNavCar->m_fArea )>1000*1000)
			       sprintf(lpszState,"面积:%fkm2",fabs(puserlayer->m_pNavCar->m_fArea )/(1000*1000));
				else
			       sprintf(lpszState,"面积:%0.0fm2",fabs(puserlayer->m_pNavCar->m_fArea));
			}
			else
			   sprintf(lpszState,"面积:%sm2","****");

    GisGetTextExtentPoint(lpszState,&cx,&cy);
	GUI_SetColor(TextColor);
	GUI_DispStringAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);
    GUI_SetAlpha(0);
		 }

}
lpCar UserFindCar(GUI_ARRAY *pThis,int ncarid)
{
	int i,nCount=0;
	lpCar pMyCar = NULL;
	if(!pThis->haHandle)
		return NULL;
	nCount=GUI_ARRAY_GetNumItems(pThis);
    for(i=0;i<nCount;i++)
	{
        pMyCar =GUI_ARRAY_GetpItem(pThis,i);
		if(pMyCar)
		{
			if(pMyCar->CarID==ncarid)
				return pMyCar;
		}
	}
	return NULL;
}
BOOLEAN DeleteCar(lpUserLayer puserlayer,INT16U ncarid)
{
	int i,nCount=0;
	lpCar pMyCar = NULL;
    if(ncarid==0)
		return FALSE;
	if(!puserlayer->carArray.haHandle)
		return FALSE;
	nCount=GUI_ARRAY_GetNumItems(&puserlayer->carArray);
    for(i=0;i<nCount;i++)
	{
        pMyCar =GUI_ARRAY_GetpItem(&puserlayer->carArray,i);
		if(pMyCar)
		{
			if(pMyCar->CarID==ncarid)
			{
				GUI_ARRAY_DeleteItem(&puserlayer->carArray,i);
				return TRUE;
			}
		}
	}
    return FALSE;
}
void AddGPSPoint(lpUserLayer puserlayer,GpsDataInfo * pPosition)
{

	int nCount=0;
	lpCar pMyCar = NULL;
	Car  car;

	if(puserlayer->carArray.InitState== GUI_ARRAY_STATE_NOT_CREATED)
		GUI_ARRAY_Create(&puserlayer->carArray);

	memset(&car,0,sizeof(Car));
	nCount=GUI_ARRAY_GetNumItems(&puserlayer->carArray);
	pMyCar=UserFindCar(&puserlayer->carArray,pPosition->m_nCarID);
	if (pMyCar==NULL)
	{

		car.m_mapCoordMap = puserlayer->m_mapCoordMap;
		car.pSoundFunc = puserlayer->m_pSoundFunc;
		strcpy(car.m_szTraceFilePath , puserlayer->m_szTraceFilePath);
		GUI_ARRAY_AddItem(&puserlayer->carArray,&car,sizeof(Car));
		pMyCar=GUI_ARRAY_GetpItem(&puserlayer->carArray,nCount);
		if(pPosition->m_nCarID==0)
		{
			pMyCar->m_carType = carMySelf;  //如果车辆ID为0，则为自己导航车辆
			strcpy(pMyCar->m_szNiceName,"我");
		    puserlayer->m_pNavCar = pMyCar;
		}
		else
		{
			sprintf(pMyCar->m_szNiceName,"%d",pPosition->m_nCarID);
		}

	}
    Car_AddPoint(pPosition,pMyCar);
}

void SetNetState(lpUserLayer puserlayer,int nState)
{
	puserlayer->m_nNetState = nState;
	if(puserlayer->m_bShowState)
		GisDrawState(puserlayer);
}

extern GUI_BITMAP bmcenter[];
extern GUI_BITMAP bmcentertrans[];

void DrawXX(lpUserLayer puserlayer)
{
	if(puserlayer->m_pBmpCenter==NULL)
	{
		puserlayer->m_pBmpCenter = bmcentertrans;//bmcenter;
	}
	if(puserlayer->m_pBmpCenter)
	{
		// 如果车没有定位，或者没有在屏幕上，择显示地图漫游标志
		GUI_SetAlpha(0x80);
		GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
		if(!puserlayer->m_pNavCar)
		    GUI_DrawBitmap(puserlayer->m_pBmpCenter,puserlayer->m_mapCoordMap->wScreen/2-15, puserlayer->m_mapCoordMap->hScreen/2-15);
		else if(!IsInScreen(puserlayer->m_mapCoordMap,puserlayer->m_pNavCar->m_gpsDataInfo.m_xx,puserlayer->m_pNavCar->m_gpsDataInfo.m_yy))
		    GUI_DrawBitmap(puserlayer->m_pBmpCenter,puserlayer->m_mapCoordMap->wScreen/2-15, puserlayer->m_mapCoordMap->hScreen/2-15);
        GUI_SetAlpha(0);
	}
}

void DrawUserLayerPoint(GpsDataInfo* pPosition,lpUserLayer puserlayer)
{
	GPSMESSAGE msg;
	int x,y;

	//if((pPosition->m_nState>2)&&(pPosition->nDataType == 1))
	if((pPosition->nDataType == 1)&&(!pPosition->bNetData))
	{
		msg.gps.xx = pPosition->m_xx;
		msg.gps.yy = pPosition->m_yy;
		msg.gps.angle = pPosition->m_fAngle;
		msg.gps.speed = pPosition->m_nSpeed;
		msg.gps.ststate = pPosition->m_nState;
		msg.gps.gpstime = pPosition->m_stTime;
		msg.gps.heigth = pPosition->m_dbHeigth;
		msg.gps.stcount = pPosition->m_nStCount;
		msg.gps.carID = 0;//atoi(g_set.m_szUser);
		//msg.gps.state = 0;

		//g_socket.Send((char*)&msg,sizeof(GPSMESSAGE));
	}
	AddGPSPoint(puserlayer,pPosition);
	if((pPosition->m_nState>1))
	{
		DPtoVP(puserlayer->m_mapCoordMap,pPosition->m_xx,pPosition->m_yy,&x,&y);

		if((x<=0)||
			(x>=puserlayer->m_mapCoordMap->wScreen)||
			(y<=0)||
			(y>=puserlayer->m_mapCoordMap->hScreen))
		{
			SetCenterINT(puserlayer->m_mapCoordMap,x,y);
		}
	}

	// 画用户层
	//g_userLayer.Draw(hdc);

}

void DrawUserLayerNetState(lpUserLayer puserlayer,int nState)
{
	SetNetState(puserlayer,nState);
	//	  EndPaint (hwnd, &ps);
}

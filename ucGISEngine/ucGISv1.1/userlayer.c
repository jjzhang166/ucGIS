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
	DrawXX(puserlayer);  // ������ͼ��
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
	// ���Ʊ�����
	
	GUI_SetColor(RecColor);
	GUI_FillRectEx(&rect);

	GUI_SetColor(LineColor);
	GUI_DrawLine(2,14,34,14);
	GUI_DrawLine(2,9,2,14);
	GUI_DrawLine(35,9,35,14);

	now = GetTickCount();
	if(puserlayer->m_pNavCar!=NULL)
	{
		// ���û���յ���Ч��GPS���ݣ���ʾ��ɫ�߿�
		// �������û���յ��������ݣ���ʾ��ɫɫ�߿�
		 if((now-puserlayer->m_pNavCar->m_dwLastComDataTime)>3*1000)
			RecColor=GIS_RGB(255,255,232 ); // ǳɫ
		else if((now-puserlayer->m_pNavCar->m_dwLastValueGpsDataTime)<5*1000)
			RecColor=GIS_RGB(255,0,0 ) ; // ��ɫ�߿�
		else
			RecColor=GIS_RGB(255,255,0 ); // ��ɫ
	}else
	{
		RecColor=GIS_RGB(255,0,0 ) ; // ��ɫ�߿�//
	}
	GUI_DrawRectEx(&rect);
		// ���������
	nDistance = puserlayer->m_mapCoordMap->blc*32*100000;

	if(nDistance>999)
		sprintf(lpszState,"%dKm",nDistance/1000);
	else
		sprintf(lpszState,"%dM",nDistance);
    GisGetTextExtentPoint(lpszState,&cx,&cy);
	GUI_SetColor(TextColor);
	GUI_DispStringAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);
	// �ٶ�
	rect.x0 = rect.x1;
	rect.x1 += 46;
    FillColor=RecColor=GIS_RGB(255,230,255);
	if(puserlayer->m_pNavCar!=NULL)
	{

		GUI_SetColor(FillColor); // ǳɫ
			GUI_FillRectEx(&rect);
		GUI_SetColor(RecColor); // ǳɫ
			GUI_DrawRectEx(&rect);

		if(puserlayer->m_pNavCar->m_gpsDataInfo.m_nSpeed==-1)
		sprintf(lpszState,"��%s","**");
		else
		sprintf(lpszState,"��%3.0f",puserlayer->m_pNavCar->m_gpsDataInfo.m_nSpeed);

		GisGetTextExtentPoint(lpszState,&cx,&cy);
		GUI_SetColor(TextColor);
		GUI_DispStringAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);
	}

		// ���θ߶�
	   rect.x0 = rect.x1;
	   rect.x1 += 42;

		GUI_SetColor(FillColor); // ǳɫ
			GUI_FillRectEx(&rect);
		GUI_SetColor(RecColor); // ǳɫ
			GUI_DrawRectEx(&rect);
		
		if(puserlayer->m_pNavCar!=NULL)
		{

		if(puserlayer->m_pNavCar->m_gpsDataInfo.m_dbHeigth==-1)
		sprintf(lpszState,"��%s","**");
		else
		sprintf(lpszState,"��%4.0f",puserlayer->m_pNavCar->m_gpsDataInfo.m_dbHeigth);

		GisGetTextExtentPoint(lpszState,&cx,&cy);
		GUI_SetColor(TextColor);
		GUI_DispStringAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);
		}

		// ��������
		rect.x0 = rect.x1;
		rect.x1 += 26;

		GUI_SetColor(FillColor); // ǳɫ
			GUI_FillRectEx(&rect);
		GUI_SetColor(RecColor); // ǳɫ
			GUI_DrawRectEx(&rect);
		if(puserlayer->m_pNavCar!=NULL)
		{
			if(puserlayer->m_pNavCar->m_gpsDataInfo.m_nStCount==-1)
			sprintf(lpszState,"��%s","**");
			else
			sprintf(lpszState,"��%d",puserlayer->m_pNavCar->m_gpsDataInfo.m_nStCount);

		GisGetTextExtentPoint(lpszState,&cx,&cy);
		GUI_SetColor(TextColor);
		GUI_DispStringAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);
		}
	// ����״̬
		rect.x0 = rect.x1;
		rect.x1 += 29;
		GUI_SetColor(FillColor); // ǳɫ
			GUI_FillRectEx(&rect);
		GUI_SetColor(RecColor); // ǳɫ
			GUI_DrawRectEx(&rect);

		switch(puserlayer->m_nNetState)
		{
		case NET_BREAK:
			sprintf(lpszState,"%s","����");
			break;
		case NET_CONNECTING:
			sprintf(lpszState,"%s","��...");
			break;
		case NET_CONNECTED:
			sprintf(lpszState,"%s","��...");
			break;
		case NET_LOGINED:
			sprintf(lpszState,"%s","����");
			break;
		case NET_LOGINFEILD:
			sprintf(lpszState,"%s","�ܴ�");
			break;
		}

    GisGetTextExtentPoint(lpszState,&cx,&cy);
	GUI_SetColor(TextColor);
	GUI_DispStringAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);

		 //
		 // ��ʾ���Ȳ������
 		 //
		 if(puserlayer->m_bShowMeasureLen)
		 {
			 
			rect.x0 = 0;
			rect.y0 = puserlayer->m_mapCoordMap->hScreen-18; 	// ��ǰ��ͼ�ĸ߶ȺͿ��;
			rect.y1 = 	rect.y0+17;
			rect.x1 = puserlayer->m_mapCoordMap->wScreen/2;
		GUI_SetColor(FillColor); // ǳɫ
			GUI_FillRectEx(&rect);
		GUI_SetColor(RecColor); // ǳɫ
			GUI_DrawRectEx(&rect);

			if(puserlayer->m_pNavCar)
			{
			    sprintf(lpszState,"���:%0.3fKm",puserlayer->m_pNavCar->m_fDistance);
			}
			else
			    sprintf(lpszState,"���:%sKm","****");

    GisGetTextExtentPoint(lpszState,&cx,&cy);
	GUI_SetColor(TextColor);
	GUI_DispStringAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);
			rect.x0 = puserlayer->m_mapCoordMap->wScreen/2;
			rect.y0 = puserlayer->m_mapCoordMap->hScreen-18; 	// ��ǰ��ͼ�ĸ߶ȺͿ��;
			rect.y1 = 	rect.y0+17;
			rect.x1 = puserlayer->m_mapCoordMap->wScreen-1;
		GUI_SetColor(FillColor); // ǳɫ
			GUI_FillRectEx(&rect);
		GUI_SetColor(RecColor); // ǳɫ
			GUI_DrawRectEx(&rect);

			if(puserlayer->m_pNavCar)
			{

			   sprintf(lpszState,"����:%0.1fKm/h",puserlayer->m_pNavCar->m_averSpeed);
			}
			else
			   sprintf(lpszState,"����:%sKm/h","****");
				

    GisGetTextExtentPoint(lpszState,&cx,&cy);
	GUI_SetColor(TextColor);
	GUI_DispStringHCenterAt(lpszState,rect.x0+((rect.x1-rect.x0)- cx)/2, rect.y0+cy/2);


		 }
		 //
		 // �������
		 // 
		 if(puserlayer->m_bShowMeasureArea)
		 {
			rect.x0 = puserlayer->m_mapCoordMap->wScreen/2;
			rect.y0 = puserlayer->m_mapCoordMap->hScreen-18; 	// ��ǰ��ͼ�ĸ߶ȺͿ��;
			rect.y1 = 	rect.y0+17;
			rect.x1 = puserlayer->m_mapCoordMap->wScreen-1;
		GUI_SetColor(FillColor); // ǳɫ
			GUI_FillRectEx(&rect);
		GUI_SetColor(RecColor); // ǳɫ
			GUI_DrawRectEx(&rect);

			if(puserlayer->m_pNavCar)
			{
				// ��������������1ƽ�����������Ϊƽ��������Ϊ��λ
				if(fabs(puserlayer->m_pNavCar->m_fArea )>1000*1000)
			       sprintf(lpszState,"���:%fkm2",fabs(puserlayer->m_pNavCar->m_fArea )/(1000*1000));
				else
			       sprintf(lpszState,"���:%0.0fm2",fabs(puserlayer->m_pNavCar->m_fArea));
			}
			else
			   sprintf(lpszState,"���:%sm2","****");

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
			pMyCar->m_carType = carMySelf;  //�������IDΪ0����Ϊ�Լ���������
			strcpy(pMyCar->m_szNiceName,"��");
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
		// �����û�ж�λ������û������Ļ�ϣ�����ʾ��ͼ���α�־
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

	// ���û���
	//g_userLayer.Draw(hdc);

}

void DrawUserLayerNetState(lpUserLayer puserlayer,int nState)
{
	SetNetState(puserlayer,nState);
	//	  EndPaint (hwnd, &ps);
}

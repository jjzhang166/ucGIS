/*********************************************************************
*													                 *
*					uc/GUI GIS App main  file  						 *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : GisApp.c
Purpose     : 
----------------------------------------------------------------------
*/
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <windows.h>
#include "GUI.h"
#include "GUI_X.h"
#include "BUTTON.h"
#include "FRAMEWIN.h"
#include "DIALOG.h"
#include "MENU.h"
#include "LISTVIEW.h"
#include "DROPDOWN.h"
#include "GisApi.h"

UcMap myUcMap;
UserLayer g_userLayer;
MapCoorTrans m_mapCoordMap;
MapCoorTrans m_EagleCoordMap;
#include <stddef.h>
#include <string.h>

//#include "GPS_TypeDefine.h"

extern GUI_CONST_STORAGE GUI_BITMAP bmExit;
extern GUI_CONST_STORAGE GUI_BITMAP bmconfirm;
CHECKBOX_Handle _hChkBox_ShowFigure;
CHECKBOX_Handle _hChkBox_ShowName;
LISTVIEW_Handle _hListView;
char _isNavi;

WM_HWIN _hFrameMapWin;
WM_HWIN _hFrameEagleMapWin;

//#include "GPS_Spec.h"

#if GUI_OS == 0
  #error Multitasking sample requires task awareness (#define GUI_OS 1)
#endif

#define ID_BUTTON_CONFIGURE		(GUI_ID_USER +  0)
#define ID_BUTTON_SEARCH		(GUI_ID_USER +  1)
#define ID_BUTTON_NAVIGATION	(GUI_ID_USER +  2)
#define ID_BUTTON_ABOUT			(GUI_ID_USER +  3)

#define ID_MENU_CONFIGURE		(GUI_ID_USER +  4)
#define ID_MENU_SEARCH			(GUI_ID_USER +  5)
#define ID_MENU_NAVIGATION		(GUI_ID_USER +  6)
#define ID_MENU_ABOUT			(GUI_ID_USER +  7)

#define ID_CONF					GUI_ID_USER + 200
#define ID_CONF_OK				ID_CONF + 0
#define ID_CONF_LISTVIEW		ID_CONF + 1
#define ID_CONF_CHK_SHOW_FIGURE	ID_CONF + 2
#define ID_CONF_CHK_SHOW_NAME	ID_CONF + 3

#define ID_NAVI					GUI_ID_USER + 220
#define ID_NAVI_DROP_SOUR		ID_NAVI + 0
#define ID_NAVI_DROP_TARG		ID_NAVI + 1

#define ID_SEARCH				GUI_ID_USER + 230
#define ID_SEARCH_DROP_SOUR		ID_SEARCH + 0
#define	ID_SEARCH_BUTTON_OK		ID_SEARCH + 1

/*******************************************************************
*
*       Define how to create a task and start multitasking
*
********************************************************************

  If not using embOS you have to change the hardware
  dependent macros to work with your OS

*/


#ifndef WIN32 
  #include "RTOS.h"    /* Definitions for embOS */
  #define CREATE_TASK(pTCB, pName, pFunc, Priority, pStack)  OS_CREATETASK(pTCB, pName, pFunc, Priority, pStack)
  #define START_MT()  OS_Terminate(0)
  #define Delay(t)    OS_Delay(t)
#else
  #include "SIM.h"     /* Definitions for the Win32 simulation */
  #define CREATE_TASK(pTCB, pName, pFunc, Priority, pStack)   SIM_CreateTask(pName, pFunc)
  #define START_MT()  SIM_Start()
  #define Delay(t)    SIM_Delay(t)
#endif

    int _Height = 720;
	int _Width =  960;



	
  #define CURRENTDIR  "\\gpsdata\\"
FILE *gpsfp;
GpsDataInfo gpsinfo;
BOOLEAN bGpsDataOK=FALSE;
void SendGpsData(char * filename)
{
	char line[256]; 
	if((gpsfp=fopen(filename,"rt"))==NULL)
	{
		return;
	}
    { 
retry:
	  if(feof(gpsfp)) return;
	  memset(line,0,256);
      
	  fgets(line,256,gpsfp);

	  if(strstr(line,"$GPRMC")!=NULL)
	  {
	    sprintf(line,"%s\r\n",line);
        ParseGPS(&gpsinfo,line);
		bGpsDataOK=TRUE;
	  }
	  else
	  if(strstr(line,"$GPGGA")!=NULL)
	  {
	  sprintf(line,"%s\r\n",line);
	  ParseGGA(&gpsinfo,line);
	  }
	  else
	  if(strstr(line,"$GPGSA")!=NULL)
	  {
	  sprintf(line,"%s\r\n",line);
	  ParseGSA(&gpsinfo,line);
	  }
	  if(bGpsDataOK)
	  {
	  DrawUserLayerPoint(&gpsinfo,&g_userLayer);
	  //AddGPSPoint(&g_userLayer,&gpsinfo);
	  memset(&gpsinfo,0,sizeof(GpsDataInfo));
	  bGpsDataOK=FALSE;
	  }
	  else goto retry;
	  //DrawCurrentPos(line); 
	  //mycom.SendData(line,strlen(line));
	  //printf(line);
	  //Sleep(1000);
	}

}
int ReadGpsData()
{
	  char line[256];
retry:
	  if(gpsfp==NULL) return 0;
	  if(feof(gpsfp)) return 0;
	 // memset(&gpsinfo,0,sizeof(GpsDataInfo));
	  memset(line,0,256);
	  fgets(line,256,gpsfp);

	  if(strstr(line,"$GPRMC")!=NULL)
	  {
	    sprintf(line,"%s\r\n",line);
        ParseGPS(&gpsinfo,line);
		bGpsDataOK=TRUE;
	  }
	  else
	  if(strstr(line,"$GPGGA")!=NULL)
	  {
	  sprintf(line,"%s\r\n",line);
	  ParseGGA(&gpsinfo,line);
	  }
	  else
	  if(strstr(line,"$GPGSA")!=NULL)
	  {
	  sprintf(line,"%s\r\n",line);
	  ParseGSA(&gpsinfo,line);
	  }
	  if(bGpsDataOK)
	  {
      DrawUserLayerPoint(&gpsinfo,&g_userLayer);
	  //AddGPSPoint(&g_userLayer,&gpsinfo);
	  memset(&gpsinfo,0,sizeof(GpsDataInfo));
	  bGpsDataOK=FALSE;
	  }
	  else goto retry;
      return 1; 
}

void closegpsfile()
{
	if(gpsfp!=NULL)
	fclose(gpsfp);

}
struct   _finddata_t   filestruct;           
 		  char currpath[256];
		  char filepath[100];
		  long   handle;

void GetGpsFileList()   
  {   
          int   n=0;   

		  //开始查找工作，找到当前目录下的第一个实体（文件或目录）   
          //“*”表示查找任何的文件或子目录，filestruct为查找结果 
		  GetCurrentDirectoryA(256,currpath);
		  strcpy(filepath,currpath);
		  strcat(currpath,CURRENTDIR);
          strcat(currpath,"*.tra");  
          handle=_findfirst(currpath,&filestruct);   
          if(handle==-1)   return;         //如果handle为-1，表示当前目录为空，则结束查找而返回   
		  printf("Read %s ...\r\n",filestruct.name);
		  sprintf(currpath,"%s%s%s",filepath,CURRENTDIR,filestruct.name);
	      memset(&gpsinfo,0,sizeof(GpsDataInfo));

		  SendGpsData(currpath);
		  printf("Read Map data OK!\r\n");
          n=0;   
  }   

int GetNextFile()
{
	     if(handle==NULL) return 0;
          //继续对当前目录中的下一个子目录或文件进行与上面同样的查找   
          if(!(_findnext(handle,&filestruct)))   
          {   
			  //printf("Read %s ...",filestruct.name);
			  sprintf(currpath,"%s%s%s",filepath,CURRENTDIR,filestruct.name);
			  SendGpsData(currpath);
			  return 1;
			  //printf("Read Map data OK!\r\n");
		  }   
          return 0;
}

void EndGpsFile()
{
	    if(handle==NULL) return;
        _findclose(handle);                   

}
	
static void _cbButton(WM_MESSAGE* pMsg) {
	WM_DefaultProc(pMsg);
}

static int _Target_X;
static int _Target_Y;
static void SetTargetPosition(int x, int y)
{
	_Target_X = x;
	_Target_Y = y;
}

/*******************************************************************
*
*       _cbAboutBox
*/
static void _cbAboutBox(WM_MESSAGE* pMsg) {
	WM_HWIN hWin = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
	case WM_PAINT:

		GUI_SetColor(GUI_BLACK);
		GUI_SetFont(&GUI_Font8x8);
		GUI_SetTextMode(GUI_TM_TRANS);
		//GUI_DrawBitmap(&_Logo, 4, 4);
		//GUI_TA_LEFT
		GUI_SetTextAlign(GUI_TA_LEFT);
		GUI_DispStringAt("uc/GUI App Demo V1.0", 1, 48);
		GUI_DispStringAt("Compiled " __DATE__ " "__TIME__, 1, 68);
		GUI_DispStringAt("xiang_xudong@163.com (c) 2009", 1, 87);

		break;
	case WM_NOTIFY_PARENT:
		if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) 
		{
			GUI_EndDialog(hWin, 1);
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

/*********************************************************************
*
*       _ShowAboutBox
*/
static void _ShowAboutBox(void) {
  WM_HWIN hFrame, hItem;
  /* Create framewin */
  hFrame = FRAMEWIN_CreateEx(70, 70, 220, 120, WM_HBKWIN, WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, "About uc/GUI GIS App", &_cbAboutBox);
  FRAMEWIN_SetClientColor   (hFrame, GUI_WHITE);
  FRAMEWIN_SetFont          (hFrame, &GUI_Font8x8);
  FRAMEWIN_SetTextAlign     (hFrame, GUI_TA_LEFT);
  /* Create dialog items */
  hItem = BUTTON_CreateEx(111, 7, 50, 32, WM_GetClientWindow(hFrame), WM_CF_SHOW, 0, GUI_ID_OK);
  BUTTON_SetBitmap(hItem, 0, &bmExit);
  /* Exec modal dialog */
  WM_SetFocus(hFrame);
  WM_MakeModal(hFrame);
  GUI_ExecCreatedDialog(hFrame);
  //WM_SetFocus(hFrame);
}


/*********************************************************************
*
*       _cbMapConfBox
*/
static void _cbMapConfBox(WM_MESSAGE* pMsg) {
	int row;
	int chk_status;
	switch (pMsg->MsgId) 
	{
	case WM_NOTIFY_PARENT:
		if (pMsg->Data.v == WM_NOTIFICATION_RELEASED || 
			pMsg->Data.v == WM_NOTIFICATION_SEL_CHANGED) 
		{
			WM_HWIN hWin = pMsg->hWin;
			switch(WM_GetId(pMsg->hWinSrc))
			{
			case GUI_ID_OK:
				GUI_EndDialog(hWin, 1);
				break;
			case ID_CONF_LISTVIEW:
				row = LISTVIEW_GetSel(_hListView);
				//CHECKBOX_SetState(_hChkBox_ShowName, 
				//				GPS_MapShowCfgTbl[row].show_name[0]=='y' ? 1 : 0);
				//CHECKBOX_SetState(_hChkBox_ShowFigure, 
				//				GPS_MapShowCfgTbl[row].show_figure[0]=='y' ? 1 : 0 );
				break;
			case ID_CONF_CHK_SHOW_FIGURE:
				row = LISTVIEW_GetSel(_hListView);
				chk_status = CHECKBOX_GetState(_hChkBox_ShowFigure);
				//GPS_MapShowCfgTbl[row].show_figure = chk_status ? "y" : "n";
				LISTVIEW_SetItemText(_hListView, 1, row, chk_status ? "y" : "n");
				break;
			case ID_CONF_CHK_SHOW_NAME:
				row = LISTVIEW_GetSel(_hListView);
				chk_status = CHECKBOX_GetState(_hChkBox_ShowName);
				//GPS_MapShowCfgTbl[row].show_name = chk_status ? "y" : "n";
				LISTVIEW_SetItemText(_hListView, 2, row, chk_status ? "y" : "n");
				break;
			}
		}
		
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

/*********************************************************************
*
*       _ShowMapConfBox
*/
static void _ShowMapConfBox(void) {
	WM_HWIN hFrame, hClient, hItem;
	
	HEADER_Handle hHeader;
	char* value[3]={"Item1","Item2","Item3"};
	int i;
	/* Create framewin */
	hFrame = FRAMEWIN_CreateEx(70, 10, 245, 220, WM_HBKWIN, WM_CF_SHOW, 
								FRAMEWIN_CF_MOVEABLE, 0, "Map settings", &_cbMapConfBox);
	FRAMEWIN_SetClientColor   (hFrame, GUI_WHITE);
	FRAMEWIN_SetFont          (hFrame, &GUI_Font8x8);
	FRAMEWIN_SetTextAlign     (hFrame, GUI_TA_HCENTER);

	hClient = WM_GetClientWindow(hFrame);
	hItem = BUTTON_CreateEx(185, 167, 50, 32, hClient, WM_CF_SHOW, 0, GUI_ID_OK);
	BUTTON_SetBitmap(hItem, 0, &bmExit);

	/* Create checkbox */
	_hChkBox_ShowFigure = CHECKBOX_CreateEx(10, 154, 140, 0, hClient, 
											WM_CF_SHOW, 0, ID_CONF_CHK_SHOW_FIGURE);
	CHECKBOX_SetText(_hChkBox_ShowFigure, "Show Figure");
	CHECKBOX_SetState(_hChkBox_ShowFigure, 1 );
	_hChkBox_ShowName = CHECKBOX_CreateEx(10, 174, 140, 0, hClient, 
											WM_CF_SHOW, 0, ID_CONF_CHK_SHOW_NAME);
	CHECKBOX_SetText(_hChkBox_ShowName, "Show Name");
	CHECKBOX_SetState(_hChkBox_ShowName, 0);

	_hListView = LISTVIEW_CreateEx(2, 2 , 236, 150, hClient, WM_CF_SHOW, 0, ID_CONF_LISTVIEW);
	hHeader = LISTVIEW_GetHeader(_hListView);
	HEADER_SetHeight(hHeader,20);
	LISTVIEW_AddColumn(_hListView, 90, "   Layer",         GUI_TA_LEFT);
	LISTVIEW_AddColumn(_hListView,  70, "Show Figure",	GUI_TA_CENTER);
	LISTVIEW_AddColumn(_hListView, 65, "Show Name",		GUI_TA_CENTER);
	SCROLLBAR_CreateAttached(_hListView, SCROLLBAR_CF_VERTICAL);

	for(i=0; i<20; i++)
	{
		//value[0]//GPS_MapShowCfgTbl[i].type_name;
		//value[1] = //GPS_MapShowCfgTbl[i].show_figure;
		//value[2] = //GPS_MapShowCfgTbl[i].show_name;
		if( !strcmp(value[0],"")) break;
		LISTVIEW_AddRow(_hListView, value);
	}
	LISTVIEW_SetSel(_hListView,0);
	LISTVIEW_SetGridVis(_hListView,1);
	/* Exec modal dialog */
	WM_SetFocus(hFrame);
	WM_MakeModal(hFrame);
	GUI_ExecCreatedDialog(hFrame);
}


DROPDOWN_Handle _hDropSource;
DROPDOWN_Handle _hDropTarget;
static void _cbShowNaviSetBox(WM_MESSAGE* pMsg)
{
	switch(pMsg->MsgId)
	{
	case WM_PAINT:
		GUI_SetColor(GUI_BLACK);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt ("起始:", 5, 103);
		GUI_DispStringAt ("目标:", 5, 128);
		break;
	case WM_NOTIFY_PARENT:
		if (pMsg->Data.v == WM_NOTIFICATION_RELEASED)
		{
			WM_HWIN hWin = pMsg->hWin;
			switch( WM_GetId(pMsg->hWinSrc) )
			{
			case GUI_ID_OK:
				GUI_EndDialog(hWin, 1);
				break;
			}
		}
		break;
	default:
		WM_DefaultProc(pMsg);

	}
}

/*********************************************************************
*
*       _ShowNaviSetBox
*/
static void _ShowNaviSetBox()
{
	WM_HWIN hFrame, hItem, hWin;
	/* Create framewin */
	hFrame = FRAMEWIN_CreateEx(65, 0, 254, 240, WM_HBKWIN, WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, "Navigation Setting", &_cbShowNaviSetBox);
	FRAMEWIN_SetClientColor   (hFrame, GUI_WHITE);
	FRAMEWIN_SetFont          (hFrame, &GUI_Font8x8);
	FRAMEWIN_SetTextAlign     (hFrame, GUI_TA_HCENTER);
	hWin = WM_GetClientWindow(hFrame);
	/* Create dialog items */
	hItem = BUTTON_CreateEx(200, 18, 50, 32, hWin, WM_CF_SHOW, 0, GUI_ID_OK);
	BUTTON_SetBitmap         (hItem, 0, &bmExit);
	_hDropSource = DROPDOWN_CreateEx(45, 100, 150, 100, hWin, WM_CF_SHOW, 0, ID_NAVI_DROP_SOUR);
	_hDropTarget = DROPDOWN_CreateEx(45, 125, 150, 100, hWin, WM_CF_SHOW, 0, ID_NAVI_DROP_TARG);
	DROPDOWN_AddString(_hDropSource, "test");

	/* Exec modal dialog */
	WM_SetFocus(hFrame);
	WM_MakeModal(hFrame);
	GUI_ExecCreatedDialog(hFrame);
}

static void GoToTarget(int x,int y)
{
	int cur_x, cur_y;
	int goto_x, goto_y;
	goto_x = x;//unit_set.pPts[pos].x;
	goto_y = y;//unit_set.pPts[pos].y;
	cur_x = WM_GetWindowOrgX(_hFrameMapWin);
	cur_y = WM_GetWindowOrgY(_hFrameMapWin);
	SetTargetPosition(goto_x, goto_y);
	WM_MoveWindow(_hFrameMapWin, -(goto_x - 160 + cur_x), -(goto_y - 120 + cur_y));
}

static void _cbShowSearchBox(WM_MESSAGE* pMsg)
{
	switch(pMsg->MsgId)
	{
	case WM_PAINT:
		GUI_SetColor(GUI_BLACK);
		//GUI_SetFont(&GUI_Font8x8);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt ("目标:", 5, 23);
		break;
	case WM_NOTIFY_PARENT:
		if (pMsg->Data.v == WM_NOTIFICATION_RELEASED)
		{
			WM_HWIN hWin = pMsg->hWin;
			switch( WM_GetId(pMsg->hWinSrc) )
			{
			case GUI_ID_OK:
				GUI_EndDialog(hWin, 1);
				break;
			case ID_SEARCH_BUTTON_OK:
				//GoToTarget(DROPDOWN_GetSel(_hDropTarget));
				GUI_EndDialog(hWin, 1);
				break;
			}
		}
		break;
	default:
		WM_DefaultProc(pMsg);

	}
}


extern const GUI_FONT GUI_FontHZ_SimSun_12;
static void _ShowSearchBox()
{
	WM_HWIN hFrame, hItem, hWin;
//	int i;

	/* Create framewin */
	hFrame = FRAMEWIN_CreateEx(65, 0, 254, 240, WM_HBKWIN, WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, "Search", &_cbShowSearchBox);
	FRAMEWIN_SetClientColor   (hFrame, GUI_WHITE);
	FRAMEWIN_SetFont          (hFrame, &GUI_Font8x8);
	FRAMEWIN_SetTextAlign     (hFrame, GUI_TA_HCENTER);
	hWin = WM_GetClientWindow(hFrame);
	/* Create dialog items */
	hItem = BUTTON_CreateEx(200, 180, 50, 32, hWin, WM_CF_SHOW, 0, GUI_ID_OK);
	BUTTON_SetBitmap         (hItem, 0, &bmExit);
	hItem = BUTTON_CreateEx(100, 180, 50, 32, hWin, WM_CF_SHOW, 0, ID_SEARCH_BUTTON_OK);
	BUTTON_SetBitmap         (hItem, 0, &bmconfirm);

	_hDropTarget = DROPDOWN_CreateEx(35, 20, 170, 150, hWin, WM_CF_SHOW, 0, ID_SEARCH_DROP_SOUR);

	DROPDOWN_SetDefaultFont(&GUI_FontHZ_SimSun_12);
	DROPDOWN_SetFont(_hDropTarget, &GUI_FontHZ_SimSun_12);
	DROPDOWN_SetAutoScroll(_hDropTarget, 1);
/*
	for(i=0; i<unit_set.iPtsCount; i++)
	{
		DROPDOWN_AddString(_hDropTarget, unit_set.pPts[i].name);
	}
*/
	/* Exec modal dialog */
	WM_SetFocus(hFrame);
	WM_MakeModal(hFrame);
	GUI_ExecCreatedDialog(hFrame);
}

/*******************************************************************
*
*       _cbCallbackT0
*/
static void _cbCallbackMenuWindow(WM_MESSAGE * pMsg) {
	int key;
	int x,y;
  switch (pMsg->MsgId) {
	case WM_NOTIFY_PARENT:
		if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
			WM_HWIN hItem = pMsg->hWinSrc;
			switch (WM_GetId(hItem)) {
				case ID_BUTTON_ABOUT:
					_ShowAboutBox();
					break;
				case ID_BUTTON_CONFIGURE:
					_ShowMapConfBox();
					break;
				case ID_BUTTON_NAVIGATION:
					_ShowNaviSetBox();
					break;
				case ID_BUTTON_SEARCH:
					_ShowSearchBox();
					break;
				case 1001:
					//enlarge();
					ZoomIn(&m_mapCoordMap);
					WM_Invalidate(_hFrameMapWin);
					break;
				case 1002:
					//shrink();
					ZoomOut(&m_mapCoordMap);
					WM_Invalidate(_hFrameMapWin);
					break;
			}
		}//if
	case WM_KEY:
		x = WM_GetWindowOrgX(_hFrameMapWin);
		y = WM_GetWindowOrgY(_hFrameMapWin);
		key = GUI_GetKey();
		if( key == 17)		//up
		{
			if (y > 120)
			{
				break;
			}
			Up(&m_mapCoordMap);
			WM_Invalidate(_hFrameMapWin);

			//WM_MoveWindow(_hFrameMapWin, 0, 16);
		}
		else if(key == 19)	//down
		{
			if (y < -(_Height-120))
			{
				break;
			}
			Down(&m_mapCoordMap);
			WM_Invalidate(_hFrameMapWin);

			//WM_MoveWindow(_hFrameMapWin, 0, -16);
		}
		else if (key == 16)	//left
		{
			if (x > 160)
			{
				break;
			}
			Left(&m_mapCoordMap);
			WM_Invalidate(_hFrameMapWin);

			//WM_MoveWindow(_hFrameMapWin, 16, 0);
		}
		else if (key == 18)	//right
		{
			if (x < -(_Width-160))
			{
				break;
			}
			Right(&m_mapCoordMap);
			WM_Invalidate(_hFrameMapWin);

			//WM_MoveWindow(_hFrameMapWin, -16, 0);
		}
		else if( key==GUI_KEY_ENTER)        /* ASCII: ENTER     Crtl-M */
        {
			WM_Invalidate(_hFrameMapWin);
            SetCenterINT(&m_mapCoordMap,x,y);

		}
		break;
   default:
       WM_DefaultProc(pMsg);
  }
}

/*******************************************************************
*
*       _cbCallbackT1
*/
#define CURRENTDIR1  "\\map\\"
char currpath1[256];

char* GetMapFileName()   
  {   
          struct   _finddata_t   filestruct1;           
          int   n=0;   
          unsigned long   style;
		  char filepath1[100];
		  long   handle1;
		  int YPos=10;
		  char warn[100];
		  //开始查找工作，找到当前目录下的第一个实体（文件或目录）   
          //“*”表示查找任何的文件或子目录，filestruct为查找结果 
		  GetCurrentDirectoryA(256,currpath1);
		  strcpy(filepath1,currpath1);
		  strcat(currpath1,CURRENTDIR1);
		  strcpy(filepath1,currpath1);
          strcat(currpath1,"*.qm2");  
          handle1=_findfirst(currpath1,&filestruct1);   
          if(handle1==-1)   return NULL;         //如果handle为-1，表示当前目录为空，则结束查找而返回   
		  sprintf(warn,"Read %s ...",filestruct1.name);
		  GUI_SetColor(GUI_RED);
		  GUI_DispStringAt(warn,100,10);
		  sprintf(currpath1,"%s%s",filepath1,filestruct1.name);
		  //load_mapinfo(currpath1);
 		  //YPos+=10;
		  //GUI_DispStringAt("Read Map data OK!",0,YPos);
          _findclose(handle1);                   

		  return currpath1;
  }
	   /*
		  style=GetFileAttributes(filestruct.name);
		  if((style   &   FILE_ATTRIBUTE_DIRECTORY)   &&   !(style   &   FILE_ATTRIBUTE_HIDDEN))   
          {   
                  //如果是目录，则进入该目录并递归调用函数GetFileList进行查找   
                  if(filestruct.name[0]!='.')         //如果目录名的首字符为'.'（即为"."或".."），则不用进行查找   
                  {   
                          _chdir(filestruct.name);   
                          n++;   
                          //InsertItem(filestruct.name,n);         //将找到的新目录插入树中   
                          GetFileList();   
                          _chdir("..");         //查找完毕后，返回上一级目录   
                  }                   
          } 
		  
          n=0;   
          //继续对当前目录中的下一个子目录或文件进行与上面同样的查找   
          while(!(_findnext(handle1,&filestruct1)))   
          {   
			  sprintf(warn,"Read %s ...",filestruct1.name);
			  GUI_DispStringAt(warn,0,YPos);
			  sprintf(currpath1,"%s%s%s",filepath1,CURRENTDIR,filestruct1.name);
			  load_mapinfo(currpath1);
 			  YPos+=10;
			  GUI_DispStringAt("Read Map data OK!",0,YPos);
             // if(n++>1) 
			//	  break;
		 
			  if(style   &   FILE_ATTRIBUTE_DIRECTORY)   //&&   !(style   &   FILE_ATTRIBUTE_HIDDEN))   
                  {   
                          if(*filestruct.name!='.')   
                          {   
                                  _chdir(filestruct.name);   
                                  n++;                                   
                                  //InsertItem(filestruct.name,n);   
                                  GetFileList();   
                                  _chdir("..");   
                          }                   
                  }                   

	}   
          _findclose(handle1);                   
  }   
*/


static void _cbCallbackMapWindow(WM_MESSAGE * pMsg) {

	GUI_POINT p[4] = {{0,0},{40,0},{30,-10},{40,-24}};
    int x,y,key;
	char * mapfile;
	WM_HWIN hWin = (FRAMEWIN_Handle)(pMsg->hWin);
	switch (pMsg->MsgId) 
	{
    case WM_CREATE:

		myUcMap.pmapCoorMap=&m_mapCoordMap;
        mapfile=GetMapFileName();
		if(mapfile==NULL) return;
		Map_ReadFromFile(&myUcMap,mapfile);//"E:\\uC-GUI\\Start\\map\\bj.qm2");
		m_mapCoordMap.wScreen=948;//GUI_GetScreenSizeX();
		m_mapCoordMap.hScreen=453;//GUI_GetScreenSizeY();
		m_mapCoordMap.wScreen=GUI_GetScreenSizeX();
		m_mapCoordMap.hScreen=GUI_GetScreenSizeY();
		m_mapCoordMap.nMapMode=1;
		myUcMap.m_bDrawScale=TRUE;
		m_mapCoordMap.blc = myUcMap.map_blc;  // 设置当前比例尺为地图保存的比例尺
		SetCenterFP(&m_mapCoordMap,myUcMap.map_fCenterX,myUcMap.map_fCenterY);
		SetScreenXY(&m_mapCoordMap);
		myUcMap.puserlayer=&g_userLayer;
		g_userLayer.m_bShowState=TRUE;
		//memcpy(&m_EagleCoordMap,&m_mapCoordMap,sizeof(MapCoorTrans));
		break;
	case WM_PAINT:
		//GUI_SetAlpha(0);
		GUI_SetBkColor(GUI_WHITE);
		GUI_SetColor(GUI_WHITE);
		GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
		GUI_Clear();
		//GUI_SetAlpha(0x80);
        myUcMap.pmapCoorMap=&m_mapCoordMap;
		Map_Draw(&myUcMap,1);
		//WM_Invalidate(_hFrameEagleMapWin);
/*		
		GUI_SetFont(&GUI_Font6x8);
		GUI_SetPenSize((int)(8*_Width/960)+1);
		GUI_SetPenSize((int)(5*_Width/960)+1);
		
		GUI_SetColor(GUI_RED);
		GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
		GUI_FillPolygon(&p, 4, _Target_X+3, _Target_Y-3);
		GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
*/
		break;

	case WM_TOUCH:
		//
		break;
//	case WM_MOUSE:
//		break;
	case WM_TIMER:
	   if(ReadGpsData()==0)
	   {
         closegpsfile();
	     if(GetNextFile()==0)
	    {
		  EndGpsFile();
		  GetGpsFileList() ;
	    }
	   }
       WM_Invalidate(hWin);
	   WM_RestartTimer(pMsg->Data.v, 1000);
		break;
	case WM_KEY:
		x = WM_GetWindowOrgX(hWin);
		y = WM_GetWindowOrgY(hWin);
		key = GUI_GetKey();
		//ZoomIn(&m_mapCoordMap);
		//WM_Invalidate(_hFrameMapWin);
        /*
		switch(key)
		{
		case GUI_KEY_BACKSPACE:         // ASCII: BACKSPACE Crtl-H 
			break;
		case GUI_KEY_TAB:         // ASCII: TAB       Crtl-I 
			break;
		case GUI_KEY_BACKTAB:
			break;
		case GUI_KEY_ENTER:        // ASCII: ENTER     Crtl-M 
			break;
        //case GUI_
		case GUI_KEY_LEFT:
			Left(&m_mapCoordMap);
			break;
		case GUI_KEY_UP:
			Up(&m_mapCoordMap);
			break;
		case GUI_KEY_RIGHT:
			Right(&m_mapCoordMap);
			break;
		case GUI_KEY_DOWN:
			Down(&m_mapCoordMap);
			break;
		case GUI_KEY_HOME:
			break;
		case GUI_KEY_END:
			break;
		case GUI_KEY_SHIFT:
			break;
		case GUI_KEY_CONTROL:
			break;
		case GUI_KEY_ESCAPE:   // ASCII: ESCAPE    0x1b   
			break;
		case GUI_KEY_INSERT:
			break;
		case GUI_KEY_DELETE:
			break;
		case GUI_KEY_SPACE:
			break;
		case GUI_KEY_PGUP:
			break;
		case GUI_KEY_PGDOWN:
			break;
		case GUI_KEY_F1:
			break;
		case GUI_KEY_F2:
			break;
		default:
			break;
		}
		*/
		/*
		if( key == 17)		//up
		{
			if (y > 120)
			{
				break;
			}
			WM_MoveWindow(_hFrameMapWin, 0, 16);
		}
		else if(key == 19)	//down
		{
			if (y < -(_Height-120))
			{
				break;
			}
			WM_MoveWindow(_hFrameMapWin, 0, -16);
		}
		else if (key == 16)	//left
		{
			if (x > 160)
			{
				break;
			}
			WM_MoveWindow(_hFrameMapWin, 16, 0);
		}
		else if (key == 18)	//right
		{
			if (x < -(_Width-160))
			{
				break;
			}
			WM_MoveWindow(_hFrameMapWin, -16, 0);
		}
		*/
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void ShowAllMap(WM_HWIN hWin,MapCoorTrans *pmapCorrTrans)
{
	float fCenterX ,fCenterY;
    GUI_RECT eagleRect;
   
	WM_GetWindowRectEx(hWin,&eagleRect); 
	pmapCorrTrans->wScreen=eagleRect.x1-eagleRect.x0;
    pmapCorrTrans->hScreen=eagleRect.y1-eagleRect.y0; 
	pmapCorrTrans->blc=1;
	/*以下确定屏幕左下角的实际坐标，即在左侧和下侧都留了10点阵的区域
	（minx,miny)在屏幕的左下角向右10点阵同时向上10点阵处。*/
	//pmapCorrTrans->xStart=//eagleRect.x0-10*pmapCorrTrans->blc;
	//pmapCorrTrans->yStart=//eagleRect.y0-10*pmapCorrTrans->blc;
	
	//将全图的显示比例放在首屏，并把屏幕设为首屏


	//// 保存文档中心地图坐标
	//fCenterX = pmapCorrTrans->xStart + VLtoDL(pmapCorrTrans,pmapCorrTrans->wScreen/2);
	//fCenterY = pmapCorrTrans->yStart + VLtoDL(pmapCorrTrans,pmapCorrTrans->hScreen/2);
	//SetCenterFP(pmapCorrTrans,fCenterX,fCenterY);
	SetScreenXY(pmapCorrTrans);
}

static void _cbCallbackEagleMapWindow(WM_MESSAGE * pMsg) {

	GUI_POINT p[4] = {{0,0},{40,0},{30,-10},{40,-24}};
    int x,y,key;
	char * mapfile;
	GUI_RECT eagleRect;
	WM_HWIN hWin = (FRAMEWIN_Handle)(pMsg->hWin);
	switch (pMsg->MsgId) 
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		GUI_SetAlpha(0);
		GUI_SetBkColor(GUI_BLUE);
		GUI_SetColor(GUI_RED);
		GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
		GUI_Clear();
		WM_GetWindowRectEx(hWin,&eagleRect); 
		GUI_DrawRectEx(&eagleRect);
	    //GUI_SetAlpha(0x80);

        ShowAllMap(hWin,&m_EagleCoordMap);
        myUcMap.pmapCoorMap=&m_EagleCoordMap;

		Map_Draw(&myUcMap,1);
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

/*******************************************************************
*
*       _cbBackgroundWin
*/
static void _cbBackgroundWin(WM_MESSAGE* pMsg) {
	
  switch (pMsg->MsgId) {
  case WM_PAINT:
    /* Handle only the paint message */
    GUI_SetBkColor(0x000000);
    GUI_Clear();
    GUI_SetFont(&GUI_Font24_ASCII);
    GUI_DispStringHCenterAt("uc/GUI GIS App \n", 150, 5);
    GUI_SetFont(&GUI_Font13_1);
    GUI_DispStringAt(" Software Development xiang_xudong@163.com", 250, 12);
	
	//
  default:
    WM_DefaultProc(pMsg);
  }
}


/*******************************************************************
*
*       _Task_0
*/
extern GUI_CONST_STORAGE GUI_BITMAP bmLaySelect;
extern GUI_CONST_STORAGE GUI_BITMAP bmNavi;
extern GUI_CONST_STORAGE GUI_BITMAP bmSearch;
extern GUI_CONST_STORAGE GUI_BITMAP bmenlarge;
extern GUI_CONST_STORAGE GUI_BITMAP bmshrink;
static void _Task_0(void) {

//	float minx,miny,maxx,maxy,bl1;
	BUTTON_Handle hButton_SetMap;	//this button is to call the map setting dialog
	BUTTON_Handle hButton_Search;	//this button is to call the map search dialog
	BUTTON_Handle hButton_SetNavi;	//this button is to call the dialog that set the start and end of navigation
	BUTTON_Handle hButton_About;
	BUTTON_Handle hBtn_Enlarge;
	BUTTON_Handle hBtn_Shrink;
	FRAMEWIN_Handle hFrameWin;
	WM_HWIN         hChildWin;
	/* Create frame window */
	//GUI_SetFont( &GUI_Font16_ASCII );
	FRAMEWIN_SetDefaultFont(&GUI_FontHZ_KaiTi_12);//GUI_Font16_ASCII );
	hFrameWin = FRAMEWIN_CreateEx(0, 20,64, 240, WM_HBKWIN, WM_CF_SHOW | WM_CF_STAYONTOP,
				 WM_CF_MEMDEV, 0, "菜单", _cbCallbackMenuWindow);
	//FRAMEWIN_AddMinButton(hFrameWin,FRAMEWIN_BUTTON_RIGHT,1);
	/* Create child window */
	//hChildWin = WM_CreateWindowAsChild(0, 0, 0, 0, WM_GetClientWindow(hFrameWin), 
	//									WM_CF_SHOW | WM_CF_MEMDEV, NULL, 0);
	FRAMEWIN_AddMinButton(hFrameWin,FRAMEWIN_BUTTON_RIGHT,1);
	FRAMEWIN_SetTitleHeight(hFrameWin, 20);
	FRAMEWIN_SetActive(hFrameWin, 0);

	hChildWin = WM_GetClientWindow(hFrameWin);

	hButton_SetMap = BUTTON_CreateEx(1, 4, 56, 48, hChildWin, WM_CF_SHOW, 0, ID_BUTTON_CONFIGURE);
	BUTTON_SetBitmap(hButton_SetMap, 0, &bmLaySelect);

	hButton_Search = BUTTON_CreateEx(1, 58, 56, 48, hChildWin, WM_CF_SHOW, 0,ID_BUTTON_SEARCH);
	BUTTON_SetBitmap(hButton_Search, 0, &bmSearch);

	hBtn_Enlarge = BUTTON_CreateEx(1, 112, 25, 38, hChildWin, WM_CF_SHOW, 0,1001);
	BUTTON_SetBitmap(hBtn_Enlarge, 0, &bmenlarge);
	hBtn_Shrink = BUTTON_CreateEx(32, 112, 25, 38, hChildWin, WM_CF_SHOW, 0,1002);
	BUTTON_SetBitmap(hBtn_Shrink, 0, &bmshrink);

	hButton_About = BUTTON_CreateEx(1, 166, 56, 48, hChildWin, WM_CF_SHOW, 0,ID_BUTTON_ABOUT);
	BUTTON_SetFont(hButton_About,&GUI_FontHZ_KaiTi_12);
	BUTTON_SetText(hButton_About,"关于");
 	/* Make sure the right window is active... */
	WM_SelectWindow(hChildWin);
	WM_SetFocus(hFrameWin);

	//_hFrameEagleMapWin = WM_CreateWindow(_Width-200, _Height-150, _Width, _Height,WM_CF_SHOW | WM_CF_MEMDEV, _cbCallbackEagleMapWindow, 0);
   // WM_SetHasTrans(_hFrameEagleMapWin);           // Set transparency
	//WM_SetFocus(_hFrameEagleMapWin);

	GUI_Exec();
}

/*******************************************************************
*
*       _Task_1
*/


static void _Task_1(void) {

	WM_HTIMER     hTimer;

	_hFrameMapWin = WM_CreateWindow(0, 0, _Width, _Height, WM_CF_SHOW | WM_CF_MEMDEV, _cbCallbackMapWindow, 0);
   hTimer=WM_CreateTimer(_hFrameMapWin,0,1000,0);

	while(1){
		GUI_Delay(200);
		GUI_Exec();
	}

}

void MultTask_MainTask(void) {


 	GUI_Init();
    GUI_SetFont(&GUI_FontHZ_KaiTi_12);
	WM_SetCreateFlags(WM_CF_MEMDEV);              /* Use memory devices on all windows to avoid flicker */
	WM_SetCallback(WM_HBKWIN, _cbBackgroundWin);  /* Set callback for background window */
	_Width = GUI_GetScreenSizeX();
    _Height = GUI_GetScreenSizeY();
	
    _Task_0();
    _Task_1();
}

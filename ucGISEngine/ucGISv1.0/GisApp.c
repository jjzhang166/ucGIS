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

#define ID_BUTTON_CONFIGURE		(GUI_ID_USER +  0)

static UcMap myUcMap;
static UserLayer g_userLayer;
static MapCoorTrans m_mapCoordMap;
static MapCoorTrans m_EagleCoordMap;
static char * psmapfile;
static WM_HWIN _hFrameMapWin;
#include <stddef.h>
#include <string.h>
#define MAPNOGRIDINDEX 0
#define USEENCRYPT 0
#define GPSDEMO    0
GUI_FONT GUI_UNI_PTR * gGisFont;
//WM_HWIN _hFrameMapWin;
//WM_HWIN _hFrameEagleMapWin;
extern GUI_CONST_STORAGE GUI_BITMAP bmZOOMOUT;
extern GUI_CONST_STORAGE GUI_BITMAP bmZOOMIN;
extern GUI_CONST_STORAGE GUI_BITMAP bmSearch;
extern GUI_CONST_STORAGE GUI_BITMAP bmReturn;
extern GUI_CONST_STORAGE GUI_BITMAP bmSearchOld;

//#include "GPS_Spec.h"

#if GUI_OS == 0
  #error Multitasking sample requires task awareness (#define GUI_OS 1)
#endif


/*******************************************************************
*
*       Define how to create a task and start multitasking
*
********************************************************************

  If not using embOS you have to change the hardware
  dependent macros to work with your OS

*/
static int _Alpha_0     = 128;
static int _Alpha_1     = 128;
static BOOLEAN bGisExit=FALSE;

 EDIT_Handle hEdit;
 LISTBOX_Handle hListBox;

static WM_CALLBACK * _pcbClient;



#if GPSDEMO
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
      GUI_LOCK();
	  DrawUserLayerPoint(&gpsinfo,&g_userLayer);
      GUI_UNLOCK();
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
      GUI_LOCK();
	  DrawUserLayerPoint(&gpsinfo,&g_userLayer);
      GUI_UNLOCK();
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
#endif

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "地图控制", 0,               50,  55, 220, 130, FRAMEWIN_CF_MOVEABLE},
  //{ TEXT_CreateIndirect,     "地点:",            GUI_ID_TEXT0,     5,  10,  50,  22, TEXT_CF_LEFT },
  //{ EDIT_CreateIndirect,     "加油站",            GUI_ID_EDIT0,     40,  10,  100,  22, TEXT_CF_LEFT },
};
static void _deletallitem(LISTBOX_Handle hListbox)
{   
	int i, itemnum;
	if(!hListbox) return;
	itemnum=LISTBOX_GetNumItems(hListbox);
	
	for(i=0;i<itemnum;i++)
	{
		LISTBOX_DeleteItem(hListbox,0);
	}
}

/*********************************************************************
*
*       _cbClient
*
* Purpose:
*   Callback routine of property dialog
*/
MyPoint layerpoints[100];
static void _cbClient(WM_MESSAGE * pMsg) {
  int  Id,i,nCount=0;
  char aBuffer[100];
  char *pstr;
  WM_HWIN hDlg;
  int NCode;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
 break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code
    switch (NCode) {
    case WM_NOTIFICATION_VALUE_CHANGED:   // Value has changed
      //_OnValueChanged(hDlg, Id);
      break;
    case WM_NOTIFICATION_RELEASED:
		switch(Id)
		{
		case GUI_ID_BUTTON4:
	        EDIT_GetText(hEdit, aBuffer, sizeof(aBuffer));
            SearchMap(&myUcMap,aBuffer, layerpoints, &nCount);
		    _deletallitem(hListBox);
			for(i=0;i<nCount;i++)
			{
                
			    WM_LOCK();
				pstr=(GISCHAR*) GUI_ALLOC_h2p(layerpoints[i].baselabel.text);
	            WM_UNLOCK();

				LISTBOX_AddString(hListBox,pstr);
			}
            break;
		case GUI_ID_LISTBOX0:
			
			i = LISTBOX_GetSel(hListBox);
			SetCenterFP(&m_mapCoordMap,layerpoints[i].m_X,layerpoints[i].m_Y);
			WM_InvalidateWindow(_hFrameMapWin);
			break;
		}
		break;
	}
    break;
  case WM_PAINT:
    GUI_SetAlpha(_Alpha_0);   // Set alpha value for drawing operations
    GUI_SetBkColor(0xAAAAAA); // Draw gray background...
    GUI_Clear();              // ...with alpha blending
    GUI_SetAlpha(0);          // Set alpha value to default
    return;
  }
  if (_pcbClient) {
    _pcbClient(pMsg);
  }
}

/*********************************************************************
*
*       _cbFrame
*
* Purpose:
*   Callback routine of frame window
*/
static void _cbFrame(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetAlpha(_Alpha_1);
    break;
  }
  FRAMEWIN_Callback(pMsg);
  GUI_SetAlpha(0);
}
static void _CreateButton(const char* pText, int x, int y, int w, int h, WM_HWIN hParent, int Id,const GUI_FONT GUI_UNI_PTR *pFon,const GUI_BITMAP *pbmMap ) {
  WM_HWIN hBut;
  hBut = BUTTON_CreateAsChild(x, y, w, h, hParent, Id, WM_CF_SHOW);
  BUTTON_SetText(hBut, pText);
  BUTTON_SetFont(hBut, pFon);
  if(pbmMap)
    BUTTON_SetBitmap(hBut, 0, pbmMap);


}

static void _createSearchDialog()
{
  WM_MESSAGE Msg = {0};
  WM_HWIN hFrame,hChildWin;
  
  hFrame = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), 0, 0, 0, 0);
  FRAMEWIN_SetFont(hFrame,gGisFont);
  WM_SetHasTrans(hFrame);           // Set transparency
  hChildWin=WM_GetClientWindow(hFrame);
  hListBox = LISTBOX_CreateEx(5, 30, 120, 60, hChildWin, WM_CF_SHOW, 0, GUI_ID_LISTBOX0,NULL);
  SCROLLBAR_CreateAttached(hListBox, SCROLLBAR_CF_VERTICAL);
  LISTBOX_SetFont(hListBox,gGisFont);

  /* Create edit widget */
  hEdit =EDIT_CreateAsChild(8, 25, 120, 22, hFrame, 13, WM_CF_SHOW, 100);
  EDIT_SetFont(hEdit, gGisFont);
  EDIT_SetText(hEdit, "加油站");
  EDIT_SetTextColor(hEdit, 0, GUI_RED);

  _CreateButton("", 150, 8, 50, 50, hChildWin, GUI_ID_BUTTON4,&GUI_FontHZ12,&bmSearchOld);
  
  FRAMEWIN_SetTitleHeight(hFrame,15);
  FRAMEWIN_AddMinButton(hFrame,FRAMEWIN_BUTTON_RIGHT,1);
  FRAMEWIN_AddCloseButton(hFrame,FRAMEWIN_BUTTON_RIGHT,1);

  WM_SetCallback(hFrame, _cbFrame); // Overwrite callback
  //
  // Set client attributes
  //
  WM_SetHasTrans(hChildWin);                         // Set transparency
 
  _pcbClient = WM_SetCallback(hChildWin, _cbClient); // Overwrite callback
  Msg.MsgId = WM_INIT_DIALOG;                      // Send WM_INIT_DIALOG
  WM_SendMessage(hChildWin, &Msg);

}

#define CURRENTDIR1  "\\map\\"
char currpath1[256];
char currpath2[256];

static char* GetMapFileName()   
  {   
          struct   _finddata_t   filestruct1;           
          int   n=0;   
//          unsigned long   style;
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
		  strcpy(currpath2,currpath1);
          strcat(currpath1,"*.qm2");  
          handle1=_findfirst(currpath1,&filestruct1);   
          
		  if(handle1==-1)   return NULL;         //如果handle为-1，表示当前目录为空，则结束查找而返回   
		  sprintf(warn,"Read %s ...",filestruct1.name);

		  sprintf(currpath1,"%s%s",filepath1,filestruct1.name);
          strcat(currpath2,"*.ind");  
          handle1=_findfirst(currpath2,&filestruct1);   
		  if(handle1==-1)   return NULL;         //如果handle为-1，表示当前目录为空，则结束查找而返回   
		  sprintf(warn,"Read %s ...",filestruct1.name);

		  sprintf(currpath2,"%s%s",filepath1,filestruct1.name);
		  
		  //load_mapinfo(currpath1);
 		  //YPos+=10;
		  //GUI_DispStringAt("Read Map data OK!",0,YPos);
          _findclose(handle1);                   

		  return currpath1;
  }

static int FirstTime=0;
static void _MoveMap() {
  int Time = GUI_GetTime();
  int Dif  = (Time - FirstTime)/1000 ;
  if (Dif > 0) {
	  if(ReadGpsData()==0)
	   {
         closegpsfile();
	     if(GetNextFile()==0)
	    {
		  EndGpsFile();
		  GetGpsFileList() ;
	    }
	   }
    
    //WM_InvalidateWindow(hMap);
    FirstTime = Time;
  }

}

static void _cbCallbackMapWindow(WM_MESSAGE * pMsg) {

  int key;
  int x,y;
  int NCode, Id;
  WM_HWIN hItem; 
  
//  GUI_RECT eagleRect;
//  char * mapfile;
  WM_HWIN hWin = (FRAMEWIN_Handle)(pMsg->hWin);
  //WM_HWIN 	hChildWin = WM_GetClientWindow(hWin);
 
  switch (pMsg->MsgId) {
  case WM_CREATE:

		myUcMap.pmapCoorMap=&m_mapCoordMap;
#if MAPNOGRIDINDEX		
		Map_ReadFromFile(&myUcMap,psmapfile);//"E:\\uC-GUI\\Start\\map\\bj.qm2");
#else
		Map_ReadFileIndex(&myUcMap,psmapfile,currpath2);
#endif
		m_mapCoordMap.wScreen=WM_GetWindowSizeX(hWin);
		m_mapCoordMap.hScreen=WM_GetWindowSizeY(hWin);
		m_mapCoordMap.nMapMode=1;
		myUcMap.m_bDrawScale=TRUE;
		m_mapCoordMap.blc = myUcMap.map_blc;  // 设置当前比例尺为地图保存的比例尺
		SetCenterFP(&m_mapCoordMap,myUcMap.map_fCenterX,myUcMap.map_fCenterY);
		SetScreenXY(&m_mapCoordMap);
		myUcMap.puserlayer=&g_userLayer;
		g_userLayer.m_bShowState=TRUE;
		g_userLayer.m_mapCoordMap=&m_mapCoordMap;
#if !(MAPNOGRIDINDEX)		
	    Map_ReadGridIndex(&myUcMap,currpath2);
        Map_ReadMapWithGridIndex(&myUcMap,psmapfile);
#endif
#if GPSDEMO
       WM_CreateTimer(hWin,0,1000,0);
#endif
	  break;
	case WM_KEY:
		x = WM_GetWindowOrgX(hWin);
		y = WM_GetWindowOrgY(hWin);
		key = GUI_GetKey();
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
		case GUI_KEY_LEFT:
			Left(&m_mapCoordMap);
		    WM_InvalidateWindow(hWin);
			break;
		case GUI_KEY_UP:
			Up(&m_mapCoordMap);
		    WM_InvalidateWindow(hWin);
			break;
		case GUI_KEY_RIGHT:
			Right(&m_mapCoordMap);
  		   WM_InvalidateWindow(hWin);
			break;
		case GUI_KEY_DOWN:
			Down(&m_mapCoordMap);
		    WM_InvalidateWindow(hWin);
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
		break;
  case WM_TIMER:
		_MoveMap();
	    WM_InvalidateWindow(hWin);
	    WM_RestartTimer(pMsg->Data.v, 1000);
        break;
  case WM_PAINT:
         GUI_SetAlpha(0);
		 GUI_SetBkColor(GUI_WHITE);
		 GUI_SetColor(GUI_WHITE);
		 GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
		 GUI_Clear();
#if !(MAPNOGRIDINDEX)		
        Map_ReadMapWithGridIndex(&myUcMap,psmapfile);
#endif
		myUcMap.pmapCoorMap=&m_mapCoordMap;
		Map_Draw(&myUcMap,1);
	  break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code
    switch (NCode) {
    case WM_NOTIFICATION_VALUE_CHANGED:   // Value has changed
      //_OnValueChanged(hDlg, Id);
        break;
    case WM_NOTIFICATION_RELEASED:
		hItem = pMsg->hWinSrc;
		switch (WM_GetId(hItem)) {
		case GUI_ID_BUTTON0:
			ZoomIn(&m_mapCoordMap);
			WM_InvalidateWindow(hWin);
			break;
		case GUI_ID_BUTTON1:
			ZoomOut(&m_mapCoordMap);
			WM_InvalidateWindow(hWin);
			break;
		case GUI_ID_BUTTON2:
			_createSearchDialog();
			break;
		case GUI_ID_BUTTON3:
			bGisExit=TRUE;
			break;
         
		}
		break;
    }

	break;

  default:
    WM_DefaultProc(pMsg);
  }
}



char  UI_Delete_gisSDK(INT16U Id)
{
    return (DeleteCar(&g_userLayer,Id));
}

char  UI_NMEA0183_gisSDK(GpsDataInfo * Paramer)
{
      GUI_LOCK();
	  DrawUserLayerPoint(Paramer,&g_userLayer);
      GUI_UNLOCK();
	  return 0;
}
char UI_gisExit(INT8U exitCode)
{
	return 0;
}
/*******************************************************************
*
*       UI_gisSDK
*/
//&GUI_FontHZ12

extern char* Encrypt(char* strIn,char *strOut);

char  UI_gisSDK(char *filepath, INT16U SizeX, INT16U SizeY, GUI_FONT GUI_UNI_PTR * pfont, GpsDataInfo * Paramer)
{

    WM_HWIN hChildWin;
#if USEENCRYPT
	char MyKeCode[50],KeyCodeStr[50],ChipIDStr[50];
#endif
	if(pfont)
	   gGisFont=pfont;
	else
       return -2;

#if USEENCRYPT
    sprintf(ChipIDStr,"%s","ARM123450ocdef12345");
	memset(KeyCodeStr,0,50);
	Encrypt(ChipIDStr,KeyCodeStr);

	GUI_SetFont(gGisFont);
	if(strcmp(KeyCodeStr,MyKeCode))
	{

       TEXT_SetDefaultFont(gGisFont);
	   FRAMEWIN_SetDefaultFont(gGisFont);
       GUI_MessageBox("您的GIS程序未授权，不能使用本功能\n，请与厂商联系!","GIS版权警告",GUI_MB_WARNING);	
	   return -1;
	}
#endif
	if(filepath)
       psmapfile=filepath;
	else
	   psmapfile=GetMapFileName();//return -1;

    bGisExit=FALSE;
	//GUI_UC_SetEncodeGBK();
	WM_SetCreateFlags(WM_CF_MEMDEV);              /* Use memory devices on all windows to avoid flicker */
 	_hFrameMapWin = WM_CreateWindow(0, 0, SizeX, SizeY, WM_CF_SHOW | WM_CF_MEMDEV, _cbCallbackMapWindow, 0);
	hChildWin = WM_GetClientWindow(_hFrameMapWin);
    _CreateButton("", 1,  15, 18, 18, hChildWin, GUI_ID_BUTTON0,&GUI_FontHZ12,&bmZOOMIN);
    _CreateButton("", 1,  19+15, 18, 18, hChildWin, GUI_ID_BUTTON1,&GUI_FontHZ12,&bmZOOMOUT);
    _CreateButton("", 1, 19*2+15, 18, 18, hChildWin, GUI_ID_BUTTON2,&GUI_FontHZ12,&bmSearch);
    _CreateButton("", 1,  19*3+15, 18, 18, hChildWin, GUI_ID_BUTTON3,&GUI_FontHZ12,&bmReturn);
	WM_SetFocus(_hFrameMapWin);

	while(1){
		GUI_Delay(100);
		GUI_Exec();
		if(bGisExit) break;
	}
   GUI_LOCK();
   MapRelease(&myUcMap);
   FreeMapCoorTrans(&m_mapCoordMap);
   UserLayerFree(&g_userLayer);
   GUI_UNLOCK();
   WM_DeleteWindow(_hFrameMapWin);
   return 0;
}

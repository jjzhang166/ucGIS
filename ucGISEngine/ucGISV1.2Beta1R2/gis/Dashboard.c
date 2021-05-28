/*
*********************************************************************************************************
*                                             uC/GUI V4.14
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : Dashboard.c
Purpose     : dashboard demo
---------------------------END-OF-HEADER------------------------------
*/
#include <stdio.h>
#include <io.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "GUI.h"
#include "LCDConf.h"
#include "MULTIPAGE.h"
#include "FRAMEWIN.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "LISTBOX.h"

#include "GisApi.h"
#if 0
GUI_FONT GUI_UNI_PTR * gGisFont;
static UcMap myUcMap;
static UserLayer g_userLayer;
static MapCoorTrans m_mapCoordMap;
static BOOLEAN bGisExit=FALSE;

//MapCoorTrans m_EagleCoordMap;

extern GUI_CONST_STORAGE GUI_BITMAP bmZOOMOUT;
extern GUI_CONST_STORAGE GUI_BITMAP bmZOOMIN;
extern GUI_CONST_STORAGE GUI_BITMAP bmSearch;
extern GUI_CONST_STORAGE GUI_BITMAP bmReturn;

#if (GUI_ALLOC_SIZE < 4000000)
  //#error GUI_ALLOC_SIZE should be at least 4 MB
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define SHOW_RECTS  0

#define AA_FACTOR   3
#define HIRES       1

#if HIRES
  #define FACTOR        AA_FACTOR
  #define HIRES_ON();   GUI_AA_EnableHiRes();
  #define HIRES_OFF();  GUI_AA_DisableHiRes();
#else
  #define FACTOR        1
  #define HIRES_ON();
  #define HIRES_OFF();
#endif

#define FLAG_SHOW_MARK      0
#define FLAG_SHOW_PITCH     1
#define FLAG_SHOW_GRAD      2
#define FLAG_SHOW_ARC       3   /* needs five bits (3 - 7) */
#define FLAG_SHOW_TEXT      8
#define FLAG_SHOW_SCALE     9
#define FLAG_NEEDLE_FRAME  10
#define FLAG_NEEDLE_LINE   11

#define PI  3.1415926536

#define NEEDLE_GRAD 720

#define ARRAY(aItems) aItems, GUI_COUNTOF(aItems)

static int _Alpha_0     = 85;
static int _Alpha_1     = 85;

static WM_CALLBACK * _pcbClient;

/*******************************************************************
*
*       _cbCallbackT1
*/

  EDIT_Handle hEdit;
  LISTBOX_Handle hListBox;
    FRAMEWIN_Handle hFrame, hWinMap;
static const GUI_ConstString _ListBox[] = {
  "地名", NULL
};
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect,   "菜单",              0,                0,  0,   120,  160, FRAMEWIN_CF_MOVEABLE},
  { BUTTON_CreateIndirect,     "漫游",            GUI_ID_BUTTON0,     2,  0,  55,  45},
  { BUTTON_CreateIndirect,     "放大",            GUI_ID_BUTTON0,     2,  50, 55,  45},
  { BUTTON_CreateIndirect,     "缩小",            GUI_ID_BUTTON1,     54,  0, 55,  45},
  { BUTTON_CreateIndirect,     "搜索",            GUI_ID_BUTTON2,     54,  50, 55, 45},
};


extern GUI_CONST_STORAGE GUI_BITMAP bmLaySelect;
extern GUI_CONST_STORAGE GUI_BITMAP bmNavi;
extern GUI_CONST_STORAGE GUI_BITMAP bmSearch;
extern GUI_CONST_STORAGE GUI_BITMAP bmenlarge;
extern GUI_CONST_STORAGE GUI_BITMAP bmshrink;

#define CURRENTDIR1  "\\map\\"
static char currpath1[256];

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
		  GUI_SetFont(&GUI_FontHZ12);
		  GUI_DispStringAt(warn,100,10);
		  sprintf(currpath1,"%s%s",filepath1,filestruct1.name);
		  //load_mapinfo(currpath1);
 		  //YPos+=10;
		  //GUI_DispStringAt("Read Map data OK!",0,YPos);
          _findclose(handle1);                   

		  return currpath1;
  }

#if 1
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
/*********************************************************************
*
*       Types
*
**********************************************************************
*/

typedef struct {
  int x;
  int y;
  int Pressed;
  int Duration;
} PID_EVENT;

typedef struct {
  int x;
  int y;
  int xHere;
  int yHere;
  int DirX;
  int DirY;
  int PPM;
  int Dif;
  int PrevTime;
  const GUI_BITMAP* pBitmap;
} NAVIMAP;

typedef struct {
  U8  Sep[4];
} COLOR;

typedef struct {
  int x;
  int y;
  int x0;
  int y0;
  int ArcStart;
  int ArcEnd;
  int ArcRadius;
  int ArcArea1;
  int ArcArea2;
  int NumMarkLines;
  int NumPitchLines;
  int LineLen1;
  int LineLen2;
  int ArcWidth;
  int GradDist;
  int PenSize1;
  int PenSize2;
  int PenSize3;
  int NeedleType;
  int NeedleRadius;
  int NumStep;
  int NumStart;
  int NumExp;
  int LinePos1;
  int LinePos2;
  int ArcPos;
  int AxisRadius;
  int TextDist;
  U16 Flags;
  WM_HWIN hWin;
  GUI_MEMDEV_Handle hMemDev;
  char acText[33];
  const GUI_BITMAP* pBitmap;
  int BitmapY;
  COLOR Color[7];
} SCALE;

typedef struct {
  int NeedlePos;
  int NeedleUPM;
  int NeedleDir;
  int NeedlePrevTime;
} NEEDLE;


static void _DrawScale(SCALE* pObj);


/*********************************************************************
*
*       static data
*
**********************************************************************
*/

static int      _InitDialog;
static unsigned _Break;

static WM_HWIN _hDialogColor;
static WM_HWIN _hDialogMark;
static WM_HWIN _hDialogPitch;
static WM_HWIN _hDialogArc;
static WM_HWIN _hDialogGrad;
static WM_HWIN _hDialogScale;
static WM_HWIN _hDialogMisc;
static WM_HWIN hFrameControl;
static DROPDOWN_Handle _hDropDownScale;
static DROPDOWN_Handle _hDropDownColor;

static SCALE   _Scale[4];
static SCALE   _ScalePrev[4];
static int     _tDiff;
static int     _AutoMode;

static GUI_MEMDEV_Handle _hBkMemDev;

static NEEDLE  _Needle[4] = {
  {0, 5, 1, 0},
  {0, 6, 1, 0},
  {0, 4, 1, 0},
  {0, 2, 1, 0}
};

static PID_EVENT _aPID_Events[] = {
  { 320, 395, 1,  1000 },
  { 320, 395, 0, 10000 },
  { 320, 420, 1,  1000 },
  { 320, 420, 0, 10000 },
  { 320, 445, 1,  1000 },
  { 320, 445, 0, 10000 },
  { 320, 445, 0,     0 },
};

static const int _Pow10[] = {1, 10, 100, 1000};

/*********************************************************************
*
*       static data, presets
*
**********************************************************************
*/

static const SCALE _Presets[4][4] = {
  {
    {
       87, 250, 0, 0,  89, 225,  62,  41, 122,  4, 2, 15,  7, 15, 24,
        2, 2, 2, 1, 100, 1, 0, 0, 0, 0, 0, 16, 25, 0x37B, 0, 0, "Fuel", 0, 0,
      {{0xFF, 0xFF, 0xFF}, {0x00, 0xFF, 0x00}, {0xFF, 0x64, 0x00, 0xFF},
       {0x98, 0x00, 0x00}, {0x90, 0x90, 0x00}, {0x00, 0x50, 0x00}, {0xE8, 0xE8, 0xFF}}
    }, {
      144, 136, 0, 0,  72, 225,  70,  93, 184,  3, 1, 15,  7, 15, 24,
        2, 2, 2, 1, 100, 1, 0, 0, 0, 0, 0, 16, 25, 0x37B, 0, 0, "Oil", 0, 0,
      {{0xFF, 0xFF, 0xFF}, {0x00, 0xFF, 0x00}, {0xFF, 0x64, 0x00, 0xFF},
       {0x98, 0x00, 0x00}, {0x90, 0x90, 0x00}, {0x00, 0x50, 0x00}, {0xE8, 0xE8, 0xFF}}
    }, {
      335, 185, 0, 0,  58, 234, 149, 258, 335, 15, 4, 25, 10, 25, 29,
        2, 2, 2, 4, 100, 1, 1, 1, 0, 5, 0, 16, 25, 0x77F, 0, 0, "Speed", 0, 0,
      {{0xFF, 0xFF, 0xFF}, {0x00, 0xFF, 0x00}, {0x00, 0x8A, 0xFF, 0x88},
       {0x00, 0x70, 0x00}, {0x90, 0x90, 0x00}, {0xAA, 0x00, 0x00}, {0xE8, 0xE8, 0xFF}}
    }, {
      522, 208, 0, 0,  59, 225, 108, 257, 311,  8, 4, 25, 10, 25, 34,
        2, 2, 2, 3, 100, 1, 0, 0, 0, 0, 0, 16, 25, 0x77F, 0, 0, "RPM", 0, 0,
      {{0xFF, 0xFF, 0xFF}, {0x00, 0xFF, 0x00}, {0xFF, 0xAA, 0x00, 0xB5},
       {0x00, 0x70, 0x00}, {0x90, 0x90, 0x00}, {0xAA, 0x00, 0x00}, {0xE8, 0xE8, 0xFF}}
    }
  }, {
    {
       96, 208, 0, 0, 40, 160,  73,  41, 122,  4, 2, 15,  7, 15, 24,
        2, 2, 2, 3, 100, 1, 0, 0, 0, 0, 0, 16, 22, 0x74B, 0, 0, "Fuel", 0, 0,
      {{0xFF, 0xFF, 0xAC}, {0x00, 0xFF, 0x70}, {0xFF, 0x64, 0x64, 0x80},
       {0x98, 0x00, 0x00}, {0x90, 0x90, 0x00}, {0x00, 0x50, 0x00}, {0xE8, 0xE8, 0xFF}}
    }, {
      395, 247, 0, 0, 90, 270,  75,  93, 184,  3, 1, 15,  7, 15, 24,
        2, 2, 2, 1, 100, 1, 0, 0, 0, 0, 0, 16, 22, 0x77B, 0, 0, "Oil", 0, 0,
      {{0xFF, 0xFF, 0xAC}, {0x00, 0xFF, 0x70}, {0xFF, 0x64, 0x64, 0x80},
       {0x98, 0x00, 0x00}, {0x90, 0x90, 0x00}, {0x00, 0x50, 0x00}, {0xE8, 0xE8, 0xFF}}
    }, {
      272, 184, 0, 0,  58, 254, 156, 261, 327, 12, 4, 19, 10, 17, 42,
        2, 2, 2, 0, 100, 2, 1, 1, 17, 22, 0, 16, 22, 0x7C7, 0, 0, "Speed", 0, 0,
      {{0xFF, 0xFF, 0xAC}, {0x00, 0xFF, 0x70}, {0x00, 0xCA, 0xFF, 0x68},
       {0x00, 0x8C, 0x00}, {0xC4, 0xC4, 0x00}, {0xD8, 0x00, 0x00}, {0x75, 0xFF, 0xFF}}
    }, {
      511, 208, 0, 0, 128, 270, 107, 257, 311,  8, 4, 25, 10, 25, 34,
        2, 2, 2, 3, 100, 1, 0, 0, 0, 0, 0, 16, 22, 0x777, 0, 0, "RPM", 0, 0,
      {{0xFF, 0xFF, 0xAC}, {0x00, 0xFF, 0x70}, {0xFF, 0xAA, 0x00, 0xB5},
       {0x00, 0x70, 0x00}, {0x90, 0x90, 0x00}, {0xAA, 0x00, 0x00}, {0xE8, 0xE8, 0xFF}}
    }
  }, {
    {
      101, 212, 0, 0,  30, 270,  66,  60, 120,  5, 1, 15,  7, 13, 34,
        2, 2, 2, 0, 100, 1, 0, 0, 0, 0, 0, 16, 25, 0xF43, 0, 0, "Fuel", 0, 0,
      {{0xFF, 0xFF, 0xFF}, {0x00, 0xFF, 0x00}, {0xA0, 0x7C, 0xFF, 0xA0},
       {0x90, 0x00, 0x00}, {0x80, 0x80, 0x00}, {0x00, 0x70, 0x00}, {0xFF, 0xFF, 0xFF}}
    }, {
      543, 215, 0, 0,  30, 270,  66, 240, 320,  5, 1, 15,  7, 15, 34,
        2, 2, 2, 0, 100, 1, 0, 0, 0, 0, 0, 16, 25, 0xF43, 0, 0, "Oil", 0, 0,
      {{0xFF, 0xFF, 0xFF}, {0x00, 0xFF, 0x00}, {0xA0, 0x7C, 0xFF, 0xA0},
       {0x00, 0x70, 0x00}, {0x90, 0x90, 0x00}, {0xAA, 0x00, 0x00}, {0xFF, 0xFF, 0xFF}}
    }, {
      325, 155, 0, 0,  30, 270,  95, 269, 317,  5, 1, 16,  8,  9, 34,
        2, 2, 2, 1, 100, 1, 0, 0, 0, 0, 0, 16, 40, 0x773, 0, 0, "Speed", 0, 0,
      {{0xFF, 0xFF, 0xFF}, {0x00, 0xFF, 0x00}, {0xFF, 0x4C, 0x38, 0xA0},
       {0x00, 0x70, 0x00}, {0x90, 0x90, 0x00}, {0xAA, 0x00, 0x00}, {0xFF, 0x64, 0x64}}
    }, {
      325, 155, 0, 0,  30, 270, 137, 228, 329, 10, 1, 16,  9, 25, 21,
        2, 2, 2, 0, 100, 2, 1, 1, 0, 0, 0, 16, 16, 0x747, 0, 0, "RPM", 0, 0,
      {{0xFF, 0xFF, 0xFF}, {0x00, 0xFF, 0x00}, {0x00, 0x8C, 0xFF, 0xA4},
       {0x00, 0x70, 0x00}, {0x90, 0x90, 0x00}, {0xAA, 0x00, 0x00}, {0xFF, 0x64, 0x64}}
    }
  }, {
    {
      102, 251, 0, 0,  90, 270,  62,  60, 120,  0, 5, 15,  7, 13, 24,
        2, 2, 2, 4, 100, 1, 0, 0, 0, 0, 0, 20, 25, 0x77F, 0, 0, "Fuel", 0, 0,
      {{0xFF, 0xFF, 0xC0}, {0x00, 0xFF, 0xFF}, {0x80, 0xFF, 0x00, 0x80},
       {0xAA, 0x00, 0x00}, {0x90, 0x90, 0x00}, {0x00, 0x70, 0x00}, {0xFF, 0xFF, 0xFF}}
    }, {
      292, 251, 0, 0,  90, 270,  62,  90, 270,  0, 3, 15,  7, 15, 24,
        2, 2, 2, 4, 100, 1, 0, 0, 0, 0, 0, 20, 25, 0x757, 0, 0, "Oil", 0, 0,
      {{0xFF, 0xFF, 0xC0}, {0x00, 0xFF, 0xFF}, {0x80, 0xFF, 0x00, 0x80},
       {0x90, 0x90, 0x00}, {0x00, 0x70, 0x00}, {0x90, 0x90, 0x00}, {0xFF, 0xFF, 0xFF}}
    }, {
      447, 192, 0, 0, 123, 291, 160, 270, 331, 13, 4, 13,  5,  7, 13,
        2, 2, 2, 2,  80, 2, 1, 1, 35, 44, 50, 16, 25, 0x73F, 0, 0, "Speed", 0, 0,
      {{0xFF, 0xFF, 0xC0}, {0x00, 0xFF, 0xFF}, {0x95, 0x64, 0xFF, 0xB0},
       {0x00, 0x70, 0x00}, {0x90, 0x90, 0x00}, {0xAA, 0x00, 0x00}, {0xFF, 0xFF, 0xFF}}
    }, {
      197, 144, 0, 0,  81, 255, 124, 257, 311,  8, 1, 13,  5,  7, 10,
        2, 2, 2, 2,  80, 1, 0, 0, 25, 34, 40, 16, 25, 0x73F, 0, 0, "RPM", 0, 0,
      {{0xFF, 0xFF, 0xC0}, {0x00, 0xFF, 0xFF}, {0x95, 0x64, 0xFF, 0xB0},
       {0x00, 0x70, 0x00}, {0x90, 0x90, 0x00}, {0xAA, 0x00, 0x00}, {0xFF, 0xFF, 0xFF}}
    }
  }
};

/*********************************************************************
*
*       static data, dialog resource
*
**********************************************************************
*/

static const GUI_WIDGET_CREATE_INFO _aDialogColor[] = {
  { WINDOW_CreateIndirect, "",         0,                0,   0, 280, 134 },
  { TEXT_CreateIndirect,     "Red:",   0,                8,  36,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Green:", 0,                8,  60,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Blue:",  0,                8,  84,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Alpha",  0,                8, 108,  70,  20, TEXT_CF_LEFT },
  { SLIDER_CreateIndirect,   NULL,    GUI_ID_SLIDER0,   80,  33, 130,  20 },
  { SLIDER_CreateIndirect,   NULL,    GUI_ID_SLIDER1,   80,  57, 130,  20 },
  { SLIDER_CreateIndirect,   NULL,    GUI_ID_SLIDER2,   80,  81, 130,  20 },
  { SLIDER_CreateIndirect,   NULL,    GUI_ID_SLIDER3,   80, 105, 130,  20 },
  { DROPDOWN_CreateIndirect, NULL,    GUI_ID_USER,     115,   5, 100,  98 }
};

static const GUI_WIDGET_CREATE_INFO _aDialogMark[] = {
  { WINDOW_CreateIndirect,   "",               0,    0,   0, 280, 134 },
  { TEXT_CreateIndirect,     "NumMarkLines:",  0,    8,  12,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "LenMarkLines:",  0,    8,  36,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "PosMarkLines:",  0,    8,  60,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "PenSize:",       0,    8,  84,  70,  20, TEXT_CF_LEFT },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER0,  80,   9, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER1,  80,  33, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER2,  80,  57, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER3,  80,  81, 130,  20 },
  { TEXT_CreateIndirect,     "Active", 0,          240,  12,  30,  20, TEXT_CF_LEFT },
  { CHECKBOX_CreateIndirect, NULL, GUI_ID_USER,    222,  11 }
};

static const GUI_WIDGET_CREATE_INFO _aDialogPitch[] = {
  { WINDOW_CreateIndirect,   "",               0,    0,   0, 280, 134 },
  { TEXT_CreateIndirect,     "NumPitchLines:", 0,    8,  12,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "LenPitchLines:", 0,    8,  36,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "PosPitchLines:", 0,    8,  60,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "PenSize:",       0,    8,  84,  70,  20, TEXT_CF_LEFT },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER0,  80,   9, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER1,  80,  33, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER2,  80,  57, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER3,  80,  81, 130,  20 },
  { TEXT_CreateIndirect,     "Active", 0,          240,  12,  30,  20, TEXT_CF_LEFT },
  { CHECKBOX_CreateIndirect, NULL, GUI_ID_USER,    222,  11 }
};

static const GUI_WIDGET_CREATE_INFO _aDialogArc[] = {
  { WINDOW_CreateIndirect,   "",               0,    0,   0, 280, 134 },
  { TEXT_CreateIndirect,     "ArcArea1:", 0,         8,  12,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "ArcArea2:", 0,         8,  36,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "ArcWidth:", 0,         8,  60,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "ArcPos:",   0,         8,  84,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "PenSize:",  0,         8, 108,  70,  20, TEXT_CF_LEFT },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER0,  80,   9, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER1,  80,  33, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER2,  80,  57, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER3,  80,  81, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER4,  80, 105, 130,  20 },
  { TEXT_CreateIndirect,     "Area1", 0,           240,  12,  30,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Area2", 0,           240,  36,  30,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Area3", 0,           240,  60,  30,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Arc1",  0,           240,  84,  30,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Arc2",  0,           240, 108,  30,  20, TEXT_CF_LEFT },
  { CHECKBOX_CreateIndirect, NULL, GUI_ID_USER+0,  222,  11 },
  { CHECKBOX_CreateIndirect, NULL, GUI_ID_USER+1,  222,  35 },
  { CHECKBOX_CreateIndirect, NULL, GUI_ID_USER+2,  222,  59 },
  { CHECKBOX_CreateIndirect, NULL, GUI_ID_USER+3,  222,  83 },
  { CHECKBOX_CreateIndirect, NULL, GUI_ID_USER+4,  222, 107 }
};

static const GUI_WIDGET_CREATE_INFO _aDialogGrad[] = {
  { WINDOW_CreateIndirect,   "",             0,      0,   0, 280, 134 },
  { TEXT_CreateIndirect,     "GradDistance:",0,      8,  12,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "StepWidth:",   0,      8,  36,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "StartNumber:", 0,      8,  60,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Exponent:",    0,      8,  84,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "TextDistance:",0,      8, 108,  70,  20, TEXT_CF_LEFT },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER0,  80,   9, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER1,  80,  33, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER2,  80,  57, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER3,  80,  81, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER4,  80, 105, 130,  20 },
  { TEXT_CreateIndirect,     "Active", 0,          240,  12,  30,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Text", 0,            240, 108,  30,  20, TEXT_CF_LEFT },
  { CHECKBOX_CreateIndirect, NULL, GUI_ID_USER+0,  222,  11 },
  { CHECKBOX_CreateIndirect, NULL, GUI_ID_USER+1,  222, 107 }
};

static const GUI_WIDGET_CREATE_INFO _aDialogScale[] = {
  { WINDOW_CreateIndirect,   "",            0,       0,   0, 280, 134 },
  { TEXT_CreateIndirect,     "ArcStart:",   0,       8,  12,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "ArcEnd:",     0,       8,  36,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "ArcRadius:",  0,       8,  60,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "X-Position:", 0,       8,  84,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Y-Position:", 0,       8, 108,  70,  20, TEXT_CF_LEFT },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER0,  80,   9, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER1,  80,  33, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER2,  80,  57, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER3,  80,  81, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER4,  80, 105, 130,  20 },
  { TEXT_CreateIndirect,     "Active", 0,          240,  12,  30,  20, TEXT_CF_LEFT },
  { CHECKBOX_CreateIndirect, NULL, GUI_ID_USER,    222,  11 }
};

static const GUI_WIDGET_CREATE_INFO _aDialogMisc[] = {
  { WINDOW_CreateIndirect,   "",              0,     0,   0, 280, 134 },
  { TEXT_CreateIndirect,     "NeedleShape:",  0,     8,  12,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "NeedleRadius:", 0,     8,  36,  70,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "AxisRadius:",   0,     8,  60,  70,  20, TEXT_CF_LEFT },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER0,  80,   9, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER1,  80,  33, 130,  20 },
  { SLIDER_CreateIndirect,   NULL, GUI_ID_SLIDER2,  80,  57, 130,  20 },
  { TEXT_CreateIndirect,     "Frame", 0,           240,  12,  30,  20, TEXT_CF_LEFT },
  { TEXT_CreateIndirect,     "Line",  0,           240,  36,  30,  20, TEXT_CF_LEFT },
  { CHECKBOX_CreateIndirect, NULL, GUI_ID_USER+0,  222,  11 },
  { CHECKBOX_CreateIndirect, NULL, GUI_ID_USER+1,  222,  35 }
};

/*********************************************************************
*
*       static data, bitmaps
*
**********************************************************************
*/
/*********************************************************************
*
*       NavigationMap
*/
static const GUI_COLOR ColorsMap[] = {
     0xEEEEEE,0x99CCFF,0xCCFFCC,0xFFFFFF
    ,0xCCCCCC,0x0000FF,0x888888,0x000000
    ,0x33FFFF,0x444444,0xDDDDDD,0xBBBBBB
    ,0x99CC99,0x777777,0xAAAAAA,0x555555
    ,0x00FFFF,0x666666,0x999999,0x660000
    ,0x669966,0xCCCCFF,0xFF0066,0xCC0033
    ,0xCCFFFF,0x9999FF,0x6666FF,0x99FFFF
};

static const GUI_LOGPALETTE PalMap = {
  28,	/* number of entries */
  0, 	/* No transparency */
  &ColorsMap[0]
};


/*********************************************************************
*
*       static data, needle polygons
*
**********************************************************************
*/

static const GUI_POINT _aNeedleSrc[5][5] = {
  {
    { FACTOR *  0, FACTOR *  -5},
    { FACTOR * -5, FACTOR *  25},
    { FACTOR *  0, FACTOR * 100},
    { FACTOR *  5, FACTOR *  25}
  },{
    { FACTOR * -4, FACTOR *   0},
    { FACTOR * -3, FACTOR *  60},
    { FACTOR *  0, FACTOR * 100},
    { FACTOR *  3, FACTOR *  60},
    { FACTOR *  4, FACTOR *   0}
  },{
    { FACTOR * -3, FACTOR * -13},
    { FACTOR * -3, FACTOR *  60},
    { FACTOR *  0, FACTOR * 100},
    { FACTOR *  3, FACTOR *  60},
    { FACTOR *  3, FACTOR * -13}
  },{
    { FACTOR * -5, FACTOR * -13},
    { FACTOR * -4, FACTOR *  20},
    { FACTOR *  0, FACTOR * 100},
    { FACTOR *  4, FACTOR *  20},
    { FACTOR *  5, FACTOR * -13}
  },{
    { FACTOR * -5, FACTOR * -13},
    { FACTOR * -4, FACTOR *  65},
    { FACTOR *  0, FACTOR * 100},
    { FACTOR *  4, FACTOR *  65},
    { FACTOR *  5, FACTOR * -13}
  }
};

static GUI_POINT _aNeedle[5];

/*********************************************************************
*
*       static code, helper functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _CreateButton
*/
static void _CreateButton(const char* pText, int x, int y, int w, int h, WM_HWIN hParent, int Id,const GUI_FONT GUI_UNI_PTR *pFon,const GUI_BITMAP *pbmMap ) {
  WM_HWIN hBut;
  hBut = BUTTON_CreateAsChild(x, y, w, h, hParent, Id, WM_CF_SHOW);
  BUTTON_SetText(hBut, pText);
  BUTTON_SetFont(hBut, pFon);
  if(pbmMap)
    BUTTON_SetBitmap(hBut, 0, pbmMap);


}

/*********************************************************************
*
*       _SetCheckbox
*/
static void _SetCheckbox(WM_HWIN hWin, int Id, int State) {
  WM_HWIN hItem;
  hItem = WM_GetDialogItem(hWin, Id);
  if (State) {
    CHECKBOX_Check(hItem);
  } else {
    CHECKBOX_Uncheck(hItem);
  }
}

/*********************************************************************
*
*       _SetSlider
*/
static void _SetSlider(WM_HWIN hWin, int Id, int Min, int Max, int Value) {
  WM_HWIN hItem;
  hItem = WM_GetDialogItem(hWin, Id);
  SLIDER_SetRange(hItem, Min, Max);
  SLIDER_SetValue(hItem, Value);
}

/*********************************************************************
*
*       _SetDialogColor
*/
static void _SetDialogColor(int Scale) {
  int ColorIndex, i;
  ColorIndex = DROPDOWN_GetSel(_hDropDownColor);
  _InitDialog = 1;
  for (i = 0; i < 4; i++) {
    _SetSlider(_hDialogColor, GUI_ID_SLIDER0 + i, 0, 255, _Scale[Scale].Color[ColorIndex].Sep[i]);
  }
  _InitDialog = 0;
}

/*********************************************************************
*
*       _SetDialogMark
*/
static void _SetDialogMark(int Scale) {
  _InitDialog = 1;
  _SetSlider(_hDialogMark, GUI_ID_SLIDER0, 0, 25, _Scale[Scale].NumMarkLines);
  _SetSlider(_hDialogMark, GUI_ID_SLIDER1, 0, 40, _Scale[Scale].LineLen1);
  _SetSlider(_hDialogMark, GUI_ID_SLIDER2, 0, 50, _Scale[Scale].LinePos1);
  _SetSlider(_hDialogMark, GUI_ID_SLIDER3, 1,  5, _Scale[Scale].PenSize1);
  _SetCheckbox(_hDialogMark, GUI_ID_USER, _Scale[Scale].Flags & (1 << FLAG_SHOW_MARK));
  _InitDialog = 0;
}

/*********************************************************************
*
*       _SetDialogPitch
*/
static void _SetDialogPitch(int Scale) {
  _InitDialog = 1;
  _SetSlider(_hDialogPitch, GUI_ID_SLIDER0, 0, 25, _Scale[Scale].NumPitchLines);
  _SetSlider(_hDialogPitch, GUI_ID_SLIDER1, 0, 40, _Scale[Scale].LineLen2);
  _SetSlider(_hDialogPitch, GUI_ID_SLIDER2, 0, 50, _Scale[Scale].LinePos2);
  _SetSlider(_hDialogPitch, GUI_ID_SLIDER3, 1,  5, _Scale[Scale].PenSize2);
  _SetCheckbox(_hDialogPitch, GUI_ID_USER, _Scale[Scale].Flags & (1 << FLAG_SHOW_PITCH));
  _InitDialog = 0;
}

/*********************************************************************
*
*       _SetDialogArc
*/
static void _SetDialogArc(int Scale) {
  int i;
  _InitDialog = 1;
  _SetSlider(_hDialogArc, GUI_ID_SLIDER0, 0, 359, _Scale[Scale].ArcArea1);
  _SetSlider(_hDialogArc, GUI_ID_SLIDER1, 0, 359, _Scale[Scale].ArcArea2);
  _SetSlider(_hDialogArc, GUI_ID_SLIDER2, 0,  40, _Scale[Scale].ArcWidth);
  _SetSlider(_hDialogArc, GUI_ID_SLIDER3, 0,  50, _Scale[Scale].ArcPos);
  _SetSlider(_hDialogArc, GUI_ID_SLIDER4, 1,   5, _Scale[Scale].PenSize3);
  for (i = 0; i < 5; i++) {
    _SetCheckbox(_hDialogArc, GUI_ID_USER + i, _Scale[Scale].Flags & (1 << (FLAG_SHOW_ARC+i)));
  }
  _InitDialog = 0;
}

/*********************************************************************
*
*       _SetDialogGrad
*/
static void _SetDialogGrad(int Scale) {
  _InitDialog = 1;
  _SetSlider(_hDialogGrad, GUI_ID_SLIDER0,   0, 80, _Scale[Scale].GradDist);
  _SetSlider(_hDialogGrad, GUI_ID_SLIDER1,   1, 20, _Scale[Scale].NumStep);
  _SetSlider(_hDialogGrad, GUI_ID_SLIDER2,   0,  9, _Scale[Scale].NumStart);
  _SetSlider(_hDialogGrad, GUI_ID_SLIDER3,   0,  3, _Scale[Scale].NumExp);
  _SetSlider(_hDialogGrad, GUI_ID_SLIDER4, -90, 90, _Scale[Scale].TextDist);
  _SetCheckbox(_hDialogGrad, GUI_ID_USER + 0, _Scale[Scale].Flags & (1 << FLAG_SHOW_GRAD));
  _SetCheckbox(_hDialogGrad, GUI_ID_USER + 1, _Scale[Scale].Flags & (1 << FLAG_SHOW_TEXT));
  _InitDialog = 0;
}

/*********************************************************************
*
*       _SetDialogScale
*/
static void _SetDialogScale(int Scale) {
  _InitDialog = 1;
  _SetSlider(_hDialogScale, GUI_ID_SLIDER0,  0, 360, _Scale[Scale].ArcStart);
  _SetSlider(_hDialogScale, GUI_ID_SLIDER1,  0, 360, _Scale[Scale].ArcEnd);
  _SetSlider(_hDialogScale, GUI_ID_SLIDER2, 35, 160, _Scale[Scale].ArcRadius);
  _SetSlider(_hDialogScale, GUI_ID_SLIDER3,  0, 639, _Scale[Scale].x);
  _SetSlider(_hDialogScale, GUI_ID_SLIDER4,  0, 479, _Scale[Scale].y);
  _SetCheckbox(_hDialogScale, GUI_ID_USER, _Scale[Scale].Flags & (1 << FLAG_SHOW_SCALE));
  _InitDialog = 0;
}

/*********************************************************************
*
*       _SetDialogMisc
*/
static void _SetDialogMisc(int Scale) {
  _InitDialog = 1;
  _SetSlider(_hDialogMisc, GUI_ID_SLIDER0,  0,   4, _Scale[Scale].NeedleType);
  _SetSlider(_hDialogMisc, GUI_ID_SLIDER1, 50, 100, _Scale[Scale].NeedleRadius);
  _SetSlider(_hDialogMisc, GUI_ID_SLIDER2, 10,  60, _Scale[Scale].AxisRadius);
  _SetCheckbox(_hDialogMisc, GUI_ID_USER + 0, _Scale[Scale].Flags & (1 << FLAG_NEEDLE_FRAME));
  _SetCheckbox(_hDialogMisc, GUI_ID_USER + 1, _Scale[Scale].Flags & (1 << FLAG_NEEDLE_LINE));
  _InitDialog = 0;
}

/*********************************************************************
*
*       _SetDialogs
*/
static void _SetDialogs(int Scale) {
  _SetDialogColor(Scale);
  _SetDialogMark(Scale);
  _SetDialogPitch(Scale);
  _SetDialogArc(Scale);
  _SetDialogGrad(Scale);
  _SetDialogScale(Scale);
  _SetDialogMisc(Scale);
}

/*********************************************************************
*
*       _GetArcLen
*/
static int _GetArcLen(const SCALE* pObj) {
  if (pObj->ArcStart > pObj->ArcEnd) {
    return 360 - (pObj->ArcStart - pObj->ArcEnd);
  } else {
    return pObj->ArcEnd - pObj->ArcStart;
  }
}

/*********************************************************************
*
*       _MagnifyPolygon
*/
static void _MagnifyPolygon(GUI_POINT* pDest, const GUI_POINT* pSrc, int NumPoints, float Mag) {
  int i;
  for (i=0; i<NumPoints; i++) {
    (pDest+i)->x = (pSrc+i)->x * Mag;
    (pDest+i)->y = (pSrc+i)->y * Mag;
  }
}

/*********************************************************************
*
*       _CalcNeedle
*/
static int _CalcNeedle(const SCALE* pObj, int Index, int Radius) {
  int NumPoints, Shape;
  float Angel;
  Shape = pObj->NeedleType;
  NumPoints = GUI_COUNTOF(_aNeedleSrc[Shape]);
  Angel = -((pObj->ArcStart * PI) + (_GetArcLen(pObj) * _Needle[Index].NeedlePos * PI) / NEEDLE_GRAD) / 180;
  _MagnifyPolygon(_aNeedle, _aNeedleSrc[Shape], NumPoints, Radius / 100.);
  GUI_RotatePolygon(_aNeedle, _aNeedle, NumPoints, Angel);
  return NumPoints;
}

/*********************************************************************
*
*       _CalcPointX
*/
static int _CalcPointX(int r, int Angel) {
  return r * cos((Angel - 90) * PI / 180.);
}

/*********************************************************************
*
*       _CalcPointY
*/
static int _CalcPointY(int r, int Angel) {
  return r * sin((Angel - 90) * PI / 180.);
}

/*********************************************************************
*
*       _Max
*/
static int _Max(int a, int b) {
  return((a > b) ? a : b);
}

/*********************************************************************
*
*       _Max3
*/
static int _Max3(int a, int b, int c) {
  int r;
  r = (a > b) ? a : b;
  r = (r > c) ? r : c;
  return r;
}

/*********************************************************************
*
*       _Min
*/
static int _Min(int a, int b) {
  return((a < b) ? a : b);
}

/*********************************************************************
*
*       _GetNeedleRect
*/
static void _GetNeedleRect(const SCALE* pObj, int Index, GUI_RECT* pRect) {
  int NumPoints, i;
  int x, y, r;
  int x0 =  4096, y0 =  4096;
  int x1 = -4096, y1 = -4096;
  r = (pObj->ArcRadius + pObj->PenSize3) * pObj->NeedleRadius / 100;
  NumPoints = _CalcNeedle(pObj, Index, r);
  for (i = 0; i < NumPoints; i++) {
    x = _aNeedle[i].x / FACTOR;
    y = _aNeedle[i].y / FACTOR;
    x0 = _Min(x0, x);
    y0 = _Min(y0, y);
    x1 = _Max(x1, x);
    y1 = _Max(y1, y);
  }
  pRect->x0 = pObj->x0 + x0 - 1;
  pRect->y0 = pObj->y0 + y0 - 1;
  pRect->x1 = pObj->x0 + x1 + 1;
  pRect->y1 = pObj->y0 + y1 + 1;
}

/*********************************************************************
*
*       _MergeRects
*/
static void _MergeRects(GUI_RECT* pR1, const GUI_RECT* pR2) {
  pR1->x0 = _Min(pR1->x0, pR2->x0);
  pR1->y0 = _Min(pR1->y0, pR2->y0);
  pR1->x1 = _Max(pR1->x1, pR2->x1);
  pR1->y1 = _Max(pR1->y1, pR2->y1);
}

/*********************************************************************
*
*       _MoveMap
*/
static int FirstTime=0;
static void _MoveMap(WM_HWIN hMap) {
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
    
    WM_InvalidateWindow(hMap);
    FirstTime = Time;
  }

}


/*********************************************************************
*
*       _MoveNeedle
*/
static void _MoveNeedle(NEEDLE* pObj, int Index) {
  if (pObj->NeedleUPM) {
    GUI_RECT rOld, rNew;
    int Dif, Time;
    _GetNeedleRect(&_Scale[Index], Index, &rOld);
    Time = GUI_GetTime();
    Dif = (Time - pObj->NeedlePrevTime) / (60000 / pObj->NeedleUPM / NEEDLE_GRAD);
    if (Dif != 0) {
      pObj->NeedlePos += (Dif * pObj->NeedleDir);
      if (pObj->NeedlePos > NEEDLE_GRAD) {
        pObj->NeedlePos = NEEDLE_GRAD;
        pObj->NeedleDir = -pObj->NeedleDir;
      } else {
        if (pObj->NeedlePos < 0) {
          pObj->NeedlePos = 0;
          pObj->NeedleDir = -pObj->NeedleDir;
        }
      }
      _GetNeedleRect(&_Scale[Index], Index, &rNew);
      _MergeRects(&rNew, &rOld);
      WM_InvalidateRect(_Scale[Index].hWin, &rNew);
      pObj->NeedlePrevTime = Time;
    }
  }
}

/*********************************************************************
*
*       _UpdateScale
*/
static void _UpdateScale(int Index) {
  int Mod = 0;
  if (_Scale[Index].hMemDev == 0) {
    GUI_RECT r;
    WM_GetWindowRect(&r);
    _Scale[Index].hMemDev = GUI_MEMDEV_CreateEx(r.x0, r.y0, r.x1 - r.x0 + 1, r.y1 - r.y0 + 1, GUI_MEMDEV_HASTRANS);
    Mod = 1;
  }
  if (Mod | memcmp(&_ScalePrev[Index], &_Scale[Index], sizeof(SCALE))) {
    GUI_MEMDEV_Handle hPrev = GUI_MEMDEV_Select(_Scale[Index].hMemDev);
    GUI_SetBkColor(GUI_BLACK);
    GUI_MEMDEV_Write(_hBkMemDev);
    GUI_MEMDEV_Clear(_Scale[Index].hMemDev);
    _DrawScale(&_Scale[Index]);
    memcpy(&_ScalePrev[Index], &_Scale[Index], sizeof(SCALE));
    GUI_MEMDEV_Select(hPrev);
  }
}

/*********************************************************************
*
*       _InvalidateScale
*/
static void _InvalidateScale(SCALE* pObj) {
  if (pObj->hMemDev) {
    GUI_MEMDEV_Delete(pObj->hMemDev);
    pObj->hMemDev = 0;
  }
}

/*********************************************************************
*
*       _ChangeScaleSize

  This function optimize the window rect
*/
static void _ChangeScaleSize(SCALE* pObj) {
  int x0, y0, x, y, w, h, ArcStart, ArcEnd, rOff;
  int PenSize, r, rNeedle, BitmapW, BitmapY0, BitmapY1;
  int TextW, TextY0, TextY1;
  /* Calculate text position */
  GUI_SetFont(&GUI_Font8x8);
  if (pObj->Flags & (1 << FLAG_SHOW_TEXT)) {
    TextW  = GUI_GetStringDistX(pObj->acText) / 2;
    TextY0 = (pObj->ArcRadius * pObj->TextDist / 100);
    TextY1 = TextY0 + 8;
    if (pObj->TextDist < 0) {
      TextY0 = -TextY0;
      TextY1 = _Max(TextY1, 0);
    } else {
      TextY0 = _Max(-TextY0, 0);
    }
  } else {
    TextW  = 0;
    TextY0 = 0;
    TextY1 = 0;
  }
  /* Calculate radius of scale */
  PenSize  = _Max3(pObj->PenSize1, pObj->PenSize2, pObj->PenSize3);
  r        = pObj->ArcRadius + PenSize;
  /* Calculate radius of needle */
  rNeedle  = pObj->ArcRadius * pObj->AxisRadius / 200;
  if (pObj->NeedleType > 1) {
    rNeedle  = _Max(rNeedle, (pObj->NeedleRadius * 22) / 100 + 1);
  }
  /* Get arcbow */
  ArcStart = pObj->ArcStart;
  ArcEnd   = pObj->ArcEnd;
  /* Calculate bitmap position */
  if (pObj->pBitmap) {
    BitmapW  = (pObj->pBitmap->XSize / 2) + 1;
    BitmapY0 = -(pObj->pBitmap->YSize / 2) + (pObj->ArcRadius / pObj->BitmapY);
    BitmapY1 = BitmapY0 + pObj->pBitmap->YSize;
    if (pObj->BitmapY < 0) {
      BitmapY0 = -BitmapY0;
      BitmapY1 = _Max(BitmapY1, 0);
    } else {
      BitmapY0 = _Max(-BitmapY0, 0);
    }
  } else {
    BitmapW  = 0;
    BitmapY0 = 0;
    BitmapY1 = 0;
  }
  /* Calculate window height*/
  if (ArcStart >= ArcEnd) {
    h = r * 2 + 2;
  } else {
    rOff = _Max3(rNeedle, TextY1, BitmapY1);
    h = _Max(r + rOff, r - _CalcPointY(pObj->ArcRadius, ArcStart) + PenSize + 2);
    h = _Max(h,        r - _CalcPointY(pObj->ArcRadius, ArcEnd)   + PenSize + 2);
  }
  /* Calculate window width */
  if (ArcStart < 270 && (ArcEnd > 270 || ArcEnd <= ArcStart)) {
    w = r * 2 + 2;
  } else {
    rOff = _Max3(rNeedle, TextW, BitmapW);
    w = _Max(r + rOff, r - _CalcPointX(pObj->ArcRadius, ArcStart) + PenSize + 2);
    w = _Max(w,        r - _CalcPointX(pObj->ArcRadius, ArcEnd)   + PenSize + 2);
  }
  /* Calculate y-position of window */
  if (((ArcStart < ArcEnd) && ((ArcStart > 180 && ArcEnd > 180) || (ArcStart < 180 && ArcEnd < 180))) ||
       (ArcStart > 180 && ArcEnd < 180)) {
    rOff = _Max3(rNeedle, TextY0, BitmapY0);
    y = _Min(r - rOff, r - _CalcPointY(pObj->ArcRadius, ArcStart) - PenSize - 2);
    y = _Min(y,        r - _CalcPointY(pObj->ArcRadius, ArcEnd)   - PenSize - 2);
    y0        = pObj->y - r + y;
    pObj->y0  = r - y;
    h        -= y;
  } else {
    y0        = pObj->y - r - 2;
    pObj->y0  = r + 2;
    h        += 2;
  }
  /* Calculate x-position of window */
  if (((ArcStart < ArcEnd) && ((ArcStart > 90 && ArcEnd > 90) || (ArcStart < 90 && ArcEnd < 90))) ||
       (ArcStart > 90 && ArcEnd < 90)) {
    rOff = _Max3(rNeedle, TextW, BitmapW);
    x = _Min(r - rOff, r - _CalcPointX(pObj->ArcRadius, ArcStart) - PenSize - 2);
    x = _Min(x,        r - _CalcPointX(pObj->ArcRadius, ArcEnd)   - PenSize - 2);
    x0        = pObj->x - r + x;
    pObj->x0  = r - x;
    w        -= x;
  } else {
    x0        = pObj->x - r - 2;
    pObj->x0  = r + 2;
    w        += 2;
  }
  /* Set new window rect */
  _InvalidateScale(pObj);
  WM_MoveTo(pObj->hWin, x0, y0);
  WM_SetSize(pObj->hWin, w, h);
  WM_InvalidateWindow(pObj->hWin);
}

/*********************************************************************
*
*       _SetPreset
*/
static void _SetPreset(int Preset, int Scale) {
  if (Preset >= 0 && Preset <= 3) {
    WM_HWIN OldhWin;
    int i, iStart, iEnd;
    iStart = (Scale == -1) ? 0 : Scale;
    iEnd   = (Scale == -1) ? 3 : Scale;
    for (i = iStart; i <= iEnd; i++) {
      _InvalidateScale(&_Scale[i]);
      /* Save old values which should not change */
      OldhWin = _Scale[i].hWin;
      /* Copy the preset */
      _Scale[i] = _Presets[Preset][i];
      /* Restore unchanged values */
      _Scale[i].hWin = OldhWin;
      /* Recalculate the scalewin */
      _ChangeScaleSize(&_Scale[i]);
    }
    _SetDialogs((Scale != -1) ? Scale : 0);
  }
}

/*********************************************************************
*
*       _CalcColor
*/
static GUI_COLOR _CalcColor(const COLOR* pColor) {
  GUI_COLOR r;
  r  = ((U32) pColor->Sep[0]);
  r += ((U32) pColor->Sep[1]) << 8;
  r += ((U32) pColor->Sep[2]) << 16;
  return r;
}

/*********************************************************************
*
*       _GetSliderValue
*/
static int _GetSliderValue(WM_HWIN hDlg, int Id) {
  return SLIDER_GetValue(WM_GetDialogItem(hDlg, Id));
}

/*********************************************************************
*
*       _AddDialog
*/
static WM_HWIN _AddDialog(const char* pText, const GUI_WIDGET_CREATE_INFO* pDialog, int NumItems,
                          WM_CALLBACK* cb, WM_HWIN hMultiPage) {
  WM_HWIN hWin;
  hWin = GUI_CreateDialogBox(pDialog, NumItems, cb, WM_GetClientWindow(hMultiPage), 0, 0);
  MULTIPAGE_AddPage(hMultiPage, 0, pText);
  return hWin;
}

/*********************************************************************
*
*       _IntToString
*/
static void _IntToString(char* pStr, int Value) {
  char* Ptr = pStr + 6;
  *(--Ptr) = 0;
  Value = _Min(Value, 99999);
  do {
    *(--Ptr) = (Value % 10) + '0';
    Value /= 10;
  } while (Value != 0);
  strcpy(pStr, Ptr);
}

/*********************************************************************
*
*       _SetClipRect
*/
static const GUI_RECT* _SetClipRect(GUI_RECT* pRect,int x0, int y0, int x1, int y1) {
  pRect->x0 = x0;
  pRect->y0 = y0;
  pRect->x1 = x1;
  pRect->y1 = y1;
  return WM_SetUserClipRect(pRect);
}

/*********************************************************************
*
*       static code, drawing functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _AA_DrawPolygon
*/
static void _AA_DrawPolygon(const GUI_POINT* pSrc, int NumPoints, int x0, int y0) {
  int i, x, y, xPrev, yPrev;
  U8 OldPenShape;
  xPrev = x0 + (pSrc+NumPoints-1)->x;
  yPrev = y0 + (pSrc+NumPoints-1)->y;
  OldPenShape = GUI_SetPenShape(GUI_PS_FLAT);
  for (i = 0; i < NumPoints; i++) {
    x = x0 + (pSrc+i)->x;
    y = y0 + (pSrc+i)->y;
    GUI_AA_DrawLine(xPrev, yPrev, x, y);
    xPrev = x;
    yPrev = y;
  }
  GUI_SetPenShape(OldPenShape);
}

/*********************************************************************
*
*       _DrawLine
*/
static void _DrawLine(const SCALE* pObj, int r1, int r2, float Angel) {
  float co = cos(Angel / 180.) * FACTOR;
  float si = sin(Angel / 180.) * FACTOR;
  int x0 = pObj->x0 * FACTOR - r1 * co;
  int y0 = pObj->y0 * FACTOR - r1 * si;
  int x1 = pObj->x0 * FACTOR - r2 * co;
  int y1 = pObj->y0 * FACTOR - r2 * si;
  GUI_AA_DrawLine(x0, y0, x1, y1);
}

/*********************************************************************
*
*       _DrawLines
*/
static void _DrawLines(const SCALE* pObj, int iEnd, int rStart, int rEnd) {
  int i, ArcLen = _GetArcLen(pObj);
  float Angel;
  GUI_SetColor(_CalcColor(&pObj->Color[0]));
  HIRES_ON();
  for (i = 0; i <= iEnd; i++) {
    Angel = (i * ArcLen * PI) / _Max(iEnd, 1) + (pObj->ArcStart - 90.) * PI;
    _DrawLine(pObj, rStart, rEnd, Angel);
  }
  HIRES_OFF();
}

/*********************************************************************
*
*       _DrawPitchLines
*/
static void _DrawPitchLines(const SCALE* pObj) {
  if (pObj->NumPitchLines > 0 && (pObj->Flags & (1 << FLAG_SHOW_PITCH))) {
    int iEnd, rStart, rEnd;
    iEnd = _Max(pObj->NumMarkLines - 1, 1) * (pObj->NumPitchLines + 1);
    rStart = pObj->ArcRadius - pObj->LinePos2;
    rEnd = rStart - pObj->LineLen2;
    GUI_SetPenSize(pObj->PenSize2);
    _DrawLines(pObj, iEnd, rStart, rEnd);
  }
}

/*********************************************************************
*
*       _DrawMarkLines
*/
static void _DrawMarkLines(const SCALE* pObj) {
  if (pObj->NumMarkLines > 0 && (pObj->Flags & (1 << FLAG_SHOW_MARK))) {
    int iEnd, rStart, rEnd;
    iEnd = pObj->NumMarkLines - 1;
    rStart = pObj->ArcRadius - pObj->LinePos1;
    rEnd = rStart - pObj->LineLen1;
    GUI_SetPenSize(pObj->PenSize1);
    _DrawLines(pObj, iEnd, rStart, rEnd);
  }
}

/*********************************************************************
*
*       _DrawGrad
*/
static void _DrawGrad(const SCALE* pObj) {
  if (pObj->NumMarkLines > 0 && (pObj->Flags & (1 << FLAG_SHOW_GRAD))) {
    int xt, yt, w, h, i, ArcLen, rStart, Cnt, wMax = 0;
    float co, si, Angel;
    char acText[6];
    GUI_SetColor(_CalcColor(&pObj->Color[1]));
    GUI_SetFont(&GUI_Font6x8);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    Cnt = pObj->NumStart * (pObj->NumStep * _Pow10[pObj->NumExp]);
    for (i = 0; i < pObj->NumMarkLines; i++) {
      _IntToString(acText, Cnt);
      wMax = _Max(GUI_GetStringDistX(acText), wMax);
      Cnt += pObj->NumStep * _Pow10[pObj->NumExp];
    }
    ArcLen = _GetArcLen(pObj);
    rStart = pObj->ArcRadius - pObj->GradDist - (wMax / 2);
    Cnt = pObj->NumStart * (pObj->NumStep * _Pow10[pObj->NumExp]);
    for (i = 0; i < pObj->NumMarkLines; i++) {
      Angel = (i * ArcLen * PI) / _Max(pObj->NumMarkLines - 1, 1) + (pObj->ArcStart - 90.) * PI;
      co = cos(Angel / 180.);
      si = sin(Angel / 180.);
      xt = pObj->x0 - rStart * co;
      yt = pObj->y0 - rStart * si;
      _IntToString(acText, Cnt);
      w = GUI_GetStringDistX(acText);
      h = si * _Max(wMax / 2 - 4, 0) + 4;
      GUI_DispStringAt(acText, xt - w / 2 + 1, yt - h + 1);
      Cnt += pObj->NumStep * _Pow10[pObj->NumExp];
    }
  }
}

/*********************************************************************
*
*       _DrawText
*/
static void _DrawText(const SCALE* pObj) {
  if (pObj->Flags & (1 << FLAG_SHOW_TEXT)) {
    int x, y;
    GUI_SetColor(_CalcColor(&pObj->Color[1]));
    GUI_SetFont(&GUI_Font8x8);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    x = pObj->x0;
    y = pObj->y0 + (pObj->ArcRadius * pObj->TextDist / 100);
    GUI_DispStringHCenterAt(pObj->acText, x, y);
  }
}

/*********************************************************************
*
*       _DrawColorArcs
*/
static void _DrawColorArcs(const SCALE* pObj) {
  int r = pObj->ArcRadius - (pObj->ArcWidth / 2) - pObj->ArcPos;
  int Start, Angel1, Angel2, ArcLen;
  ArcLen = _GetArcLen(pObj);
  Start = (pObj->ArcStart < pObj->ArcEnd) ? 270 - pObj->ArcStart : 630 - pObj->ArcStart;
  Angel1 = pObj->ArcArea1 * ArcLen / 359;
  Angel2 = pObj->ArcArea2 * ArcLen / 359;
  GUI_SetPenSize(pObj->ArcWidth);
  if (pObj->Flags & (1 << (FLAG_SHOW_ARC+0))) {
    GUI_SetColor(_CalcColor(&pObj->Color[3]));
    GUI_AA_DrawArc(pObj->x0, pObj->y0, r, r, Start - Angel1, Start);
  }
  if (pObj->Flags & (1 << (FLAG_SHOW_ARC+1))) {
    GUI_SetColor(_CalcColor(&pObj->Color[4]));
    GUI_AA_DrawArc(pObj->x0, pObj->y0, r, r, Start - Angel2, Start - Angel1);
  }
  if (pObj->Flags & (1 << (FLAG_SHOW_ARC+2))) {
    GUI_SetColor(_CalcColor(&pObj->Color[5]));
    GUI_AA_DrawArc(pObj->x0, pObj->y0, r, r, Start - ArcLen, Start - Angel2);
  }
}

/*********************************************************************
*
*       _DrawArcs
*/
static void _DrawArcs(const SCALE* pObj) {
  int Start, End, r1, r2;
  Start = (pObj->ArcStart < pObj->ArcEnd) ? 270 - pObj->ArcStart : 630 - pObj->ArcStart;
  End   = 270 - pObj->ArcEnd;
  r1 = pObj->ArcRadius;
  r2 = pObj->ArcRadius - pObj->ArcPos - pObj->ArcWidth;
  GUI_SetColor(_CalcColor(&pObj->Color[0]));
  GUI_SetPenSize(pObj->PenSize3);
  if (pObj->Flags & (1 << (FLAG_SHOW_ARC+3))) {
    GUI_AA_DrawArc(pObj->x0, pObj->y0, r1, r1, End, Start);
  }
  if (pObj->Flags & (1 << (FLAG_SHOW_ARC+4))) {
    GUI_AA_DrawArc(pObj->x0, pObj->y0, r2, r2, End, Start);
    if (pObj->Flags & (1 << (FLAG_SHOW_ARC+3))) {
      GUI_SetPenSize(pObj->PenSize1);
      HIRES_ON();
      _DrawLine(pObj, r1, r2, (pObj->ArcStart - 90.) * PI);
      _DrawLine(pObj, r1, r2, (pObj->ArcEnd   - 90.) * PI);
      HIRES_OFF();
    }
  }
}

/*********************************************************************
*
*       _DrawNeedleThread
*/
static void _DrawNeedleThread(const SCALE* pObj, int Index) {
  int x0, y0, x1, y1;
  int NumPoints, Radius;
  Radius = (pObj->ArcRadius + pObj->PenSize3) * pObj->NeedleRadius / 100;
  GUI_SetPenSize(3);
  NumPoints = _CalcNeedle(pObj, Index, Radius - 3);
  x0 = pObj->x0 * FACTOR;
  y0 = pObj->y0 * FACTOR;
  x1 = x0 + _aNeedle[NumPoints/2].x;
  y1 = y0 + _aNeedle[NumPoints/2].y;
  if ((NumPoints % 2) != 0) {
    x0 += (_aNeedle[0].x + _aNeedle[NumPoints - 1].x) / 2;
    y0 += (_aNeedle[0].y + _aNeedle[NumPoints - 1].y) / 2;
  }
  GUI_SetColor(GUI_RED);
  GUI_AA_DrawLine(x0, y0, x1, y1);
}

/*********************************************************************
*
*       _DrawNeedleFrame
*/
static void _DrawNeedleFrame(const SCALE* pObj, int Index) {
  int NumPoints, Radius;
  Radius = (pObj->ArcRadius + pObj->PenSize3) * pObj->NeedleRadius / 100;
  GUI_SetPenSize(2);
  NumPoints = _CalcNeedle(pObj, Index, Radius - 2);
  GUI_SetColor(_CalcColor(&pObj->Color[2]));
  _AA_DrawPolygon(_aNeedle, NumPoints, pObj->x0 * FACTOR, pObj->y0 * FACTOR);
}

/*********************************************************************
*
*       _DrawNeedle
*/
static void _DrawNeedle(const SCALE* pObj, int Index) {
  int NumPoints, Radius;
  GUI_MEMDEV_Handle hPrev;
  static GUI_MEMDEV_Handle hMemDev;
  GUI_RECT r;
  #if SHOW_RECTS
    _GetNeedleRect(pObj, Index, &r);
    GUI_SetColor(GUI_BLUE);
    GUI_FillRect(r.x0, r.y0, r.x1, r.y1);
  #endif
  WM_GetWindowRect(&r);
  HIRES_ON();
  hMemDev = GUI_MEMDEV_CreateEx(r.x0, r.y0, r.x1 - r.x0 + 1, r.y1 - r.y0 + 1, GUI_MEMDEV_HASTRANS);
  hPrev = GUI_MEMDEV_Select(hMemDev);
  GUI_MEMDEV_Write(pObj->hMemDev);
  GUI_MEMDEV_Clear(hMemDev);
  Radius = (pObj->ArcRadius + pObj->PenSize3) * pObj->NeedleRadius / 100;
  NumPoints = _CalcNeedle(pObj, Index, Radius);
  GUI_SetColor(_CalcColor(&pObj->Color[2]));
  GUI_AA_SetFactor(AA_FACTOR);
  GUI_AA_FillPolygon(_aNeedle, NumPoints, pObj->x0 * FACTOR, pObj->y0 * FACTOR);
  if (pObj->Flags & (1 << FLAG_NEEDLE_LINE)) {
    _DrawNeedleThread(pObj, Index);
  }
  GUI_MEMDEV_Select(hPrev);
  if (pObj->Color[2].Sep[3] != 0xFF) {
    GUI_MEMDEV_WriteAlpha(hMemDev, pObj->Color[2].Sep[3]);
    if (pObj->Flags & (1 << FLAG_NEEDLE_FRAME)) {
      _DrawNeedleFrame(pObj, Index);
    }
  } else {
    GUI_MEMDEV_Write(hMemDev);
    if ((pObj->Flags & (1 << FLAG_NEEDLE_FRAME)) && (pObj->Flags & (1 << FLAG_NEEDLE_LINE))) {
      _DrawNeedleFrame(pObj, Index);
    }
  }
  GUI_MEMDEV_Delete(hMemDev);
  HIRES_OFF();
  GUI_SetColor(_CalcColor(&pObj->Color[6]));
  GUI_AA_FillCircle(pObj->x0, pObj->y0, pObj->ArcRadius * pObj->AxisRadius / 200);
}

/*********************************************************************
*
*       _DrawScale
*/
static void _DrawScale(SCALE* pObj) {
  #if SHOW_RECTS
    GUI_SetBkColor(GUI_GRAY);
    GUI_Clear();
  #endif
  if (pObj->ArcStart != pObj->ArcEnd) {
    if (pObj->pBitmap) {
      int x = pObj->x0 - (pObj->pBitmap->XSize / 2);
      int y = pObj->y0 - (pObj->pBitmap->YSize / 2) + (pObj->ArcRadius / pObj->BitmapY);
      GUI_DrawBitmap(pObj->pBitmap, x, y);
    }
    GUI_AA_SetFactor(AA_FACTOR);
    GUI_SetPenShape(GUI_PS_ROUND);
    _DrawColorArcs(pObj);
    _DrawArcs(pObj);
    _DrawPitchLines(pObj);
    _DrawMarkLines(pObj);
    _DrawGrad(pObj);
    _DrawText(pObj);
  }
}

/*********************************************************************
*
*       _DrawFrame
*/
static void _DrawFrame(int xDist, int yDist, int rx, int ry, int yEnd,
                       int PenSize, GUI_COLOR Color, GUI_COLOR BkColor) {
  const GUI_RECT* OldClip;
  GUI_RECT ClipRect;
  OldClip = _SetClipRect(&ClipRect, xDist - rx, yDist - ry, xDist, yDist);
  GUI_SetPenSize(10);
  GUI_SetColor(Color);
  GUI_DrawEllipse(xDist, yDist, rx, ry);
  GUI_SetColor(BkColor);
  GUI_FillEllipse(xDist, yDist, rx - PenSize, ry - PenSize);
  _SetClipRect(&ClipRect, 639 - xDist, yDist - ry, 639 - xDist + rx, yDist);
  GUI_SetColor(Color);
  GUI_DrawEllipse(639 - xDist, yDist, rx, ry);
  GUI_SetColor(BkColor);
  GUI_FillEllipse(639 - xDist, yDist, rx - PenSize, ry - PenSize);
  WM_SetUserClipRect(OldClip);
  GUI_SetColor(Color);
  GUI_FillRect(xDist, yDist - ry, 639 - xDist, yDist - ry + PenSize - 1);
  GUI_FillRect(xDist - rx, yEnd - PenSize + 1, 639 - xDist + rx, yEnd);
  GUI_FillRect(xDist - rx, yDist, xDist - rx + PenSize - 1, yEnd);
  GUI_FillRect(639 - xDist + rx - PenSize + 1, yDist, 639 - xDist + rx, yEnd);
}

/*********************************************************************
*
*       _CreateBackGround
*/
static void _CreateBackGround(void) {
  GUI_MEMDEV_Handle hMemPrev;
  GUI_RECT r;
  r.x0 = 0;
  r.x1 = LCD_XSIZE - 1;
  r.y0 = 0;
  r.y1 = LCD_YSIZE - 195;
  _hBkMemDev = GUI_MEMDEV_CreateEx(r.x0, r.y0, r.x1 + 1, r.y1 + 1, GUI_MEMDEV_NOTRANS);
  hMemPrev = GUI_MEMDEV_Select(_hBkMemDev);
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  _DrawFrame(240, 180, 230, 170, 285, 1, 0x444444, GUI_BLACK);
  _DrawFrame(240, 180, 229, 169, 284, 1, 0x808080, GUI_BLACK);
  _DrawFrame(240, 180, 228, 168, 283, 1, 0xA0A0A0, GUI_BLACK);
  _DrawFrame(240, 180, 227, 167, 282, 1, 0xC0C0C0, GUI_BLACK);
  _DrawFrame(240, 180, 226, 166, 281, 1, 0xA0A0A0, GUI_BLACK);
  _DrawFrame(240, 180, 225, 165, 280, 1, 0x808080, GUI_BLACK);
  _DrawFrame(240, 180, 224, 164, 279, 1, 0x444444, GUI_BLACK);
  GUI_MEMDEV_Select(hMemPrev);
}

/*********************************************************************
*
*       static code, window callbacks
*
**********************************************************************
*/

/*********************************************************************
*
*       _cbBkWindow
*/
static void _cbBkWindow(WM_MESSAGE* pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_MEMDEV_Write(_hBkMemDev);
    break;
  case WM_KEY:
    if (((WM_KEY_INFO *)pMsg->Data.p)->Key == GUI_KEY_ESCAPE) {
      _Break = 1;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

void _deletallitem(LISTBOX_Handle hListbox)
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
*       _cbFrameWin
*/
static void _cbFrameWin(WM_MESSAGE* pMsg) {
  int Scale, Id,i,nCount=0;
  char aBuffer[100];
  MyPoint layerpoints[100];
  char *pstr;

  switch (pMsg->MsgId) {

  case WM_NOTIFY_PARENT:
    if (WM_IsWindow(_hDropDownScale)) {
      Scale = DROPDOWN_GetSel(_hDropDownScale) - 1;
    }
    Id    = WM_GetId(pMsg->hWinSrc);         
    switch (pMsg->Data.v) {
    case WM_NOTIFICATION_RELEASED:
		switch(Id)
		{
		case GUI_ID_BUTTON0:
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
		case GUI_ID_BUTTON1:
			break;
		case GUI_ID_BUTTON2:
			ZoomIn(&m_mapCoordMap);
			WM_Invalidate(hWinMap);

			break;
		case GUI_ID_BUTTON3:
			ZoomOut(&m_mapCoordMap);
			WM_Invalidate(hWinMap);
			break;
		default:
			break;
		}

      break;
    case WM_NOTIFICATION_SEL_CHANGED:
      if (pMsg->hWinSrc == _hDropDownScale) {
        _SetDialogs((Scale != -1) ? Scale : 0);
      }
      break;
    }
    break;

  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbWinMap
*/
static void _cbWinMap(WM_MESSAGE* pMsg) {
  int key;
  int x,y;
  int NCode, Id;
  WM_HWIN hItem; 
  
  GUI_RECT eagleRect;
  char * mapfile;
  WM_HWIN hWin = (FRAMEWIN_Handle)(pMsg->hWin);
  WM_HWIN 	hChildWin = WM_GetClientWindow(hWin);
 
  switch (pMsg->MsgId) {
  case WM_CREATE:

		myUcMap.pmapCoorMap=&m_mapCoordMap;
        mapfile=GetMapFileName();
		if(mapfile==NULL) return;
		Map_ReadFromFile(&myUcMap,mapfile);//"E:\\uC-GUI\\Start\\map\\bj.qm2");
		m_mapCoordMap.wScreen=948;//GUI_GetScreenSizeX();
		m_mapCoordMap.hScreen=453;//GUI_GetScreenSizeY();
	    WM_GetWindowRectEx(hWin,&eagleRect); 

		m_mapCoordMap.wScreen=GUI_GetScreenSizeX();
		m_mapCoordMap.hScreen=GUI_GetScreenSizeY();
	    m_mapCoordMap.wScreen=eagleRect.x1-eagleRect.x0;
        m_mapCoordMap.hScreen=eagleRect.y1-eagleRect.y0; 
		m_mapCoordMap.nMapMode=1;
		myUcMap.m_bDrawScale=TRUE;
		m_mapCoordMap.blc = myUcMap.map_blc;  // 设置当前比例尺为地图保存的比例尺
		SetCenterFP(&m_mapCoordMap,myUcMap.map_fCenterX,myUcMap.map_fCenterY);
		SetScreenXY(&m_mapCoordMap);
		myUcMap.puserlayer=&g_userLayer;
		g_userLayer.m_bShowState=TRUE;
		g_userLayer.m_mapCoordMap=&m_mapCoordMap;
	//g_userLayer.m_mapCoordMap=&m_mapCoordMap;

	  break;
	case WM_KEY:
		x = WM_GetWindowOrgX(hChildWin);
		y = WM_GetWindowOrgY(hChildWin);
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
		    WM_InvalidateWindow(hChildWin);
			break;
		case GUI_KEY_UP:
			Up(&m_mapCoordMap);
		    WM_InvalidateWindow(hChildWin);
			break;
		case GUI_KEY_RIGHT:
			Right(&m_mapCoordMap);
  		   WM_InvalidateWindow(hChildWin);
			break;
		case GUI_KEY_DOWN:
			Down(&m_mapCoordMap);
		    WM_InvalidateWindow(hChildWin);
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

  case WM_PAINT:
         GUI_SetAlpha(0);
		 GUI_SetBkColor(GUI_WHITE);
		 GUI_SetColor(GUI_WHITE);
		 GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
		 GUI_Clear();
        myUcMap.pmapCoorMap=&m_mapCoordMap;
		Map_Draw(&myUcMap,1);
	  break;
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
			WM_InvalidateWindow(hChildWin);
			break;
		case GUI_ID_BUTTON1:
			ZoomOut(&m_mapCoordMap);
			WM_InvalidateWindow(hChildWin);
			break;
		case GUI_ID_BUTTON2:
			//ZoomOut(&m_mapCoordMap);
			//WM_Invalidate(hDlg);
			break;
		case GUI_ID_BUTTON3:
			//ZoomOut(&m_mapCoordMap);
			//WM_Invalidate(hDlg);
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

/*********************************************************************
*
*       _cbScaleWin
*/
static void _cbScaleWin(WM_MESSAGE* pMsg) {
  int Index;
  switch (pMsg->MsgId) {
  case WM_PAINT:
    for (Index = 0; Index < 4; Index++) {
      if (_Scale[Index].hWin == pMsg->hWin) {
        if (_Scale[Index].Flags & (1 << FLAG_SHOW_SCALE)) {
          _UpdateScale(Index);
          GUI_MEMDEV_Write(_Scale[Index].hMemDev);
          _DrawNeedle(&_Scale[Index], Index);
        }
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
*       static code, dialog callbacks
*
**********************************************************************
*/

/*********************************************************************
*
*       _cbDialogColor
*/
static void _cbDialogColor(WM_MESSAGE * pMsg) {
  int Scale, Index, Id;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    _hDialogColor = pMsg->hWin;
    _hDropDownColor = WM_GetDialogItem(_hDialogColor, GUI_ID_USER);
    DROPDOWN_AddString(_hDropDownColor, "Scale");
    DROPDOWN_AddString(_hDropDownColor, "Text");
    DROPDOWN_AddString(_hDropDownColor, "Needle");
    DROPDOWN_AddString(_hDropDownColor, "Arc 1");
    DROPDOWN_AddString(_hDropDownColor, "Arc 2");
    DROPDOWN_AddString(_hDropDownColor, "Arc 3");
    DROPDOWN_AddString(_hDropDownColor, "Axis");
    _SetDialogColor(0);
    break;
  case WM_NOTIFY_PARENT:
    if (_InitDialog) break;
    switch (pMsg->Data.v) {
    case WM_NOTIFICATION_SEL_CHANGED: /* Value has changed */
      Scale = DROPDOWN_GetSel(_hDropDownScale) - 1;
      if (pMsg->hWinSrc == _hDropDownColor) {
        _SetDialogColor((Scale == -1) ? 0 : Scale);
      }
      break;
    case WM_NOTIFICATION_VALUE_CHANGED: /* Value has changed */
      Scale = DROPDOWN_GetSel(_hDropDownScale) - 1;
      Id    = WM_GetId(pMsg->hWinSrc);         /* Id of widget */
      Index = DROPDOWN_GetSel(_hDropDownColor);
      if (Id >= GUI_ID_SLIDER0 && Id <= GUI_ID_SLIDER3) {
        int v = _GetSliderValue(pMsg->hWin, Id);
        int i, iStart, iEnd;
        iStart = (Scale == -1) ? 0 : Scale;
        iEnd   = (Scale == -1) ? 3 : Scale;
        for (i = iStart; i <= iEnd; i++) {
          _Scale[i].Color[Index].Sep[Id - GUI_ID_SLIDER0] = v;
          WM_InvalidateWindow(_Scale[i].hWin);
        }
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbDialogMark
*/
static void _cbDialogMark(WM_MESSAGE * pMsg) {
  int Scale, Id;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    _hDialogMark = pMsg->hWin;
    _SetDialogMark(0);
    break;
  case WM_NOTIFY_PARENT:
    if (_InitDialog) break;
    Id    = WM_GetId(pMsg->hWinSrc);         /* Id of widget */
    if (WM_IsWindow(_hDropDownScale)) {
      Scale = DROPDOWN_GetSel(_hDropDownScale) - 1;
    }
    switch (pMsg->Data.v) {
    case WM_NOTIFICATION_VALUE_CHANGED: /* Value has changed */
      if (Id >= GUI_ID_SLIDER0 && Id <= GUI_ID_SLIDER3) {
        int v = _GetSliderValue(pMsg->hWin, Id);
        int i, iStart, iEnd;
        iStart = (Scale == -1) ? 0 : Scale;
        iEnd   = (Scale == -1) ? 3 : Scale;
        for (i = iStart; i <= iEnd; i++) {
          switch (Id) {
          case GUI_ID_SLIDER0:
            _Scale[i].NumMarkLines = v;
            break;
          case GUI_ID_SLIDER1:
            _Scale[i].LineLen1 = v;
            break;
          case GUI_ID_SLIDER2:
            _Scale[i].LinePos1 = v;
            break;
          case GUI_ID_SLIDER3:
            _Scale[i].PenSize1 = v;
            _ChangeScaleSize(&_Scale[i]);
            break;
          }
          WM_InvalidateWindow(_Scale[i].hWin);
        }
      }
      break;
    case WM_NOTIFICATION_CLICKED:
      if (Id == GUI_ID_USER) {
        WM_HWIN hItem = WM_GetDialogItem(pMsg->hWin, Id);
        int v = CHECKBOX_IsChecked(hItem);
        int i, iStart, iEnd;
        iStart = (Scale == -1) ? 0 : Scale;
        iEnd   = (Scale == -1) ? 3 : Scale;
        for (i = iStart; i <= iEnd; i++) {
          _Scale[i].Flags &= ~(1 << FLAG_SHOW_MARK);
          _Scale[i].Flags |= v << FLAG_SHOW_MARK;
          WM_InvalidateWindow(_Scale[i].hWin);
        }
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbDialogPitch
*/
static void _cbDialogPitch(WM_MESSAGE * pMsg) {
  int Scale, Id;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    _hDialogPitch = pMsg->hWin;
    _SetDialogPitch(0);
    break;
  case WM_NOTIFY_PARENT:
    if (_InitDialog) break;
    Id    = WM_GetId(pMsg->hWinSrc);         /* Id of widget */
    if (WM_IsWindow(_hDropDownScale)) {
      Scale = DROPDOWN_GetSel(_hDropDownScale) - 1;
    }
    switch (pMsg->Data.v) {
    case WM_NOTIFICATION_VALUE_CHANGED: /* Value has changed */
      if (Id >= GUI_ID_SLIDER0 && Id <= GUI_ID_SLIDER3) {
        int v = _GetSliderValue(pMsg->hWin, Id);
        int i, iStart, iEnd;
        iStart = (Scale == -1) ? 0 : Scale;
        iEnd   = (Scale == -1) ? 3 : Scale;
        for (i = iStart; i <= iEnd; i++) {
          switch (Id) {
          case GUI_ID_SLIDER0:
            _Scale[i].NumPitchLines = v;
            break;
          case GUI_ID_SLIDER1:
            _Scale[i].LineLen2 = v;
            break;
          case GUI_ID_SLIDER2:
            _Scale[i].LinePos2 = v;
            break;
          case GUI_ID_SLIDER3:
            _Scale[i].PenSize2 = v;
            _ChangeScaleSize(&_Scale[i]);
            break;
          }
          WM_InvalidateWindow(_Scale[i].hWin);
        }
      }
      break;
    case WM_NOTIFICATION_CLICKED:
      if (Id == GUI_ID_USER) {
        WM_HWIN hItem = WM_GetDialogItem(pMsg->hWin, Id);
        int v = CHECKBOX_IsChecked(hItem);
        int i, iStart, iEnd;
        iStart = (Scale == -1) ? 0 : Scale;
        iEnd   = (Scale == -1) ? 3 : Scale;
        for (i = iStart; i <= iEnd; i++) {
          _Scale[i].Flags &= ~(1 << FLAG_SHOW_PITCH);
          _Scale[i].Flags |= v << FLAG_SHOW_PITCH;
          WM_InvalidateWindow(_Scale[i].hWin);
        }
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbDialogArc
*/
static void _cbDialogArc(WM_MESSAGE * pMsg) {
  int Scale, Id;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    _hDialogArc = pMsg->hWin;
    _SetDialogArc(0);
    break;
  case WM_NOTIFY_PARENT:
    if (_InitDialog) break;
    Id    = WM_GetId(pMsg->hWinSrc);         /* Id of widget */
    if (WM_IsWindow(_hDropDownScale)) {
      Scale = DROPDOWN_GetSel(_hDropDownScale) - 1;
    }
    switch (pMsg->Data.v) {
    case WM_NOTIFICATION_VALUE_CHANGED: /* Value has changed */
      if (Id >= GUI_ID_SLIDER0 && Id <= GUI_ID_SLIDER4) {
        int v = _GetSliderValue(pMsg->hWin, Id);
        int i, iStart, iEnd;
        iStart = (Scale == -1) ? 0 : Scale;
        iEnd   = (Scale == -1) ? 3 : Scale;
        for (i = iStart; i <= iEnd; i++) {
          switch (Id) {
          case GUI_ID_SLIDER0:
            _Scale[i].ArcArea1 = v;
            break;
          case GUI_ID_SLIDER1:
            _Scale[i].ArcArea2 = v;
            break;
          case GUI_ID_SLIDER2:
            _Scale[i].ArcWidth = v;
            break;
          case GUI_ID_SLIDER3:
            _Scale[i].ArcPos = v;
            break;
          case GUI_ID_SLIDER4:
            _Scale[i].PenSize3 = v;
            _ChangeScaleSize(&_Scale[i]);
            break;
          }
          WM_InvalidateWindow(_Scale[i].hWin);
        }
      }
      break;
    case WM_NOTIFICATION_CLICKED:
      if ((Id >= GUI_ID_USER+0) && (Id <= GUI_ID_USER+4)) {
        WM_HWIN hItem = WM_GetDialogItem(pMsg->hWin, Id);
        int v = CHECKBOX_IsChecked(hItem);
        int f = Id - GUI_ID_USER;
        int i, iStart, iEnd;
        iStart = (Scale == -1) ? 0 : Scale;
        iEnd   = (Scale == -1) ? 3 : Scale;
        for (i = iStart; i <= iEnd; i++) {
          _Scale[i].Flags &= ~(1 << (FLAG_SHOW_ARC+f));
          _Scale[i].Flags |= v << (FLAG_SHOW_ARC+f);
          WM_InvalidateWindow(_Scale[i].hWin);
        }
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbDialogGrad
*/
static void _cbDialogGrad(WM_MESSAGE * pMsg) {
  int Scale, Id;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    _hDialogGrad = pMsg->hWin;
    _SetDialogGrad(0);
    break;
  case WM_NOTIFY_PARENT:
    if (_InitDialog) break;
    Id    = WM_GetId(pMsg->hWinSrc);         /* Id of widget */
    if (WM_IsWindow(_hDropDownScale)) {
      Scale = DROPDOWN_GetSel(_hDropDownScale) - 1;
    }
    switch (pMsg->Data.v) {
    case WM_NOTIFICATION_VALUE_CHANGED: /* Value has changed */
      if (Id >= GUI_ID_SLIDER0 && Id <= GUI_ID_SLIDER4) {
        int v = _GetSliderValue(pMsg->hWin, Id);
        int i, iStart, iEnd;
        iStart = (Scale == -1) ? 0 : Scale;
        iEnd   = (Scale == -1) ? 3 : Scale;
        for (i = iStart; i <= iEnd; i++) {
          switch (Id) {
          case GUI_ID_SLIDER0:
            _Scale[i].GradDist = v;
            break;
          case GUI_ID_SLIDER1:
            _Scale[i].NumStep = v;
            break;
          case GUI_ID_SLIDER2:
            _Scale[i].NumStart = v;
            break;
          case GUI_ID_SLIDER3:
            _Scale[i].NumExp = v;
            break;
          case GUI_ID_SLIDER4:
            _Scale[i].TextDist = v;
            _ChangeScaleSize(&_Scale[i]);
            break;
          }
          if (Id < GUI_ID_SLIDER4) {
            WM_InvalidateWindow(_Scale[i].hWin);
          }
        }
      }
      break;
    case WM_NOTIFICATION_CLICKED:
      if ((Id >= GUI_ID_USER+0) && (Id <= GUI_ID_USER+1)) {
        WM_HWIN hItem = WM_GetDialogItem(pMsg->hWin, Id);
        int v = CHECKBOX_IsChecked(hItem);
        int i, iStart, iEnd;
        iStart = (Scale == -1) ? 0 : Scale;
        iEnd   = (Scale == -1) ? 3 : Scale;
        for (i = iStart; i <= iEnd; i++) {
          switch (Id) {
          case GUI_ID_USER+0:
            _Scale[i].Flags &= ~(1 << FLAG_SHOW_GRAD);
            _Scale[i].Flags |= v << FLAG_SHOW_GRAD;
            WM_InvalidateWindow(_Scale[i].hWin);
            break;
          case GUI_ID_USER+1:
            _Scale[i].Flags &= ~(1 << FLAG_SHOW_TEXT);
            _Scale[i].Flags |= v << FLAG_SHOW_TEXT;
            _ChangeScaleSize(&_Scale[i]);
            break;
          }
        }
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbDialogScale
*/
static void _cbDialogScale(WM_MESSAGE * pMsg) {
  int Scale, Id;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    _hDialogScale = pMsg->hWin;
    _SetDialogScale(0);
    break;
  case WM_NOTIFY_PARENT:
    if (_InitDialog) break;
    Id    = WM_GetId(pMsg->hWinSrc);         /* Id of widget */
    if (WM_IsWindow(_hDropDownScale)) {
      Scale = DROPDOWN_GetSel(_hDropDownScale) - 1;
    }
    switch (pMsg->Data.v) {
    case WM_NOTIFICATION_VALUE_CHANGED: /* Value has changed */
      if (Id >= GUI_ID_SLIDER0 && Id <= GUI_ID_SLIDER4) {
        int v = _GetSliderValue(pMsg->hWin, Id);
        int i, iStart, iEnd;
        iStart = (Scale == -1) ? 0 : Scale;
        iEnd   = (Scale == -1) ? 3 : Scale;
        for (i = iStart; i <= iEnd; i++) {
          switch (Id) {
          case GUI_ID_SLIDER0:
            _Scale[i].ArcStart = v;
            break;
          case GUI_ID_SLIDER1:
            _Scale[i].ArcEnd = v;
            break;
          case GUI_ID_SLIDER2:
            _Scale[i].ArcRadius = v;
            break;
          case GUI_ID_SLIDER3:
            _Scale[i].x = v;
            break;
          case GUI_ID_SLIDER4:
            _Scale[i].y = v;
            break;
          }
          _ChangeScaleSize(&_Scale[i]);
        }
      }
      break;
    case WM_NOTIFICATION_CLICKED:
      if (Id == GUI_ID_USER) {
        WM_HWIN hItem = WM_GetDialogItem(pMsg->hWin, Id);
        int v = CHECKBOX_IsChecked(hItem);
        int i, iStart, iEnd;
        iStart = (Scale == -1) ? 0 : Scale;
        iEnd   = (Scale == -1) ? 3 : Scale;
        for (i = iStart; i <= iEnd; i++) {
          _Scale[i].Flags &= ~(1 << FLAG_SHOW_SCALE);
          _Scale[i].Flags |= v << FLAG_SHOW_SCALE;
          WM_InvalidateWindow(_Scale[i].hWin);
        }
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbDialogMisc
*/
static void _cbDialogMisc(WM_MESSAGE * pMsg) {
  int Scale, Id;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    _hDialogMisc = pMsg->hWin;
    _SetDialogMisc(0);
    break;
  case WM_NOTIFY_PARENT:
    if (_InitDialog) break;
    Id    = WM_GetId(pMsg->hWinSrc);         /* Id of widget */
    if (WM_IsWindow(_hDropDownScale)) {
      Scale = DROPDOWN_GetSel(_hDropDownScale) - 1;
    }
    switch (pMsg->Data.v) {
    case WM_NOTIFICATION_VALUE_CHANGED: /* Value has changed */
      if (Id >= GUI_ID_SLIDER0 && Id <= GUI_ID_SLIDER2) {
        int v = _GetSliderValue(pMsg->hWin, Id);
        int i, iStart, iEnd;
        iStart = (Scale == -1) ? 0 : Scale;
        iEnd   = (Scale == -1) ? 3 : Scale;
        for (i = iStart; i <= iEnd; i++) {
          switch (Id) {
          case GUI_ID_SLIDER0:
            _Scale[i].NeedleType = v;
            break;
          case GUI_ID_SLIDER1:
            _Scale[i].NeedleRadius = v;
            break;
          case GUI_ID_SLIDER2:
            _Scale[i].AxisRadius = v;
            break;
          }
          _ChangeScaleSize(&_Scale[i]);
        }
      }
      break;
    case WM_NOTIFICATION_CLICKED:
      if ((Id >= GUI_ID_USER+0) && (Id <= GUI_ID_USER+1)) {
        WM_HWIN hItem = WM_GetDialogItem(pMsg->hWin, Id);
        int v = CHECKBOX_IsChecked(hItem);
        int i, iStart, iEnd;
        iStart = (Scale == -1) ? 0 : Scale;
        iEnd   = (Scale == -1) ? 3 : Scale;
        for (i = iStart; i <= iEnd; i++) {
          switch (Id) {
          case GUI_ID_USER+0:
            _Scale[i].Flags &= ~(1 << FLAG_NEEDLE_FRAME);
            _Scale[i].Flags |= v << FLAG_NEEDLE_FRAME;
            WM_InvalidateWindow(_Scale[i].hWin);
            break;
          case GUI_ID_USER+1:
            _Scale[i].Flags &= ~(1 << FLAG_NEEDLE_LINE);
            _Scale[i].Flags |= v << FLAG_NEEDLE_LINE;
            WM_InvalidateWindow(_Scale[i].hWin);
            break;
          }
        }
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
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

/*********************************************************************
*
*       static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _cbClient
*
* Purpose:
*   Callback routine of property dialog
*/
static void _cbClient(WM_MESSAGE * pMsg) {
  WM_HWIN hDlg, hItem;
  int NCode, Id;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
/*
	  hItem = WM_GetDialogItem(hDlg, GUI_ID_SLIDER0);
    SLIDER_SetRange(hItem, 0, 255);
    SLIDER_SetValue(hItem, _Alpha_0);
    hItem = WM_GetDialogItem(hDlg, GUI_ID_SLIDER1);
    SLIDER_SetRange(hItem, 0, 255);
    SLIDER_SetValue(hItem, _Alpha_1);
    hItem = WM_GetDialogItem(hDlg, GUI_ID_SLIDER2);
    SLIDER_SetRange(hItem, 60, 200);
    SLIDER_SetValue(hItem, _TimerPeriod);
    hItem = WM_GetDialogItem(hDlg, GUI_ID_SLIDER2);
*/  
  break;
  
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code
    switch (NCode) {
    case WM_NOTIFICATION_VALUE_CHANGED:   // Value has changed
      //_OnValueChanged(hDlg, Id);
      break;
    }
    break;
  case WM_PAINT:
    GUI_SetAlpha(80);   // Set alpha value for drawing operations
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
*       _CreateScaleWindow
*/
static void _CreateScaleWindow(SCALE* pObj) {
  pObj->hWin = WM_CreateWindow(0, 0, 10, 10, WM_CF_SHOW | WM_CF_HASTRANS, &_cbScaleWin, 0);
  _ChangeScaleSize(pObj);
}

/*********************************************************************
*
*       _ClearState
*/
static void _ClearState(void) {
  GUI_PID_STATE State = {0};
  GUI_PID_StoreState(&State);
}

/*********************************************************************
*
*       _EPSON_Demo
*/
static void _EPSON_Demo(void) {
  int tNow;
  int tEnd;
  int tNextTouchEvent;
  int iTouchEvent;
  int i;
  WM_HWIN  hClient,hChildWin;

  //char aBuffer[100];
  MULTIPAGE_Handle hMultiPage;

//  WM_HWIN hClient;
  CHECKBOX_Handle hCheck;
 // BUTTON_Handle hButton0;
  _Break    = 0;
  _AutoMode = 1;
  /* Use memory devices */
  WM_SetCreateFlags(WM_CF_MEMDEV);
  WM_EnableMemdev(WM_HBKWIN);
  /* Set callback for background */
  WM_SetCallback(WM_HBKWIN, &_cbBkWindow);
  /* Set preset */
  _Scale[0] = _Presets[0][0];
  _Scale[1] = _Presets[0][1];
  _Scale[2] = _Presets[0][2];
  _Scale[3] = _Presets[0][3];
  /* Create backgrund */
  _CreateBackGround();
  /* Create the scale-windows */
  _CreateScaleWindow(&_Scale[0]);
  _CreateScaleWindow(&_Scale[1]);
  _CreateScaleWindow(&_Scale[2]);
  _CreateScaleWindow(&_Scale[3]);
  /* Create framewindow */
  hFrame = FRAMEWIN_Create("控制", &_cbFrameWin, WM_CF_SHOW, 5, 290, 370, 184);
  FRAMEWIN_SetActive(hFrame, 1);
  FRAMEWIN_SetFont(hFrame,&GUI_FontHZ12);
    /* Create edit widget */
  hEdit =EDIT_CreateAsChild(10, 20, 200, 22, hFrame, 13, WM_CF_SHOW, 100);
  // EDIT_CreateAsChild( 50, 110, 220, 25, ' ', 100, 0 );
  /* Modify edit widget */
  gGisFont=&GUI_FontHZ12;
  EDIT_SetFont(hEdit, &GUI_FontHZ12);
  EDIT_SetText(hEdit, "清水河");
  EDIT_SetTextColor(hEdit, 0, GUI_RED);

  /* Set keyboard focus to edit widget */
  WM_SetFocus(hEdit);
/*
  hButton_SetMap = BUTTON_CreateEx(1, 4, 56, 48, hChildWin, WM_CF_SHOW, 0, ID_BUTTON_CONFIGURE);
	BUTTON_SetBitmap(hButton_SetMap, 0, &bmLaySelect);

	hButton_Search = BUTTON_CreateEx(1, 58, 56, 48, hChildWin, WM_CF_SHOW, 0,ID_BUTTON_SEARCH);
	BUTTON_SetBitmap(hButton_Search, 0, &bmSearch);

	hBtn_Enlarge = BUTTON_CreateEx(1, 112, 25, 38, hChildWin, WM_CF_SHOW, 0,1001);
	BUTTON_SetBitmap(hBtn_Enlarge, 0, &bmenlarge);
	hBtn_Shrink = BUTTON_CreateEx(32, 112, 25, 38, hChildWin, WM_CF_SHOW, 0,1002);
	BUTTON_SetBitmap(hBtn_Shrink, 0, &bmshrink);
*/
  _CreateButton("", 220,  4, 56, 48, WM_GetClientWindow(hFrame), GUI_ID_BUTTON0,&GUI_FontHZ12,&bmSearch);
  _CreateButton("", 220,  54, 56, 48, WM_GetClientWindow(hFrame), GUI_ID_BUTTON1,&GUI_FontHZ12,&bmLaySelect);
  _CreateButton("", 300,  10, 25, 38, WM_GetClientWindow(hFrame), GUI_ID_BUTTON2,&GUI_FontHZ12,&bmenlarge);
  _CreateButton("", 300,  50, 25, 38, WM_GetClientWindow(hFrame), GUI_ID_BUTTON3,&GUI_FontHZ12,&bmshrink);

  //BUTTON_SetFont(hButton0, &GUI_FontHZ12);


  hListBox = LISTBOX_CreateAsChild(_ListBox, WM_GetClientWindow(hFrame), 10, 40, 200, 100, WM_CF_SHOW);
  LISTBOX_SetFont(hListBox, &GUI_FontHZ12);

  /* Create multipage */
/*
  hMultiPage = MULTIPAGE_CreateEx(5, 5, 280, 154, WM_GetClientWindow(hFrame), WM_CF_SHOW, 0, 0);
  _hDialogColor   = _AddDialog("Color",   ARRAY(_aDialogColor),   &_cbDialogColor,   hMultiPage);
  _hDialogMark    = _AddDialog("Mark",    ARRAY(_aDialogMark),    &_cbDialogMark,    hMultiPage);
  _hDialogPitch   = _AddDialog("Pitch",   ARRAY(_aDialogPitch),   &_cbDialogPitch,   hMultiPage);
  _hDialogArc     = _AddDialog("Arc",     ARRAY(_aDialogArc),     &_cbDialogArc,     hMultiPage);
  _hDialogGrad    = _AddDialog("Grad",    ARRAY(_aDialogGrad),    &_cbDialogGrad,    hMultiPage);
  _hDialogScale   = _AddDialog("Scale",   ARRAY(_aDialogScale),   &_cbDialogScale,   hMultiPage);
  _hDialogMisc    = _AddDialog("Misc",    ARRAY(_aDialogMisc),    &_cbDialogMisc,    hMultiPage);
  MULTIPAGE_SelectPage(hMultiPage, 0);
*/ 
 /* Create drop-down box */
  /*
  hClient = WM_GetClientWindow(hFrame);
  _hDropDownScale = DROPDOWN_Create(hClient, 292, 30, 65, 73, WM_CF_SHOW);
  DROPDOWN_AddString(_hDropDownScale, "All");
  DROPDOWN_AddString(_hDropDownScale, "Scale 1");
  DROPDOWN_AddString(_hDropDownScale, "Scale 2");
  DROPDOWN_AddString(_hDropDownScale, "Scale 3");
  DROPDOWN_AddString(_hDropDownScale, "Scale 4");
  */
  /* Create checkbox for automode */
  /*
  hCheck = CHECKBOX_CreateEx(292, 6, 65, 20, hClient, WM_CF_SHOW, 0, GUI_ID_CHECK0);
  CHECKBOX_SetText(hCheck, "Auto");
  CHECKBOX_SetState(hCheck, 1);
  */
  /* Create buttons for presets */
  /*
  _CreateButton("Preset 1", 292,  60, 65, 20, hClient, GUI_ID_BUTTON0);
  _CreateButton("Preset 2", 292,  84, 65, 20, hClient, GUI_ID_BUTTON1);
  _CreateButton("Preset 3", 292, 108, 65, 20, hClient, GUI_ID_BUTTON2);
  _CreateButton("Preset 4", 292, 132, 65, 20, hClient, GUI_ID_BUTTON3);
  _CreateButton("query", 292, 156, 65, 20, hClient, GUI_ID_BUTTON4);
  */
  /* Create framewindow for map */
  
  hWinMap  = FRAMEWIN_Create("Map to Hilden", &_cbWinMap, WM_CF_SHOW, 385, 290, 250, 184);

  FRAMEWIN_SetActive(hWinMap, 0);
  WM_SetFocus(hWinMap);
  hChildWin = WM_GetClientWindow(hWinMap);
  
  _CreateButton("", 1,  10, 18, 18, hChildWin, GUI_ID_BUTTON0,&GUI_FontHZ12,&bmZOOMIN);
       _CreateButton("", 1,  19+10, 18, 18, hChildWin, GUI_ID_BUTTON1,&GUI_FontHZ12,&bmZOOMOUT);
       _CreateButton("", 1, 19*2+10, 18, 18, hChildWin, GUI_ID_BUTTON2,&GUI_FontHZ12,&bmSearch);
       _CreateButton("", 1,  19*3+10, 18, 18, hChildWin, GUI_ID_BUTTON3,&GUI_FontHZ12,&bmReturn);
  
  //WM_SelectWindow(hChildWin);

  //WM_CreateTimer(hWinMap,0,1000,0);
 // FRAMEWIN_SetFont(hWinMap,&GUI_FontHZ12);
  
  /* Handle the demo */
  //GUI_CURSOR_Show();
  iTouchEvent = 0;
  tNextTouchEvent = GUI_GetTime() + 10000; /* First touch event */
  tEnd = GUI_GetTime() + 44000;
  _ClearState();
/*  
  while(1){
		GUI_Delay(200);
		GUI_Exec();
	}

  */
    GUI_Exec();

  do {
    GUI_PID_STATE CurrentState;
    GUI_PID_GetState(&CurrentState);
    tNow = GUI_GetTime();

    _MoveMap(hChildWin);
    //WM_InvalidateWindow(hWinMap);

	_MoveNeedle(&_Needle[0], 0);
    _MoveNeedle(&_Needle[1], 1);
    _MoveNeedle(&_Needle[2], 2);
    _MoveNeedle(&_Needle[3], 3);
    GUI_Exec();
    _tDiff = GUI_GetTime() - tNow;
    if (_tDiff < 25) {                 // Make sure we have no more than a certain number of frames/sec 
      GUI_Delay(25 - _tDiff);
    }
    if (_AutoMode) {
      tEnd = tNow + 10000;
      if ((CurrentState.Pressed) && ((CurrentState.x != _aPID_Events[iTouchEvent].x) || (CurrentState.y != _aPID_Events[iTouchEvent].y))) {
        WM_HWIN hItem;
        hItem = WM_GetDialogItem(hFrame, GUI_ID_CHECK0);
        CHECKBOX_SetState(hItem, 0);
        _AutoMode = 0;
      } else {
        if (tNow >= tNextTouchEvent) {
          GUI_PID_STATE State = {0};
          State.x = _aPID_Events[iTouchEvent].x;
          State.y = _aPID_Events[iTouchEvent].y;
          State.Pressed = _aPID_Events[iTouchEvent].Pressed;
          tNextTouchEvent = tNow + _aPID_Events[iTouchEvent].Duration;
          GUI_PID_StoreState(&State);
          iTouchEvent++;
          if (iTouchEvent >= GUI_COUNTOF(_aPID_Events)) {
            _Break = 1;
          }
        }
      }
    } else {
      GUI_PID_STATE OldState;
      if (memcmp(&OldState, &CurrentState, sizeof(GUI_PID_STATE)) != 0) {
        tEnd = tNow + 10000;
      }
      OldState = CurrentState;
    }
  } while (1 );//&& (tNow < tEnd));
  GUI_LOCK();
  MapRelease(&myUcMap);
  FreeMapCoorTrans(&m_mapCoordMap);
  UserLayerFree(&g_userLayer);
  GUI_UNLOCK();
  /* Unset callback */
  WM_SetCallback(WM_HBKWIN, NULL);
  /* Clear memory */
  for (i = 0; i < GUI_COUNTOF(_Scale); i++) {
    WM_DeleteWindow(_Scale[i].hWin);
    GUI_MEMDEV_Delete(_Scale[i].hMemDev);
  }
  GUI_MEMDEV_Delete(_hBkMemDev);
  WM_DeleteWindow(WM_GetParent(hWinMap));
  WM_DeleteWindow(hFrame);
  _hDropDownScale = 0;
}

/*********************************************************************
*
*       AppDashBoard
*
**********************************************************************
*/

int AppDashBoard(void);
int AppDashBoard(void) {
  WM_SetCallback(WM_HBKWIN, NULL);
  GUI_Clear();
  _EPSON_Demo();
  return _Break;
}
#endif
extern char  UI_gisSDK(char *filepath, INT16U SizeX, INT16U SizeY, GUI_FONT GUI_UNI_PTR * pfont, GpsDataInfo * Paramer);
#if 1
void MainTask(void) {
  GpsDataInfo gisParamer;

	//MultTask_MainTask();
	//DSB_Demo();
	GUI_Init();
	GUI_SetFont(&GUI_FontHZ12);
    GUI_UC_SetEncodeGBK();
#if 0
	AppDashBoard();
#else
	gisParamer.m_xx=104.09539;
	gisParamer.m_yy=30.627087;
	gisParamer.m_nCarID=1;
 UI_gisSDK(NULL, LCD_GetXSize(), LCD_GetYSize(), &GUI_FontHZ12, &gisParamer);
#endif
 while(1);
  //MainTask_Game();
}
#endif

/*************************** End of file ****************************/

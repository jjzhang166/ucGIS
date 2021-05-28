#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <windows.h>
#include <math.h>
#include "readmapdata.h"
#include "coortrans.h"

//#include "D:\备份\2410\uC-GUI-V3.94\Start\serial\Serial.h"
#define FACTOR        1//AA_FACTOR
//CSerial mycom;
void OpenCom(int comport,int baudrate);
void SendString(char *str);
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

typedef struct {
  U8  Sep[4];
} COLOR;

static const GUI_POINT _aArrow[] = {
  {  0, 0   +15},
  {-40, -35 +15},
  {-10, -25 +15},
  {-10, -85 +15},
  { 10, -85 +15},
  { 10, -25 +15},
  { 40, -35 +15}
};

static void _MagnifyPolygon(GUI_POINT* pDest, const GUI_POINT* pSrc, int NumPoints, float Mag) {
  int i;
  for (i=0; i<NumPoints; i++) {
    (pDest+i)->x = (pSrc+i)->x * Mag;
    (pDest+i)->y = (pSrc+i)->y * Mag;
  }
}

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

static GUI_POINT _aNeedle[5];

static GUI_COLOR _CalcColor(const COLOR* pColor) {
  GUI_COLOR r;
  r  = ((U32) pColor->Sep[0]);
  r += ((U32) pColor->Sep[1]) << 8;
  r += ((U32) pColor->Sep[2]) << 16;
  return r;
}


enum ParseState {
   PS_WHITESPACE,
   PS_TOKEN,
   PS_STRING,
   PS_SPECCHAR,
   PS_ESCAPE
};

enum ParseState stackedState;

void parsewithspeccharargs(char *argstr, char specchar,int *argc_p, char **argv)
{
  int argc = 0;
  char c;
  enum ParseState lastState = PS_WHITESPACE;
  #ifdef MsgDebug
  SendConsoleStr("parseargs");
  SendConsoleStr(argstr);
  #endif  
  /* tokenize the argstr */
  while ((c = *argstr) != 0) 
  {
    enum ParseState newState;
      
    if (lastState == PS_ESCAPE) 
    {
      newState = stackedState;
    } 
    else if (lastState == PS_STRING) 
    {
      if (c == '"') 
      {
        newState = PS_WHITESPACE;
        *argstr = 0;
      } 
      else 
      {
        newState = PS_STRING;
      }
    } 
    else if ((c == ' ') || (c == '\t')) 
    {
      /* whitespace character */
      *argstr = 0;
      newState = PS_WHITESPACE;
    } 
    else if (c == '"') 
    {
      newState = PS_STRING;
      *argstr++ = 0;
      argv[argc++] = argstr;
    } 
    else if (c == '\\') 
    {
      stackedState = lastState;
      newState = PS_ESCAPE;
    }
    else if(c==specchar)
    {
      newState = PS_SPECCHAR;
      if(*(argstr+1)==specchar)
	  {
       argv[argc++] ="";  
	   *argstr=0;
	  }
	  else
      {
       *argstr++ = 0;
       argv[argc++] = argstr;
      }
    }
    else 
    {
      /* token */
      if (lastState == PS_WHITESPACE) 
      {
        argv[argc++] = argstr;
      }      
      newState = PS_TOKEN;
    }

    lastState = newState;
    argstr++;
  }
#ifdef CMDMsgDebug
  if (1) 
  { 
    int i;
    putLabeledWord("parseargs: argc=", argc);
    for (i = 0; i < argc; i++) 
    {
      SendConsoleStr("   ");
      SendConsoleStr(argv[i]);
      SendConsoleStr("\r\n");
    }
  }
#endif
  argv[argc] = NULL;
  if (argc_p != NULL)
    *argc_p = argc;
}

/*
#define STREAMLEN 100
int ReadFileHeader()
{
	if(m_pfile==NULL)
		return false;
	char chTmp[STREAMLEN]={0,};
	char tmpchar[2];
	char DataType=0;
	int i=0;
	do{
	tmpchar[0]='\0';
	tmpchar[1]='\0';
	fread(tmpchar,1,1,m_pfile);
    chTmp[i++]=tmpchar[0];
	if(tmpchar[0]==0x0A)
		break;
	}while(1);

	while(1)
	{
	   if(strcmp(chTmp,"Data")==0) break;
	   if(strcmp(chTmp,"data")==0) break;

	   memset(chTmp,0,STREAMLEN);
		i=0;
		do{
	      tmpchar[0]='\0';
	      tmpchar[1]='\0';
	      fread(tmpchar,1,1,m_pfile);
          chTmp[i++]=tmpchar[0];
	      if(tmpchar[0]==0x0A)
		  {
            chTmp[4]='\0';
		    break;
		  }
		}while(true);

		//fread(&chTmp[3],1,1,m_pfile);
	}
	m_iHeaderDataLen=ftell(m_pfile)-4;
	char *chData = new char[m_iHeaderDataLen+4];
	memset(chData,0,m_iHeaderDataLen+4);
	fseek(m_pfile,0,0);
	fread(chData,m_iHeaderDataLen,1,m_pfile);

	int iPosKeyWord[8];
	int iLenKeyWord[7]={7,7,9,6,5,8,7};
	iPosKeyWord[7]=m_iHeaderDataLen;

	strcpy(chTmp,"Version");
	iPosKeyWord[0]=findPosInCstring(chData,m_iHeaderDataLen,chTmp,7);
	strcpy(chTmp,"Charset");
	iPosKeyWord[1]=findPosInCstring(chData,m_iHeaderDataLen,chTmp,7);
	strcpy(chTmp,"Delimiter");
	iPosKeyWord[2]=findPosInCstring(chData,m_iHeaderDataLen,chTmp,9);
	strcpy(chTmp,"Unique");
	iPosKeyWord[3]=findPosInCstring(chData,m_iHeaderDataLen,chTmp,6);
	strcpy(chTmp,"Index");
	iPosKeyWord[4]=findPosInCstring(chData,m_iHeaderDataLen,chTmp,5);
	strcpy(chTmp,"Coordsys");
	iPosKeyWord[5]=findPosInCstring(chData,m_iHeaderDataLen,chTmp,8);
	strcpy(chTmp,"columns");
	iPosKeyWord[6]=findPosInCstring(chData,m_iHeaderDataLen,chTmp,7);

	int iPos;
	for(int iLoop=0;iLoop<7;iLoop++)
	{
		if(iPosKeyWord[iLoop]>=0)
		{
			for(int iLoop1=iLoop+1;iLoop1<8;iLoop1++)
				if(iPosKeyWord[iLoop1]>=0)
					break;
			if(iLoop1<8)
			{
				switch(iLoop)
				{
					case 0: //Version
					{
						iPos = iPosKeyWord[iLoop]+iLenKeyWord[iLoop];
						memset(chTmp,0,STREAMLEN);
						memcpy(chTmp,&chData[iPos],iPosKeyWord[iLoop1]-iPos);
						m_FileHeader.MapInfoVersion=atoi(chTmp);
						break;
					}
					case 1: //Charset
					{
						iPos = iPosKeyWord[iLoop]+iLenKeyWord[iLoop];
						m_FileHeader.strDataCharSet=new char[iPosKeyWord[iLoop1]-iPos];
						memcpy(m_FileHeader.strDataCharSet,&chData[iPos],iPosKeyWord[iLoop1]-iPos);
						break;
					}
					case 2: //Delimiter
					{
						iPos = iPosKeyWord[iLoop]+iLenKeyWord[iLoop];
						memset(chTmp,0,STREAMLEN);
						memcpy(chTmp,&chData[iPos],iPosKeyWord[iLoop1]-iPos);
						m_FileHeader.strDataDelimiter=new char[iPosKeyWord[iLoop1]-iPos];
						char chTmp1[2]="\"";
						int iPosTmp=findPosInCstring(chTmp,iPosKeyWord[iLoop1]-iPos,chTmp1,1);
						strcpy(m_FileHeader.strDataDelimiter,&chTmp[iPosTmp+1]);
						int iPosTmp1=findPosInCstring(m_FileHeader.strDataDelimiter,iPosKeyWord[iLoop1]-iPos-iPosTmp-1,chTmp1,1);
						if(iPosTmp1>=0)
							m_FileHeader.strDataDelimiter[iPosTmp1]=0;
						break;
					}
					case 3: //Unique
					{
						break;
					}
					case 4: //Index
					{
						break;
					}
					case 5: //CoordSys
					{
						iPos = iPosKeyWord[iLoop]+iLenKeyWord[iLoop];
						m_FileHeader.strCoordSys=new char[iPosKeyWord[iLoop1]-iPos];
						memcpy(m_FileHeader.strCoordSys,&chData[iPos],iPosKeyWord[iLoop1]-iPos);
						break;
					}
					case 6: //Columns
					{
						iPos = iPosKeyWord[iLoop]+iLenKeyWord[iLoop];
						memset(chTmp,0,STREAMLEN);
						sscanf(&chData[iPos],"%s",chTmp);
						int iLenChar=strlen(chTmp) ;
						m_FileHeader.Columns.uiColumnCount=atoi(chTmp);
						m_FileHeader.Columns.pRec_Columns=new Column[m_FileHeader.Columns.uiColumnCount];
						for(int iLoop2=0;iLoop2<m_FileHeader.Columns.uiColumnCount;iLoop2++)
						{
							int iPosTmp=findPosInCstring(&chData[iPos],iPosKeyWord[iLoop1]-iPos,chTmp,iLenChar);
							iPos+=(iPosTmp+iLenChar);
							memset(chTmp,0,STREAMLEN);
							sscanf(&chData[iPos],"%s",chTmp);
							iLenChar=strlen(chTmp) ;
							if(iLenChar>50)
								return false;
							strcpy(m_FileHeader.Columns.pRec_Columns[iLoop2].chColumnName,chTmp);
							
							iPosTmp=findPosInCstring(&chData[iPos],iPosKeyWord[iLoop1]-iPos,chTmp,iLenChar);
							iPos+=(iPosTmp+iLenChar);
							memset(chTmp,0,STREAMLEN);
							sscanf(&chData[iPos],"%s",chTmp);
							iLenChar=strlen(chTmp) ;
							char chTmp1[20]="(";
							iPosTmp=findPosInCstring(chTmp,iLenChar,chTmp1,1);
							if(iPosTmp<0)
							{
								if(strcmp(chTmp,"integer")==0
								||strcmp(chTmp,"Integer")==0
								||strcmp(chTmp,"INTEGER")==0)
								{
									m_FileHeader.Columns.pRec_Columns[iLoop2].uiColumnType=3;
									m_FileHeader.Columns.pRec_Columns[iLoop2].uiLength=4;
								}
								else if(strcmp(chTmp,"smallint")==0
								||strcmp(chTmp,"Smallint")==0
								||strcmp(chTmp,"SMALLINT")==0)
								{
									m_FileHeader.Columns.pRec_Columns[iLoop2].uiColumnType=2;
									m_FileHeader.Columns.pRec_Columns[iLoop2].uiLength=2;
								}
								else if(strcmp(chTmp,"float")==0
								||strcmp(chTmp,"Float")==0
								||strcmp(chTmp,"FLOAT")==0)
								{
									m_FileHeader.Columns.pRec_Columns[iLoop2].uiColumnType=4;
									m_FileHeader.Columns.pRec_Columns[iLoop2].uiLength=4;
								}
								else if(strcmp(chTmp,"date")==0
								||strcmp(chTmp,"Date")==0
								||strcmp(chTmp,"DATE")==0)
								{
									m_FileHeader.Columns.pRec_Columns[iLoop2].uiColumnType=5;
									m_FileHeader.Columns.pRec_Columns[iLoop2].uiLength=20;
								}
								else if(strcmp(chTmp,"logical")==0
								||strcmp(chTmp,"Logical")==0
								||strcmp(chTmp,"LOGICAL")==0)
								{
									m_FileHeader.Columns.pRec_Columns[iLoop2].uiColumnType=6;
									m_FileHeader.Columns.pRec_Columns[iLoop2].uiLength=1;
								}
								else return false;
							}
							else
							{
								memcpy(chTmp1,chTmp,iPosTmp);
								if(strcmp(chTmp1,"Char")==0
									||strcmp(chTmp1,"char")==0
									||strcmp(chTmp1,"CHAR")==0)
								{
									m_FileHeader.Columns.pRec_Columns[iLoop2].uiColumnType=1;
									m_FileHeader.Columns.pRec_Columns[iLoop2].uiLength=atoi(&chTmp[iPosTmp+1]);
								}
								else if(strcmp(chTmp1,"decimal")==0
									||strcmp(chTmp1,"Decimal")==0
									||strcmp(chTmp1,"DECIMAL")==0)
								{
									m_FileHeader.Columns.pRec_Columns[iLoop2].uiColumnType=7;
									strcpy(chTmp1,","); 
									int iPosDecimal = findPosInCstring(&chTmp[iPosTmp+1],iLenChar-iPosTmp-1,chTmp1,1);
									m_FileHeader.Columns.pRec_Columns[iLoop2].uiLength
										=((atoi(&chTmp[iPosTmp+1]))<<8)+(atoi(&chTmp[iPosTmp+iPosDecimal+2]));
								}
								else return false;
							}

						}
						break;
					}

					default:
						break;
				}
			}
			iLoop=iLoop1-1;
		}
	}

	delete []chData;
	return true;

}
*/

void GeoEqual(GeoPoint *point1,GeoPoint point2)
{
	point1->lon=point2.lon;
	point1->lat=point2.lat;
	point1->hgt=point2.hgt;

}
//读取基本的标注
bool Lable_ReadFromFile(FILE *fp,GText *lpLabel)
{
	BYTE nLableLen = 0;
	char pszLabel[100];
	fread(&nLableLen,1,sizeof(BYTE),fp);
	pszLabel[nLableLen] = '\0';
	if(nLableLen>0)
		fread(pszLabel,sizeof(TCHAR),nLableLen,fp);

	fread(&lpLabel->m_nLayer,1,sizeof(lpLabel->m_nLayer),fp);
	fread(&lpLabel->m_Rect,1,sizeof(lpLabel->m_Rect),fp);
	fread(&lpLabel->m_nType,1,sizeof(lpLabel->m_nType),fp);
	fread(&lpLabel->m_nSubType,1,sizeof(lpLabel->m_nSubType),fp);

	lpLabel->text = GUI_ALLOC_AllocZero(nLableLen+2);
    if (!lpLabel->text) 
	{
   	  MessageBox(NULL,_T("申请内存失败！"),_T("错误"),MB_ICONERROR);
	  return false;
	}
	else
	{
      
	  WM_LOCK();
	  memset((char*) GUI_ALLOC_h2p(lpLabel->text),0,nLableLen);
      strcpy((char*) GUI_ALLOC_h2p(lpLabel->text), pszLabel);
	  WM_UNLOCK();
	}
	//memset(m_lpszLabel,0,nLableLen);
	//lstrcpy(m_lpszLabel,pszLabel);
    return true;
}
//
bool Pline_ReadFromFile(FILE *fp,GPLine *lpGpline)
{
	GeoPoint geoPoint;
	Lable_ReadFromFile(fp,&lpGpline->baselable);

	fread(&lpGpline->num_pt,1,sizeof(int),fp);
	fread(&lpGpline->plinepen,1,sizeof(GPen),fp);
    GUI_ARRAY_CREATE(&lpGpline->pointarray);
    if(lpGpline->pointarray)
	{
		for(int i=0;i<lpGpline->num_pt;i++)
		{
			fread(&geoPoint.lat,sizeof(float),1,fp);
			fread(&geoPoint.lon,sizeof(float),1,fp);
            geoPoint.hgt=0;
			GUI_ARRAY_AddItem(&lpGpline->pointarray, &geoPoint, sizeof(GeoPoint));
		}
		return true;
	}
	else
	{
		return false;
	}

}
bool Line_ReadFromFile((FILE *fp,GLine *lpLine)
{
	Lable_ReadFromFile();
	return true;
}
void Layer_ReadFromFile(FILE *fp,GMapLayer  *lpGmaplayer)
{
	GeoPoint point;
	GeoPoint firstpoint,lastpoint;
    GPoint gpoint; 
    GPLine pline;
	GRegion region;
	GText  text;
	GLine  gline;
    memset(&point,0,sizeof(GeoPoint));
    memset(&pline,0,sizeof(GPLine));

	fread(&m_bAutoLabel,sizeof(bool),1,fp);
	fread(&m_bVisible,sizeof(bool),1,fp);
	fread(&m_nEyeShotMin,sizeof(m_nEyeShotMin),1,fp);
	fread(&m_nEyeShotMax,sizeof(m_nEyeShotMax),1,fp);
	fread(m_pszName,sizeof(m_pszName),1,fp);

	int nCountRgn = 0;
	int nCountPline = 0;
	int nCountLine = 0;
	int nCountPoint = 0;

	// 多边形
	fread(&nCountRgn,sizeof(int),1,fp);
	for(int i=0;i<nCountRgn;i++)
	{
		if(feof(fp)) return;
		//region
		//CPlineRgn* rgn = new CPlineRgn;
		//rgn->ReadFromFile(fp);
		//m_PlineRgnVector.push_back(rgn);
	}
	// 曲线
	fread(&nCountPline,sizeof(int),1,fp);
	for(int j=0;j<nCountPline;j++)
	{
		if(feof(fp)) return;
		CPline* pline = new CPline;
		pline->ReadFromFile(fp);
		m_PlineVector.push_back(pline);
	}
	// 直线
	fread(&nCountLine,sizeof(int),1,fp);
	for(int k=0;k<nCountLine;k++)
	{
		if(feof(fp)) return;
		CLine* line = new CLine;
		line->ReadFromFile(fp);
		m_LineVector.push_back(line);
	}
	// 点
	fread(&nCountPoint,sizeof(int),1,fp);
	for(int n=0;n<nCountPoint;n++)
	{
		if(feof(fp)) return;
		CMyPoint* point = new CMyPoint;
		point->ReadFromFile(fp);
		m_MyPointVector.push_back(point);
	}
}
// 从文件读取数据
bool load_mapinfo(char* pszFileName)
{
	//lstrcpy(m_pszMapFilePath,pszFileName);
	char path[255];
	int nCount ;
	//WToA(pszFileName,path);
			// 显示忙光标
	//SetCursor (LoadCursor (NULL, IDC_WAIT));
	FILE	*fp;
	GMapLayer  maplayer;

	sprintf(path,"%s",pszFileName);
	fp = fopen(path,"rb");
	if(!fp)	return false;
	fread(&m_fCenterX,sizeof(float),1,fp);  // 
	fread(&m_fCenterY,sizeof(float),1,fp);  // 
	fread(&blc,sizeof(float),1,fp);  // 
	
	fread(&m_lpszMapName,sizeof(m_lpszMapName),1,fp);  // 

	fread(&nCount,sizeof(int),1,fp);  // 写层的数量
	for(int i=0;i<nCount;i++)
	{
		//CLayer* layer = new CLayer;
		//m_LayerVector.push_back(layer);
		//m_LayerVector[i]->ReadFromFile(fp);
      memset(&maplayer,0,sizeof(GMapLayer));
    GUI_ARRAY_CREATE(&maplayer.pointdata.PointArray);
	GUI_ARRAY_CREATE(&maplayer.linedata.LineArray);
    GUI_ARRAY_CREATE(&maplayer.plinedata.PLineArray);
    GUI_ARRAY_CREATE(&maplayer.regiondata.RegionArray);
    GUI_ARRAY_CREATE(&maplayer.arcdata.ArcArray);
    GUI_ARRAY_CREATE(&maplayer.textdata.TextArray);
    GUI_ARRAY_CREATE(&maplayer.rectdata.RectangleArray);
    GUI_ARRAY_CREATE(&maplayer.roundrectdata.RoundedRectangleArray);
    GUI_ARRAY_CREATE(&maplayer.ellipsedata.EllipseArray);

	  Layer_ReadFromFile(fp,&maplayer);
	  GUI_ARRAY_AddItem(&mymap.MaplayerArray,&maplayer, sizeof(GMapLayer));
	}
	fclose(fp);
	m_mapCoordMap->m_bMapChanged = true;
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return true;
}

/*
void load_mapinfo(char * mapfile)
{
    char line[256],penstr[20],widthstr[10]; 
	char *argv[20];
    int argc;
	int pointnum=0,pointnum1=0;
	int counter=0,counter1=0;
	int plineindex=0,regionindex=0,textindex=0,lineindex=0,pointindex=0;
	int len=0;
	int tempdata;
	char *tmptext;
	int bIvalid;
	//double lat;
    FILE *fp,*fpmid;

	GeoPoint point;
	GeoPoint firstpoint,lastpoint;
    GPoint gpoint; 
    GPLine pline;
	GRegion region[20];
	GText  text;
	GLine  gline;
	GMapLayer  maplayer;
    memset(&point,0,sizeof(GeoPoint));
    memset(&pline,0,sizeof(GPLine));
    memset(&maplayer,0,sizeof(GMapLayer));


    if((fp=fopen(mapfile,"rt"))==NULL)
	{
		return;
	}
	//
	mapfile[strlen(mapfile)-1]='d';

    if((fpmid=fopen(mapfile,"rt"))==NULL)
	{
	}


    GUI_ARRAY_CREATE(&maplayer.pointdata.PointArray);
	GUI_ARRAY_CREATE(&maplayer.linedata.LineArray);
    GUI_ARRAY_CREATE(&maplayer.plinedata.PLineArray);
    GUI_ARRAY_CREATE(&maplayer.regiondata.RegionArray);
    GUI_ARRAY_CREATE(&maplayer.arcdata.ArcArray);
    GUI_ARRAY_CREATE(&maplayer.textdata.TextArray);
    GUI_ARRAY_CREATE(&maplayer.rectdata.RectangleArray);
    GUI_ARRAY_CREATE(&maplayer.roundrectdata.RoundedRectangleArray);
    GUI_ARRAY_CREATE(&maplayer.ellipsedata.EllipseArray);

	sprintf(maplayer.name,"%s",mapfile);
    maplayer.id=mymap.num_pg;

	mymap.num_pg++;
	maplayer.m_fMaxX=maplayer.m_fMaxY=-600000000.0f;
	maplayer.m_fMinX=maplayer.m_fMinY= 600000000.0f;
	bIvalid=0;
    while(1) 
    { 
	  if(feof(fp)) break;
	  memset(line,0,256);
      fgets(line,256,fp);

retry:
	  if( (strncmp(line,"Pline",5)==0) || (strncmp(line,"pline",5)==0))
	  {
		memset(&pline,0,sizeof(GPLine));
		GUI_ARRAY_CREATE(&pline.pointarray);
        pline.id=plineindex;
		plineindex++;

		sprintf(pline.name,"PLINE%d",pline.id);
		//Pline数据处理
        parsewithspeccharargs(line, ' ',&argc,argv);
		pointnum=atoi(argv[1]);
        pline.num_pt=pointnum;
		for(counter=0;counter<pointnum;counter++)
		{
          if(feof(fp)) break;
	      memset(line,0,256);
          fgets(line,256,fp);
          parsewithspeccharargs(line, ' ',&argc,argv);
          point.hgt=0;
		  point.lon=atof(argv[0]);
		  point.lat=atof(argv[1]);
		  if(counter==0)
		     GeoEqual(&firstpoint,point);  
 		  maplayer.m_fMinX = maplayer.m_fMinX < point.lon ? maplayer.m_fMinX : point.lon;
		  maplayer.m_fMaxX = maplayer.m_fMaxX > point.lon ? maplayer.m_fMaxX : point.lon;
		  maplayer.m_fMinY = maplayer.m_fMinY < point.lat ? maplayer.m_fMinY : point.lat;			
		  maplayer.m_fMaxY = maplayer.m_fMaxY > point.lat ? maplayer.m_fMaxY : point.lat;
          bIvalid=1;
          GUI_ARRAY_AddItem(&pline.pointarray, &point, sizeof(GeoPoint));
		}
        GeoEqual(&lastpoint,point);  

	
		//pen 
        if(feof(fp)) break;
	    memset(line,0,256);
        fgets(line,256,fp);
		if(strstr(line,"Pen")==NULL) break;
		strcpy(penstr,strchr(line,'('));
        parsewithspeccharargs(penstr+1, ',',&argc,argv);
	    pline.plinepen.width=atoi(argv[0]);
        pline.plinepen.patter=atoi(argv[1]);
		memset(widthstr,0,10);
		len=strlen(argv[2])-1;
		memcpy(widthstr,argv[2],len);
	    //转换颜色位定义
	    //由于MapInfo与Windows使用不同的位定义RGB颜色，必须转换
		pline.plinepen.color=atoi(widthstr);
		pline.plinepen.color=CONVERRGB(pline.plinepen.color);//(pline.plinepen.color & 0xff00) + ((pline.plinepen.color & 0xff0000) >> 16) + ((pline.plinepen.color & 0xff) << 16);
		pline.m_bSmooth=0;
		bIvalid=1;

		if(fpmid)
		{
		  memset(line,0,256);
          fgets(line,256,fpmid);
		  parsewithspeccharargs(line, ',',&argc,argv);
          if(strncmp(argv[2],"\"",2)!=0)
		  {
	  		memset(&text,0,sizeof(GText));
            text.id=textindex;
		    textindex++;
		    text.text = GUI_ALLOC_AllocZero(strlen(argv[2]) + 1);
            tmptext=strchr(argv[2],'"');		
            if (text.text) 
			{
              WM_LOCK();
              strcpy((char*) GUI_ALLOC_h2p(text.text), tmptext+1);
		      WM_UNLOCK();
			}
		    GeoEqual(&text.boundrectpoint[0],firstpoint);
		    GeoEqual(&text.boundrectpoint[1],lastpoint);

            text.textfont.style=0;
			text.textfont.size=1;
		    text.textfont.forecolor=pline.plinepen.color;
            text.textfont.backcolor=GUI_GetBkColor();
		    text.textangle=atof(argv[1]);
		    sprintf(text.name,"TEXT%d",text.id);
		    GUI_ARRAY_AddItem(&maplayer.textdata.TextArray, &text, sizeof(GText));
		  }
		}

		GUI_ARRAY_AddItem(&maplayer.plinedata.PLineArray, &pline, sizeof(GPLine));
	  }
	  else if((strncmp(line,"Region",6)==0)||(strncmp(line,"region",6)==0))
	  {
		//region数据处理
        parsewithspeccharargs(line, ' ',&argc,argv);
		pointnum1=atoi(argv[1]);
	    
		for(counter1=0;counter1<pointnum1;counter1++)
		{
			memset(&region[counter1],0,sizeof(GRegion));
			GUI_ARRAY_CREATE(&region[counter1].pointarray);
			region[counter1].id=regionindex;
			regionindex++;

			sprintf(region[counter1].name,"REGION%d",region[counter1].id);

			if(counter1==0)
			{

			if(feof(fp)) break;
			memset(line,0,256);
			fgets(line,256,fp);	
			parsewithspeccharargs(line, ' ',&argc,argv);
			}
			pointnum=atoi(argv[0]);
			region[counter1].num_pt=pointnum;
			
			for(counter=0;counter<pointnum;counter++)
			{
			  if(feof(fp)) break;
			  memset(line,0,256);
			  fgets(line,256,fp);
			  parsewithspeccharargs(line, ' ',&argc,argv);
			  point.hgt=0;
			  point.lon=atof(argv[0]);
			  //lat=atof(argv[0]);
			  point.lat=atof(argv[1]);

			  maplayer.m_fMinX = maplayer.m_fMinX < point.lon ? maplayer.m_fMinX : point.lon;
			  maplayer.m_fMaxX = maplayer.m_fMaxX > point.lon ? maplayer.m_fMaxX : point.lon;
			  maplayer.m_fMinY = maplayer.m_fMinY < point.lat ? maplayer.m_fMinY : point.lat;			
			  maplayer.m_fMaxY = maplayer.m_fMaxY > point.lat ? maplayer.m_fMaxY : point.lat;
			  bIvalid=1;
			  GUI_ARRAY_AddItem(&region[counter1].pointarray, &point, sizeof(GeoPoint));
			}
			//pen 
			if(feof(fp)) break;
			memset(line,0,256);
			fgets(line,256,fp);
			if(strstr(line,"Pen")==NULL) 
				continue;
			strcpy(penstr,strchr(line,'('));
			parsewithspeccharargs(penstr+1, ',',&argc,argv);
			
			region[counter1].regionpen.width=atoi(argv[0]);
			region[counter1].regionpen.patter=atoi(argv[1]);
			
			memset(widthstr,0,10);
			len=strlen(argv[2])-1;
			memcpy(widthstr,argv[2],len);
			region[counter1].regionpen.color=atoi(widthstr);
			region[counter1].regionpen.color=CONVERRGB(region[counter1].regionpen.color);
			//brush
			if(feof(fp)) break;
			memset(line,0,256);
			fgets(line,256,fp);
			if(strstr(line,"Brush")==NULL) continue;
			strcpy(penstr,strchr(line,'('));
			parsewithspeccharargs(penstr+1, ',',&argc,argv);
			region[counter1].regionbrush.patter=atoi(argv[0]);
			region[counter1].regionbrush.forcolor=atoi(argv[1]);
			region[counter1].regionbrush.forcolor=CONVERRGB(region[counter1].regionbrush.forcolor);//(region[counter1].regionbrush.forcolor & 0xff00) + ((region[counter1].regionbrush.forcolor & 0xff0000) >> 16) + ((region[counter1].regionbrush.forcolor & 0xff) << 16);

			memset(widthstr,0,10);
			len=strlen(argv[2])-1;
			memcpy(widthstr,argv[2],len);
			region[counter1].regionbrush.backcolor=atoi(widthstr);
			region[counter1].regionbrush.backcolor=CONVERRGB(region[counter1].regionbrush.backcolor);//(region[counter1].regionbrush.backcolor & 0xff00) + ((region[counter1].regionbrush.backcolor & 0xff0000) >> 16) + ((region[counter1].regionbrush.backcolor & 0xff) << 16);

			//center
			if(feof(fp)) break;
			memset(line,0,256);
			fgets(line,256,fp);
			parsewithspeccharargs(line, ',',&argc,argv);
			region[counter1].centerpoint.hgt=0;
			region[counter1].centerpoint.lon=atof(argv[1]);
			region[counter1].centerpoint.lat=atof(argv[2]);
			bIvalid=1;  

		if(fpmid)
		{
		  memset(line,0,256);
          fgets(line,256,fpmid);
		  parsewithspeccharargs(line, ',',&argc,argv);
          if(strncmp(argv[2],"\"",2)!=0)
		  {
	  		memset(&text,0,sizeof(GText));
            text.id=textindex;
		    textindex++;
		    text.text = GUI_ALLOC_AllocZero(strlen(argv[2]) + 1);
            tmptext=strchr(argv[2],'"');		
            if (text.text) 
			{
              WM_LOCK();
              strcpy((char*) GUI_ALLOC_h2p(text.text), tmptext+1);
		      WM_UNLOCK();
			}
		    GeoEqual(&text.boundrectpoint[0],region[counter1].centerpoint);
		    GeoEqual(&text.boundrectpoint[1],region[counter1].centerpoint);

            text.textfont.style=0;
			text.textfont.size=10;
		    text.textfont.forecolor=region[counter1].regionpen.color;
            text.textfont.backcolor=GUI_GetBkColor();
		    text.textangle=atof(argv[1]);
		    sprintf(text.name,"TEXT%d",text.id);
		    GUI_ARRAY_AddItem(&maplayer.textdata.TextArray, &text, sizeof(GText));
		  }
		}

			if((counter1==pointnum1-1)&&(pointnum1>1))
			{
			    for(counter=0;counter<pointnum1;counter++)
				{
				    if(counter!=counter1)
					{
				       region[counter].regionpen.width=region[counter1].regionpen.width;
			           region[counter].regionpen.patter=region[counter1].regionpen.patter;
			           region[counter].regionpen.color=region[counter1].regionpen.color;
					   region[counter].regionbrush.patter=region[counter1].regionbrush.patter;
			           region[counter].regionbrush.forcolor=region[counter1].regionbrush.forcolor;
			           region[counter].regionbrush.forcolor=region[counter1].regionbrush.forcolor;
					   region[counter].regionbrush.backcolor=region[counter1].regionbrush.backcolor;
			           region[counter].regionbrush.backcolor=region[counter1].regionbrush.backcolor;

			           region[counter].centerpoint.hgt=region[counter1].centerpoint.hgt;
			           region[counter].centerpoint.lon=region[counter1].centerpoint.lon;
			           region[counter].centerpoint.lat=region[counter1].centerpoint.lat;
					}
					GUI_ARRAY_AddItem(&maplayer.regiondata.RegionArray, &region[counter], sizeof(GRegion));
				}
			}
			else
			{
					GUI_ARRAY_AddItem(&maplayer.regiondata.RegionArray, &region[0], sizeof(GRegion));
			}
		}
	  }else if((strncmp(line,"Text",4)==0)||(strncmp(line,"text",4)==0))
	  {
		memset(&text,0,sizeof(GText));
        text.id=textindex;
		textindex++;
        //text string
        if(feof(fp)) break;
	    memset(line,0,256);
        fgets(line,256,fp);

        text.text = GUI_ALLOC_AllocZero(strlen(line) + 1);
		line[strlen(line)-2]='\0';
        tmptext=strchr(line,'"');		
		
        if (text.text) {
          WM_LOCK();
          strcpy((char*) GUI_ALLOC_h2p(text.text), tmptext+1);
		  WM_UNLOCK();
		}
        //text rect
        if(feof(fp)) break;
	    memset(line,0,256);
        fgets(line,256,fp);
        parsewithspeccharargs(line, ' ',&argc,argv);
        text.boundrectpoint[0].hgt=0;
        text.boundrectpoint[0].lon=atof(argv[0]);
        text.boundrectpoint[0].lat=atof(argv[1]);
        text.boundrectpoint[1].lon=atof(argv[2]);
        text.boundrectpoint[1].lat=atof(argv[3]);
        text.boundrectpoint[1].hgt=0;
 		
		maplayer.m_fMinX = maplayer.m_fMinX < text.boundrectpoint[0].lon ? maplayer.m_fMinX : text.boundrectpoint[0].lon;
		maplayer.m_fMaxX = maplayer.m_fMaxX > text.boundrectpoint[0].lon ? maplayer.m_fMaxX : text.boundrectpoint[0].lon;
		maplayer.m_fMinY = maplayer.m_fMinY < text.boundrectpoint[0].lat ? maplayer.m_fMinY : text.boundrectpoint[0].lat;			
		maplayer.m_fMaxY = maplayer.m_fMaxY > text.boundrectpoint[0].lat ? maplayer.m_fMaxY : text.boundrectpoint[0].lat;

		maplayer.m_fMinX = maplayer.m_fMinX < text.boundrectpoint[1].lon ? maplayer.m_fMinX : text.boundrectpoint[1].lon;
		maplayer.m_fMaxX = maplayer.m_fMaxX > text.boundrectpoint[1].lon ? maplayer.m_fMaxX : text.boundrectpoint[1].lon;
		maplayer.m_fMinY = maplayer.m_fMinY < text.boundrectpoint[1].lat ? maplayer.m_fMinY : text.boundrectpoint[1].lat;			
		maplayer.m_fMaxY = maplayer.m_fMaxY > text.boundrectpoint[1].lat ? maplayer.m_fMaxY : text.boundrectpoint[1].lat;



		//font
        if(feof(fp)) break;
	    memset(line,0,256);
        fgets(line,256,fp);
 		if(strstr(line,"Font")==NULL) 
		{   
			bIvalid=1;
			GUI_ARRAY_AddItem(&maplayer.textdata.TextArray, &text, sizeof(GText));
			goto retry;
        }
		strcpy(penstr,strchr(line,'(')+1);
        parsewithspeccharargs(penstr, ',',&argc,argv);
        sprintf(text.textfont.fontname,"%s",argv[0]);
        text.textfont.style=atoi(argv[1]);
		text.textfont.size=atoi(argv[2]);
		argv[3][strlen(argv[3])-2]='\0';
		text.textfont.forecolor=atoi(argv[3]);
		text.textfont.forecolor=CONVERRGB(text.textfont.forecolor);
        text.textfont.backcolor=GUI_GetBkColor();
		//angle
        if(feof(fp)) break;
	    memset(line,0,256);
        fgets(line,256,fp);
 		if(strstr(line,"Angle")==NULL) 
		{
			bIvalid=1;
			GUI_ARRAY_AddItem(&maplayer.textdata.TextArray, &text, sizeof(GText));
			goto retry;
        }
        parsewithspeccharargs(line, ',',&argc,argv);
		text.textangle=atof(argv[1]);
		sprintf(text.name,"TEXT%d",text.id);
		bIvalid=1;
		GUI_ARRAY_AddItem(&maplayer.textdata.TextArray, &text, sizeof(GText));

	  }else if((strncmp(line,"Line",4)==0)||(strncmp(line,"line",4)==0))
	  {
		  //Line 104.103427 30.723094 104.103863 30.724208
          //Pen (2,2,16711680) 
		memset(&gline,0,sizeof(GLine));
        gline.id=lineindex;
		lineindex++;
        //line  point
        parsewithspeccharargs(line, ' ',&argc,argv);
        gline.point[0].hgt=0;
        gline.point[0].lon=atof(argv[1]);
        gline.point[0].lat=atof(argv[2]);
        gline.point[1].lon=atof(argv[3]);
        gline.point[1].lat=atof(argv[4]);
        gline.point[1].hgt=0;
 		
		maplayer.m_fMinX = maplayer.m_fMinX < gline.point[0].lon ? maplayer.m_fMinX : gline.point[0].lon;
		maplayer.m_fMaxX = maplayer.m_fMaxX > gline.point[0].lon ? maplayer.m_fMaxX : gline.point[0].lon;
		maplayer.m_fMinY = maplayer.m_fMinY < gline.point[0].lat ? maplayer.m_fMinY : gline.point[0].lat;			
		maplayer.m_fMaxY = maplayer.m_fMaxY > gline.point[0].lat ? maplayer.m_fMaxY : gline.point[0].lat;
		
		maplayer.m_fMinX = maplayer.m_fMinX < gline.point[1].lon ? maplayer.m_fMinX : gline.point[1].lon;
		maplayer.m_fMaxX = maplayer.m_fMaxX > gline.point[1].lon ? maplayer.m_fMaxX : gline.point[1].lon;
		maplayer.m_fMinY = maplayer.m_fMinY < gline.point[1].lat ? maplayer.m_fMinY : gline.point[1].lat;			
		maplayer.m_fMaxY = maplayer.m_fMaxY > gline.point[1].lat ? maplayer.m_fMaxY : gline.point[1].lat;

		//Pen
        if(feof(fp)) break;
	    memset(line,0,256);
        fgets(line,256,fp);
 		if(strstr(line,"Pen")==NULL) 
		{   
			bIvalid=1;
			GUI_ARRAY_AddItem(&maplayer.linedata.LineArray, &gline, sizeof(GLine));
			goto retry;
        }
		strcpy(penstr,strchr(line,'('));
		parsewithspeccharargs(penstr+1, ',',&argc,argv);
		gline.linepen.width=atoi(argv[0]);
		gline.linepen.patter=atoi(argv[1]);
		memset(widthstr,0,10);
	    len=strlen(argv[2])-1;
		memcpy(widthstr,argv[2],len);
		gline.linepen.color=atoi(widthstr);
		gline.linepen.color=CONVERRGB(gline.linepen.color);
		sprintf(gline.name,"LINE%d",gline.id);
		bIvalid=1;

		if(fpmid)
		{
		  memset(line,0,256);
          fgets(line,256,fpmid);
		  parsewithspeccharargs(line, ',',&argc,argv);
          if(strncmp(argv[2],"\"",2)!=0)
		  {
	  		memset(&text,0,sizeof(GText));
            text.id=textindex;
		    textindex++;
		    text.text = GUI_ALLOC_AllocZero(strlen(argv[2]) + 1);
            tmptext=strchr(argv[2],'"');		
            if (text.text) 
			{
              WM_LOCK();
              strcpy((char*) GUI_ALLOC_h2p(text.text), tmptext+1);
		      WM_UNLOCK();
			}
		    GeoEqual(&text.boundrectpoint[0],gline.point[0]);
		    GeoEqual(&text.boundrectpoint[1],gline.point[1]);

            text.textfont.style=0;
			text.textfont.size=10;
		    text.textfont.forecolor=gline.linepen.color;
            text.textfont.backcolor=GUI_GetBkColor();
		    text.textangle=atof(argv[1]);
		    sprintf(text.name,"TEXT%d",text.id);
		    GUI_ARRAY_AddItem(&maplayer.textdata.TextArray, &text, sizeof(GText));
		  }
		}
		GUI_ARRAY_AddItem(&maplayer.linedata.LineArray, &gline, sizeof(GLine));
	  }else if((strncmp(line,"Point",4)==0)||(strncmp(line,"point",4)==0))
	  {
        //Point 104.013958 30.673823
        //Symbol (35,8421631,18) 		
	    memset(&gline,0,sizeof(GLine));
        gpoint.id=pointindex;
		pointindex++;
        // point
        parsewithspeccharargs(line, ' ',&argc,argv);
        gpoint.hgt=0;
        gpoint.lon=atof(argv[1]);
        gpoint.lat=atof(argv[2]);
 		
		maplayer.m_fMinX = maplayer.m_fMinX < gpoint.lon ? maplayer.m_fMinX : gpoint.lon;
		maplayer.m_fMaxX = maplayer.m_fMaxX > gpoint.lon ? maplayer.m_fMaxX : gpoint.lon;
		maplayer.m_fMinY = maplayer.m_fMinY < gpoint.lat ? maplayer.m_fMinY : gpoint.lat;			
		maplayer.m_fMaxY = maplayer.m_fMaxY > gpoint.lat ? maplayer.m_fMaxY : gpoint.lat;

		//Symbol
        //Symbol (35,8421631,18) 		
        if(feof(fp)) break;
	    memset(line,0,256);
        fgets(line,256,fp);
 		if(strstr(line,"Symbol")==NULL) 
		{   
			bIvalid=1;
			GUI_ARRAY_AddItem(&maplayer.pointdata.PointArray, &gpoint, sizeof(GPoint));
			goto retry;
        }
		strcpy(penstr,strchr(line,'('));
		parsewithspeccharargs(penstr+1, ',',&argc,argv);
		gpoint.ponitsymbol.shape=atoi(argv[0]);
		gpoint.ponitsymbol.color=CONVERRGB(atoi(argv[1]));
		
		memset(widthstr,0,10);
	    len=strlen(argv[2])-1;
		memcpy(widthstr,argv[2],len);
		gpoint.ponitsymbol.size=atoi(widthstr);
		sprintf(gpoint.name,"POINT%d",gpoint.id);
		bIvalid=1;

		if(fpmid)
		{
		  memset(line,0,256);
          fgets(line,256,fpmid);
		  parsewithspeccharargs(line, ',',&argc,argv);
          if(strncmp(argv[2],"\"",2)!=0)
		  {
	  		memset(&text,0,sizeof(GText));
            text.id=textindex;
		    textindex++;
		    text.text = GUI_ALLOC_AllocZero(strlen(argv[2]) + 1);
            tmptext=strchr(argv[2],'"');		
            if (text.text) 
			{
              WM_LOCK();
              strcpy((char*) GUI_ALLOC_h2p(text.text), tmptext+1);
		      WM_UNLOCK();
			}
            point.hgt=gpoint.hgt;
			point.lat=gpoint.lat;
			point.lon=gpoint.lon;
		    GeoEqual(&text.boundrectpoint[0],point);
		    GeoEqual(&text.boundrectpoint[1],point);

            text.textfont.style=0;
			text.textfont.size=10;
		    text.textfont.forecolor=gpoint.ponitsymbol.color;
            text.textfont.backcolor=GUI_GetBkColor();
		    text.textangle=atof(argv[1]);
		    sprintf(text.name,"TEXT%d",text.id);
		    GUI_ARRAY_AddItem(&maplayer.textdata.TextArray, &text, sizeof(GText));
		  }
		}

		GUI_ARRAY_AddItem(&maplayer.pointdata.PointArray, &gpoint, sizeof(GPoint));
	  }

	} 
    fclose(fp);
	if(fpmid)
      fclose(fpmid);
	if(bIvalid==1)
	{
	sprintf(maplayer.plinedata.name,"%s","PLINE");
    maplayer.plinedata.num_ln=plineindex;
	sprintf(maplayer.regiondata.name,"%s","REGION");
    maplayer.regiondata.num_pg=regionindex;
	sprintf(maplayer.textdata.name,"%s","TEXT");
    maplayer.textdata.num_pg=textindex;
	sprintf(maplayer.linedata.name,"%s","LINE");
    maplayer.linedata.num_ln=lineindex;
	sprintf(maplayer.pointdata.name,"%s","POINT");
    maplayer.pointdata.num_pt=pointindex;

    GUI_ARRAY_AddItem(&mymap.MaplayerArray,&maplayer, sizeof(GMapLayer));
	}
	else
	{
    GUI_ARRAY_Delete(&maplayer.pointdata.PointArray);
	GUI_ARRAY_Delete(&maplayer.linedata.LineArray);
    GUI_ARRAY_Delete(&maplayer.plinedata.PLineArray);
    GUI_ARRAY_Delete(&maplayer.regiondata.RegionArray);
    GUI_ARRAY_Delete(&maplayer.arcdata.ArcArray);
    GUI_ARRAY_Delete(&maplayer.textdata.TextArray);
    GUI_ARRAY_Delete(&maplayer.rectdata.RectangleArray);
    GUI_ARRAY_Delete(&maplayer.roundrectdata.RoundedRectangleArray);
    GUI_ARRAY_Delete(&maplayer.ellipsedata.EllipseArray);

	}
}
*/
void InitMap()
{
    OpenCom(1,9600);
	memset(&mymap,0,sizeof(GMapLayerType)); 
    GUI_ARRAY_CREATE(&mymap.MaplayerArray);
	sprintf(mymap.name,"%s","温州");
}

void AnalyzeMapCoordSys(int xpixels,int ypixels)
{
	char mystr[300];
	 //4.比较点,线,多边形的点坐标(x,y)的最大值,最小值,得到x_max,y_max,x_min,y_min
     double x_max ,y_max ,x_min ,y_min;
	 //5.计算重心坐标和x,y轴缩放因子Mx,My
     double delta_x,delta_y,Mx,My;
	 int nlayers,npline,nport;
	 GeoPoint* ppoint;
     GPLine* ppline;
	 GRegion* pregion;
	 GMapLayer*  pmaplayer;
   	 //赋初值
	 WM_LOCK();
     y_max=x_max=-600000;
	 x_min=y_min=600000;
	 for(nlayers=0;nlayers<mymap.num_pg;nlayers++)
	 {
         //
		 pmaplayer=(GMapLayer*)GUI_ARRAY_GetpItem(&mymap.MaplayerArray,nlayers);
		 if(pmaplayer==NULL) continue;
		 
		 x_max = x_max < pmaplayer->m_fMaxX ? pmaplayer->m_fMaxX:x_max;
	     y_max = y_max < pmaplayer->m_fMaxY ? pmaplayer->m_fMaxY:y_max;
	     x_min = x_min > pmaplayer->m_fMinX ? pmaplayer->m_fMinX:x_min;
	     y_min = y_min > pmaplayer->m_fMinY ? pmaplayer->m_fMinY:y_min;
	 }
	 delta_x = (x_max - x_min);
     delta_y = (y_max - y_min);

	 Mx = xpixels/delta_x;
	 My = ypixels/delta_y;
     //InitCoordSys(y_min,y_max,x_min,x_max,Mx,My,xpixels,ypixels,delta_x,delta_y);
	 //::MessageBox(NULL,)
	   //sprintf(mystr,"%f %f %f %f %f %f %f %f %f %f\r\n",x_min,x_max,y_min,y_max,Mx,My,xpixels,ypixels,delta_x,delta_y);
	   //GUI_DispStringAt(mystr,0,50);
	   //GUI_Delay(1000);
	 InitCoordSys(x_min,x_max,y_min,y_max,Mx,My,xpixels,ypixels,delta_x,delta_y);
     WM_UNLOCK();
}

void DrawCurrentPos(char *gpsdata);
GUI_BITMAP bmwhite;
GUI_BITMAP bmuCar1;
GUI_CONST_STORAGE GUI_BITMAP bmuCCar1;
GUI_CONST_STORAGE GUI_BITMAP bmHouse;
GUI_CONST_STORAGE GUI_BITMAP bmComp;
GUI_CONST_STORAGE GUI_BITMAP bmblack;
GeoPoint  curpoint;
TIME_STRUC    curtime;
#define CURRENTDIR  "\\gpsdata\\"
FILE *gpsfp;

void SendGpsData(char * filename)
{
	char line[256]; 
	if((gpsfp=fopen(filename,"rt"))==NULL)
	{
		return;
	}
    //while(1) 
    { 
retry:
	  if(feof(gpsfp)) return;
	  memset(line,0,256);
      fgets(line,256,gpsfp);
	  if(strstr(line,"$GPRMC")==NULL) goto retry;
	  sprintf(line,"%s\r\n",line);
      DrawCurrentPos(line); 
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
	  memset(line,0,256);
      fgets(line,256,gpsfp);
	  if(strstr(line,"$GPRMC")==NULL) goto retry;
	  sprintf(line,"%s\r\n",line);
      DrawCurrentPos(line); 
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
		  //load_mapinfo(currpath);
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

void RefreshWeek(TIME_STRUC *time)
{
   INT32U Y,M,D,A;
   Y=time->year;
   M=time->month;
   D=time->day;

   if ((M == 1) || (M == 2)){

   M += 12;

   Y--;

   }
   
   A = ((D + (2*M) + ((3*(M+1))/5) + Y + (Y/4) - (Y/100) + (Y/400)) % 7)+1;
   
   time->weekday= A;

}

void DrawCurrentPos(char *gpsdata)
{
	//char gpsstr[]="GPRMC,082653.010,A,3037.3840,N,10402.3986,E,000.0,062.3,210705,001.4,W*79";
    char *argv[20];
    int argc;
    int i,x,y,integer;
	double speed,mm,sina,cosa,direct,angel;
    char tempgpsstr[100];
    //sprintf(tempgpsstr,"%s","GPRMC,125302.000,A,3035.1441,N,10411.4546,E,0.15,345.50,201006,,*01");
	//sprintf(tempgpsstr,"%s","GPRMC,062057.253,A,3040.5620,N,10400.6415,E,0.00,,071106,,,A*76,0,0");
    //sprintf(tempgpsstr,"%s","GPRMC,082653.010,A,3037.3840,N,10402.3986,E,000.0,062.3,210705,001.4,W*79");
    //sprintf(tempgpsstr,"%s","GPRMC,055313.352,A,3040.6217,N,10401.1738,E,9.52,204.01,071106,,,A*6F,0,0");
	sprintf(tempgpsstr,"%s",gpsdata);//"GPRMC,073111.951,A,3037.3496,N,10402.3289,E,000.0,-01.0,071106,002.7,W*79");
//	mycom.SendData(tempgpsstr,strlen(tempgpsstr));
    SendString(gpsdata);
	parsewithspeccharargs(tempgpsstr,',',&argc,argv);
    if(argc>=12)
	{
/* 
1) 标准定位时间（UTC time）格式：时时分分秒秒.秒秒秒（hhmmss.sss）。 
2) 定位状态，A = 数据可用，V = 数据不可用。 
3) 纬度，格式：度度分分.分分分分（ddmm.mmmm）。 
4) 纬度区分，北半球（N）或南半球（S）。 
5) 经度，格式：度度分分.分分分分。
6) 经度区分，东（E）半球或西（W）半球。 
7) 相对位移速度， 0.0 至 1851.8 knots   Knots=0.514m/s
8) 相对位移方向，000.0 至 359.9度。实际值。 
9) 日期，格式：日日月月年年（ddmmyy）。 
10) 磁极变量，000.0 至180.0。 
//11) 度数。 
11) Checksum.(检查位) 
*/
   //if(argv[0]
   //时间
   if(argv[1]!="")
   {
       curtime.hour= (argv[1][0]-0x30)*10+(argv[1][1]-0x30);

	  curtime.minu=(argv[1][2]-0x30)*10+(argv[1][3]-0x30);

	  curtime.sec=(argv[1][4]-0x30)*10+(argv[1][5]-0x30);   
   }
   //定位属性
   if(argv[2]!="")
   {
   }
   //
   if(argv[3]!="")
   {
   	curpoint.lat = atof(argv[3]);
    integer=(int)(curpoint.lat/100);
    mm=curpoint.lat-integer*100;
    mm/=60;
    curpoint.lat=integer+mm;

	//curpoint.lat /=100;
    //curpoint.lat+=0.3;
   }
   if(argv[4]!="")
   {
    if(argv[4][0] == 'S') curpoint.lat = -curpoint.lat;
   }
   if(argv[5]!="")
   {
    curpoint.lon = atof(argv[5]);
    integer=(int)(curpoint.lon/100);
    mm=curpoint.lon-integer*100;
    mm/=60;
    curpoint.lon=integer+mm;
   }
   if(argv[6]!="")
   {
    if(argv[6][0] == 'W') curpoint.lon = -curpoint.lon;
   }
   if(argv[7]!="")
   {
   	    speed=atof(argv[7]);
	    speed*=1.852f;
	    //speed/=1000;//公里每小时
	    //GPSMilAdd((INT16U)(GpsInfo.VelHS.speed.f));  

   }
   else
	    speed=0.00f;
   
   if(argv[8]!="")
   {
     direct=atof(argv[8]);
	    integer=(int)(direct/100);
    mm=direct-integer*100;
    mm/=60;
    direct=integer+mm;

   }
   if(argv[9]!="")
   {
    if(strlen(argv[9])>=6)
    {

	 curtime.day=(argv[9][0]-0x30)*10+(argv[9][1]-0x30);
     
     curtime.month=(argv[9][2]-0x30)*10+(argv[9][3]-0x30);
    
     curtime.year=(argv[9][4]-0x30)*10+(argv[9][5]-0x30)+2000;
     RefreshWeek(&curtime);
    }
   }
   if(argv[10]!="")
   {
   }
   curpoint.hgt=speed;
   GUI_SetColor(GUI_RED);//pregion->regionpen.color);
   ConvToXY(curpoint.lon,curpoint.lat,&x,&y);
   //GUI_DrawBitmap((GUI_BITMAP*)&bmCar,x,y);
   //GL_FillCircle(x,y,3);
   //GUI_DrawBitmapEx(&bmuCCar1,x,y,bmuCCar1.XSize/2-1,bmuCCar1.YSize/2-1, 1*cos(75),1);
   //GUI_DrawBitmap((GUI_BITMAP*)&bmuCCar1,x,y);
   
   angel=((direct*3)/180)*3.14159;
   sina=sin(angel);
   cosa=cos(angel);

   GUI_DrawBitmap((GUI_BITMAP*)&bmComp,1,1);

   GUI_DrawBitmapCircleRotate(&bmuCCar1,x,y,45,sina,cosa);
   //GUI_DrawBMP(bmwhite)
   //sprintf(tempgpsstr,"cur lon %f  lat %f",curpoint.lon,curpoint.lat);
   //GUI_DispStringAt(tempgpsstr,300,470);
   GUI_DispStringAt("当前位置",x+5,y);
 }
 else
 {
 }

}
extern int m_hScreen,m_wScreen;
extern GUI_MEMDEV_Handle hMem;
int xx=0,yy=0;
float Angle=0;

//extern  NEEDLE  _Needle[4] ;
extern void DrawNeedle();
void DrawMap()
{
     static GUI_POINT aPoint[7];
    float Angle ;
 	int nlayers,npline,nport;
	 int x,y;
	COLOR  color;
	 GUI_POINT guipoint[5000];
	 GUI_COLOR oldbkcolor,oldcolor;
	 GUI_RECT rect;
	 GeoPoint* ppoint;
     GPLine* ppline;
	 GPoint* pgpoint;
	 GRegion*pregion;
	 GText* ptext;
     GLine* pgline;  
	 char *text;

	 GMapLayer*  pmaplayer;
 	 int NumPoints,Shape,Radius;
	 GUI_Clear();
	 WM_LOCK();
	 
		 //region
	 for(nlayers=0;nlayers<mymap.num_pg;nlayers++)
	 {
		oldcolor=GUI_GetColor();
		oldbkcolor=GUI_GetBkColor();

		 pmaplayer=(GMapLayer*)GUI_ARRAY_GetpItem(&mymap.MaplayerArray,nlayers);
		 if(pmaplayer==NULL) continue;

         for(npline=0;npline<pmaplayer->regiondata.num_pg;npline++)
		 {
		     pregion=(GRegion*)GUI_ARRAY_GetpItem(&pmaplayer->regiondata.RegionArray,npline);
             if(pregion==NULL) continue;
			
			 GUI_SetColor(pregion->regionbrush.forcolor);//pregion->regionpen.color);
			 GUI_SetBkColor(pregion->regionbrush.backcolor);
			 GUI_SetPenSize(pregion->regionpen.width);
			 for(nport=0;nport<pregion->num_pt;nport++)
			 {
			     ppoint=(GeoPoint*)GUI_ARRAY_GetpItem(&pregion->pointarray,nport);
				 ConvToXY(ppoint->lon,ppoint->lat,&x,&y);
  				 guipoint[nport].x=x;
                 guipoint[nport].y=y;
 			 }
			 ConvToXY(pregion->centerpoint.lon,pregion->centerpoint.lat,&x,&y);
			 GUI_FillPolygon(guipoint,nport,0,0);
		 }
		 GUI_SetBkColor(oldbkcolor);
		 GUI_SetColor(oldcolor);
	 }

   //line
	 for(nlayers=0;nlayers<mymap.num_pg;nlayers++)
	 {
		 oldcolor=GUI_GetColor();
		 oldbkcolor=GUI_GetBkColor();

		 pmaplayer=(GMapLayer*)GUI_ARRAY_GetpItem(&mymap.MaplayerArray,nlayers);
		 if(pmaplayer==NULL) continue;
         //text 
		 for(npline=0;npline<pmaplayer->linedata.num_ln;npline++)
		 {

			 pgline=(GText*)GUI_ARRAY_GetpItem(&pmaplayer->linedata.LineArray,npline);
		     if(pgline==NULL) continue;
             GUI_GetColor(pgline->linepen.color);
             GUI_SetPenSize(pgline->linepen.width);
			 ConvToXY(pgline->point[0].lon,pgline->point[0].lat,&x,&y);
 		     GUI_MoveTo(x,y);
			 ConvToXY(pgline->point[1].lon,pgline->point[1].lat,&x,&y);
             GUI_DrawLineTo(x,y); 

		 }
	   GUI_SetBkColor(oldbkcolor);
	   GUI_SetColor(oldcolor);
	 }
 
        //pline
	 for(nlayers=0;nlayers<mymap.num_pg;nlayers++)
	 {
		 oldcolor=GUI_GetColor();
		 oldbkcolor=GUI_GetBkColor();

		 pmaplayer=(GMapLayer*)GUI_ARRAY_GetpItem(&mymap.MaplayerArray,nlayers);
		 if(pmaplayer==NULL) continue;
 
		 for(npline=0;npline<pmaplayer->plinedata.num_ln;npline++)
		 {
		     ppline=(GPLine*)GUI_ARRAY_GetpItem(&pmaplayer->plinedata.PLineArray,npline);
             if(ppline==NULL) continue;

			 GUI_SetColor(ppline->plinepen.color);
			 GUI_SetPenSize(ppline->plinepen.width+1);
			 for(nport=0;nport<ppline->num_pt;nport++)
			 {
			     ppoint=(GeoPoint*)GUI_ARRAY_GetpItem(&ppline->pointarray,nport);
				 ConvToXY(ppoint->lon,ppoint->lat,&x,&y);
				 
				 if(nport==0)
				   GUI_MoveTo(x,y);
                 else
                   GUI_DrawLineTo(x,y); 
 			 }
		 }
		 GUI_SetBkColor(oldbkcolor);
		 GUI_SetColor(oldcolor);
	 }

         //text 
	 for(nlayers=0;nlayers<mymap.num_pg;nlayers++)
	 {
		 oldcolor=GUI_GetColor();
		 oldbkcolor=GUI_GetBkColor();

		 pmaplayer=(GMapLayer*)GUI_ARRAY_GetpItem(&mymap.MaplayerArray,nlayers);
		 if(pmaplayer==NULL) continue;

		 for(npline=0;npline<pmaplayer->textdata.num_pg;npline++)
		 {

			 ptext=(GText*)GUI_ARRAY_GetpItem(&pmaplayer->textdata.TextArray,npline);
		     if(ptext==NULL) continue;
			 if (ptext->text) {
               text = (const char*) GUI_ALLOC_h2p(ptext->text);
			   if(strstr(text,".")!=NULL) continue;
			   GUI_SetColor(ptext->textfont.forecolor);
               GUI_SetPenSize(ptext->textfont.size);
               ConvToXY(ptext->boundrectpoint[0].lon,ptext->boundrectpoint[0].lat,&x,&y);
			   rect.x0=x;
			   rect.y1=y;
               ConvToXY(ptext->boundrectpoint[1].lon,ptext->boundrectpoint[1].lat,&x,&y);
			   rect.x1=x;
			   rect.y0=y;
			   GUI_SetTextMode(GUI_TEXTMODE_TRANS);
			   if(rect.y1!=rect.y0)
                 GUI_DispStringInRect(text,&rect,GUI_TA_CENTER);  
			   else
			     GUI_DispStringAt(text,rect.x0,rect.y0);

			 }
		 }
	   GUI_SetBkColor(oldbkcolor);
	   GUI_SetColor(oldcolor);
	 }
         //point 
	 for(nlayers=0;nlayers<mymap.num_pg;nlayers++)
	 {
		 oldcolor=GUI_GetColor();
		 oldbkcolor=GUI_GetBkColor();

		 pmaplayer=(GMapLayer*)GUI_ARRAY_GetpItem(&mymap.MaplayerArray,nlayers);
		 if(pmaplayer==NULL) continue;

		 for(npline=0;npline<pmaplayer->pointdata.num_pt;npline++)
		 {

			 pgpoint=(GText*)GUI_ARRAY_GetpItem(&pmaplayer->pointdata.PointArray,npline);
		     if(pgpoint==NULL) continue;
			 GUI_SetColor(pgpoint->ponitsymbol.color);
               //GUI_SetPenSize(ptext->textfont.size);
             ConvToXY(pgpoint->lon,pgpoint->lat,&x,&y);
			 GL_FillCircle(x,y,2);
			 //GUI_DrawBitmap((GUI_BITMAP*)&bmComp,x,y);
		 }
	   GUI_SetBkColor(oldbkcolor);
	   GUI_SetColor(oldcolor);
	 }
	 
	 if(ReadGpsData()==0)
	 {
       closegpsfile();
	   if(GetNextFile()==0)
	   {
		  EndGpsFile();
		  GetGpsFileList() ;
	   }
	 }
     
     //DrawCurrentPos();
    //Angle = 0.02 * (float)2;
   // GUI_RotatePolygon(aPoint, _aArrow, 7, Angle);
   /// GUI_SetColor(GUI_WHITE);
  //  GUI_FillPolygon(&aPoint[0], 7, 10, 10);

     //GUI_SetDrawMode()
     //GUI_DrawBitmap((GUI_BITMAP*)&bmComp,10,10);
	 //GUI_DrawBitmap((GUI_BITMAP*)&bmblack,10,10);
//	   int NumPoints, Shape;
//  
	 /*
     color.Sep[0]=0xE8;
	 color.Sep[1]=0xE8;
	 color.Sep[2]=0xFF;
     GUI_SetColor(_CalcColor(&color));
	 //Angle=90;
	 if(Angle++>180) Angle=0;
     Radius=50;
     Shape = 1;//pObj->NeedleType;
     NumPoints = GUI_COUNTOF(_aNeedleSrc[Shape]);
     _MagnifyPolygon(_aNeedle, _aNeedleSrc[Shape], NumPoints, Radius / 100.);
     GUI_RotatePolygon(_aNeedle, _aNeedle, GUI_COUNTOF(_aNeedleSrc[Shape]), Angle);
	 _AA_DrawPolygon(_aNeedle, NumPoints, 100 * FACTOR, 100 * FACTOR);
*/
     
     WM_UNLOCK();
	 // GUI_Clear();
    // GUI_MEMDEV_CopyToLCDAt(hMem, m_wScreen,m_hScreen);
}

void CloseMap()
{
//	  mycom.Close();
}
#define outportb _outp
#define inportb _inp

#define TXR             0       /*  Transmit register (WRITE) */
#define RXR             0       /*  Receive register  (READ)  */
#define IER             1       /*  Interrupt Enable          */
#define IIR             2       /*  Interrupt ID              */
#define LCR             3       /*  Line control              */
#define MCR             4       /*  Modem control             */
#define LSR             5       /*  Line Status               */
#define MSR             6       /*  Modem Status              */
#define DLL             0       /*  Divisor Latch Low         */
#define DLH             1       /*  Divisor latch High        */

#define FIFOCON         2       /*  FIFOCON                   */
int comport=1;
unsigned comportbase=0x3f8;
unsigned int gmode=0;
unsigned int fmode=0;
unsigned int rmode=0;
unsigned int divider=3;
unsigned int noinit=0;
char *rxbuf;
int timeoutlimit=100000;

//baud rate. n=12(9600),6(19200),3(38400),2(57600),1(115200)\n");	
void OpenCom(int comport,int baudrate)
{

      unsigned char temp;
	  return;
	  divider=12;
	  switch(comport)
		{
		case 1:
			comport=0;
		    comportbase=0x3f8;  //COM1
 		    break;
	    case 2:
		    comport=1;
		    comportbase=0x2f8;  //COM2
		    break;
		default:
			comport=0;
		    comportbase=0x3f8;  //COM1
			break;
		}

        outportb((unsigned short)(comportbase+LCR),0x3);

        temp = inportb((unsigned short)(comportbase+LCR));
        outportb((unsigned short)(comportbase+LCR),(int)temp | 0x80);
        outportb((unsigned short)(comportbase+DLH),(int)(divider/256)); /* 1843200/(divider*16) */
        outportb((unsigned short)(comportbase+DLL),(int)divider%256);
        outportb((unsigned short)(comportbase+LCR),(int)temp);
}
void istbe(void)
{
    //while(    (  ( (unsigned int)bioscom(3,0,comport) ) & 0x2000  )==0   );
    while((inportb((unsigned short)(comportbase+LSR)) & 0x20) == 0);
}

		void sendbyte(unsigned int data)
{
  
        if(comport>=0)
        {
            istbe();
            outportb((unsigned short)(comportbase+TXR),(int)data);
        }
}




void _sendchar(unsigned int data)
{
            istbe();
            outportb((unsigned short)(comportbase+TXR),(int)data);
}


char _receivechar(void)
{
    while((inportb((unsigned short)(comportbase+LSR)) & 0x1)==0);
	return inportb((unsigned short)(comportbase+RXR));
}
void SendString(char *str)
{
	unsigned char ch;
    return;
	while(ch=*str++)
	{
      sendbyte(ch);
	}
}




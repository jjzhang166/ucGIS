#define GPSDATAINFO_GLOBALES
#include "gpsdatainfo.h"
GpsDataInfo * GpsDataInfoSetValue(GpsDataInfo *gpsinfo1,   GpsDataInfo gpsinfo)
{
	gpsinfo1->m_xx = gpsinfo.m_xx;
	gpsinfo1->m_yy = gpsinfo.m_yy;
	gpsinfo1->m_nSpeed = gpsinfo.m_nSpeed;
	gpsinfo1->m_fAngle = gpsinfo.m_fAngle;
	gpsinfo1->m_stTime = gpsinfo.m_stTime;
	memcpy(&gpsinfo1->m_stUTCTime,&gpsinfo.m_stUTCTime,sizeof(TIME_STRUC));

	strcpy(gpsinfo1->m_szXX,gpsinfo.m_szXX);
	strcpy(gpsinfo1->m_szYY,gpsinfo.m_szYY);
	strcpy(gpsinfo1->m_szSpeed,gpsinfo.m_szSpeed);
	strcpy(gpsinfo1->m_szAngle,gpsinfo.m_szAngle);

	gpsinfo1->m_nStCount = gpsinfo.m_nStCount;
	gpsinfo1->m_dbHeigth = gpsinfo.m_dbHeigth;
	gpsinfo1->m_nState = gpsinfo.m_nState;
	gpsinfo1->nDataType = gpsinfo.nDataType;
	gpsinfo1->bNetData = gpsinfo.bNetData;
	gpsinfo1->m_nCarID = gpsinfo.m_nCarID;
	
	return gpsinfo1;
}

GpsDataInfo * GpsDataInfoSetpValue(GpsDataInfo *gpsinfo1,   GpsDataInfo* pGpsinfo)
{

	gpsinfo1->m_xx = pGpsinfo->m_xx;
	gpsinfo1->m_yy = pGpsinfo->m_yy;
	gpsinfo1->m_nSpeed = pGpsinfo->m_nSpeed;
	gpsinfo1->m_fAngle = pGpsinfo->m_fAngle;
	gpsinfo1->m_stTime = pGpsinfo->m_stTime;
	memcpy(&gpsinfo1->m_stUTCTime,&pGpsinfo->m_stUTCTime,sizeof(TIME_STRUC));

	strcpy(gpsinfo1->m_szXX,pGpsinfo->m_szXX);
	strcpy(gpsinfo1->m_szYY,pGpsinfo->m_szYY);
	strcpy(gpsinfo1->m_szAngle,pGpsinfo->m_szAngle);

	gpsinfo1->m_nStCount = pGpsinfo->m_nStCount;
	gpsinfo1->m_dbHeigth = pGpsinfo->m_dbHeigth;
	gpsinfo1->m_nState = pGpsinfo->m_nState;
	gpsinfo1->nDataType = pGpsinfo->nDataType;
	gpsinfo1->bNetData = pGpsinfo->bNetData;
	gpsinfo1->m_nCarID = pGpsinfo->m_nCarID;

	return gpsinfo1;
}

BOOLEAN GetStatusBit(char* ch, int nBitNum)
{
	//ASSERT(nBitNum < 8 && nBitNum > -1);
    INT8U byteData;
	int nRet = 0;
	//BYTE byteData = (BYTE)ch;
	int nTemp = atoi(ch);
	byteData = (INT8U)nTemp;

	nRet = (byteData >> nBitNum) & 0x01;

	return (BOOLEAN)nRet;
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

static int m_nState=-1;
BOOLEAN ParseGPS( GpsDataInfo *pgpsinfo,char* lpgpsdata)
{
	//char gpsstr[]="GPRMC,082653.010,A,3037.3840,N,10402.3986,E,000.0,062.3,210705,001.4,W*79";
    char *argv[20];
    int argc;
    int integer;
	double mm;
    char tempgpsstr[200];
    //sprintf(tempgpsstr,"%s","GPRMC,125302.000,A,3035.1441,N,10411.4546,E,0.15,345.50,201006,,*01");
	//sprintf(tempgpsstr,"%s","GPRMC,062057.253,A,3040.5620,N,10400.6415,E,0.00,,071106,,,A*76,0,0");
    //sprintf(tempgpsstr,"%s","GPRMC,082653.010,A,3037.3840,N,10402.3986,E,000.0,062.3,210705,001.4,W*79");
    //sprintf(tempgpsstr,"%s","GPRMC,055313.352,A,3040.6217,N,10401.1738,E,9.52,204.01,071106,,,A*6F,0,0");
	sprintf(tempgpsstr,"%s",lpgpsdata);//"GPRMC,073111.951,A,3037.3496,N,10402.3289,E,000.0,-01.0,071106,002.7,W*79");
//	mycom.SendData(tempgpsstr,strlen(tempgpsstr));
    //SendString(gpsdata);
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
	   pgpsinfo->m_stUTCTime.hour= (argv[1][0]-0x30)*10+(argv[1][1]-0x30);

	   pgpsinfo->m_stUTCTime.minu=(argv[1][2]-0x30)*10+(argv[1][3]-0x30);

	   pgpsinfo->m_stUTCTime.sec=(argv[1][4]-0x30)*10+(argv[1][5]-0x30);   
   }
   //定位属性
   if(argv[2]!="")
   {
	   if(strcmp(argv[2],"V")==0)
			m_nState = 0;
		else
			if (m_nState==-1) m_nState=2;
	   pgpsinfo->m_nState=m_nState;
   }
   //
   if(argv[3]!="")
   {
    pgpsinfo->m_yy = atof(argv[3]);
    integer=(int)(pgpsinfo->m_yy /100);
    mm=pgpsinfo->m_yy-integer*100;
    mm/=60;
    pgpsinfo->m_yy=integer+mm;

	//curpoint.lat /=100;
    //curpoint.lat+=0.3;
   }
   if(argv[4]!="")
   {
    if(argv[4][0] == 'S') pgpsinfo->m_yy = 0.0-pgpsinfo->m_yy ;
   }
   if(argv[5]!="")
   {
    pgpsinfo->m_xx= atof(argv[5]);
    integer=(int)(pgpsinfo->m_xx/100);
    mm=pgpsinfo->m_xx-integer*100;
    mm/=60;
    pgpsinfo->m_xx=integer+mm;
   }
   if(argv[6]!="")
   {
    if(argv[6][0] == 'W') pgpsinfo->m_yy = 0.0-pgpsinfo->m_yy;
   }
   if(argv[7]!="")
   {
	   pgpsinfo->m_nSpeed=atof(argv[7]);
	   pgpsinfo->m_nSpeed*=1.852f;
	    //speed/=1000;//公里每小时
	    //GPSMilAdd((INT16U)(GpsInfo.VelHS.speed.f));  

   }
   else
	    pgpsinfo->m_nSpeed=0.00f;
   
   if(argv[8]!="")
   {
	   pgpsinfo->m_fAngle=atof(argv[8]);
	  //  integer=(int)(pgpsinfo->m_fAngle/100);
    //mm=pgpsinfo->m_fAngle-integer*100;
    //mm/=60;
    //pgpsinfo->m_fAngle=integer+mm;
   //((direct*3)/180)*3.14159;
   }
   if(argv[9]!="")
   {
    if(strlen(argv[9])>=6)
    {

	 pgpsinfo->m_stUTCTime.day=(argv[9][0]-0x30)*10+(argv[9][1]-0x30);
     
     pgpsinfo->m_stUTCTime.month=(argv[9][2]-0x30)*10+(argv[9][3]-0x30);
    
     pgpsinfo->m_stUTCTime.year=(argv[9][4]-0x30)*10+(argv[9][5]-0x30)+2000;
     RefreshWeek(&pgpsinfo->m_stUTCTime);
    }
   }
   if(argv[10]!="")
   {
   }
   }
 else
 {
 }
 pgpsinfo->nDataType = 1;
 return TRUE;
}
/*

int	FieldVar(const char *buf, int fldnum, int sep, char *dest)
{
	int i;
	const char *ptr = buf;
	
	for (i=0; i < fldnum; i++)
	{
		if ((ptr = (char *) strchr(ptr,sep)))
			ptr++;
		else
			return 0;
	}
	
	for (i=0; (*ptr != sep) && (*ptr) && (i < 4096); i++)
		*dest++ = *ptr++;
	*dest = '\0';
	
	if (*ptr != sep)
		return 0;
	
	return i;
}
*/
/*
BOOLEAN ParseGPS( const char* lpszGPS)
{
	//lpszGPS = "GPRMC,142000.000,A,3209.4990,N,10343.8209,E,006.3,048.0,180106,001.4,W*7D";
	//lpszGPS = "?GPGGA,012608.000,3035.1274,N,10411.4782,E,1,05,3.0,570.4,M,-31.8,M,,0000*7F";
	char		angle[200] = {0};
	char		time[200] = {0};
	char		isresult[40] = {0};
	char		yy[200] = {0};
	char		nors[40] = {0};
	char		xx[200] = {0};
	char		eorw[40] = {0};
	char		speed[200] = {0};
	char		date[200] = {0};
	char		state[200] = {0};
	char		sign[200];
	char hour[3]={0},min[3]={0},sec[3]={0},
			year[3]={0}, mon[3]={0},day[3]={0};

	int	nLen = strlen(lpszGPS);
	// max length of gps string is 1024
	if ( nLen >= 1024 ) return FALSE;	

	
	//判断是否为正确的数据
	if( nLen > 200 || nLen < 30 )
	{
		return FALSE;
	}

	FieldVar(lpszGPS,0,',',sign);
	if(strstr(sign,"GPRMC") == NULL )
		return FALSE;
	

	//提取数据,主要是经纬度，时间，速度等
	FieldVar(lpszGPS,1,',',time);
	FieldVar(lpszGPS,2,',',isresult);
	FieldVar(lpszGPS,3,',',yy);
	FieldVar(lpszGPS,4,',',nors);
	FieldVar(lpszGPS,5,',',xx);
	FieldVar(lpszGPS,6,',',eorw);
	FieldVar(lpszGPS,7,',',speed);
	FieldVar(lpszGPS,8,',',angle);
	FieldVar(lpszGPS,9,',',date);
	try
	{
		if((strlen(time)!=10)||(strlen(date)!=6)) return false;

		memcpy(hour,time,2);
		memcpy(min,time+2,2);
		memcpy(sec,time+4,2);

		memcpy(year,date+4,2);
		memcpy(mon,date+2,2);
		memcpy(day,date,2);

		int nHour = atoi(hour);	
		int nMin = atoi(min);
		int nSec = atoi(sec);
		
		int nYear = atoi(year) + 2000;
		int nMon  = atoi(mon);
		int nDay  = atoi(day);

		// 判断时间是否有错
		bool bTimeCorrect = false;
		if( (nMon>0 && nMon<13) && (nDay>0 && nDay<=31) && (nHour>=0 && nHour<24) 
			&& (nMin>=0 && nMin<60) && (nSec>=0 && nSec<60) && (nYear>=2002 && nYear<2020) )
		{
			// 转换时间**************************************
			SYSTEMTIME st;
			st.wHour = nHour;
			st.wMinute = nMin;
			st.wSecond = nSec;		
			st.wYear = nYear;
			st.wMonth = nMon;
			st.wDay = nDay;
			st.wMilliseconds = 0;
			m_stUTCTime = st;

			FILETIME fUTC,fLocal;		
			::SystemTimeToFileTime(&st,&fUTC);
			::FileTimeToLocalFileTime(&fUTC,&fLocal);
			::FileTimeToSystemTime(&fLocal,&st);

			m_stTime = st;
			//*************************************************		
		}
		else
			return false;

		if(strcmp(isresult,"V")==0)
			m_nState = 0;
		else
			if (m_nState==-1) m_nState=2;
		// GPS数据的速度是以节为单位，一节 = 1.852公里
		if(strlen(speed)>0)
		{
			m_nSpeed = atof(speed)*(1.852);
			sprintf(speed, "%f",m_nSpeed);
		}
		else return false;
		// 角度
		//if(strlen(angle)>0)
		{
			m_fAngle = atof(angle);
		}
		//else return false;
		if((strlen(xx)==10)&&(strlen(yy)==9))
		{
			char xxh[10] = {0};memcpy(xxh,xx,3);//xx1.Left(3);
			char yyh[10] = {0};memcpy(yyh,yy,2);//yy1.Left(2);

			char xxl[10] = {0};memcpy(xxl,xx+3,strlen(xx)-3);
			char yyl[10] = {0};memcpy(yyl,yy+2,strlen(xx)-2);
			
			double	xxH = atof(xxh);
			double	yyH = atof(yyh);
			
			double	xxL = atof(xxl)/60;
			double  yyL = atof(yyl)/60;
			
			xxH += xxL;
			yyH += yyL;

			m_xx = xxH;
			m_yy = yyH;
		}
		else
			return false;
	}
	catch(...)
	{
		Log("解析GPS数据错误！错误的GPS数据为：");
		Log(lpszGPS);
	}
	nDataType = 1;
	return true;
}
*/
BOOLEAN ParseGGA( GpsDataInfo *pgpsinfo, char* lpszGPS)
{

//$GPGGA,180105.000,3035.1412,N,10411.4570,E,1,05,2.2,536.8,M,-31.8,M,,0000*7B
//lpszGPS = "?GPGGA,012608.000,3035.1274,N,10411.4782,E,1,05,3.0,570.4,M,-31.8,M,,0000*7F";

	char		count[200] = {0};
	char		heigth[200] = {0};
	char		sign[200];
	int         nLen,ncount; 
	float       h;
	ncount = 0;
	nLen = strlen(lpszGPS);	
	
	//判断是否为正确的数据
	if( nLen > 200 || nLen < 5 )
	{
		return FALSE;
	}
	FieldVar(lpszGPS,0,',',sign);
	if(strstr(sign,"GPGGA") == NULL )
		return FALSE;
	


	FieldVar(lpszGPS,7,',',count);
	FieldVar(lpszGPS,9,',',heigth);
	
	if((strlen(count)<=0)||(strlen(heigth)<=0)) return FALSE;

	ncount = atoi(count);
	h = 0;
	h = atof(heigth);
	pgpsinfo->m_dbHeigth = h;
	pgpsinfo->m_nStCount = ncount;
	if(ncount<3) pgpsinfo->m_nState = 0;
	pgpsinfo->nDataType = 2;
	return TRUE;
}
BOOLEAN ParseGSA(GpsDataInfo *pgpsinfo, char* lpszGPS)
{
	int	nLen;
	char		sign[200];
	char		state[200] = {0};
	nLen= strlen(lpszGPS);
	
	//判断是否为正确的数据
	if( nLen > 200 || nLen < 5 )
	{
		return FALSE;
	}

	FieldVar(lpszGPS,0,',',sign);
	if(strstr(sign,"GPGSA") == NULL )
		return FALSE;
	
	FieldVar(lpszGPS,2,',',state);
	if(strlen(state)<=0) return FALSE;
	pgpsinfo->m_nState = atoi(state);
	pgpsinfo->nDataType = 3;
	return TRUE;
}


void InitializeGPSDataInfo(GpsDataInfo *pgpsinfo)
{
	pgpsinfo->m_nCarID = 0;
	pgpsinfo->m_xx = -1.00;
	pgpsinfo->m_yy = -1.00;
	pgpsinfo->m_nSpeed = 0.00;
	pgpsinfo->m_fAngle = 0.00;
	pgpsinfo->m_nStCount = -1;
	pgpsinfo->m_dbHeigth = -1.00;
	pgpsinfo->m_nState = -1;
	pgpsinfo->nDataType = 0;
	pgpsinfo->bNetData = FALSE;

	strcpy(pgpsinfo->m_szXX,("-1.0"));
	strcpy(pgpsinfo->m_szYY,("-1.0"));
	strcpy(pgpsinfo->m_szSpeed,("-1.0"));
	strcpy(pgpsinfo->m_szAngle,("-1.0"));
}

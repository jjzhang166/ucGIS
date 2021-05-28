// GpsDataInfo.cpp: implementation of the CGpsDataInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GpsDataInfo.h"
#include "winbase.h"
#include "log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGpsDataInfo::CGpsDataInfo()
{
	Initialize();
}

CGpsDataInfo::~CGpsDataInfo()
{

}

CGpsDataInfo& CGpsDataInfo::operator =(const CGpsDataInfo& gpsinfo)
{
	m_xx = gpsinfo.m_xx;
	m_yy = gpsinfo.m_yy;
	m_nSpeed = gpsinfo.m_nSpeed;
	m_fAngle = gpsinfo.m_fAngle;
	m_stTime = gpsinfo.m_stTime;
	m_stUTCTime = gpsinfo.m_stUTCTime;

	lstrcpy(m_szXX,gpsinfo.m_szXX);
	lstrcpy(m_szYY,gpsinfo.m_szYY);
	lstrcpy(m_szSpeed,gpsinfo.m_szSpeed);
	lstrcpy(m_szAngle,gpsinfo.m_szAngle);

	m_nStCount = gpsinfo.m_nStCount;
	m_dbHeigth = gpsinfo.m_dbHeigth;
	m_nState = gpsinfo.m_nState;
	nDataType = gpsinfo.nDataType;
	bNetData = gpsinfo.bNetData;
	m_nCarID = gpsinfo.m_nCarID;
	
	return *this;
}

CGpsDataInfo& CGpsDataInfo::operator =(const CGpsDataInfo* pGpsinfo)
{
	if( this == pGpsinfo )
		return *this;

	m_xx = pGpsinfo->m_xx;
	m_yy = pGpsinfo->m_yy;
	m_nSpeed = pGpsinfo->m_nSpeed;
	m_fAngle = pGpsinfo->m_fAngle;
	m_stTime = pGpsinfo->m_stTime;
	m_stUTCTime = pGpsinfo->m_stUTCTime;

	lstrcpy(m_szXX,pGpsinfo->m_szXX);
	lstrcpy(m_szYY,pGpsinfo->m_szYY);
	lstrcpy(m_szAngle,pGpsinfo->m_szAngle);

	m_nStCount = pGpsinfo->m_nStCount;
	m_dbHeigth = pGpsinfo->m_dbHeigth;
	m_nState = pGpsinfo->m_nState;
	nDataType = pGpsinfo->nDataType;
	bNetData = pGpsinfo->bNetData;
	m_nCarID = pGpsinfo->m_nCarID;

	return *this;
}

bool GetStatusBit(char* ch, int nBitNum)
{
	ASSERT(nBitNum < 8 && nBitNum > -1);

	int nRet = 0;
	//BYTE byteData = (BYTE)ch;
	int nTemp = atoi(ch);
	BYTE byteData = (BYTE)nTemp;

	nRet = (byteData >> nBitNum) & 0x01;

	return (bool)nRet;
}
int	CGpsDataInfo::FieldVar(const char *buf, int fldnum, int sep, char *dest)
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
bool CGpsDataInfo::ParseGPS( const char* lpszGPS)
{
	//lpszGPS = "GPRMC,142000.000,A,3209.4990,N,10343.8209,E,006.3,048.0,180106,001.4,W*7D";
	//lpszGPS = "?GPGGA,012608.000,3035.1274,N,10411.4782,E,1,05,3.0,570.4,M,-31.8,M,,0000*7F";
 	int	nLen = strlen(lpszGPS);
	// max length of gps string is 1024
	if ( nLen >= 1024 ) return false;	

	char		sign[200];
	
	//判断是否为正确的数据
	if( nLen > 200 || nLen < 30 )
	{
		return false;
	}

	FieldVar(lpszGPS,0,',',sign);
	if(strstr(sign,"GPRMC") == NULL )
		return false;
	
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

		char hour[3]={0},min[3]={0},sec[3]={0},
			year[3]={0}, mon[3]={0},day[3]={0};
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

bool CGpsDataInfo::ParseGGA(const char* lpszGPS)
{

//$GPGGA,180105.000,3035.1412,N,10411.4570,E,1,05,2.2,536.8,M,-31.8,M,,0000*7B
//lpszGPS = "?GPGGA,012608.000,3035.1274,N,10411.4782,E,1,05,3.0,570.4,M,-31.8,M,,0000*7F";
	int	nLen = strlen(lpszGPS);	
	
	//判断是否为正确的数据
	if( nLen > 200 || nLen < 5 )
	{
		return false;
	}
	char		sign[200];
	FieldVar(lpszGPS,0,',',sign);
	if(strstr(sign,"GPGGA") == NULL )
		return false;
	

	char		count[200] = {0};
	char		heigth[200] = {0};


	FieldVar(lpszGPS,7,',',count);
	FieldVar(lpszGPS,9,',',heigth);
	
	if((strlen(count)<=0)||(strlen(heigth)<=0)) return false;

	int ncount = 0;
	ncount = atoi(count);
	float h = 0;
	h = atof(heigth);
	m_dbHeigth = h;
	m_nStCount = ncount;
	if(ncount<3) m_nState = 0;
	nDataType = 2;
	return true;
}
bool CGpsDataInfo::ParseGSA(const char* lpszGPS)
{
	int	nLen = strlen(lpszGPS);
	
	//判断是否为正确的数据
	if( nLen > 200 || nLen < 5 )
	{
		return false;
	}

	char		sign[200];
	FieldVar(lpszGPS,0,',',sign);
	if(strstr(sign,"GPGSA") == NULL )
		return false;
	
	char		state[200] = {0};
	FieldVar(lpszGPS,2,',',state);
	if(strlen(state)<=0) return false;
	m_nState = atoi(state);
	nDataType = 3;
	return true;
}


void CGpsDataInfo::Initialize()
{
	m_nCarID = 0;
	m_xx = -1.00;
	m_yy = -1.00;
	m_nSpeed = 0.00;
	m_fAngle = 0.00;
	m_nStCount = -1;
	m_dbHeigth = -1.00;
	m_nState = -1;
	nDataType = 0;
	bNetData = false;

	lstrcpy(m_szXX,TEXT("-1.0"));
	lstrcpy(m_szYY,TEXT("-1.0"));
	lstrcpy(m_szSpeed,TEXT("-1.0"));
	lstrcpy(m_szAngle,TEXT("-1.0"));
}

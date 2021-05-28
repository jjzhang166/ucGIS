#ifndef _GPSDATAINFO_H_INCLUDED
#define _GPSDATAINFO_H_INCLUDED
#ifdef GPSDATAINFO_GLOBALES
#define GPSDATAINFO_EXT 

#else 
#define GPSDATAINFO_EXT extern
#endif
#include "gisbase.h"

// 记录一个GPS数据状态
// 解析字符GPRMC格式的GPS数据为数字格式
// 把数字格式的GPS数据合成为GPRMC格式的GPS字符串
typedef struct 
{

	int     m_nCarID;
	float	m_xx;
	float	m_yy;
	float	m_nSpeed;
	float	m_fAngle;
	int     m_tGpsTime;
	int     m_nStCount; // 卫星数量
	float   m_dbHeigth; // 海拔高度
	int     m_nState;  // 当前状态 1—无定位信息，2—2D  3—3D
	TIME_STRUC m_stTime;
	TIME_STRUC m_stUTCTime;

	GISCHAR	m_szXX[200];
	GISCHAR	m_szYY[200];
	GISCHAR	m_szSpeed[200];
	GISCHAR	m_szAngle[200];

	int nDataType ;   // 数据类型； 1 GPS数据，2 海拔卫星情况， 3 定位状态情况
	int bNetData;     // 是否来自网络的数据
	

}GpsDataInfo,*lpGpsDataInfo;

GPSDATAINFO_EXT BOOLEAN ParseGPS( GpsDataInfo *pgpsinfo, char* lpgpsdata);
GPSDATAINFO_EXT void parsewithspeccharargs(char *argstr, char specchar,int *argc_p, char **argv);
GPSDATAINFO_EXT void RefreshWeek(TIME_STRUC *time);
GPSDATAINFO_EXT BOOLEAN GetStatusBit(char* ch, int nBitNum);
GPSDATAINFO_EXT GpsDataInfo * GpsDataInfoSetpValue(GpsDataInfo *gpsinfo1,   GpsDataInfo* pGpsinfo);
GPSDATAINFO_EXT GpsDataInfo * GpsDataInfoSetValue(GpsDataInfo *gpsinfo1,   GpsDataInfo gpsinfo);
GPSDATAINFO_EXT void InitializeGPSDataInfo(GpsDataInfo *pgpsinfo);
GPSDATAINFO_EXT BOOLEAN ParseGGA( GpsDataInfo *pgpsinfo, char* lpszGPS);
GPSDATAINFO_EXT BOOLEAN ParseGSA(GpsDataInfo *pgpsinfo, char* lpszGPS);

#endif
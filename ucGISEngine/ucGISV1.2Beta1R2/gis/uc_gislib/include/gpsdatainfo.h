#ifndef _GPSDATAINFO_H_INCLUDED
#define _GPSDATAINFO_H_INCLUDED
#ifdef GPSDATAINFO_GLOBALES
#define GPSDATAINFO_EXT 

#else 
#define GPSDATAINFO_EXT extern
#endif
#include "gisbase.h"

// ��¼һ��GPS����״̬
// �����ַ�GPRMC��ʽ��GPS����Ϊ���ָ�ʽ
// �����ָ�ʽ��GPS���ݺϳ�ΪGPRMC��ʽ��GPS�ַ���
typedef struct 
{

	int     m_nCarID;
	float	m_xx;
	float	m_yy;
	float	m_nSpeed;
	float	m_fAngle;
	int     m_tGpsTime;
	int     m_nStCount; // ��������
	float   m_dbHeigth; // ���θ߶�
	int     m_nState;  // ��ǰ״̬ 1���޶�λ��Ϣ��2��2D  3��3D
	TIME_STRUC m_stTime;
	TIME_STRUC m_stUTCTime;

	GISCHAR	m_szXX[200];
	GISCHAR	m_szYY[200];
	GISCHAR	m_szSpeed[200];
	GISCHAR	m_szAngle[200];

	int nDataType ;   // �������ͣ� 1 GPS���ݣ�2 ������������� 3 ��λ״̬���
	int bNetData;     // �Ƿ��������������
	

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
#ifndef _CAR_H_INCLUDED
#define _CAR_H_INCLUDED
#ifdef CAR_GLOBALES
#define CAR_EXT 

#else 
#define CAR_EXT extern
#endif
#include "gisbase.h"
#include "gpsdatainfo.h"

typedef struct 
{
	float xx;
	float yy;
}GPSPOINT;

typedef 
enum {
	carMySelf = 0,     // GPS�������Լ�
	carNetCar,         // ͨ����·��ص�������
	carNetCarFreind,   // ͨ�������ص����ѳ���
	carNetCarEnemy	   // ͨ�������صĵ��˳���
}CarType;

typedef struct{ 
    int CarID;
	CarType m_carType;          // �������ֳ�Ŀ���Ǳ�������������������
	lpMapCoorTrans m_mapCoordMap; // ����ӳ��
	//CPlaySound* m_pSound;  // ��������
    PFV  pSoundFunc;
	BOOLEAN m_bSaveTrace;   // ����켣��
	BOOLEAN m_bShowTrace;   // ��ʾ�켣��
	GISCHAR m_szTraceFilePath[260]; // �켣�ļ��ı���·��
	GISCHAR m_szNiceName[100];           // �����ǳ�
	GpsDataInfo      m_gpsDataInfo;   // ��ǰ��λ����Ϣ
	FP32 m_fDistance;  // ������̵�λ����
	FP64 m_fArea;  // ���������λƽ������
	FP32 m_averSpeed; // ƽ���ٶ�
	GUI_ARRAY   guiArrayPoints;  // �켣����
	void*    hGPSFile; 			// �켣�ļ�,��¼GPSԭʼ������
	void*    hMifFile; 			// �켣�ļ�,��¼MIF���ݵĹ켣�ļ�
	void*    hMidFile; 			// �켣�ļ�,��¼MID���ݵĹ켣�ļ�
	GISLONG  nLastSpeedAlterTime;
	GISLONG  m_dwLastValueGpsDataTime;  // �յ����һ����ЧGPS���ݵ�ʱ��
	GISLONG  m_dwLastComDataTime;  // �յ����һ���������ݵ�ʱ��
	GISLONG  m_dwDistanceBeginDataTime;  // ƽ���ٶȼ��㿪ʼʱ��
	TIME_STRUC m_stBeginTime;

	BOOLEAN m_bCalledDraw;   // ������Draw��

}Car, *lpCar;

CAR_EXT void SetDistance(lpCar pcar);
CAR_EXT void DrawTrace(lpCar pcar);
CAR_EXT void DrawCar(lpCar pcar);
CAR_EXT void GetAngleXY(const float fmyAngle,const int x, const int y, const int nLineLen,int* x1,int* y1);
CAR_EXT void Car_AddPoint(GpsDataInfo* pGpsData,lpCar pcar);
CAR_EXT void DrawArrow(GUI_POINT m_One, GUI_POINT m_Two);
CAR_EXT void Car_Draw(lpCar pcar);
CAR_EXT void Car_ClearTrack(lpCar pcar);

#endif
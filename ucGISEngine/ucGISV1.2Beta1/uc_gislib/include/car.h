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
	carMySelf = 0,     // GPS导航的自己
	carNetCar,         // 通过网路监控的其他车
	carNetCarFreind,   // 通过网络监控的朋友车辆
	carNetCarEnemy	   // 通过网络监控的敌人车辆
}CarType;

typedef struct{ 
    int CarID;
	CarType m_carType;          // 用来区分车目标是本车导航还是网络来车
	lpMapCoorTrans m_mapCoordMap; // 坐标映射
	//CPlaySound* m_pSound;  // 声音引擎
    PFV  pSoundFunc;
	BOOLEAN m_bSaveTrace;   // 保存轨迹吗？
	BOOLEAN m_bShowTrace;   // 显示轨迹吗？
	GISCHAR m_szTraceFilePath[260]; // 轨迹文件的保存路径
	GISCHAR m_szNiceName[100];           // 车辆昵称
	GpsDataInfo      m_gpsDataInfo;   // 当前定位点信息
	FP32 m_fDistance;  // 运行里程单位公里
	FP64 m_fArea;  // 运行面积单位平方公里
	FP32 m_averSpeed; // 平均速度
	GUI_ARRAY   guiArrayPoints;  // 轨迹数据
	void*    hGPSFile; 			// 轨迹文件,记录GPS原始的数据
	void*    hMifFile; 			// 轨迹文件,记录MIF数据的轨迹文件
	void*    hMidFile; 			// 轨迹文件,记录MID数据的轨迹文件
	GISLONG  nLastSpeedAlterTime;
	GISLONG  m_dwLastValueGpsDataTime;  // 收到最后一个有效GPS数据的时间
	GISLONG  m_dwLastComDataTime;  // 收到最后一个串口数据的时间
	GISLONG  m_dwDistanceBeginDataTime;  // 平局速度计算开始时间
	TIME_STRUC m_stBeginTime;

	BOOLEAN m_bCalledDraw;   // 调用了Draw吗

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
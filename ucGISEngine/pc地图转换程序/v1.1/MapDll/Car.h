// Car.h: interface for the CCar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAR_H__00466021_F63E_408C_8C28_D8FC1FB25390__INCLUDED_)
#define AFX_CAR_H__00466021_F63E_408C_8C28_D8FC1FB25390__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "draw.h"
#include "GpsDataInfo.h"
#include <wingdi.h>
#include "PlaySound.h"

using namespace std;
#include <vector>


struct GPSPOINT
{
	float xx;
	float yy;
};

typedef 
enum {
	carMySelf = 0,     // GPS导航的自己
	carNetCar,         // 通过网路监控的其他车
	carNetCarFreind,   // 通过网络监控的朋友车辆
	carNetCarEnemy	   // 通过网络监控的敌人车辆
}CarType;

class AFX_EXT_CLASS CCar  
{
public:
	CCar();
	virtual ~CCar();

	void Draw(HDC hDC);
public: 
	CarType m_carType;          // 用来区分车目标是本车导航还是网络来车
	mapCOORDMAP* m_mapCoordMap; // 坐标映射

	CPlaySound* m_pSound;  // 声音引擎
public:
	void SaveLiCheng();
	void ClearTrack();   // 清除轨迹
	void SaveTrackToMif();   // 把轨迹保存成mif文件
	bool m_bSaveTrace;   // 保存轨迹吗？
	bool m_bShowTrace;   // 显示轨迹吗？
	TCHAR m_szTraceFilePath[MAX_PATH]; // 轨迹文件的保存路径
	TCHAR m_szNiceName[100];           // 车辆昵称
	CGpsDataInfo           m_gpsDataInfo;   // 当前定位点信息
	float m_fDistance;  // 运行里程单位公里
	double m_fArea;  // 运行面积单位平方公里
	float m_averSpeed; // 平均速度

	void AddPoint(CGpsDataInfo* pGpsData, HDC hDC);
	DWORD  m_dwLastValueGpsDataTime;  // 收到最后一个有效GPS数据的时间 
	DWORD  m_dwLastComDataTime;  // 收到最后一个串口数据的时间
	DWORD  m_dwDistanceBeginDataTime;  // 平局速度计算开始时间
private:
	vector<GPSPOINT *>     m_vectorPoints;  // 轨迹数据
	
	HANDLE                 hGPSFile; 			// 轨迹文件,记录GPS原始的数据
	HANDLE                 hMifFile; 			// 轨迹文件,记录MIF数据的轨迹文件
	HANDLE                 hMidFile; 			// 轨迹文件,记录MID数据的轨迹文件

	void DrawCar(HDC hDC);
	void DrawTrace(HDC hDC);
	void SaveTrace(GPSPOINT* point);
	// 根据角度计算倾斜线的另一个点
	void GetAngleXY(const float fmyAngle,const int x, const int y, const int nLineLen, int& x1,int& y1);

	DWORD  nLastSpeedAlterTime;

	SYSTEMTIME m_stBeginTime;

	bool m_bCalledDraw;   // 调用了Draw吗
	CRITICAL_SECTION m_csDraw;		// 与绘制相关的临界段
public:
	void SetDistance();
	void GetPoints(mapMAPPOINTSTRUCT* points,int& nLen);
#ifdef UNDER_WINCE
	CCEDraw*   m_ceDraw;         // gapi画布
#endif
};

#endif // !defined(AFX_CAR_H__00466021_F63E_408C_8C28_D8FC1FB25390__INCLUDED_)

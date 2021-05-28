// UserLayer.h: interface for the CUserLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERLAYER_H__D637BE69_32AE_4A19_B1F5_CF2F065CB851__INCLUDED_)
#define AFX_USERLAYER_H__D637BE69_32AE_4A19_B1F5_CF2F065CB851__INCLUDED_

//#include "gps/GPSDataFromCom.h"
//#include "gps/GpsDataInfo.h"
#include "car.h"
#include "PlaySound.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "draw.h"

//#include <vector>
#include <map>
using namespace std;

//typedef vector<CCar *> PCarVector;
typedef map<int,CCar*> PCarMap;

class AFX_EXT_CLASS CUserLayer  
{
public:
	CUserLayer();
	virtual ~CUserLayer();
public:
	void SetNetState(HDC hDC,int nState);
	//void DrawMe(HDC hDC, int nAngle);
	void Draw(HDC hDC);
	mapCOORDMAP* m_mapCoordMap; 
	CCar* m_pNavCar;                // 本地当航车目标
	void AddGPSPoint(CGpsDataInfo * pPosition,HDC hDC);

	bool m_bShowState;        // 显示状态吗？
	bool m_bShowMeasureLen;   // 显示长度测量吗？
	bool m_bShowMeasureArea;   // 显示面积测量吗？
	TCHAR m_szTraceFilePath[MAX_PATH];
	CPlaySound* m_pSound;
private:
	//PCarVector m_carVector;        // 保存车辆的vector
	PCarMap    m_carMap;           // 保存车辆的map

	int   m_nNetState;
	void DrawState(HDC hDC);

	CRITICAL_SECTION m_csDraw;		// 与绘制相关的临界段
public:
#ifdef UNDER_WINCE
	CCEBitmap* m_pceBmpCenter;              // 中心图标的图像
	CCEDraw*   m_ceDraw;         // gapi画布
#endif
protected:
	void DrawXX();
};

#endif // !defined(AFX_USERLAYER_H__D637BE69_32AE_4A19_B1F5_CF2F065CB851__INCLUDED_)

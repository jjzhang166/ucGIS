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
	CCar* m_pNavCar;                // ���ص�����Ŀ��
	void AddGPSPoint(CGpsDataInfo * pPosition,HDC hDC);

	bool m_bShowState;        // ��ʾ״̬��
	bool m_bShowMeasureLen;   // ��ʾ���Ȳ�����
	bool m_bShowMeasureArea;   // ��ʾ���������
	TCHAR m_szTraceFilePath[MAX_PATH];
	CPlaySound* m_pSound;
private:
	//PCarVector m_carVector;        // ���泵����vector
	PCarMap    m_carMap;           // ���泵����map

	int   m_nNetState;
	void DrawState(HDC hDC);

	CRITICAL_SECTION m_csDraw;		// �������ص��ٽ��
public:
#ifdef UNDER_WINCE
	CCEBitmap* m_pceBmpCenter;              // ����ͼ���ͼ��
	CCEDraw*   m_ceDraw;         // gapi����
#endif
protected:
	void DrawXX();
};

#endif // !defined(AFX_USERLAYER_H__D637BE69_32AE_4A19_B1F5_CF2F065CB851__INCLUDED_)

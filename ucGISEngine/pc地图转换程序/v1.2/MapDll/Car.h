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
	carMySelf = 0,     // GPS�������Լ�
	carNetCar,         // ͨ����·��ص�������
	carNetCarFreind,   // ͨ�������ص����ѳ���
	carNetCarEnemy	   // ͨ�������صĵ��˳���
}CarType;

class AFX_EXT_CLASS CCar  
{
public:
	CCar();
	virtual ~CCar();

	void Draw(HDC hDC);
public: 
	CarType m_carType;          // �������ֳ�Ŀ���Ǳ�������������������
	mapCOORDMAP* m_mapCoordMap; // ����ӳ��

	CPlaySound* m_pSound;  // ��������
public:
	void SaveLiCheng();
	void ClearTrack();   // ����켣
	void SaveTrackToMif();   // �ѹ켣�����mif�ļ�
	bool m_bSaveTrace;   // ����켣��
	bool m_bShowTrace;   // ��ʾ�켣��
	TCHAR m_szTraceFilePath[MAX_PATH]; // �켣�ļ��ı���·��
	TCHAR m_szNiceName[100];           // �����ǳ�
	CGpsDataInfo           m_gpsDataInfo;   // ��ǰ��λ����Ϣ
	float m_fDistance;  // ������̵�λ����
	double m_fArea;  // ���������λƽ������
	float m_averSpeed; // ƽ���ٶ�

	void AddPoint(CGpsDataInfo* pGpsData, HDC hDC);
	DWORD  m_dwLastValueGpsDataTime;  // �յ����һ����ЧGPS���ݵ�ʱ�� 
	DWORD  m_dwLastComDataTime;  // �յ����һ���������ݵ�ʱ��
	DWORD  m_dwDistanceBeginDataTime;  // ƽ���ٶȼ��㿪ʼʱ��
private:
	vector<GPSPOINT *>     m_vectorPoints;  // �켣����
	
	HANDLE                 hGPSFile; 			// �켣�ļ�,��¼GPSԭʼ������
	HANDLE                 hMifFile; 			// �켣�ļ�,��¼MIF���ݵĹ켣�ļ�
	HANDLE                 hMidFile; 			// �켣�ļ�,��¼MID���ݵĹ켣�ļ�

	void DrawCar(HDC hDC);
	void DrawTrace(HDC hDC);
	void SaveTrace(GPSPOINT* point);
	// ���ݽǶȼ�����б�ߵ���һ����
	void GetAngleXY(const float fmyAngle,const int x, const int y, const int nLineLen, int& x1,int& y1);

	DWORD  nLastSpeedAlterTime;

	SYSTEMTIME m_stBeginTime;

	bool m_bCalledDraw;   // ������Draw��
	CRITICAL_SECTION m_csDraw;		// �������ص��ٽ��
public:
	void SetDistance();
	void GetPoints(mapMAPPOINTSTRUCT* points,int& nLen);
#ifdef UNDER_WINCE
	CCEDraw*   m_ceDraw;         // gapi����
#endif
};

#endif // !defined(AFX_CAR_H__00466021_F63E_408C_8C28_D8FC1FB25390__INCLUDED_)

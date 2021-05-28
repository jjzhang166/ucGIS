// MyPoint.h: interface for the CMyPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPOINT_H__83A1E5F9_BC68_48E0_9DF2_F11B5D631538__INCLUDED_)
#define AFX_MYPOINT_H__83A1E5F9_BC68_48E0_9DF2_F11B5D631538__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Draw.h"

#include <map>
using namespace std;

#ifdef UNDER_WINCE
typedef map<int,CCEBitmap*> mapIcoBMP;
#else
typedef map<int,HBITMAP> mapIcoBMP;
#endif

static mapIcoBMP   myIcoMap;

using namespace std;
#include <vector>

class AFX_EXT_CLASS CMyPoint : public CDraw  
{
public:
	CMyPoint();
	CMyPoint(TCHAR* pszLabel, short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType,
		float X,float Y)
		:CDraw(pszLabel, nLayer, nType, nSubType){
			CMyPoint();
			m_X = X;
			m_Y = Y;
			GetRect(&m_Rect.fMinX,&m_Rect.fMinY,&m_Rect.fMaxX,&m_Rect.fMaxY);
			InitializeCriticalSection(&m_csDraw);
			m_bShowTrace = false;
		}
	virtual ~CMyPoint();

	virtual void Draw(HDC hDC,int nDrawMode);
	virtual void DrawLabel(HDC hDC);
	virtual void ReadFromFile(FILE *fp,int &fileoffset);
	virtual void WriteToFile(FILE *fp);
	virtual void GetRect(float* minX, float* minY, float* maxX, float* maxY)
	{
		*minX = *maxX = m_X;
		*minY = *maxY = m_Y;
	}
	void GetTraceRect(float* minX, float* minY, float* maxX, float* maxY);
	void IntiBMPPath();
	virtual int GetFileoffsetLen(){return m_fileoffsetlen;};

protected:
	int m_fileoffsetlen;
	float        m_X;         // 点横坐标
	float        m_Y;         // 点纵坐标

	vector<mapGPSPOINT *>     m_vectorGpsPoints;  // 轨迹数据
	void DrawTrace(HDC hDC);
	//mapSYMBOLSTRUCT m_Symbol;     // 点符号类型
private:
	CRITICAL_SECTION m_csDraw;		// 与绘制相关的临界段
	// 显示轨迹吗
	bool m_bShowTrace;
public:
	int m_nCarID;
public:
	// 设置目标点的经纬度
	void SetXY(float x, float y);
	// 某点在不在我这图形上
	bool IsOnMe(mapMAPPOINTSTRUCT* point);
	// 添加一个轨迹点到车车
	void SetXY(mapGPSPOINT* mapPt);
	void SetShowTrace(bool bShowTrace);
	bool GetShowTrace(void);
};

#endif // !defined(AFX_MYPOINT_H__83A1E5F9_BC68_48E0_9DF2_F11B5D631538__INCLUDED_)

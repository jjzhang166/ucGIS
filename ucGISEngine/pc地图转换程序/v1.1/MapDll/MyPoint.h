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
	float        m_X;         // �������
	float        m_Y;         // ��������

	vector<mapGPSPOINT *>     m_vectorGpsPoints;  // �켣����
	void DrawTrace(HDC hDC);
	//mapSYMBOLSTRUCT m_Symbol;     // ���������
private:
	CRITICAL_SECTION m_csDraw;		// �������ص��ٽ��
	// ��ʾ�켣��
	bool m_bShowTrace;
public:
	int m_nCarID;
public:
	// ����Ŀ���ľ�γ��
	void SetXY(float x, float y);
	// ĳ���ڲ�������ͼ����
	bool IsOnMe(mapMAPPOINTSTRUCT* point);
	// ���һ���켣�㵽����
	void SetXY(mapGPSPOINT* mapPt);
	void SetShowTrace(bool bShowTrace);
	bool GetShowTrace(void);
};

#endif // !defined(AFX_MYPOINT_H__83A1E5F9_BC68_48E0_9DF2_F11B5D631538__INCLUDED_)

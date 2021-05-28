// Draw.h: interface for the CDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAW_H__44059172_9E46_4993_A239_C1B773FE8A20__INCLUDED_)
#define AFX_DRAW_H__44059172_9E46_4993_A239_C1B773FE8A20__INCLUDED_

#include "stdafx.h"
#include <wingdi.h>
#include <tchar.h>
#include <stdlib.h>
#include <winuser.h>


class CLayer;
#ifdef UNDER_WINCE
#include <cegl.h>
#endif

#include <vector>
using namespace std;

#pragma pack(4)

// ͼ������
typedef struct LayerStruct
{
	// �Ƿ������Զ���ע
	bool AutoLabel;
	// �Ƿ�����༭�ò�
	bool Editable;
	// �Ƿ���ʾ��ͼ��
	bool Visible;
	// ͼ������
	TCHAR Name[100];
	// ��Ұ��Χ
	float nEyeShotMin;
	float nEyeShotMax;

	short id;
	CLayer * l;
}LayerStruct;

// �������ŵĽṹ
typedef struct mapSYMBOLSTRUCT
{
	bool isBmp;
	short shape;
	COLORREF color;
	short size;
	TCHAR fontname[32];
	short fontstyle;
	short rotation;
	TCHAR filename[50];
	short customstyle;
}mapSYMBOLSTRUCT;

// ����һ����ͼ������εĽṹ
typedef struct mapMAPRECTSTRCT
{
	float fMinX;
	float fMinY;
	float fMaxX;
	float fMaxY;
}mapMAPRECTSTRCT;
typedef struct mapRECT
{
    float    left;
    float    top;
    float    right;
    float    bottom;
} mapRECT;
// �Զ��廭�ʽṹ
typedef struct mapPENSTRUCT
{
	short width;
	short pattern;
	COLORREF color;
}mapPENSTRUCT;
typedef struct mapGPSPOINT
{
	float x;
	float y;
	int   fx;
	float speed;
	bool speedalter;
	bool emergentalter;
}mapGPSPOINT;
// ��ͼ�����
typedef struct mapMAPPOINTSTRUCT
{
	mapMAPPOINTSTRUCT()
	{
		x = y = 0;
	}
	float x;
	float y;
}mapMAPPOINTSTRUCT;

// �Զ���ˢ�ӽṹ
typedef struct mapBRUSHSTRUCT
{
	short pattern;
	COLORREF forecolor;
	COLORREF backcolor;
}mapBRUSHSTRUCT;



typedef 
enum ToolConstants{
	miArrowTool = 1000,
	miPanTool = 1001,
	miCenterTool = 1002,
	miZoomInTool = 1003,
	miZoomOutTool = 1004,
	miSymbolTool = 1005,
	miTextTool = 1006,
	miSelectTool = 1007,
	miRadiusSelectTool = 1008,
	miRectSelectTool = 1009,
	miPolygonSelectTool = 1010,
	miLabelTool = 1011,
	miAddLineTool = 1012,
	miAddPolylineTool = 1013,
	miAddRegionTool = 1014,
	miAddPointTool = 1015
} ToolConstants;

// ��Ļ�������
typedef 
enum CursorConstants{
	miDefaultCursor = 0,
	miArrowCursor = 1,
	miCrossCursor = 2,
	miIBeamCursor = 3,
	miIconCursor = 4,
	miSizeCursor = 5,
	miSizeNESWCursor = 6,
	miSizeNSCursor = 7,
	miSizeNWSECursor = 8,
	miSizeEWCursor = 9,
	miUpArrowCursor = 10,
	miHourglassCursor = 11,
	miNoDropCursor = 12,
	miArrowHourglassCursor = 13,
	miArrowQuestionCursor = 14,
	miSizeAllCursor = 15,
	miArrowToolCursor = 16,
	miPanCursor = 17,
	miCenterCursor = 18,
	miZoomInCursor = 19,
	miZoomOutCursor = 20,
	miSymbolCursor = 21,
	miTextCursor = 22,
	miSelectCursor = 23,
	miRadiusSelectCursor = 24,
	miRectSelectCursor = 25,
	miRegionSelectCursor = 26,
	miInfoCursor = 27,
	miSelectPlusCursor = 28,
	miSelectRadiusPlusCursor = 29,
	miSelectRectPlusCursor = 30,
	miSelectRegionPlusCursor = 31,
	miSelectMinusCursor = 32,
	miSelectRadiusMinusCursor = 33,
	miSelectRectMinusCursor = 34,
	miSelectRegionMinusCursor = 35,
	miLabelCursor = 36,
	miDrilldownExpandCursor = 37,
	miDrilldownContractCursor = 38,
	miInfoCursorOld = 39,
	miCustomCursor = 40
} CursorConstants;

// ��ͼ��������
typedef 
enum ToolTypeConstants{
	miToolTypePoint = 0,
	miToolTypeLine = 1,
	miToolTypeCircle = 2,
	miToolTypeMarquee = 3,
	miToolTypePoly = 4,
	miToolTypePolygon = 5
} ToolTypeConstants;


// ��Ŀ��ͼ�θ�ʽ
typedef
enum SymbolTypeConstants{
	miSymbolTypeTrueTypeFont = 0,
	miSymbolTypeBitmap = 1,
	miSymbolTypeVector = 2
} SymbolTypeConstants;

// ��ͼԪ������
typedef 
enum ObjectTypeConstants {
	otNull           = 1,     // ������
	// ��Ŀ������
	otBuilding       = 1000,  // ����
	otCommunication  = 1100,  // ͨѶ����
	otCommunicationPostOffice  = 1101,  // �ʵ��
	otCommunicationTelecom  = 1102,  // �ʵ��
	otEducation      = 1200,  // ��������
	otEnterprise     = 1300,  // ��ҵ

	otFinance        = 1400,  // ��������
	otFinanceATM     = 1401,  // ATM

	otGOV            = 1500,  // ����
	otHospital       = 1600,  // ҽԺ
	otHotel          = 1700,  // �Ƶ�
	otMarket         = 1800,  // �г�
	otMedia          = 1900,  // ý��
	otPolice         = 2000,  // �����
	otResidential    = 2100,  // ��סС��

	otRestaurant     = 2200,  // ����
	otRestaurantEat     = 2201,  // �͹�
	otRestaurantTea     = 2202,  // �跻

	otTourSite       = 2300,  // ���ε�

	otTraffic            = 2400,  // ��ͨ
	otTrafficPack        = 2401,  // ͣ����
	otTrafficBackShop    = 2402,  // ����
	otTrafficStop        = 2403,  // ����վ
	otTrafficGasStation  = 2404,  // ����վ
	otTrafficAirport     = 2405,  // ����
	otTrafficPort        = 2406,  // �ۿ�
	otTrafficTicketOffice  = 2407,  // ��Ʊ��
	otTrafficRailwayStation   = 2408,  // ��վ
	otTrafficBusStop   = 2409,  // ����վ

	otPublic         = 2500,  // ��������

	otSport          = 2600,  // �˶�����

	otCinema         = 2700,  // ��ӰԺ

	// ��Ŀ������
	otXD             = 5000,  // �ص�
	otSD             = 5100,  // ʡ��
	otGD             = 5200,  // ����
	otGS             = 5300,  // ���ٹ�·
	otTL             = 5400,  // ��·
	otHL             = 5500,  // ����
	otJD             = 5600,  // �ֵ�

	// ��Ŀ������
	otWater          = 8000  // ˮ��


} ObjectTypeConstants;

extern BOOL G_PtInPolygon(mapMAPPOINTSTRUCT *rgpts, WORD wnumpts, mapMAPPOINTSTRUCT ptTest,

				   mapRECT *prbound) ;

#define WINPC 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef vector<RECT*> LabelRectVector;
// ���ڼ��������
typedef struct AFX_EXT_CLASS mapCOORDMAP
{
	// ʵ������ӳ����߼�����
	// x:���ȣ�y:γ��
	// X:��Ļ������ Y:��Ļ������
	void DPtoVP(float x, float y, int* X, int* Y);


	// �߼�����ת��Ϊʵ������
	// 
	void VPtoDP(int x, int y, float* X, float* Y);

	virtual ~mapCOORDMAP();

	mapCOORDMAP();

	// �жϱ�ע�����Ƿ�ʹ�ã������׼�ظ�
	bool IsLabelRectUsed(RECT rect);
	
	// �����ע����
	void ClearLabelRect();

	// ��ȡ��ǰ��Ļ���м���Ļ����
	void GetScreenCenter(int& x,int& y);

	// ��С
	void ZoomIn();

	// �Ŵ�
	void ZoomOut();

	// �Ŵ���
	void ZoomTo(int x1,int y1,int x2,int y2);

	// ���õ�ͼ����Ļ�ϵ�����λ�õ�ƫ����
	void SetCenterOffset(int x, int y);

	/*
	��������㾭γ�ȱ�ʾ����Ļ����
	*/
	void SetScreenXY();

	// ��γ���Ƿ�����Ļ��
	bool IsInScreen(float x,float y);

	// ���õ�ͼ����Ļ�ϵ�����λ��
	void SetCenter(float x, float y);

	// ���õ�ͼ����Ļ�ϵ�����λ��;
	void SetCenter(int x, int y);

	void Left();

	void Right();

	void Up();

	void Down();

	int DLtoVL(float l);


	float VLtoDL(int l);


	void SetStart(void);


	LabelRectVector rectVector;   // ��Ļ��ע�ľ�����
	float xStart;   // ����ͼ��ʱ����Ļ�������
	float yStart;   // ����ͼ��ʱ����Ļ�������
	float xMinScreen; 
	float yMinScreen; 
	float xMaxScreen;
	float yMaxScreen;

	float scale;      // ������
	int wScreen; 	// ��ǰ��ͼ�ĸ߶ȺͿ��
	int hScreen; 	// ��ǰ��ͼ�ĸ߶ȺͿ��
	int nMapMode;   // ӳ��ģʽ
	int nAngle;     // ��ͼ��ת�Ƕ�

	float m_fCenterX,m_fCenterY;
	int m_nCenterX,m_nCenterY;
	bool m_bMapChanged ;   

}mapCOORDMAP;

class AFX_EXT_CLASS CDraw 
{
public:
	virtual void Draw(HDC hDC,int nDrawMode);
	virtual void DrawLabel(HDC hDC);

	virtual void ReadFromFile(FILE	*fp,int &fileoffset);
	virtual void WriteToFile(FILE	*fp);
	virtual void GetRect(float* minX, float* minY, float* maxX, float* maxY){};

	TCHAR* GetLabel()
	{
		return m_lpszLabel;
	};

	bool IsRectCross();

	CDraw();
	CDraw(TCHAR* pszLabel, short nLayer,ObjectTypeConstants nType,ObjectTypeConstants nSubType)
	{
		CDraw();

		m_lpszLabel = new TCHAR[lstrlen(pszLabel)+2];
		if(!m_lpszLabel)
		{
			MessageBox(NULL,_T("�����ڴ�ʧ�ܣ�"),_T("����"),MB_ICONERROR);
		}

		//memset(m_lpszLabel,0,strlen(pszLabel)*2+2);
		lstrcpy(m_lpszLabel,pszLabel);

		m_nLayer = nLayer;
		m_nType = nType;
		m_nSubType = nSubType;
	}
	virtual ~CDraw();
protected:
	// �߼�����ת��Ϊʵ������
	void VPtoDP(int x, int y, float* X, float* Y);
	// ʵ������ӳ����߼�����
	// x:���ȣ�y:γ��
	// X:��Ļ������ Y:��Ļ������
	void DPtoVP(float x, float y, int* X, int* Y);
	// �ж϶����Ƿ�����Ļ�ڣ��Ա�����Ƿ���ƣ�
	bool IsRectCross(float minx,float miny,float maxx,float maxy);  // �ж��Ƿ��н���

	short m_nLayer;                      // ͼ����

	TCHAR* m_lpszLabel;                  // unicode��ע
	bool  m_AtoWLabeled;                 // �Ƿ��Ѿ�ת����ע

	ObjectTypeConstants m_nType;         // ��������
	ObjectTypeConstants m_nSubType;      // ����������

	mapMAPRECTSTRCT m_Rect;              // �߽����
	void AToW(char *sourceChar,TCHAR *wChar);

	void AToWLabel();

	bool m_bEditMode;       // �ǳ��ڱ༭������״̬��һ�߻��Ƽ���״̬ͼ��
	int  m_fileoffsetlen;   //��ע�ڵ�ͼ�ļ��е�������ռ����
	//int  fileoffsetend;     ////��ע�ڵ�ͼ�ļ��е�������ʼֵ

	
public:
	virtual int GetFileoffsetLen(){return m_fileoffsetlen;};
	bool m_bShowAllLabel;   // ���б�ע����ʾ��һ����ʾ����վ̨�ı�ע
	mapCOORDMAP* m_mapCoordMap;  // ���ڼ�������ӳ��
#ifdef UNDER_WINCE
	CCEDraw*   m_ceDraw;         // gapi����
#endif
protected:
	// ��Unicodeת����Ansi
	bool WToA(TCHAR* wChar, char* cChar);
	// �Զ�����ַ�ת��Ϊ����
	int myatoi(TCHAR* ls);

public:
	// ��������
	void SetType(ObjectTypeConstants nType, bool bIsSub);
	void SetDrawType(bool bEditMode)
	{
		m_bEditMode = bEditMode;
	}
 //����㵽ֱ�ߵľ���ĺ���   
  float PointLine(float xx,float yy,float x1,float y1,float x2,float y2);   
  //���������ľ���ĺ���   
  float CalDisp(float x1,float y1,float x2,float y2);
};

#endif // !defined(AFX_DRAW_H__44059172_9E46_4993_A239_C1B773FE8A20__INCLUDED_)

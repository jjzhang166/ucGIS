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

// 图层设置
typedef struct LayerStruct
{
	// 是否允许自动标注
	bool AutoLabel;
	// 是否允许编辑该层
	bool Editable;
	// 是否显示该图层
	bool Visible;
	// 图层名字
	TCHAR Name[100];
	// 视野范围
	float nEyeShotMin;
	float nEyeShotMax;

	short id;
	CLayer * l;
}LayerStruct;

// 描述符号的结构
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

// 描述一个地图坐标矩形的结构
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
// 自定义画笔结构
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
// 地图坐标点
typedef struct mapMAPPOINTSTRUCT
{
	mapMAPPOINTSTRUCT()
	{
		x = y = 0;
	}
	float x;
	float y;
}mapMAPPOINTSTRUCT;

// 自定义刷子结构
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

// 屏幕光标类型
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

// 画图工具类型
typedef 
enum ToolTypeConstants{
	miToolTypePoint = 0,
	miToolTypeLine = 1,
	miToolTypeCircle = 2,
	miToolTypeMarquee = 3,
	miToolTypePoly = 4,
	miToolTypePolygon = 5
} ToolTypeConstants;


// 点目标图形格式
typedef
enum SymbolTypeConstants{
	miSymbolTypeTrueTypeFont = 0,
	miSymbolTypeBitmap = 1,
	miSymbolTypeVector = 2
} SymbolTypeConstants;

// 地图元素类型
typedef 
enum ObjectTypeConstants {
	otNull           = 1,     // 无类型
	// 点目标类型
	otBuilding       = 1000,  // 建筑
	otCommunication  = 1100,  // 通讯机构
	otCommunicationPostOffice  = 1101,  // 邮电局
	otCommunicationTelecom  = 1102,  // 邮电局
	otEducation      = 1200,  // 教育机构
	otEnterprise     = 1300,  // 企业

	otFinance        = 1400,  // 金融银行
	otFinanceATM     = 1401,  // ATM

	otGOV            = 1500,  // 政府
	otHospital       = 1600,  // 医院
	otHotel          = 1700,  // 酒店
	otMarket         = 1800,  // 市场
	otMedia          = 1900,  // 媒体
	otPolice         = 2000,  // 警察局
	otResidential    = 2100,  // 居住小区

	otRestaurant     = 2200,  // 餐饮
	otRestaurantEat     = 2201,  // 餐馆
	otRestaurantTea     = 2202,  // 茶坊

	otTourSite       = 2300,  // 旅游地

	otTraffic            = 2400,  // 交通
	otTrafficPack        = 2401,  // 停车场
	otTrafficBackShop    = 2402,  // 修理厂
	otTrafficStop        = 2403,  // 汽车站
	otTrafficGasStation  = 2404,  // 加油站
	otTrafficAirport     = 2405,  // 机场
	otTrafficPort        = 2406,  // 港口
	otTrafficTicketOffice  = 2407,  // 售票处
	otTrafficRailwayStation   = 2408,  // 火车站
	otTrafficBusStop   = 2409,  // 公交站

	otPublic         = 2500,  // 公共场所

	otSport          = 2600,  // 运动场所

	otCinema         = 2700,  // 电影院

	// 线目标类型
	otXD             = 5000,  // 县道
	otSD             = 5100,  // 省道
	otGD             = 5200,  // 国道
	otGS             = 5300,  // 高速公路
	otTL             = 5400,  // 铁路
	otHL             = 5500,  // 河流
	otJD             = 5600,  // 街道

	// 面目标类型
	otWater          = 8000  // 水域


} ObjectTypeConstants;

extern BOOL G_PtInPolygon(mapMAPPOINTSTRUCT *rgpts, WORD wnumpts, mapMAPPOINTSTRUCT ptTest,

				   mapRECT *prbound) ;

#define WINPC 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef vector<RECT*> LabelRectVector;
// 用于计算坐标的
typedef struct AFX_EXT_CLASS mapCOORDMAP
{
	// 实际坐标映象成逻辑坐标
	// x:经度；y:纬度
	// X:屏幕横坐标 Y:屏幕纵坐标
	void DPtoVP(float x, float y, int* X, int* Y);


	// 逻辑坐标转换为实际坐标
	// 
	void VPtoDP(int x, int y, float* X, float* Y);

	virtual ~mapCOORDMAP();

	mapCOORDMAP();

	// 判断标注区域是否被使用，避免标准重复
	bool IsLabelRectUsed(RECT rect);
	
	// 清除标注矩形
	void ClearLabelRect();

	// 获取当前屏幕的中间屏幕坐标
	void GetScreenCenter(int& x,int& y);

	// 缩小
	void ZoomIn();

	// 放大
	void ZoomOut();

	// 放大至
	void ZoomTo(int x1,int y1,int x2,int y2);

	// 设置地图在屏幕上的中心位置的偏移量
	void SetCenterOffset(int x, int y);

	/*
	计算出浮点经纬度表示的屏幕矩形
	*/
	void SetScreenXY();

	// 经纬度是否在屏幕中
	bool IsInScreen(float x,float y);

	// 设置地图在屏幕上的中心位置
	void SetCenter(float x, float y);

	// 设置地图在屏幕上的中心位置;
	void SetCenter(int x, int y);

	void Left();

	void Right();

	void Up();

	void Down();

	int DLtoVL(float l);


	float VLtoDL(int l);


	void SetStart(void);


	LabelRectVector rectVector;   // 屏幕标注的举行们
	float xStart;   // 绘制图形时的屏幕起点坐标
	float yStart;   // 绘制图形时的屏幕起点坐标
	float xMinScreen; 
	float yMinScreen; 
	float xMaxScreen;
	float yMaxScreen;

	float scale;      // 比例尺
	int wScreen; 	// 当前视图的高度和宽度
	int hScreen; 	// 当前视图的高度和宽度
	int nMapMode;   // 映射模式
	int nAngle;     // 地图旋转角度

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
			MessageBox(NULL,_T("申请内存失败！"),_T("错误"),MB_ICONERROR);
		}

		//memset(m_lpszLabel,0,strlen(pszLabel)*2+2);
		lstrcpy(m_lpszLabel,pszLabel);

		m_nLayer = nLayer;
		m_nType = nType;
		m_nSubType = nSubType;
	}
	virtual ~CDraw();
protected:
	// 逻辑坐标转换为实际坐标
	void VPtoDP(int x, int y, float* X, float* Y);
	// 实际坐标映象成逻辑坐标
	// x:经度；y:纬度
	// X:屏幕横坐标 Y:屏幕纵坐标
	void DPtoVP(float x, float y, int* X, int* Y);
	// 判断对象是否在屏幕内（以便决定是否绘制）
	bool IsRectCross(float minx,float miny,float maxx,float maxy);  // 判断是否有交叉

	short m_nLayer;                      // 图层编号

	TCHAR* m_lpszLabel;                  // unicode标注
	bool  m_AtoWLabeled;                 // 是否已经转化标注

	ObjectTypeConstants m_nType;         // 数据类型
	ObjectTypeConstants m_nSubType;      // 数据子类型

	mapMAPRECTSTRCT m_Rect;              // 边界矩形
	void AToW(char *sourceChar,TCHAR *wChar);

	void AToWLabel();

	bool m_bEditMode;       // 是出于编辑，激活状态吗，一边绘制激活状态图型
	int  m_fileoffsetlen;   //标注在地图文件中的数据所占长度
	//int  fileoffsetend;     ////标注在地图文件中的数据起始值

	
public:
	virtual int GetFileoffsetLen(){return m_fileoffsetlen;};
	bool m_bShowAllLabel;   // 所有标注都显示吗，一边显示所有站台的标注
	mapCOORDMAP* m_mapCoordMap;  // 用于计算坐标映射
#ifdef UNDER_WINCE
	CCEDraw*   m_ceDraw;         // gapi画布
#endif
protected:
	// 从Unicode转换到Ansi
	bool WToA(TCHAR* wChar, char* cChar);
	// 自定义的字符转化为数字
	int myatoi(TCHAR* ls);

public:
	// 设置类型
	void SetType(ObjectTypeConstants nType, bool bIsSub);
	void SetDrawType(bool bEditMode)
	{
		m_bEditMode = bEditMode;
	}
 //计算点到直线的距离的函数   
  float PointLine(float xx,float yy,float x1,float y1,float x2,float y2);   
  //计算两点间的距离的函数   
  float CalDisp(float x1,float y1,float x2,float y2);
};

#endif // !defined(AFX_DRAW_H__44059172_9E46_4993_A239_C1B773FE8A20__INCLUDED_)

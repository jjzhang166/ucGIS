#ifndef __READMAPDATA_H
#define __READMAPDATA_H
#include "GUI_Array.h"
//颜色压缩与释放[unsigned int - 4 bytes]
#define PACK_COLOR(R,G,B,A)   ( (A)<<24 | (B)<<16 | (G)<<8 | (R) )
#define UNPACK_RED(X)         ( (X) & 0xff )
#define UNPACK_GREEN(X)       ( ( (X) >> 8 ) & 0xff )
#define UNPACK_BLUE(X)        ( ( (X) >> 16 ) & 0xff )
#define UNPACK_ALPHA(X)       ( ( (X) >> 24 ) & 0xff )

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int  INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

// 地图元素类型
typedef
enum {
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
//时间结构体
typedef struct{
	INT16U year;
	INT8U month;
	INT8U day;
	INT8U weekday;
	INT8U hour;
	INT8U minu;
	INT8U sec;
}TIME_STRUC;

typedef enum
{
	ONESPACE=1,
	ONEANDHALF=2,
	TWOSPACE=3
}GSpace;
typedef enum
{
	LEFT=1,
	CENTER=2,
	RIGHT=3
}GJustify;
typedef enum
{
	PLAIN=0,
	BOLD=1,
	BLACKBORDER=16,
	DROPSHADOW=32,
	WHITEBORDER=256
}GFontStyle;
typedef enum 
{
	FONT_PLAIN=0,
	FONT_BOLD=1,
	FONT_ITALIC=2,
	FONT_UNDERLINE=4,
	FONT_OUTLINE=16,
	FONT_SHADOW=32,
	FONT_HALO=256,
	FONT_ALLCAPS=512,
	FONT_EXPANDED=1024
}GFontStyles;


typedef struct
{
	short width; //1-7 pixels  0 only when patter is 1 for invisible lines 
	           //11-2047 values for converted to points 
	           //width=(number of ponits *10)+10
	short patter;//1-118, 1 is invisible

	DWORD color; //representing a 24-bit RGB color value
}GPen;

typedef struct
{
	short patter;//1-71 ,1 is no fill,2 is a solid fill ,9-11 are reserved
	DWORD forcolor;//representing a 24-bit RGB color value
	DWORD backcolor;//representing a 24-bit RGB color value
}GBrush;
typedef struct 
{
	int shape; //31 or larger,31 represents a blank symbol 32-67
	int color; //representing a 24-bit RGB color value
	int size;  //1-48,representing a point size
}GSymbol;

typedef struct 
{
	int shape; //31 or larger,31 represents a blank symbol 32-67
	int color; //representing a 24-bit RGB color value
	int size;  //1-48,representing a point size
	char fontname[50];
	int fontstyle; 
    double rotation;
}GSymbolTrueTypeFont;
typedef struct 
{
	char *filename[20];
	int color; //representing a 24-bit RGB color value
	int size;  //1-48,representing a point size
	char *fontname;
	int customstyle; 
}GSymbolBitmap;

typedef struct 
{
	char fontname[20];
	int  style; //
	int  size;
	int  forecolor;
	int  backcolor;
}GFont;

typedef struct 
{
	GSpace textspace;
    GJustify textjustify;
    double textangle;
}GTextStyle;

//地理点结构[用于构造线、多边形等]
typedef struct 
{
	float lon, lat, hgt;		//位置
}GeoPoint;
//地理站目标结构[用于测站、城市等]//point
typedef struct
{
	float lon, lat, hgt;		//位置
    GSymbol ponitsymbol;        //点符号
}GPoint;

// 描述一个地图坐标矩形的结构
typedef struct 
{
	float fMinX;
	float fMinY;
	float fMaxX;
	float fMaxY;
}mapMAPRECTSTRCT;
//地理文本目标结构 //text
typedef struct 
{
    int  id;                     //ID
	short m_nLayer;
	WM_HMEM text;             //文本内容 
	mapMAPRECTSTRCT m_Rect;
	ObjectTypeConstants m_nType;
	ObjectTypeConstants m_nSubType;
}GBaseLabel;
//地理线目标结构 //line
typedef struct 
{
	char name[16];				//名称
	GeoPoint point[2] ;         //点坐标
	int num_pt;					//地理点数目
    GPen  linepen;

}GLine;

//地理线目标结构 //polyline
typedef struct
{
//	char name[16];				//名称
	int num_pt;					//连续直线的定点数目
	GUI_ARRAY pointarray;       //存储定点坐标的数组指针
	GUI_ARRAY lablecoordarray;
	GBaseLabel baselable;		//线的基本标注信息
	GPen plinepen;
}GPLine;
//地理区域目标结构 region
typedef struct
{
	GUI_ARRAY plinearray;       //线数组
    GBrush regionbrush;
    GeoPoint centerpoint;       //中心点坐标
	bool m_bTransparent;          // 是否透明
	bool m_bFill;                 // 是否需要填充
}GRegion;

//地理圆弧目标结构 arc
typedef struct 
{
	char name[16];				//名称
    GeoPoint boundrectpoint[2]; //圆弧所在矩形的对角坐标点
	double startangles;
	double endangles;
	GPen  regionpen;
//	char *others;			//属性描述
    int  id;                     //ID
}GArc;

//地理矩形目标结构 //rectangle
typedef struct 
{
	char name[16];				//名称
    GeoPoint boundrectpoint[2]; //矩形的对角坐标点
	GPen  rectpen;
    GBrush rectbrush;
//	char *others;			//属性描述
    int  id;                     //ID
}GRectangle;
//地理圆角矩形目标结构 //rounded rectangle
typedef struct 
{
	char name[16];				//名称
    GeoPoint boundrectpoint[2]; //矩形的对角坐标点
	double rounddegree;        
	GPen  roundrectpen;
    GBrush roundrectbrush;
//	char *others;//属性描述
    int  id;                     //ID
}GRoundedRectangle;
//地理椭圆目标结构 //ellipse
typedef struct 
{
	char name[16];				//名称
    GeoPoint boundrectpoint[2]; //矩形的对角坐标点
	GPen  rectpen;
    GBrush rectbrush;
//	char *others;			     //属性描述
    int  id;                     //ID
}GEllipse;
////////////////////////////////////////
//地图数据存储结构

//点数据存储,以GPoint为元数存储在PointArray中
typedef struct 
{
	char name[16];			 	//类型名
	int num_pt;					//点数目
 	GUI_ARRAY PointArray;       //点表
}GPointType;

//线数据存储,以GLine为元数存储在LineArray中
typedef struct 
{
	char name[16];				//类型名
	int num_ln;					//线数目
	GUI_ARRAY LineArray;        //线表 
}GLineType;

//多点线数据存储,以GPLine为元数存储在PLineArray中
typedef struct 
{
	char name[16];				//类型名
	int num_ln;					//线数目
    GUI_ARRAY PLineArray;        //线表
}GPLineType;
//多边形区域数据存储,以GRegion为元数存储在RegionArray中
typedef struct 
{	
	char name[16];				//类型名
	int num_pg;					//多边形数目
	GUI_ARRAY RegionArray;      //多边形表
}GRegionType;
//地理圆弧数据存储,以GArc为元数存储在ArcArray中
typedef struct 
{
	char name[16];				//类型名
	int num_pg;					//圆弧数目
	GUI_ARRAY ArcArray;         //圆弧表
}GArcType;
//地理文本数据存储,以GText为元数存储在TextArray中
typedef struct 
{
	char name[16];				//类型名
	int num_pg;					//文本数目
	GUI_ARRAY TextArray;        //文本表
}GTextType;
//地理矩形数据存储,以GRectangle为元数存储在RectangleArray中
typedef struct 
{
	char name[16];				//类型名
	int num_pg;					//数目
	GUI_ARRAY RectangleArray;        //表
}GRectangleType;
//地理数据存储,以GRoundedRectangle为元数存储在RoundedRectangleArray中
typedef struct 
{
	char name[16];				//类型名
	int num_pg;					//数目
	GUI_ARRAY RoundedRectangleArray;        //表
}GRoundedRectangleType;
//地理数据存储,以GEllipse为元数存储在EllipseArray中
typedef struct 
{
	char name[16];				//类型名
	int num_pg;					//数目
	GUI_ARRAY EllipseArray;     //表
}GEllipseType;
//地图图层结构体
typedef struct 
{
    char name[16];
	int id;
	double m_fMaxX;
	double m_fMaxY;
	double m_fMinX;
	double m_fMinY;
	GPointType    pointdata;
    GLineType     linedata;
    GPLineType    plinedata;
    GRegionType   regiondata;
    GArcType      arcdata;
	GTextType     textdata;
    GRectangleType   rectdata; 
    GRoundedRectangleType   roundrectdata;
    GEllipseType  ellipsedata;   
}GMapLayer;
//地图图层存储,以GMapLayer为元素存储在MaplayerArray中
typedef struct 
{
	char name[16];				//类型名
	int num_pg;					//数目
    GUI_ARRAY MaplayerArray;    //
}GMapLayerType;

//mapinfo mif header file 

typedef	struct 
{
	unsigned short uiProjectionType;
	unsigned short uiDatum;
	char chUnitName[20];
	float fOrigin_Longitude;
	float fOrigin_Latitude;
	float fStandard_parallel_1;
	float fStandard_parallel_2;
	float fAzimuth;
	float fScale_factor;
	float fRange;
	float fminX;
	float fminY;
	float fmaxX;
	float fmaxY;
	char chPaperunitname[20];
	char chTablename[20];
	unsigned short uiWindow_id;
	unsigned TransformXmult;
	unsigned TransformYmult;
	unsigned TransformXDisp;
	unsigned TransformYDisp;
}CoordSys;

typedef	struct 
{
	char chColumnName[50];
	unsigned short uiColumnType;
	unsigned short uiLength;
}Column;

typedef	struct 
{
	unsigned short uiColumnCount;
    GUI_ARRAY Rec_ColumnsArray;    //
	//Column *pRec_Columns;

}Columns;

typedef struct 
{
	unsigned short MapInfoVersion;     //制图软体MapInfo版本号
	WM_HMEM strDataCharSet;              //数据文字类型,最大长度固定为100
	WM_HMEM strDataDelimiter;            //数据Attribute间隔方式,最大长度固定为20
	unsigned short uiDataUniqueColCount; //主属性列的个数
	unsigned short *uiDataUniqueCol;      //主属性列号(Key Attribute)
	unsigned short uiDataIndexColCount;  //参与索引的列个数
	unsigned short *uiDataIndexCol;       //参与索引的列号(Key Attribute)
	CoordSys CoordSys;
	WM_HMEM strCoordSys;
	Columns Columns;
}FileHeader;

typedef enum {
	VERSION=1,//version
	CHARSET=2,//charset
	DELIMITER=3,//Delimiter
	UNIQUE=4,//unique
	INDEX=5,//Index
	COORDSYS=6,//CoordSys
	TRANSFORM=7,//Transform
	COLUMNS=8,//Columns
	DATA=9//Data	
}HeaderDataType;

/*
typedef struct
{
    char        *m_pszVersion;
    char        *m_pszCharset;
    char        *m_pszDelimiter;
    char        *m_pszUnique;
    char        *m_pszIndex;
    char        *m_pszCoordSys;
}MifHeader;
*/
GMapLayerType mymap;

FileHeader    m_FileHeader;

#endif
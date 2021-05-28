#ifndef _GISBASE_H_INCLUDED
#define _GISBASE_H_INCLUDED
#ifdef GISBASE_GLOBALES
#define GISBASE_EXT 

#else 
#define GISBASE_EXT extern
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "GUI_Array.h"
//��ɫѹ�����ͷ�[unsigned int - 4 bytes]
#define PACK_COLOR(R,G,B,A)   ( (A)<<24 | (B)<<16 | (G)<<8 | (R) )
#define UNPACK_RED(X)         ( (X) & 0xff )
#define UNPACK_GREEN(X)       ( ( (X) >> 8 ) & 0xff )
#define UNPACK_BLUE(X)        ( ( (X) >> 16 ) & 0xff )
#define UNPACK_ALPHA(X)       ( ( (X) >> 24 ) & 0xff )
#define GISMIN(v0,v1) ((v0>v1) ? v1 : v0)
#define GISMAX(v0,v1) ((v0>v1) ? v0 : v1)
#define GEO_PI  (FP64)(3.14159265353846)

#ifndef FALSE			/* in case these macros already exist */
#define FALSE	0		/* values of boolean */
#endif
#ifndef TRUE
#define TRUE	1
#endif

typedef char           GISCHAR;
typedef unsigned short GISTCHAR;
typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int   INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int   INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */
typedef unsigned long  GISULONG;
typedef long           GISLONG;
typedef	void          (* PrVoid)(void);
typedef	PrVoid        (*PrPrVoid)(void);
/* Pointer to Function returning Void (any number of parameters) */
typedef void          (*PFV)();
/* Pointer to Function returning INT_32 (any number of parameters) */
typedef INT32U        (*PFI)();
#define GIS_RGB(r,g,b)          ((GISLONG)(((INT8U)(r)|((GISLONG)((INT8U)(g))<<8))|(((GISLONG)(INT8U)(b))<<16)))
#define GetTickCount()      1    

#define IDS_PLINE                       32781
#define IDS_POLYGON                     32782
#define IDS_TEXT                        32783
#define IDS_POINT                       32784
#define IDS_LINE                        32785


// ��ͼԪ������
typedef
enum {
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
	otWater          = 8000,  // ˮ��
    otCodeEnd        = 8001

} ObjectTypeConstants;
//ʱ��ṹ��
typedef struct{
	INT16U year;
	INT8U month;
	INT8U day;
	INT8U weekday;
	INT8U hour;
	INT8U minu;
	INT8U sec;
}TIME_STRUC;

typedef struct{
    INT16S    left;
    INT16S    top;
    INT16S    right;
    INT16S    bottom;
}GIS_RECT;

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
	INT16U width; //1-7 pixels  0 only when patter is 1 for invisible lines 
	           //11-2047 values for converted to points 
	           //width=(number of ponits *10)+10
	INT16U patter;//1-118, 1 is invisible

	GISLONG color; //representing a 24-bit RGB color value
}GPen;

typedef struct
{
	INT16U patter;//1-71 ,1 is no fill,2 is a solid fill ,9-11 are reserved
	GISLONG forcolor;//representing a 24-bit RGB color value
	GISLONG backcolor;//representing a 24-bit RGB color value
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

//�����ṹ[���ڹ����ߡ�����ε�]
typedef struct 
{
	FP32 lon, lat, hgt;		//λ��
}GeoPoint;
//����վĿ��ṹ[���ڲ�վ�����е�]//point
typedef struct
{
	FP32 lon, lat, hgt;		//λ��
    GSymbol ponitsymbol;        //�����
}GPoint;

// ����һ����ͼ������εĽṹ
typedef struct 
{
	FP32 fMinX;
	FP32 fMinY;
	FP32 fMaxX;
	FP32 fMaxY;
}mapMAPRECTSTRCT;

typedef struct mapRECT
{
    FP32    left;
    FP32    top;
    FP32    right;
    FP32    bottom;
} mapRECT;
//�����ı�Ŀ��ṹ //text
typedef struct
{
	GUI_ARRAY rectVector;   // ��Ļ��ע�ľ�����
	FP32 xStart;   // ����ͼ��ʱ����Ļ�������
	FP32 yStart;   // ����ͼ��ʱ����Ļ�������
	FP32 xMinScreen; 
	FP32 yMinScreen; 
	FP32 xMaxScreen;
	FP32 yMaxScreen;

	FP32 blc;      // ������
	INT32S wScreen; 	// ��ǰ��ͼ�ĸ߶ȺͿ��
	INT32S hScreen; 	// ��ǰ��ͼ�ĸ߶ȺͿ��
	INT32S nMapMode;   // ӳ��ģʽ
	INT32S nAngle;     // ��ͼ��ת�Ƕ�

	FP32 m_fCenterX,m_fCenterY;
	INT32S m_nCenterX,m_nCenterY;
	BOOLEAN m_bMapChanged ;
}MapCoorTrans,*lpMapCoorTrans;

typedef struct 
{
	INT16U m_nLayer;
	WM_HMEM text;             //�ı����� 
	mapMAPRECTSTRCT m_Rect;
	MapCoorTrans* pCoordMap; 
	ObjectTypeConstants m_nType;
	ObjectTypeConstants m_nSubType;
}GBaseLabel;



//����Բ��Ŀ��ṹ arc
typedef struct 
{
//	char name[16];				//����
    GeoPoint boundrectpoint[2]; //Բ�����ھ��εĶԽ������
	FP32 startangles;
	FP32 endangles;
	GPen  regionpen;
//	char *others;			//��������
 //   int  id;                     //ID
}GArc;

//�������Ŀ��ṹ //rectangle
typedef struct 
{
	char name[16];				//����
    GeoPoint boundrectpoint[2]; //���εĶԽ������
	GPen  rectpen;
    GBrush rectbrush;
//	char *others;			//��������
    int  id;                     //ID
}GRectangle;
//����Բ�Ǿ���Ŀ��ṹ //rounded rectangle
typedef struct 
{
	char name[16];				//����
    GeoPoint boundrectpoint[2]; //���εĶԽ������
	double rounddegree;        
	GPen  roundrectpen;
    GBrush roundrectbrush;
//	char *others;//��������
    int  id;                     //ID
}GRoundedRectangle;
//������ԲĿ��ṹ //ellipse
typedef struct 
{
	char name[16];				//����
    GeoPoint boundrectpoint[2]; //���εĶԽ������
	GPen  rectpen;
    GBrush rectbrush;
//	char *others;			     //��������
    int  id;                     //ID
}GEllipse;
////////////////////////////////////////
//��ͼ���ݴ洢�ṹ
/*
//�����ݴ洢,��GPointΪԪ���洢��PointArray��
typedef struct 
{
	char name[16];			 	//������
	INT32U num_pt;					//����Ŀ
 	GUI_ARRAY PointArray;       //���
}GPointType;

//�����ݴ洢,��GLineΪԪ���洢��LineArray��
typedef struct 
{
	char name[16];				//������
	int num_ln;					//����Ŀ
	GUI_ARRAY LineArray;        //�߱� 
}GLineType;

//��������ݴ洢,��GPLineΪԪ���洢��PLineArray��
typedef struct 
{
	char name[16];				//������
	int num_ln;					//����Ŀ
    GUI_ARRAY PLineArray;        //�߱�
}GPLineType;
//������������ݴ洢,��GRegionΪԪ���洢��RegionArray��
typedef struct 
{	
	char name[16];				//������
	int num_pg;					//�������Ŀ
	GUI_ARRAY RegionArray;      //����α�
}GRegionType;
//����Բ�����ݴ洢,��GArcΪԪ���洢��ArcArray��
typedef struct 
{
	char name[16];				//������
	int num_pg;					//Բ����Ŀ
	GUI_ARRAY ArcArray;         //Բ����
}GArcType;
//�����ı����ݴ洢,��GTextΪԪ���洢��TextArray��
typedef struct 
{
	char name[16];				//������
	int num_pg;					//�ı���Ŀ
	GUI_ARRAY TextArray;        //�ı���
}GTextType;
//����������ݴ洢,��GRectangleΪԪ���洢��RectangleArray��
typedef struct 
{
	char name[16];				//������
	int num_pg;					//��Ŀ
	GUI_ARRAY RectangleArray;        //��
}GRectangleType;
//�������ݴ洢,��GRoundedRectangleΪԪ���洢��RoundedRectangleArray��
typedef struct 
{
	char name[16];				//������
	int num_pg;					//��Ŀ
	GUI_ARRAY RoundedRectangleArray;        //��
}GRoundedRectangleType;
//�������ݴ洢,��GEllipseΪԪ���洢��EllipseArray��
typedef struct 
{
	char name[16];				//������
	int num_pg;					//��Ŀ
	GUI_ARRAY EllipseArray;     //��
}GEllipseType;
//��ͼͼ��ṹ��
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
//��ͼͼ��洢,��GMapLayerΪԪ�ش洢��MaplayerArray��
typedef struct 
{
	char name[16];				//������
	int num_pg;					//��Ŀ
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
	unsigned short MapInfoVersion;     //��ͼ����MapInfo�汾��
	WM_HMEM strDataCharSet;              //������������,��󳤶ȹ̶�Ϊ100
	WM_HMEM strDataDelimiter;            //����Attribute�����ʽ,��󳤶ȹ̶�Ϊ20
	unsigned short uiDataUniqueColCount; //�������еĸ���
	unsigned short *uiDataUniqueCol;      //�������к�(Key Attribute)
	unsigned short uiDataIndexColCount;  //�����������и���
	unsigned short *uiDataIndexCol;       //�����������к�(Key Attribute)
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
*/
extern GUI_FONT GUI_UNI_PTR * gGisFont;
extern GISCHAR* GISDispCharAt(GISCHAR *gbstr,INT32U x,INT32U y);
extern INT32U GBStrlen(GISCHAR *gbstr);
extern void GisGetTextExtentPoint(GISCHAR* s,INT32U * cx,INT32U * cy);
extern INT32U  WToA(GISTCHAR *unistr,GISCHAR *gbstr,INT32U msg_len);
GISBASE_EXT INT32S GIS_IntersectRects(GIS_RECT* pDest, const GIS_RECT* pr0, const GIS_RECT* pr1);
GISBASE_EXT BOOLEAN Lable_ReadFromFile(FILE *fp,GBaseLabel *lpLabel);
GISBASE_EXT void GisGetTextExtentPoint(GISCHAR* s,INT32U * cx,INT32U * cy);
GISBASE_EXT void Label_Free(GBaseLabel *pgBaseLabel);  

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
//GISBASE_EXT GMapLayerType mymap;

//GISBASE_EXT FileHeader    m_FileHeader;

#endif
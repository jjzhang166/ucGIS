#ifndef _COORTRANS_H_INCLUDED
#define _COORTRANS_H_INCLUDED
//}mapCOORDMAP;

//mapCOORDMAP m_mapCoordMap;
/*

INT32S SetOriginLongitude(double dOriginLongitude);// 设置中央子午线经度
INT32S LoLaToxy();//经纬度坐标转为大地坐标
INT32S xyToLoLa();//大地坐标转为经纬度坐标
INT32S SetTempxy(double dx, double dy);//设置大地坐标
INT32S SetCoordSysType(INT32S type);//设置标准椭球体类型,默认为0(0为克鲁索夫斯基椭球体，1为国家UGG1975椭球体,2为WGS84椭球体)
INT32S SetAB(double da,double db);//设置其他椭球体;da和db为椭球体的长半径和短半径
INT32S SetParaDegree(double ParaDegree);//设置分段度数,默认为6度
INT32S SetZoneNum(double ZoneNum);//设置工区带号,默认为21带
INT32S SetLoLa(double dLo, double dLa);//设置经纬度坐标
INT32S GetLoLa(double *dLo, double *dLa);//得到经纬度坐标
INT32S Setxy(double dx, double dy);//设置大地坐标
INT32S Getxy(double *dx, double *dy);//得到大地坐标
INT32S GetSpaceCoord(double *dx, double *dy,double *dz);//得到空间坐标
double Dms2Rad(double Dms);//角度到弧度的转化
double Rad2Dms(double Rad);//弧度到角度的转化
void CenterPoINT32S(double jing, double wei);
INT32S PoINT32SInRect(long xx,long yy,long left,long right,long bottom,long top);

#define CONVERRGB(MAP_COLOR) (MAP_COLOR & 0xff00)+((MAP_COLOR & 0xff0000)>>16)+((MAP_COLOR & 0xff)<<16)
*/
#ifdef COORTRANS_GLOBALES
#define COORTRANS_EXT 

#else 
#define COORTRANS_EXT extern
#endif

#include "gisbase.h"

COORTRANS_EXT void DPtoVP(MapCoorTrans *pcoortrans,FP32 x, FP32 y, INT32S* X, INT32S* Y);
	// 逻辑坐标转换为实际坐标
	// 
COORTRANS_EXT void VPtoDP(MapCoorTrans *pcoortrans,INT32S x, INT32S y, FP32* X, FP32* Y);
	// 判断标注区域是否被使用，避免标准重复
COORTRANS_EXT BOOLEAN IsLabelRectUsed(MapCoorTrans *pcoortrans,GIS_RECT rect);
	// 清除标注矩形
COORTRANS_EXT void ClearLabelRect(MapCoorTrans *pcoortrans);
	// 获取当前屏幕的中间屏幕坐标
COORTRANS_EXT void GetScreenCenter(MapCoorTrans *pcoortrans,INT32S * x,INT32S * y);
	// 放大至
COORTRANS_EXT void ZoomTo(MapCoorTrans *pcoortrans,INT32S x1,INT32S y1,INT32S x2,INT32S y2);
	// 缩小
COORTRANS_EXT void ZoomIn(MapCoorTrans *pcoortrans);	// 放大
COORTRANS_EXT void ZoomOut(MapCoorTrans *pcoortrans);
	/*
	 计算出浮点经纬度表示的屏幕矩形
	*/
COORTRANS_EXT void SetScreenXY(MapCoorTrans *pcoortrans);
	// 经纬度是否在屏幕中
COORTRANS_EXT BOOLEAN IsInScreen(MapCoorTrans *pcoortrans,FP32 x,FP32 y);
// 设置地图在屏幕上的中心位置
COORTRANS_EXT void SetCenterFP(MapCoorTrans *pcoortrans,FP32 x, FP32 y);
// 设置地图在屏幕上的中心位置
COORTRANS_EXT void SetCenterINT(MapCoorTrans *pcoortrans,INT32S x, INT32S y);
// 设置地图在屏幕上的中心位置的偏移量
COORTRANS_EXT void SetCenterOffset(MapCoorTrans *pcoortrans,INT32S x, INT32S y);
COORTRANS_EXT void Left(MapCoorTrans *pcoortrans);
COORTRANS_EXT void Right(MapCoorTrans *pcoortrans);
COORTRANS_EXT void Up(MapCoorTrans *pcoortrans);
COORTRANS_EXT void Down(MapCoorTrans *pcoortrans);
COORTRANS_EXT INT32S DLtoVL(MapCoorTrans *pcoortrans,FP32 l);
COORTRANS_EXT FP32 VLtoDL(MapCoorTrans *pcoortrans,INT32S l);
COORTRANS_EXT void SetStart(MapCoorTrans *pcoortrans);
COORTRANS_EXT BOOLEAN IsRectCrossRect(MapCoorTrans *pcoortrans,GIS_RECT gisRect);
COORTRANS_EXT BOOLEAN IsRectCross(MapCoorTrans *pcoortrans,FP32 minx,FP32 miny,FP32 maxx,FP32 maxy);
#endif /* ndef _COORTRANS_H_INCLUDED */

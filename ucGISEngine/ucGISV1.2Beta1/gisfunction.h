
#ifndef _GISFUNCTION_H_INCLUDED
#define _GISFUNCTION_H_INCLUDED
#ifdef GISFUNCTION_GLOBALES
#define GISFUNCTION_EXT 
#else 
#define GISFUNCTION_EXT extern
#endif
#include "gisbase.h"
GISFUNCTION_EXT FP64 GetDistance(FP64 x1,FP64 y1,FP64 x2,FP64 y2);
GISFUNCTION_EXT void GetPointCrossPolygon(int mode, int num, FP64 *xx, FP64 *yy, FP64 *centerx, FP64 *centery);
GISFUNCTION_EXT int compareTime(int year1, int month1, int day1, int hour1, int minute1, int second1, int year2, int month2, int day2, int hour2, int minute2, int second2);
GISFUNCTION_EXT BOOLEAN rectIncludeRect(FP64 minx, FP64 maxx, FP64 miny, FP64 maxy, FP64 left, FP64 right, FP64 bottom, FP64 top);
GISFUNCTION_EXT void getCoordFromLine(FP64 *xx, FP64 *yy, int num, int sep, int ser, FP64 *cx, FP64 *cy,FP64 *angle);
GISFUNCTION_EXT void rotateOfFP64(FP64 *xx, FP64 *yy, FP64 x0, FP64 y0, FP64 angle);
GISFUNCTION_EXT FP64 GetAngleEx(FP64 dx,FP64 dy);
GISFUNCTION_EXT FP64 getAngle(FP64 dx, FP64 dy);
GISFUNCTION_EXT FP64	getArea(FP64 x1,FP64 y1,FP64 x2,FP64 y2,FP64 x3,FP64 y3);
GISFUNCTION_EXT FP64	pointToLine(FP64 pointx,FP64 pointy,FP64 x1,FP64 y1,FP64 x2,FP64 y2);
GISFUNCTION_EXT int getMinDistance(FP64 d1, FP64 d2, FP64 d3, FP64 d4);
GISFUNCTION_EXT FP64 distOfPointAndPoint(FP64 x1, FP64 y1, FP64 x2, FP64 y2);
GISFUNCTION_EXT BOOLEAN searchLine(FP64 xx,FP64 yy,long pointnum,FP64 *xcoord,FP64 *ycoord,FP64 searchID);
GISFUNCTION_EXT FP64	ifLineCut(FP64 x0,FP64 x1,FP64 y0,FP64 y1,FP64 yy);
GISFUNCTION_EXT int	pointInPolygon(FP64 xx,FP64 yy,int num,GeoPoint *pgeopoint);//FP64 *x,FP64 *y);
GISFUNCTION_EXT BOOLEAN rectInRect(FP64 minx, FP64 maxx, FP64 miny, FP64 maxy, FP64 left, FP64 right, FP64 bottom, FP64 top);
GISFUNCTION_EXT BOOLEAN pointInRect(FP64 xx, FP64 yy, FP64 left, FP64 right, FP64 bottom, FP64 top);
GISFUNCTION_EXT BOOLEAN findcrosspoint(FP64	x11,FP64	y11,FP64	x12,FP64	y12,FP64	x21,FP64	y21,FP64	x22,FP64	y22,FP64	*xx,FP64	*yy);
#endif
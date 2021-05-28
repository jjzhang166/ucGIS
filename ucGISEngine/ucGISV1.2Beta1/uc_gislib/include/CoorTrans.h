#ifndef _COORTRANS_H_INCLUDED
#define _COORTRANS_H_INCLUDED
//}mapCOORDMAP;

//mapCOORDMAP m_mapCoordMap;
/*

INT32S SetOriginLongitude(double dOriginLongitude);// �������������߾���
INT32S LoLaToxy();//��γ������תΪ�������
INT32S xyToLoLa();//�������תΪ��γ������
INT32S SetTempxy(double dx, double dy);//���ô������
INT32S SetCoordSysType(INT32S type);//���ñ�׼����������,Ĭ��Ϊ0(0Ϊ��³����˹�������壬1Ϊ����UGG1975������,2ΪWGS84������)
INT32S SetAB(double da,double db);//��������������;da��dbΪ������ĳ��뾶�Ͷ̰뾶
INT32S SetParaDegree(double ParaDegree);//���÷ֶζ���,Ĭ��Ϊ6��
INT32S SetZoneNum(double ZoneNum);//���ù�������,Ĭ��Ϊ21��
INT32S SetLoLa(double dLo, double dLa);//���þ�γ������
INT32S GetLoLa(double *dLo, double *dLa);//�õ���γ������
INT32S Setxy(double dx, double dy);//���ô������
INT32S Getxy(double *dx, double *dy);//�õ��������
INT32S GetSpaceCoord(double *dx, double *dy,double *dz);//�õ��ռ�����
double Dms2Rad(double Dms);//�Ƕȵ����ȵ�ת��
double Rad2Dms(double Rad);//���ȵ��Ƕȵ�ת��
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
	// �߼�����ת��Ϊʵ������
	// 
COORTRANS_EXT void VPtoDP(MapCoorTrans *pcoortrans,INT32S x, INT32S y, FP32* X, FP32* Y);
	// �жϱ�ע�����Ƿ�ʹ�ã������׼�ظ�
COORTRANS_EXT BOOLEAN IsLabelRectUsed(MapCoorTrans *pcoortrans,GIS_RECT rect);
	// �����ע����
COORTRANS_EXT void ClearLabelRect(MapCoorTrans *pcoortrans);
	// ��ȡ��ǰ��Ļ���м���Ļ����
COORTRANS_EXT void GetScreenCenter(MapCoorTrans *pcoortrans,INT32S * x,INT32S * y);
	// �Ŵ���
COORTRANS_EXT void ZoomTo(MapCoorTrans *pcoortrans,INT32S x1,INT32S y1,INT32S x2,INT32S y2);
	// ��С
COORTRANS_EXT void ZoomIn(MapCoorTrans *pcoortrans);	// �Ŵ�
COORTRANS_EXT void ZoomOut(MapCoorTrans *pcoortrans);
	/*
	 ��������㾭γ�ȱ�ʾ����Ļ����
	*/
COORTRANS_EXT void SetScreenXY(MapCoorTrans *pcoortrans);
	// ��γ���Ƿ�����Ļ��
COORTRANS_EXT BOOLEAN IsInScreen(MapCoorTrans *pcoortrans,FP32 x,FP32 y);
// ���õ�ͼ����Ļ�ϵ�����λ��
COORTRANS_EXT void SetCenterFP(MapCoorTrans *pcoortrans,FP32 x, FP32 y);
// ���õ�ͼ����Ļ�ϵ�����λ��
COORTRANS_EXT void SetCenterINT(MapCoorTrans *pcoortrans,INT32S x, INT32S y);
// ���õ�ͼ����Ļ�ϵ�����λ�õ�ƫ����
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

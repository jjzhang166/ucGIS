// CoorTrans.cpp: implementation of the CoorTrans class.
//
//////////////////////////////////////////////////////////////////////
#include "CoorTrans.h"
#include <math.h>
double Dms2Rad(double Dms)
{
	double Degree, Miniute;
	double Second;
	int Sign;
	double Rad;
	if(Dms >= 0)
		Sign = 1;
	else
		Sign = -1;
	Dms = fabs(Dms);
	Degree = floor(Dms);
	Miniute = floor(fmod(Dms * 100.0, 100.0));
	Second = fmod(Dms * 10000.0, 100.0);
	Rad = Sign * (Degree + Miniute / 60.0 + Second / 3600.0) * PI / 180.0;
	return Rad;
}
double Rad2Dms(double Rad)
{
	double Degree, Miniute;
	double Second;
	int Sign;
	double Dms;
	if(Rad >= 0)
		Sign = 1;
	else
		Sign = -1;
	Rad = fabs(Rad * 180.0 / PI);
	Degree = floor(Rad);
	Miniute = floor(fmod(Rad * 60.0, 60.0));
	Second = fmod(Rad * 3600.0, 60.0);
	Dms = Sign * (Degree + Miniute / 100.0 + Second / 10000.0);
	return Dms;
}
///////////////////////////////////////////////////
// Definition of PrjPoint
///////////////////////////////////////////////////
PrjPoint::PrjPoint()
{
	m_CoordSysType=0;
	a = 6378245;
	b = 6356863.01880;
	f = a/(a-b);
	e2 = 1 - ((f - 1) / f) * ((f - 1) / f);
	e12 = (f / (f - 1)) * (f / (f - 1)) - 1;
	n=(a-b)/(a+b);
	A1 = a*(1-n+(pow(n,2)-pow(n,3))*5/4+(pow(n,4)-pow(n,5))*81/64)*PI/180;
	A2 = (n-pow(n,2)+(pow(n,3)-pow(n,4))*7/8+pow(n,5)*55/64)*3*a/2;
	A3 = (pow(n,2)-pow(n,3)+(pow(n,4)-pow(n,5))*3/4)*15*a/16;
	A4 = (pow(n,3)-pow(n,4)+pow(n,5)*11/16)*35*a/48;
	m_ParaDegree=6;
	SetZoneNum(21);
}
PrjPoint::~PrjPoint()
{

}
BOOL PrjPoint::LoLaToxy()
{
	double X, N, t, t2, m, m2, ng2;
	double sinB, cosB;
	X = A1 * m_Lo * 180.0 / PI + A2 * sin(2 * m_Lo) + A3 * sin(4 * m_Lo) + A4 * sin(6 * 
	m_Lo);
	sinB = sin(m_Lo);
	cosB = cos(m_Lo);
	t = tan(m_Lo);
	t2 = t * t;
	N = a / sqrt(1 - e2 * sinB * sinB);
	m = cosB * (m_La - m_OriginLongitude);
	m2 = m * m;
	ng2 = cosB * cosB * e2 / (1 - e2);
	x = X + N * t * ((0.5 + ((5 - t2 + 9 * ng2 + 4 * ng2 * ng2) / 24.0 + (61 -58 * t2 + t2 * t2) * m2 / 720.0) * m2) * m2);
	y = N * m * ( 1 + m2 * ( (1 - t2 + ng2) / 6.0 + m2 * ( 5 - 18 * t2 + t2 * t2 + 14 * ng2 - 58 * ng2 * t2 ) / 120.0));
	y += 500000;
	return TRUE;
}
BOOL PrjPoint::xyToLoLa()
{
	double sinB, cosB, t, t2, N ,ng2, V, yN;
	double preB0, B0;
	double eta;
	double dy=y;
	dy-= 500000;
	B0 = x / A1;
	do
	{
		preB0 = B0;
		B0 = B0 * PI / 180.0;
		B0 = (x - (A2 * sin(2 * B0) + A3 * sin(4 * B0) + A4 * sin(6 * B0))) / A1;
		eta = fabs(B0 - preB0);
	}while(eta > 0.000000001);
	B0 = B0 * PI / 180.0;
	m_Lo = Rad2Dms(B0);
	sinB = sin(B0);
	cosB = cos(B0);
	t = tan(B0);
	t2 = t * t;
	N = a / sqrt(1 - e2 * sinB * sinB);
	ng2 = cosB * cosB * e2 / (1 - e2);
	V = sqrt(1 + ng2);
	yN = dy / N;
	m_Lo = B0 - (yN * yN - (5 + 3 * t2 + ng2 - 9 * ng2 * t2) * yN * yN * yN * yN /
	12.0 + (61 + 90 * t2 + 45 * t2 * t2) * yN * yN * yN * yN * yN * yN / 360.0)
	* V * V * t / 2;
	m_La = m_OriginLongitude + (yN - (1 + 2 * t2 + ng2) * yN * yN * yN / 6.0 + (5 + 28 * t2 + 24 
	* t2 * t2 + 6 * ng2 + 8 * ng2 * t2) * yN * yN * yN * yN * yN / 120.0) / cosB
	;
	return TRUE;
}
BOOL PrjPoint::SetCoordSysType(int type)
{
	if((type<0)||(type>1)){
		AfxMessageBox("类型错误，0为克鲁索夫斯椭球体，1为国家UGG1975椭球体,2为WGS84椭球体");
		return FALSE;
	}
	else{
		m_CoordSysType=type;
		if(type==0)
		{
			a = 6378245;
			b = 6356863.01880;
		}else if(type==1){
			a = 6378140;
			b = 6356755.28820;
		}else if(type==2){
			a = 6378137.000;
			b = 6356752.31420;	
		}
		f = a/(a-b);
		e2 = 1 - ((f - 1) / f) * ((f - 1) / f);
		e12 = (f / (f - 1)) * (f / (f - 1)) - 1;
		n=(a-b)/(a+b);
		A1 = a*(1-n+(pow(n,2)-pow(n,3))*5/4+(pow(n,4)-pow(n,5))*81/64)*PI/180;
		A2 = (n-pow(n,2)+(pow(n,3)-pow(n,4))*7/8+pow(n,5)*55/64)*3*a/2;
		A3 = (pow(n,2)-pow(n,3)+(pow(n,4)-pow(n,5))*3/4)*15*a/16;
		A4 = (pow(n,3)-pow(n,4)+pow(n,5)*11/16)*35*a/48;		
		return TRUE;
	}
}
BOOL PrjPoint::SetAB(double da,double db)
{
	a=da;
	b=db;
	f = a/(a-b);
	e2 = 1 - ((f - 1) / f) * ((f - 1) / f);
	e12 = (f / (f - 1)) * (f / (f - 1)) - 1;
	n=(a-b)/(a+b);
	A1 = a*(1-n+(pow(n,2)-pow(n,3))*5/4+(pow(n,4)-pow(n,5))*81/64)*PI/180;
	A2 = (n-pow(n,2)+(pow(n,3)-pow(n,4))*7/8+pow(n,5)*55/64)*3*a/2;
	A3 = (pow(n,2)-pow(n,3)+(pow(n,4)-pow(n,5))*3/4)*15*a/16;
	A4 = (pow(n,3)-pow(n,4)+pow(n,5)*11/16)*35*a/48;
	return TRUE;
}
BOOL PrjPoint::SetParaDegree(double ParaDegree)
{
	m_ParaDegree=ParaDegree;
	return TRUE;
}
BOOL PrjPoint::SetZoneNum(double ZoneNum)
{
	m_ZoneNum=ZoneNum;
	double dOriginLongitude=(ZoneNum-1)*m_ParaDegree+m_ParaDegree/2;
	m_OriginLongitude = Dms2Rad(dOriginLongitude);
	return TRUE;
}
BOOL PrjPoint::SetOriginLongitude(double dOriginLongitude)
{
	m_OriginLongitude = Dms2Rad(dOriginLongitude);
	return TRUE;
}
BOOL PrjPoint::SetLoLa(double dLo, double dLa)
{
	m_Lo = Dms2Rad(dLo);
	m_La = Dms2Rad(dLa);
	m_Lo = dLo;
	m_La = dLa;
	LoLaToxy();
	return TRUE;
}
BOOL PrjPoint::GetLoLa(double *dLo, double *dLa)
{
	*dLo = Rad2Dms(m_Lo);
	*dLa = Rad2Dms(m_La);
	return TRUE;
}
BOOL PrjPoint::GetSpaceCoord(double *dx, double *dy,double *dz)
{
	dx=ab/sqrt(b*b+a*a*tan(m_La)*tan(m_La));
	dy=dx*tan(m_Lo);
	dz=dx*tan(m_La);
	return TRUE;
}
BOOL PrjPoint::Setxy(double dx, double dy)
{
	double ddy=floor(dy/1000000);
	double newX,newY;
	newX=dx;newY=dy;
	if(m_ZoneNum!=ddy) 
	{
		double dOriginLongitude=(ddy-1)*m_ParaDegree+m_ParaDegree/2;
		SetOriginLongitude(dOriginLongitude);
		SetTempxy(dx,dy);
		double dB,dL;
		GetLoLa(&dB,&dL);
		SetZoneNum(m_ZoneNum);
		//SetLoLa(dB,dL);
		LoLaToxy();
		Getxy(&newX,&newY);
	}
	SetTempxy(newX,newY);
	return TRUE;
}
BOOL PrjPoint::SetTempxy(double dx, double dy)
{
	double tempy=fmod(dy, 1000000);
	x = dx;
	y = tempy;
	xyToLoLa();
	return TRUE;
}
BOOL PrjPoint::Getxy(double *dx, double *dy)
{
	double newX,newY;
	newX=x;newY=m_ZoneNum*1000000+y;
	/*CString cs;
	cs.Format("%f",newY);
	AfxMessageBox(cs);*/
	*dx = newX;
	*dy = newY;
	return TRUE;
}
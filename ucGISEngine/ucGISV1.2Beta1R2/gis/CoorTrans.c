// CoorTrans.cpp: implementation of the CoorTrans class.
//
//////////////////////////////////////////////////////////////////////
/*
const double GEO_PI = 3.14159265353846;

double m_OriginLongitude; // ���������߾���
double m_Lo, m_La; // ��γ������
double x, y; // ��˹ͶӰƽ������
double m_ZoneNum;//����������
double m_ParaDegree;//�ֶζ���
int m_CoordSysType;//��׼����(0Ϊ��³����˹����׼��1Ϊ����UGG1975��׼,2ΪWGS84������)
double a,b,n, f, e2, e12; // �����������,a���뾶,b�̰뾶,n=(a-b)/(a+b),f����,e2��һƫ����ƽ��,e12�ڶ�ƫ����ƽ��
double A1, A2, A3, A4; // ���ڼ���X���������
double m_fMapjw[4];
double m_xStart,m_yStart,blc,x_blc,y_blc;//������
double m_fMaxCX, m_fMaxCY;//��ǰ��ͼ�����ʾ��Χ��ֱ�����꣩
int m_MapMode;
int m_hScreen,m_wScreen;	//��ǰ��ͼ�ĸ߶ȺͿ��
int nxoffset,nyoffset;

int SetOriginLongitude(double dOriginLongitude);// �������������߾���
int LoLaToxy();//��γ������תΪ�������
int xyToLoLa();//�������תΪ��γ������
int SetTempxy(double dx, double dy);//���ô������
int SetCoordSysType(int type);//���ñ�׼����������,Ĭ��Ϊ0(0Ϊ��³����˹�������壬1Ϊ����UGG1975������,2ΪWGS84������)
int SetAB(double da,double db);//��������������;da��dbΪ������ĳ��뾶�Ͷ̰뾶
int SetParaDegree(double ParaDegree);//���÷ֶζ���,Ĭ��Ϊ6��
int SetZoneNum(double ZoneNum);//���ù�������,Ĭ��Ϊ21��
int SetLoLa(double dLo, double dLa);//���þ�γ������
int GetLoLa(double *dLo, double *dLa);//�õ���γ������
int Setxy(double dx, double dy);//���ô������
int Getxy(double *dx, double *dy);//�õ��������
int GetSpaceCoord(double *dx, double *dy,double *dz);//�õ��ռ�����
double Dms2Rad(double Dms);//�Ƕȵ����ȵ�ת��
double Rad2Dms(double Rad);//���ȵ��Ƕȵ�ת��

*/
/*
double relfa(double r)
{
	return log(tan(r*0.0087266 + 0.7853981));
}

double absfa(double s)
{
	return (atan(exp(s)) - 0.7853981) * 114.59156; // 57.29578*2
}

//��γ��ת��ֱ������
void ConvToXY(double jing, double wei, long* cx, long* cy)
//void ConvToXY(double wei,double jing, long* cx, long* cy)
{
	*cx = (long)(((jing-m_fMapjw[0])*x_blc)*blc)+nxoffset;
    *cy = (long)((m_hScreen-(wei-m_fMapjw[3])*y_blc)*blc)+nyoffset;
}
//ֱ������ת����γ��
void ConvToJW(long cx, long cy, double* jing, double* wei)
{
	*jing=(cx-nxoffset)/(blc*x_blc)+m_fMapjw[0];
	//*jing = m_fMapjw[0] + cx*(m_fMapjw[1] - m_fMapjw[0]) / x_blc;
	*wei=(m_hScreen-(cy-nyoffset)/blc)/y_blc+m_fMapjw[3];
	//*wei = (absfa(relfa(m_fMapjw[2]) - cy * (relfa(m_fMapjw[2]) - relfa(m_fMapjw[3])) / m_fMaxCY));
	for(; *jing > 180.0 ; *jing -=360);
	for(; *jing < -180.0 ; *jing += 360);
}
void CenterPoint(double jing, double wei)
{

	nxoffset=m_wScreen/2-(long)(((jing-m_fMapjw[0])*x_blc)*blc);
    nyoffset=m_hScreen/2-(long)((m_hScreen-(wei-m_fMapjw[3])*y_blc)*blc);
}
void ConvToXYs(double* jw, long* xy, int n)
{
	int i;
	double jing, wei;
	for(i=0;i<n;i++)
	{
		jing = *(jw + i*2);
		wei = *(jw + i*2 + 1);
		*(xy + i*2) = (long)((m_fMaxCX * (jing-m_fMapjw[0]))/(m_fMapjw[1]-m_fMapjw[0])+0.5);
		*(xy + i*2+1) = (long)((m_fMaxCY * (relfa(m_fMapjw[2])-relfa(wei)))/(relfa(m_fMapjw[2]) - relfa(m_fMapjw[3]))+0.5);
	}
}
void DPtoVP(FP32 x,FP32 y,int *X,int *Y)
{
	*X=(int)((x-m_xStart)/blc);
	if(m_MapMode==1)
		*Y=m_hScreen-(int)((y-m_yStart)/blc);
	else
		*Y=(int)((y-m_yStart)/blc)-m_hScreen;
}

void VPtoDP(int x,int y,FP32 *X,FP32 *Y)
{
	*X=m_xStart+x*blc;
	if(m_MapMode==1)
		*Y=m_yStart+blc*(m_hScreen-y);
	else
		*Y=m_yStart+blc*(y+m_hScreen);
}

FP32 VLtoDL(int l)
{
	return blc*l;
}

int DLtoVL(FP32 l)
{
	return (int)(l/blc);
}


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
	Rad = Sign * (Degree + Miniute / 60.0 + Second / 3600.0) * GEO_PI / 180.0;
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
	Rad = fabs(Rad * 180.0 / GEO_PI);
	Degree = floor(Rad);
	Miniute = floor(fmod(Rad * 60.0, 60.0));
	Second = fmod(Rad * 3600.0, 60.0);
	Dms = Sign * (Degree + Miniute / 100.0 + Second / 10000.0);
	return Dms;
}
///////////////////////////////////////////////////
// Definition of PrjPoint
///////////////////////////////////////////////////
void InitCoordSys(double MinLon,double MaxLon,double MinLat,double MaxLat,double nxblc,double nyblc,int LcdxLen,int LcdyLen,double centerx,double centery)
{
	blc=1;
	m_CoordSysType=0;
	a = 6378245;
	b = 6356863.01880;
	f = a/(a-b);
	e2 = 1 - ((f - 1) / f) * ((f - 1) / f);
	e12 = (f / (f - 1)) * (f / (f - 1)) - 1;
	n=(a-b)/(a+b);
	A1 = a*(1-n+(pow(n,2)-pow(n,3))*5/4+(pow(n,4)-pow(n,5))*81/64)*GEO_PI/180;
	A2 = (n-pow(n,2)+(pow(n,3)-pow(n,4))*7/8+pow(n,5)*55/64)*3*a/2;
	A3 = (pow(n,2)-pow(n,3)+(pow(n,4)-pow(n,5))*3/4)*15*a/16;
	A4 = (pow(n,3)-pow(n,4)+pow(n,5)*11/16)*35*a/48;
	m_ParaDegree=6;
	SetZoneNum(21);
	m_fMapjw[0]=MinLon;
	m_fMapjw[1]=MaxLon;
	m_fMapjw[2]=MaxLat;
	m_fMapjw[3]=MinLat;
	x_blc=nxblc;
	y_blc=nyblc;
	m_fMaxCX=MaxLon-MinLon;//526.875000;
    m_fMaxCY=MaxLat-MinLat;//660.000000;
	m_xStart=0;
	m_yStart=0;
	m_MapMode=1;
	m_wScreen=LcdxLen;
	m_hScreen=LcdyLen;
	
	//m_fMapjw[0]=-180.0;//��ͼ��߽羭��
	//m_fMapjw[1]=180.0;//��ͼ�ұ߽羭��
	//m_fMapjw[2]=90.0;//��ͼ�ϱ߽�γ��
	//m_fMapjw[3]=-90.0;//��ͼ�±߽�γ�ȣ���γΪ����
	//m_fMaxCX=526.875000;
	//m_fMaxCY=660.000000;
	nxoffset=nyoffset=0;
}

int LoLaToxy()
{
	double X, N, t, t2, m, m2, ng2;
	double sinB, cosB;
	X = A1 * m_Lo * 180.0 / GEO_PI + A2 * sin(2 * m_Lo) + A3 * sin(4 * m_Lo) + A4 * sin(6 * 
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
	return 1;
}
int xyToLoLa()
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
		B0 = B0 * GEO_PI / 180.0;
		B0 = (x - (A2 * sin(2 * B0) + A3 * sin(4 * B0) + A4 * sin(6 * B0))) / A1;
		eta = fabs(B0 - preB0);
	}while(eta > 0.000000001);
	B0 = B0 * GEO_PI / 180.0;
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
	return 1;
}
int SetCoordSysType(int type)
{
	if((type<0)||(type>1)){
		//GUI_DispStringAt("���ʹ���0Ϊ��³����˹�����壬1Ϊ����UGG1975������,2ΪWGS84������",0,0);
		return 0;
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
		A1 = a*(1-n+(pow(n,2)-pow(n,3))*5/4+(pow(n,4)-pow(n,5))*81/64)*GEO_PI/180;
		A2 = (n-pow(n,2)+(pow(n,3)-pow(n,4))*7/8+pow(n,5)*55/64)*3*a/2;
		A3 = (pow(n,2)-pow(n,3)+(pow(n,4)-pow(n,5))*3/4)*15*a/16;
		A4 = (pow(n,3)-pow(n,4)+pow(n,5)*11/16)*35*a/48;		
		return 1;
	}
}
int SetAB(double da,double db)
{
	a=da;
	b=db;
	f = a/(a-b);
	e2 = 1 - ((f - 1) / f) * ((f - 1) / f);
	e12 = (f / (f - 1)) * (f / (f - 1)) - 1;
	n=(a-b)/(a+b);
	A1 = a*(1-n+(pow(n,2)-pow(n,3))*5/4+(pow(n,4)-pow(n,5))*81/64)*GEO_PI/180;
	A2 = (n-pow(n,2)+(pow(n,3)-pow(n,4))*7/8+pow(n,5)*55/64)*3*a/2;
	A3 = (pow(n,2)-pow(n,3)+(pow(n,4)-pow(n,5))*3/4)*15*a/16;
	A4 = (pow(n,3)-pow(n,4)+pow(n,5)*11/16)*35*a/48;
	return 1;
}
int SetParaDegree(double ParaDegree)
{
	m_ParaDegree=ParaDegree;
	return 1;
}
int SetZoneNum(double ZoneNum)
{
    double dOriginLongitude;
	m_ZoneNum=ZoneNum;
	dOriginLongitude=(ZoneNum-1)*m_ParaDegree+m_ParaDegree/2;
	m_OriginLongitude = Dms2Rad(dOriginLongitude);
	return 1;
}
int SetOriginLongitude(double dOriginLongitude)
{
	m_OriginLongitude = Dms2Rad(dOriginLongitude);
	return 1;
}
int SetLoLa(double dLo, double dLa)
{
	m_Lo = Dms2Rad(dLo);
	m_La = Dms2Rad(dLa);
	m_Lo = dLo;
	m_La = dLa;
	LoLaToxy();
	return 1;
}
int GetLoLa(double *dLo, double *dLa)
{
	*dLo = Rad2Dms(m_Lo);
	*dLa = Rad2Dms(m_La);
	return 1;
}
int GetSpaceCoord(double *dx, double *dy,double *dz)
{
	double tmpx;
	tmpx=(a*b)/sqrt(b*b+a*a*tan(m_La)*tan(m_La));
	*dx=tmpx;
	*dy=tmpx*tan(m_Lo);
	*dz=tmpx*tan(m_La);
	return 1;
}
int Setxy(double dx, double dy)
{
	double ddy=floor(dy/1000000);
	double newX,newY;
    double dB,dL;
	newX=dx;newY=dy;
	if(m_ZoneNum!=ddy) 
	{
		double dOriginLongitude=(ddy-1)*m_ParaDegree+m_ParaDegree/2;
		SetOriginLongitude(dOriginLongitude);
		SetTempxy(dx,dy);

		GetLoLa(&dB,&dL);
		SetZoneNum(m_ZoneNum);
		//SetLoLa(dB,dL);
		LoLaToxy();
		Getxy(&newX,&newY);
	}
	SetTempxy(newX,newY);
	return 1;
}
int SetTempxy(double dx, double dy)
{
	double tempy=fmod(dy, 1000000);
	x = dx;
	y = tempy;
	xyToLoLa();
	return 1;
}
int Getxy(double *dx, double *dy)
{
	double newX,newY;
	newX=x;newY=m_ZoneNum*1000000+y;
	*dx = newX;
	*dy = newY;
	return 1;
}

void SetBlc(double nblc)
{
	blc=nblc;
}

void Setxoffset(int xoffset)
{
	nxoffset=xoffset;
}

void Setyoffset(int yoffset)
{
	nyoffset=yoffset;
}

int PointInRect(long xx,long yy,long left,long right,long bottom,long top)
{
	if((xx>left && xx<right) && (yy<bottom && yy>top))
		return	1;
	return	0;
}

bool IsRectCross(FP32 minx,FP32 miny,FP32 maxx,FP32 maxy)
{
	if(minx>=m_mapCoordMap->xMaxScreen||maxx<=m_mapCoordMap->xMinScreen||miny>=m_mapCoordMap->yMaxScreen||maxy<=m_mapCoordMap->yMinScreen)
		return FALSE;
	else
		return TRUE;
}
*/
#define COORTRANS_GLOBALES
#include <math.h>
#include "GUI_Array.h"
#include "gisbase.h"
#include "CoorTrans.h"
// ���ڼ��������


	// ʵ������ӳ����߼�����
	// x:���ȣ�y:γ��
	// X:��Ļ������ Y:��Ļ������
void DPtoVP(MapCoorTrans *pcoortrans,FP32 x, FP32 y, INT32S* X, INT32S* Y)
{

		*X = (INT32S)((x-pcoortrans->xStart)/pcoortrans->blc);
		if(pcoortrans->nMapMode==1)
			*Y = pcoortrans->hScreen - (INT32S)((y-pcoortrans->yStart)/pcoortrans->blc);
		else
			*Y = (INT32S)((y-pcoortrans->yStart)/pcoortrans->blc) - pcoortrans->hScreen ;
}

	// �߼�����ת��Ϊʵ������
	// 
	void VPtoDP(MapCoorTrans *pcoortrans,INT32S x, INT32S y, FP32* X, FP32* Y)
	{
		*X = pcoortrans->xStart + x*pcoortrans->blc;
		if(pcoortrans->nMapMode==1)
			*Y = pcoortrans->yStart+pcoortrans->blc*(pcoortrans->hScreen-y);
		else
			*Y = pcoortrans->yStart+pcoortrans->blc*(y+pcoortrans->hScreen);
	}
	// �жϱ�ע�����Ƿ�ʹ�ã������׼�ظ�
	BOOLEAN IsLabelRectUsed(MapCoorTrans *pcoortrans,GIS_RECT rect)
	{
		INT32S i,nCount;
		/*
		rect.left   -= 1;
		rect.top    -= 1;
		*/
		//return TRUE;
		rect.right  += 1;
		rect.bottom += 1;
        if(pcoortrans->rectVector.InitState== GUI_ARRAY_STATE_NOT_CREATED)
	       GUI_ARRAY_CREATE(&pcoortrans->rectVector);
		nCount = GUI_ARRAY_GetNumItems(&pcoortrans->rectVector);
		for(i=0;i<nCount;i++)
		{
			GIS_RECT xRect;
			GIS_RECT*oRect = GUI_ARRAY_GetpItem(&pcoortrans->rectVector,i);
			if(GIS_IntersectRects(&xRect,&rect,oRect))
			  return TRUE;
		}
		//GIS_RECT* prect = new RECT;
		//if(!prect) return TRUE;
		//*prect = rect;
	    GUI_ARRAY_AddItem(&pcoortrans->rectVector, &rect, sizeof(GIS_RECT));
		return FALSE;
	}
	// �����ע����
	void ClearLabelRect(MapCoorTrans *pcoortrans)
	{
		if(pcoortrans->rectVector.haHandle)
		{
		  GUI_ARRAY_Delete(&pcoortrans->rectVector);
		  memset(&pcoortrans->rectVector,0,sizeof(GUI_ARRAY));
		}
	}
	// ��ȡ��ǰ��Ļ���м���Ļ����
void GetScreenCenter(MapCoorTrans *pcoortrans,INT32S * x,INT32S * y)
{
	*x = pcoortrans->wScreen/2;
	*y = pcoortrans->hScreen/2;
}
	// �Ŵ���
void ZoomTo(MapCoorTrans *pcoortrans,INT32S x1,INT32S y1,INT32S x2,INT32S y2)
{
		//VPtoDP((x1+(x2-x1)/2),(y2+(y1-y2)/2),&xStart,&yStart);
		//SetStart();
	FP32 b11,b12;
	SetCenterINT(pcoortrans,(x1+(x2-x1)/2),(y2+(y1-y2)/2));
	b11 = (FP32)pcoortrans->wScreen/(FP32)(x2-x1);
	b12 = (FP32)pcoortrans->hScreen/(FP32)(y1-y2);
	if(b12<b11)
		b11 = b12;
	pcoortrans->blc = pcoortrans->blc/b11;	
	pcoortrans->m_bMapChanged = TRUE;				
}
	// ��С
void ZoomIn(MapCoorTrans *pcoortrans)
{
		FP32 b11,b12;
		INT32S x1 = pcoortrans->wScreen/6;
		INT32S y1 = 5*pcoortrans->hScreen/6;

		INT32S x2 = 5*pcoortrans->wScreen/6;
		INT32S y2 = pcoortrans->hScreen/6;

		VPtoDP(pcoortrans,(x1+(x2-x1)/2),(y2+(y1-y2)/2),&pcoortrans->xStart,&pcoortrans->yStart);

		b11 = (FP32)pcoortrans->wScreen/(FP32)(x2-x1);
		b12 = (FP32)pcoortrans->hScreen/(FP32)(y1-y2);
		if(b12<b11)
			b11 = b12;
		pcoortrans->blc = pcoortrans->blc/b11;	
		pcoortrans->m_bMapChanged = TRUE;				
	}
	// �Ŵ�
void ZoomOut(MapCoorTrans *pcoortrans)
{
		FP32 b11,b12;
	    INT32S x1 = pcoortrans->wScreen/6;
		INT32S y1 = 5*pcoortrans->hScreen/6;

		INT32S x2 = 5*pcoortrans->wScreen/6;
		INT32S y2 = pcoortrans->hScreen/6;

		VPtoDP(pcoortrans,(x1+(x2-x1)/2),(y2+(y1-y2)/2),&pcoortrans->xStart,&pcoortrans->yStart);

		b11 = (FP32)pcoortrans->wScreen/(FP32)(x2-x1);
		b12 = (FP32)pcoortrans->hScreen/(FP32)(y1-y2);

		if(b12<b11)
			b11 = b12;				
		pcoortrans->blc = pcoortrans->blc*b11;

		pcoortrans->m_bMapChanged = TRUE;
}

	/*
	 ��������㾭γ�ȱ�ʾ����Ļ����
	*/
void SetScreenXY(MapCoorTrans *pcoortrans)
{
		pcoortrans->xMinScreen = pcoortrans->xStart;
		pcoortrans->yMinScreen = pcoortrans->yStart;
		pcoortrans->xMaxScreen = pcoortrans->xMinScreen + pcoortrans->blc*pcoortrans->wScreen;
		pcoortrans->yMaxScreen = pcoortrans->yMinScreen + pcoortrans->blc*pcoortrans->hScreen;
}
	// ��γ���Ƿ�����Ļ��
BOOLEAN IsInScreen(MapCoorTrans *pcoortrans,FP32 x,FP32 y)
{
	if( x < pcoortrans->xMaxScreen && y <pcoortrans->yMaxScreen && x >= pcoortrans->xMinScreen && y>= pcoortrans->yMinScreen  ) return TRUE;
	return FALSE; 
}
    // ���õ�ͼ����Ļ�ϵ�����λ��
void SetCenterFP(MapCoorTrans *pcoortrans,FP32 x, FP32 y)
{
	  if(pcoortrans->m_fCenterX!=x)
	  {
		pcoortrans->m_fCenterX = x;  //����ͼ��ʱ����Ļ����
		pcoortrans->m_fCenterY = y;  // ����ͼ��ʱ����Ļ����
		SetStart(pcoortrans);
		pcoortrans->m_bMapChanged = TRUE;
	  }
}
// ���õ�ͼ����Ļ�ϵ�����λ��
void SetCenterINT(MapCoorTrans *pcoortrans,INT32S x, INT32S y)
{
	if(!pcoortrans->m_nCenterX!=x)
	{
		pcoortrans->m_nCenterX = x;  //����ͼ��ʱ����Ļ����
		pcoortrans->m_nCenterY = y;  // ����ͼ��ʱ����Ļ����
		VPtoDP(pcoortrans,x,y,&pcoortrans->m_fCenterX,&pcoortrans->m_fCenterY);
		SetStart(pcoortrans);
		pcoortrans->m_bMapChanged = TRUE;
	}
}
// ���õ�ͼ����Ļ�ϵ�����λ�õ�ƫ����
void SetCenterOffset(MapCoorTrans *pcoortrans,INT32S x, INT32S y)
{
		pcoortrans->m_nCenterX -= x;  //����ͼ��ʱ����Ļ����
		pcoortrans->m_nCenterY -= y;  // ����ͼ��ʱ����Ļ����
		VPtoDP(pcoortrans,pcoortrans->m_nCenterX,pcoortrans->m_nCenterY,&pcoortrans->m_fCenterX,&pcoortrans->m_fCenterY);
		SetStart(pcoortrans);
		pcoortrans->m_bMapChanged = TRUE;
}
void Left(MapCoorTrans *pcoortrans)
{
	DPtoVP(pcoortrans,pcoortrans->m_fCenterX,pcoortrans->m_fCenterY,&pcoortrans->m_nCenterX,&pcoortrans->m_nCenterY);
	SetCenterINT(pcoortrans,pcoortrans->m_nCenterX-30,pcoortrans->m_nCenterY);
}
void Right(MapCoorTrans *pcoortrans)
{
	DPtoVP(pcoortrans,pcoortrans->m_fCenterX,pcoortrans->m_fCenterY,&pcoortrans->m_nCenterX,&pcoortrans->m_nCenterY);
	SetCenterINT(pcoortrans,pcoortrans->m_nCenterX+30,pcoortrans->m_nCenterY);
}
void Up(MapCoorTrans *pcoortrans)
{
	DPtoVP(pcoortrans,pcoortrans->m_fCenterX,pcoortrans->m_fCenterY,&pcoortrans->m_nCenterX,&pcoortrans->m_nCenterY);
	SetCenterINT(pcoortrans,pcoortrans->m_nCenterX,pcoortrans->m_nCenterY-30);
}
void Down(MapCoorTrans *pcoortrans)
{
	DPtoVP(pcoortrans,pcoortrans->m_fCenterX,pcoortrans->m_fCenterY,&pcoortrans->m_nCenterX,&pcoortrans->m_nCenterY);
	SetCenterINT(pcoortrans,pcoortrans->m_nCenterX,pcoortrans->m_nCenterY+30);
}
INT32S DLtoVL(MapCoorTrans *pcoortrans,FP32 l)
{
	return (INT32S)l/pcoortrans->blc;
}

FP32 VLtoDL(MapCoorTrans *pcoortrans,INT32S l)
{
	return l*pcoortrans->blc;
}

void SetStart(MapCoorTrans *pcoortrans)
{
	pcoortrans->xStart = pcoortrans->m_fCenterX - VLtoDL(pcoortrans,pcoortrans->wScreen/2);
	pcoortrans->yStart = pcoortrans->m_fCenterY - VLtoDL(pcoortrans,pcoortrans->hScreen/2);
	DPtoVP(pcoortrans,pcoortrans->m_fCenterX,pcoortrans->m_fCenterY,&pcoortrans->m_nCenterX,&pcoortrans->m_nCenterY);
}

BOOLEAN IsRectCrossRect(MapCoorTrans *pcoortrans,GIS_RECT gisRect)
{
	return IsRectCross(pcoortrans,gisRect.left,gisRect.top,gisRect.right,gisRect.bottom);
}
BOOLEAN IsRectCross(MapCoorTrans *pcoortrans,FP32 minx,FP32 miny,FP32 maxx,FP32 maxy)
{
	if(minx>=pcoortrans->xMaxScreen||maxx<=pcoortrans->xMinScreen||miny>=pcoortrans->yMaxScreen||maxy<=pcoortrans->yMinScreen)
		return FALSE;
	else
		return TRUE;
}
void FreeMapCoorTrans(MapCoorTrans *pcoortrans)
{
   ClearLabelRect(pcoortrans);
   memset(pcoortrans,0,sizeof(MapCoorTrans));
}
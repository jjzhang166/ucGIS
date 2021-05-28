// Draw.cpp: implementation of the CDraw class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"	
#include "Draw.h"
#include "tchar.h"
#include "winnls.h"
#include ".\draw.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 用于计算坐标的

void mapCOORDMAP::DPtoVP(float x, float y, int* X, int* Y)
{

	*X = (int)((x-xStart)/scale);
	if(nMapMode==1)
		*Y = hScreen - (int)((y-yStart)/scale);
	else
		*Y = (int)((y-yStart)/scale) - hScreen ;
}

// 逻辑坐标转换为实际坐标
// 
void mapCOORDMAP::VPtoDP(int x, int y, float* X, float* Y)
{
	*X = xStart + x*scale;
	if(nMapMode==1)
		*Y = yStart+scale*(hScreen-y);
	else
		*Y = yStart+scale*(y+hScreen);
}
mapCOORDMAP::~mapCOORDMAP()
{
	ClearLabelRect();
}
mapCOORDMAP::mapCOORDMAP()
{
	scale = 0.00001;
	m_bMapChanged = true;
}
// 判断标注区域是否被使用，避免标准重复
bool mapCOORDMAP::IsLabelRectUsed(RECT rect)
{
	/*
	rect.left   -= 1;
	rect.top    -= 1;
	*/
	rect.right  += 1;
	rect.bottom += 1;

	int nCount = rectVector.size();
	for(int i=0;i<nCount;i++)
	{
		RECT xRect;
		RECT oRect = *rectVector[i];
		IntersectRect(&xRect,&rect,&oRect);
		if(xRect.bottom)
			return true;
	}
	RECT* prect = new RECT;
	if(!prect) return true;
	*prect = rect;
	rectVector.push_back(prect);
	return false;
}
// 清除标注矩形
void mapCOORDMAP::ClearLabelRect()
{
	int nCount = rectVector.size();
	for(int i=0;i<nCount;i++)
	{
		delete rectVector[i];
		rectVector[i] = NULL;
	}
	rectVector.clear();
}
// 获取当前屏幕的中间屏幕坐标
void mapCOORDMAP::GetScreenCenter(int& x,int& y)
{
	x = wScreen/2;
	y = hScreen/2;
}
// 缩小
void mapCOORDMAP::ZoomIn()
{
	int x1 = wScreen/6;
	int y1 = 5*hScreen/6;

	int x2 = 5*wScreen/6;
	int y2 = hScreen/6;
	float b11 = (float)wScreen/(float)(x2-x1);
	float b12 = (float)hScreen/(float)(y1-y2);
	if(b12<b11)
		b11 = b12;
	if(scale/b11<3.3840945e-007) return;

	VPtoDP((x1+(x2-x1)/2),(y2+(y1-y2)/2),&xStart,&yStart);

	//float b11 = (float)wScreen/(float)(x2-x1);
	//float b12 = (float)hScreen/(float)(y1-y2);
	//if(b12<b11)
	//	b11 = b12;
	scale = scale/b11;	
	m_bMapChanged = true;				
}
// 放大
void mapCOORDMAP::ZoomOut()
{

	int x1 = wScreen/6;
	int y1 = 5*hScreen/6;

	int x2 = 5*wScreen/6;
	int y2 = hScreen/6;


	VPtoDP((x1+(x2-x1)/2),(y2+(y1-y2)/2),&xStart,&yStart);

	float b11 = (float)wScreen/(float)(x2-x1);
	float b12 = (float)hScreen/(float)(y1-y2);

	if(b12<b11)
		b11 = b12;				
	scale = scale*b11;

	m_bMapChanged = true;
}
// 放大至
void mapCOORDMAP::ZoomTo(int x1,int y1,int x2,int y2)
{
	float b11 = (float)wScreen/(float)(x2-x1);
	float b12 = (float)hScreen/(float)(y1-y2);
	if(b12<b11)
		b11 = b12;
	if(scale/b11<3.3840945e-007) return;
	//VPtoDP((x1+(x2-x1)/2),(y2+(y1-y2)/2),&xStart,&yStart);
	//SetStart();
	SetCenter((x1+(x2-x1)/2),(y2+(y1-y2)/2));


	scale = scale/b11;	
	m_bMapChanged = true;				
}
// 设置地图在屏幕上的中心位置的偏移量
void mapCOORDMAP::SetCenterOffset(int x, int y)
{
	VPtoDP(wScreen/2-x,hScreen/2-y,&m_fCenterX,&m_fCenterY);
	SetStart();
	m_bMapChanged = true;
}
/*
计算出浮点经纬度表示的屏幕矩形
*/
void mapCOORDMAP::SetScreenXY()
{
	xMinScreen = xStart;
	yMinScreen = yStart;
	xMaxScreen = xMinScreen + scale*wScreen;
	yMaxScreen = yMinScreen + scale*hScreen;
}
// 经纬度是否在屏幕中
bool mapCOORDMAP::IsInScreen(float x,float y)
{
	if( x < xMaxScreen && y <yMaxScreen && x >= xMinScreen && y>= yMinScreen  ) return true;
	return false; 
}
// 设置地图在屏幕上的中心位置
void mapCOORDMAP::SetCenter(float x, float y)
{
	if(m_fCenterX!=x)
	{
		m_fCenterX = x;  //绘制图形时的屏幕中心
		m_fCenterY = y;  // 绘制图形时的屏幕中心
		DPtoVP(x,y,&m_nCenterX,&m_nCenterY);
		SetStart();
		m_bMapChanged = true;		
	}
}
// 设置地图在屏幕上的中心位置
void mapCOORDMAP::SetCenter(int x, int y)
{
	if(!m_nCenterX!=x)
	{
		m_nCenterX = x;  //绘制图形时的屏幕中心
		m_nCenterY = y;  // 绘制图形时的屏幕中心
		VPtoDP(x,y,&m_fCenterX,&m_fCenterY);
		SetStart();
		m_bMapChanged = true;
	}
}
void mapCOORDMAP::Left()
{
	DPtoVP(m_fCenterX,m_fCenterY,&m_nCenterX,&m_nCenterY);
	SetCenter(m_nCenterX-30,m_nCenterY);
}
void mapCOORDMAP::Right()
{
	DPtoVP(m_fCenterX,m_fCenterY,&m_nCenterX,&m_nCenterY);
	SetCenter(m_nCenterX+30,m_nCenterY);
}
void mapCOORDMAP::Up()
{
	DPtoVP(m_fCenterX,m_fCenterY,&m_nCenterX,&m_nCenterY);
	SetCenter(m_nCenterX,m_nCenterY-30);
}
void mapCOORDMAP::Down()
{
	DPtoVP(m_fCenterX,m_fCenterY,&m_nCenterX,&m_nCenterY);
	SetCenter(m_nCenterX,m_nCenterY+30);
}
int mapCOORDMAP::DLtoVL(float l)
{
	return (int)l/scale;
}

float mapCOORDMAP::VLtoDL(int l)
{
	return l*scale;
}

void mapCOORDMAP::SetStart(void)
{
	xStart = m_fCenterX - VLtoDL(wScreen/2);
	yStart = m_fCenterY - VLtoDL(hScreen/2);
}


CDraw::CDraw()
{
	memset(&m_Rect,0,sizeof(m_Rect));

	m_AtoWLabeled = false;
	m_nLayer = 0;

	m_lpszLabel = NULL;

	m_mapCoordMap = NULL;
	m_nType = otNull;
	m_nSubType = otNull;

	m_bEditMode = false;
	m_bShowAllLabel = false;
    m_fileoffsetlen=0;
#ifdef UNDER_WINCE
	m_ceDraw = NULL;
#endif
}

CDraw::~CDraw()
{

	if(m_lpszLabel)
		delete[] m_lpszLabel;

	m_lpszLabel = NULL;
}
void CDraw::AToWLabel()
{
	/*
	if(!m_AtoWLabeled)
	{
	m_lpszLabel = new TCHAR[strlen(m_pszLabel)];
	AToW(m_pszLabel,m_lpszLabel);
	m_AtoWLabeled = true;
	}
	*/
}

void CDraw::AToW(char *sourceChar,TCHAR *wChar)
{
	if(!wChar) return;
	char	*p = sourceChar;
	int widecharlen=MultiByteToWideChar( //计算从Ansi转换到Unicode后需要的字节数
		CP_ACP,
		MB_COMPOSITE,
		(char*)p, //要转换的Ansi字符串
		-1, //自动计算长度
		0,
		0
		);
#ifdef UNDER_NT
	LPWSTR lpBuf=NULL;  
	lpBuf = (LPWSTR) LocalAlloc (LPTR, widecharlen*2);
	if(!lpBuf) return;

	memset(lpBuf,0,widecharlen*2);

	MultiByteToWideChar( //从Ansi转换到Unicode字符
		CP_ACP,
		MB_COMPOSITE,
		(char*)p,
		-1,
		lpBuf,	// 转换到 strReceive
		widecharlen // 最多转换widecharlen个Unicode字符
		);
	CString str(lpBuf);

	memcpy(wChar, str,widecharlen*2);

	LocalFree (lpBuf);  // Free lpBuf
#else
	MultiByteToWideChar( //从Ansi转换到Unicode字符
		CP_ACP,
		MB_COMPOSITE,
		(char*)p,
		-1,
		wChar,	// 转换到 strReceive
		widecharlen // 最多转换widecharlen个Unicode字符
		);	
#endif
}
// 从Unicode转换到Ansi
bool CDraw::WToA(TCHAR* wChar, char* cChar)
{

	int multibytelen=WideCharToMultiByte(  //计算从Unicode转换到Ansi后需要的字节数
		CP_ACP,  //根据ANSI code page转换
		WC_COMPOSITECHECK | WC_DEFAULTCHAR, //转换出错用缺省字符代替
		wChar,  //要转换的字符串地址
		lstrlen(wChar),  //要转换的个数
		0,  //转换后字符串放置的地址
		0,  //最多转换字符的个数，为0表示返回转换Unicode后需要多少个字节
		0,  //缺省的字符:"\0"
		0   //缺省的设置
		);
	if(multibytelen > 0)
	{
		WideCharToMultiByte(  //转换Unicode到Ansi
			CP_ACP,
			WC_COMPOSITECHECK | WC_DEFAULTCHAR,
			wChar,
			lstrlen(wChar),
			(char *)cChar,  //转换到缓冲区中
			128,  //最多128个字节
			0,
			0);
	}
	cChar[multibytelen] = '\0';
	return true;
}

TCHAR pszLabel[100];
void CDraw::ReadFromFile(FILE *fp,int &fileoffset)
{
	BYTE nLableLen = 0;
    m_fileoffsetlen=fileoffset;
	fread(&nLableLen,1,sizeof(BYTE),fp);
	fileoffset+=sizeof(BYTE);
	//char* pszLabel = new char[nLableLen+1];
	if(!pszLabel)
	{
		MessageBox(NULL,_T("申请内存失败！"),_T("错误"),MB_ICONERROR);
	}
	pszLabel[nLableLen] = '\0';
	if(nLableLen>0)
		fread(pszLabel,sizeof(TCHAR),nLableLen,fp);
   	fileoffset+=nLableLen*sizeof(TCHAR);

	fread(&m_nLayer,1,sizeof(m_nLayer),fp);
 	fileoffset+=sizeof(m_nLayer);
	fread(&m_Rect,1,sizeof(m_Rect),fp);
 	fileoffset+=sizeof(m_Rect);
	fread(&m_nType,1,sizeof(m_nType),fp);
 	fileoffset+=sizeof(m_nType);
	fread(&m_nSubType,1,sizeof(m_nSubType),fp);
 	fileoffset+=sizeof(m_nSubType);
	m_lpszLabel = new TCHAR[lstrlen(pszLabel)+2];
	if(!m_lpszLabel)
	{
		MessageBox(NULL,_T("申请内存失败！"),_T("错误"),MB_ICONERROR);
		return ;
	}
	memset(m_lpszLabel,0,nLableLen);
	lstrcpy(m_lpszLabel,pszLabel);
	//delete[] pszLabel;
}
void CDraw::WriteToFile(FILE	*fp)
{

	BYTE nLableLen = lstrlen(m_lpszLabel);
	fwrite(&nLableLen,1,sizeof(BYTE),fp);
	if(nLableLen>0)
		fwrite(m_lpszLabel,sizeof(TCHAR),nLableLen,fp);
	fwrite(&m_nLayer,1,sizeof(m_nLayer),fp);
	fwrite(&m_Rect,1,sizeof(m_Rect),fp);
	fwrite(&m_nType,1,sizeof(m_nType),fp);
	fwrite(&m_nSubType,1,sizeof(m_nSubType),fp);


}
void CDraw::Draw(HDC hDC,int nDrawMode)
{

}
void CDraw::DrawLabel(HDC hDC)
{
	AToWLabel();
}


// 实际坐标映象成逻辑坐标
// x:经度；y:纬度
// X:屏幕横坐标 Y:屏幕纵坐标
void CDraw::DPtoVP(float x, float y, int* X, int* Y)
{
	if(!m_mapCoordMap) return;	
	*X = (int)((x-m_mapCoordMap->xStart)/m_mapCoordMap->scale);
	if(m_mapCoordMap->nMapMode==1)
		*Y = m_mapCoordMap->hScreen - (int)((y-m_mapCoordMap->yStart)/m_mapCoordMap->scale);
	else
		*Y = (int)((y-m_mapCoordMap->yStart)/m_mapCoordMap->scale) - m_mapCoordMap->hScreen ;
}


// 逻辑坐标转换为实际坐标
// 
void CDraw::VPtoDP(int x, int y, float* X, float* Y)
{
	if(!m_mapCoordMap) return;
	*X = m_mapCoordMap->xStart + x*m_mapCoordMap->scale;
	if(m_mapCoordMap->nMapMode==1)
		*Y = m_mapCoordMap->hScreen+m_mapCoordMap->scale*(m_mapCoordMap->hScreen-y);
	else
		*Y = y-m_mapCoordMap->yStart+m_mapCoordMap->scale*(y+m_mapCoordMap->hScreen);
}
bool CDraw::IsRectCross()
{
	return IsRectCross(m_Rect.fMinX,m_Rect.fMinY,m_Rect.fMaxX,m_Rect.fMaxY);
}
bool CDraw::IsRectCross(float minx,float miny,float maxx,float maxy)
{
	if(minx>=m_mapCoordMap->xMaxScreen||maxx<=m_mapCoordMap->xMinScreen||miny>=m_mapCoordMap->yMaxScreen||maxy<=m_mapCoordMap->yMinScreen)
		return false;
	else
		return true;
}

// 自定义的字符转化为数字
int CDraw::myatoi(TCHAR* ls)
{
	
	char ch[50]={0};
	WToA(ls,ch);

	return atoi(ch);
}

// 设置类型
void CDraw::SetType(ObjectTypeConstants nType, bool bIsSub)
{
	if(!bIsSub)
		m_nType = nType;
	else
		m_nSubType = nType;
}
const float pi = 3.14;
//计算点(x1,y1)与点(x2,y2)间的距离   
float   CDraw::CalDisp(float x1,float y1,float x2,float y2)
{   
	return   (float)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));   
}   

//作用：计算点（xx,yy)到线段（x1,y1)(x2,y2)的距离，返回计算的距离值   

//函数PointRgn作用：判断一个点是否在任意多边形区域内   
//参数：Numble-边界点数，x,y是点的坐标，多边形顶点的坐标在结构数组PointList中   
//返回：1-点在多边形区域0-不在   
float   CDraw::PointLine(float xx,float yy,float x1,float y1,float x2,float y2)
{   
	float   a,   b,   c,   ang1,   ang2,   ang;   
	//计算三条边的距离   
	a=CalDisp(x1,   y1,   xx,   yy); if(a   ==   0.0) return   0.0;   
	b=CalDisp(x2,   y2,   xx,   yy); if(b   ==   0.0) return   0.0;   
	c=CalDisp(x1,   y1,   x2,   y2);   

	//如果（x1,y1)和(x2,y2)是一个点直接返回距离   
	if(c==0.0)   return   a;   
	if(a<b)   //如果(xx,yy)的点(x1,y1）这条边较短   
	{   
		if(y1==y2)   
		{   
			if(x1<x2)   
				ang1=0;   
			else   
				ang1=(float)pi;   
		}   
		else   
		{   
			ang1=(float)acos((x2-x1)/c);   
			if(y1>y2)ang1=(float)pi*2-ang1;     //直线(x1,y1)-(x2,y2)的弧度   
		}   
		ang2=(float)acos((xx-x1)/a);   
		if(y1>yy)ang2=(float)pi*2-ang2;     //直线(x1,y1)-(xx,yy)的弧度   
		ang=ang2-ang1;   
		if(ang<0)ang=-ang;   
		if(ang>pi)   ang=(float)pi*2-ang;     //交角的大小   
		if(ang>pi/2)   return   a;         //如果为钝角，直接返回距离   
		else     
			return   (a*(float)sin(ang));   //否则返回计算得到的距离     
	}   
	else   //如果(xx,yy)的点(x2,y2）这条边较短   
	{   
		if(y1==y2)   
		{   
			if(x1<x2)   
				ang1=(float)pi;   
			else   
				ang1=0;   
		}   
		else   
		{   
			ang1=(float)acos((x1-x2)/c);           //直线（x2,y2)-(x1,y1)的斜率的弧度   
			if(y2>y1)ang1=(float)pi*2-ang1;   
		}   
		ang2=(float)acos((xx-x2)/b);           //直线（x2,x1）-(xx,yy)的斜率的弧度   
		if(y2>yy)ang2=(float)pi*2-ang2;   
		ang=ang2-ang1;   
		if(ang<0)   ang=-ang;   
		if(ang>pi)   ang=(float)pi*2-ang;     //交角的大小   ?   
		if(ang>pi/2)     return   b;       //如果为钝角，直接返回距离   
		else     
			return(b*(float)sin(ang));     //否则返回计算得到的距离   
	}   
}   

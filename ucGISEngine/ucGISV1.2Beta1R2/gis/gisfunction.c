#define GISFUNCTION_GLOBALES
#include "gisfunction.h"
#include <math.h> 
//线段求交点函数
BOOLEAN findcrosspoint(FP64	x11,FP64	y11,FP64	x12,FP64	y12,FP64	x21,FP64	y21,FP64	x22,FP64	y22,FP64	*xx,FP64	*yy)
{
	FP64 t1, z1, m1, t2, z2, m2;

	*xx = *yy = 0.0;

	z1 = (x22 - x21) * (y21 - y11) - (x21 - x11) * (y22 - y21);
	m1 = (x22 - x21) * (y12 - y11) - (x12 - x11) * (y22 - y21);

	if(fabs(m1) < 1e-10)	// 平行 
	{
		*xx	= (x11+x12+x21+x22)/4;
		*yy	= (y11+y12+y21+y22)/4;
		return FALSE;
	}
	else
		t1 = z1 / m1;

	z2 = (x21 - x11) * (y12 - y11) - (x12 - x11) * (y21 - y11);
	m2 = (-1.0) * m1;
	t2 = z2 / m2;

	if(t1 > 1e-10 && t1 < 1.0-1e-10 && t2 > 1e-10 && t2 < 1.0-1e-10)
	{
		*xx = x11 + t1 * (x12 - x11);
		*yy = y11 + t1 * (y12 - y11);

		return TRUE;
	}
	else
	{
		*xx = x11 + t1 * (x12 - x11);
		*yy = y11 + t1 * (y12 - y11);
	
		return FALSE;
	}
}

//点在矩形中的判断函数
BOOLEAN pointInRect(FP64 xx, FP64 yy, FP64 left, FP64 right, FP64 bottom, FP64 top)
{
	if((xx>left && xx<right) && (yy>bottom && yy<top))
		return	TRUE;
	return	FALSE;
}

//矩形和矩形相交判断函数
BOOLEAN rectInRect(FP64 minx, FP64 maxx, FP64 miny, FP64 maxy, FP64 left, FP64 right, FP64 bottom, FP64 top)
{
	if(maxx < left || minx > right || miny > top || maxy < bottom)
		return	FALSE;
	return	TRUE;
}

//矩形在多边形中的判断函数
int	pointInPolygon(FP64 xx,FP64 yy,int num,GeoPoint *pgeopoint)//FP64 *x,FP64 *y)
{
	int		insideFlag,flag;
	int		i;
	FP64	x1,y1,x0,y0,x2,y2;
	FP64 xp;
	insideFlag	=0;

	for(i=0;i<=num-1;i++)
	{
		
		x1	=pgeopoint[i].lon;		y1	=pgeopoint[i].lat;
		if(i==num-1)
		{
			x0	=pgeopoint[i].lon;	y0	=pgeopoint[i].lat;
		}
		else
		{
			x0	=pgeopoint[i+1].lon;	y0	=pgeopoint[i+1].lat;
		}
		if(i==0)
		{
			x2	=pgeopoint[i].lon;	y2	=pgeopoint[i].lat;
		}
		else
		{
			x2	=pgeopoint[i-1].lon;	y2	=pgeopoint[i-1].lat;
		}	
		if(y1!=yy)
		{
			if(y0>yy&&y1<yy||y0<yy&&y1>yy)
			{
				if(x0<xx&&x1<xx)
				{
					insideFlag	=insideFlag;
				}
				if(x0>xx&&x1>xx)
				{
					if(insideFlag==0)		flag=1;
					if(insideFlag==1)		flag=0;

					insideFlag	=flag;
				}
				if(x0>=xx&&x1<=xx||x0<=xx&&x1>=xx)
				{
					xp=ifLineCut(x0,x1,y0,y1,yy);
					if(xp<xx)
					{
						insideFlag	=insideFlag;
					}
					if(xp>=xx)
					{
						if(insideFlag==0)		flag=1;
						if(insideFlag==1)		flag=0;

						insideFlag	=flag;
					}
				}
			}
			else
			{
				insideFlag	=insideFlag;
			}
		}
		else
		{
			if(x1<xx)
				insideFlag	=insideFlag;
			else
			{
				if(y0>yy)
				{
					if(insideFlag==0)		flag=1;
					if(insideFlag==1)		flag=0;
					insideFlag	=flag;

				}
				if(y2>yy)
				{
					if(insideFlag==0)		flag=1;
					if(insideFlag==1)		flag=0;

					insideFlag	=flag;
				}
			}
		}
	}
	return	insideFlag;
}

//点在多边形中函数判断的辅助函数
FP64	ifLineCut(FP64 x0,FP64 x1,FP64 y0,FP64 y1,FP64 yy)
{
	FP64 xx;
	FP64 k;
	if(x1==x0)
	{
		xx	=x1;	
	}
	else
	{
		k	=(y1-y0)/(x1-x0);

		xx	=x0+(yy-y0)/k;
	}
	return	xx;
}

//线捕捉函数
BOOLEAN searchLine(FP64 xx,FP64 yy,long pointnum,FP64 *xcoord,FP64 *ycoord,FP64 searchID)
{
	FP64	minx,maxx,miny,maxy;
	long	i;
	BOOLEAN	bsearch;
	FP64		xb,yb;
	FP64		xe,ye;
	FP64	mindx,mindy;
	FP64	dd;

	bsearch		= FALSE;
	
	//计算最大最小值
	for(i=0;i<pointnum;i++)
	{
		if(i == 0)
		{
			minx	= maxx	= xcoord[i];
			miny	= maxy	= ycoord[i];
		}
		else
		{
			if(minx > xcoord[i])		minx	= xcoord[i];
			if(maxx < xcoord[i])		maxx	= xcoord[i];
			if(miny > ycoord[i])		miny	= ycoord[i];
			if(maxy < ycoord[i])		maxy	= ycoord[i];
		}
	}
	
	if(pointInRect(xx,yy,minx-searchID,maxx+searchID,miny-searchID,maxy+searchID))
	{
		for(i=0;i<pointnum-1;i++)
		{
			
			xb		= xcoord[i];
			yb		= ycoord[i];
			xe		= xcoord[i+1];
			ye		= ycoord[i+1];
			
			if(xb > xe)	
			{
				minx	= xe;	maxx	= xb;
			}
			else
			{
				minx	= xb;	maxx	= xe;
			}
			
			if(yb > ye)
			{
				miny	= ye;	maxy	= yb;
			}
			else
			{
				miny	= yb;	maxy	= ye;
			}
			
			if(pointInRect(xx,yy,minx-searchID,maxx+searchID,miny-searchID,maxy+searchID))
			{
				if(fabs(yb-ye) <= 1e-6)		//==0
				{
					mindx	= mindy	= fabs(yy-yb);
					dd		= mindx;
				}
				else
				{
					mindx	= fabs((xe-xb)*(yy-yb)/(ye-yb)+xb-xx);
					mindy	= fabs((ye-yb)*(xx-xb)/(xe-xb)+yb-yy);

					if(mindx > mindy)	dd	= mindy;
					else				dd	= mindx;
				}
				
				if(dd <searchID)
				{	
					bsearch	= TRUE;	
					break;
				}
			}
		}
	}

	return	bsearch;
}

FP64 distOfPointAndPoint(FP64 x1, FP64 y1, FP64 x2, FP64 y2)
{
	FP64	dist;

	dist	= (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
	dist	= sqrt(dist);

	return	dist;
}

int getMinDistance(FP64 d1, FP64 d2, FP64 d3, FP64 d4)
{
	int	code;
	if(d1>d2)
	{
		if(d3>d4)
		{
			if(d2>d4)
				code	= 4;
			else
				code	= 2;
		}
		else	//x3<x4
		{
			if(d2>d3)	
				code	= 3;
			else
				code	= 2;
		}
	}
	else	//x1<x2
	{
		if(d3>d4)
		{
			if(d1>d4)
				code	= 4;
			else
				code	= 1;
		}
		else	//x3<x4
		{
			if(d1>d3)
				code	= 3;
			else
				code	= 1;
		}
	}

	return	code;
}

//点到直线的距离
FP64	pointToLine(FP64 pointx,FP64 pointy,FP64 x1,FP64 y1,FP64 x2,FP64 y2)
{
	FP64	distance;
	FP64	area;
	FP64	s;

	area	= getArea(pointx,pointy,x1,y1,x2,y2);
	if(area==0.0)
	{
		distance	= 0.0;
	}
	else
	{
		s	= sqrt(pow((x1-x2),2.0)+pow((y1-y2),2.0));
		if(s==0.0)
		{
			distance = sqrt(pow((pointx-x2),2.0)+pow((pointy-y2),2.0));
		}
		else
		{
			distance	= 2*area/s;
		}
	}

	return	 distance;
}

//面积算法
FP64	getArea(FP64 x1,FP64 y1,FP64 x2,FP64 y2,FP64 x3,FP64 y3)
{
	FP64 m_Area;
	m_Area=((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))/2;
	if(m_Area<0.0)
		m_Area	= -m_Area;
	return	m_Area;
}

//获取角度
FP64 getAngle(FP64 dx, FP64 dy)
{
	FP64	angle;

	if(fabs(dy) < 1e-10)
	{
		if(dx > 0.0)					angle	= 0.0;
		else							angle	= GEO_PI;
	}
	else	if(dy > 1e-10)
	{
		if(dx > 1e-10)					angle	= atan2(fabs(dy),fabs(dx));		
		else	if(fabs(dx) < 1e-10)	angle	= GEO_PI/2;
		else	if(dx < 1e-10)			angle	= GEO_PI-atan2(fabs(dy),fabs(dx));
	}
	else
	{
		if(dx > 1e-10)					angle	= GEO_PI*2-atan2(fabs(dy),fabs(dx));
		else	if(fabs(dx) < 1e-10)	angle	= GEO_PI*3/2;
		else	if(dx < 1e-10)			angle	= GEO_PI+atan2(fabs(dy),fabs(dx));
	}
	return	angle;
}


FP64 GetAngleEx(FP64 dx,FP64 dy)
{
	FP64	angle;

	if(fabs(dy) < 1e-10)
	{
		if(dx > 0.0)					angle	= GEO_PI/2;
		else							angle	= 3*GEO_PI/2;
	}
	else	if(dy > 1e-10)
	{
		if(dx > 1e-10)					angle	= atan2(fabs(dy),fabs(dx));		
		else	if(fabs(dx) < 1e-10)	angle	= 0.0;
		else	if(dx < 1e-10)			angle	= 3*GEO_PI/2+atan2(fabs(dy),fabs(dx));
	}
	else
	{
		if(dx > 1e-10)					angle	= GEO_PI/2+atan2(fabs(dy),fabs(dx));
		else	if(fabs(dx) < 1e-10)	angle	= GEO_PI;
		else	if(dx < 1e-10)			angle	= 3*GEO_PI/2-atan2(fabs(dy),fabs(dx));
	}
	return	angle*180/GEO_PI;
}

void rotateOfFP64(FP64 *xx, FP64 *yy, FP64 x0, FP64 y0, FP64 angle)
{
	FP64	xt,yt;
	xt	= (x0+(*xx)*cos(angle)-(*yy)*sin(angle));
	yt	= (y0+(*yy)*cos(angle)+(*xx)*sin(angle));

	*xx	= xt;
	*yy	= yt;
}

void getCoordFromLine(FP64 *xx, FP64 *yy, int num, int sep, int ser, FP64 *cx, FP64 *cy,FP64 *angle)
{
	FP64	dist;
	FP64	subdist;
	int		i;

	dist	= 0.0;
	for(i=0;i<num-1;i++)
	{
		dist	= dist + sqrt((xx[i]-xx[i+1])*(xx[i]-xx[i+1]) + (yy[i]-yy[i+1])*(yy[i]-yy[i+1]));
	}

	subdist	= dist/sep * ser;
	
	for(i=0;i<num-1;i++)
	{
		dist	= sqrt((xx[i]-xx[i+1])*(xx[i]-xx[i+1]) + (yy[i]-yy[i+1])*(yy[i]-yy[i+1]));

		if(dist>subdist)
		{
			*angle	= getAngle(xx[i+1]-xx[i],yy[i+1]-yy[i]);
			
			*cx	= xx[i] + subdist*cos(*angle);
			*cy	= yy[i] + subdist*sin(*angle);

			break;
		}
		else
		{
			subdist	= subdist - dist;
		}
	}
}

BOOLEAN rectIncludeRect(FP64 minx, FP64 maxx, FP64 miny, FP64 maxy, FP64 left, FP64 right, FP64 bottom, FP64 top)
{
	if((minx>left&&minx<right) && (maxx>left&&maxx<right))
	{
		if((miny>bottom&&miny<top) && (maxy>bottom&&maxy<top))
			return	TRUE;
	}

	return	FALSE;
}

//比较时间
//返回值：	 0 时间相等
//			 1 时间1早于时间2
//			-1 时间1晚于时间2

int compareTime(int year1, int month1, int day1, int hour1, int minute1, int second1, int year2, int month2, int day2, int hour2, int minute2, int second2)
{
	int	sign;

	sign	= 0;
	
	if(year1 < year2)		//
	{
		sign	= 1;
	}
	else	if(year1 > year2)
	{
		sign	= -1;
	}
	else	//年份相同
	{
		if(month1 < month2)
		{
			sign	= 1;
		}
		else	if(month1 > month2)
		{
			sign	= -1;
		}
		else	//月份相同
		{
			if(day1 < day2)
			{
				sign	= 1;
			}
			else	if(day1 > day2)
			{
				sign	= -1;
			}
			else	//日分相同
			{
				if(hour1 < hour2)
				{
					sign	= 1;
				}
				else	if(hour1 > hour2)
				{
					sign	= -1;
				}
				else	//时相同
				{
					if(minute1 < minute2)
					{
						sign	= 1;
					}
					else	if(minute1 > minute2)
					{
						sign	= -1;
					}
					else	//分相同
					{
						if(second1 < second2)
						{
							sign	= 1;
						}
						else	if(second1 > second2)
						{
							sign	= -1;
						}
						else	//秒相同
						{
							sign	= 0;
						}
					}
				}
			}
		}
	}

	return	sign;
}

void GetPointCrossPolygon(int mode, int num, FP64 *xx, FP64 *yy, FP64 *centerx, FP64 *centery)
{
	FP64	cx[100];
	int		i,j,k;
	FP64	width;
	int		I;
	FP64	cy[100];
	FP64	dx,dy;
	FP64	t;

	if(mode == 0)	//水平切
	{
		//centery有意义

		k	=	0;
		for(i=0;i<num-1;i++)
		{
			if((*centery>=yy[i]&&*centery<=yy[i+1]) || (*centery>=yy[i+1]&&*centery<=yy[i]))
			{

				dy	= yy[i+1]-yy[i]; 
				dx	= xx[i+1]-xx[i];
				
				if(fabs(dy)<=1e-5||fabs(dx)<=1e-5)
				{ 
					if(fabs(dy)<=1e-5 && (*centery>=yy[i]&&*centery<=yy[i+1]))
						cx[k]	= xx[i];
					if(fabs(dy)<=1e-5 && (*centery>=yy[i+1]&&*centery<=yy[i]))
						cx[k]	= xx[i+1];
					if(fabs(dx)<=1e-5)
						cx[k]	= (xx[i]+xx[i+1])/2.0;
				}
				else 
					cx[k]	= xx[i]+dx*(*centery-yy[i])/dy;

				k++;
			}
		}
		
		for(i=0;i<k;i++)
		{
			for(j=i+1;j<k;j++)
			{
				if(cx[i]>=cx[j])
				{ 
					t		= cx[i]; 
					cx[i]	= cx[j]; 
					cx[j]	= t; 
				}
			}
		}
		
		if(k > 2)
		{

			width	= 0.0;
			for(i=0;i<k/2;i++)
			{
				if(width == 0.0)
				{
					width	= fabs(cx[2*i+1]-cx[2*i+0]);
					I		= i;
				}
				else
				{
					if(width < fabs(cx[2*i+1]-cx[2*i+0]))
					{
						width	= fabs(cx[2*i+1]-cx[2*i+0]);
						I	= i;
					}
				}
			}

			*centerx	= (cx[2*I+0]+cx[2*I+1])/2.0;

		}
		else
			*centerx	= (cx[0]+cx[1])/2.0;
	}
	else			//垂直切
	{
		//centerx有意义

		k	=	0;
		for(i=0;i<num-1;i++)
		{
			if((*centerx>=xx[i]&&*centerx<=xx[i+1]) || (*centerx>=xx[i+1]&&*centerx<=xx[i]))
			{

				dy	= yy[i+1]-yy[i]; 
				dx	= xx[i+1]-xx[i];
				
				if(fabs(dy)<=1e-5||fabs(dx)<=1e-5)
				{ 
					if(fabs(dx)<=1e-5 && (*centerx>=xx[i]&&*centerx<=xx[i+1]))
						cy[k]	= yy[i];
					if(fabs(dx)<=1e-5 && (*centerx>=xx[i+1]&&*centerx<=xx[i]))
						cy[k]	= yy[i+1];
					if(fabs(dy)<=1e-5)
						cy[k]	= (yy[i]+yy[i+1])/2.0;
				}
				else 
					cy[k]	= yy[i]+dy*(*centerx-xx[i])/dx;

				k++;
			}
		}
		
		for(i=0;i<k;i++)
		{
			for(j=i+1;j<k;j++)
			{
				if(cy[i]>=cy[j])
				{ 
					FP64	t;
					t		= cy[i]; 
					cy[i]	= cy[j]; 
					cy[j]	= t; 
				}
			}
		}

		if(k > 2)
		{

			width	= 0.0;
			for(i=0;i<k/2;i++)
			{
				if(width == 0.0)
				{
					width	= fabs(cy[2*i+1]-cy[2*i+0]);
					I		= i;
				}
				else
				{
					if(width < fabs(cy[2*i+1]-cy[2*i+0]))
					{
						width	= fabs(cy[2*i+1]-cy[2*i+0]);
						I	= i;
					}
				}
			}

			*centery	= (cy[2*I+0]+cy[2*I+1])/2.0;

		}
		else
			*centery	= (cy[0]+cy[1])/2.0;
	}
}

FP64 GetDistance(FP64 x1,FP64 y1,FP64 x2,FP64 y2)
{
	FP64	D;
	FP64	tmpVal;
	FP64	fi1,fi2;
	FP64	tmp1,tmp2;
	FP64	dis;

	FP64	drda	= x1 - x2;
	drda	= drda * GEO_PI/180.0;
	fi1		= y1;
	fi2		= y2;
	fi1		= fi1 * GEO_PI / 180.0;
	fi2		= fi2 * GEO_PI / 180.0;
	
	tmpVal	= sin(fi1)*sin(fi2) + cos(fi1)*cos(fi2)*cos(drda);
	if(fabs(tmpVal) > 1.0)
	{
	//	AfxMessageBox(_T("参数有误!"));
		return -1.0;
	}
	D		= acos(tmpVal);
	
	tmp1	= sin(fi1) + sin(fi2);
	tmp2	= sin(fi1) - sin(fi2);
	tmpVal	= ((3*sin(D) - D)*tmp1*tmp1)/(1+cos(D));
	tmpVal	= tmpVal - ((3*sin(D) + D)*tmp2*tmp2)/(1-cos(D));
	
	dis	= EARTH_WGS84_A * D + (EARTH_WGS84_A/(4*EARTH_WGS84_FLATTENING))*tmpVal; //in meter
	return dis;
}
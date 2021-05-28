// GpsDataInfo.h: interface for the CGpsDataInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPSDATAINFO_H__06B1D0EC_DBB0_46E2_B456_72CBD9904616__INCLUDED_)
#define AFX_GPSDATAINFO_H__06B1D0EC_DBB0_46E2_B456_72CBD9904616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 记录一个GPS数据状态
// 解析字符GPRMC格式的GPS数据为数字格式
// 把数字格式的GPS数据合成为GPRMC格式的GPS字符串
class AFX_EXT_CLASS CGpsDataInfo  
{
public:
	CGpsDataInfo();
	 ~CGpsDataInfo();
public:
	void Initialize();

	int     m_nCarID;
	float	m_xx;
	float	m_yy;
	float	m_nSpeed;
	float	m_fAngle;
	int     m_tGpsTime;
	int     m_nStCount; // 卫星数量
	float   m_dbHeigth; // 海拔高度
	int     m_nState;  // 当前状态 1—无定位信息，2—2D  3—3D
	SYSTEMTIME m_stTime;
	SYSTEMTIME m_stUTCTime;

	TCHAR	m_szXX[200];
	TCHAR	m_szYY[200];
	TCHAR	m_szSpeed[200];
	TCHAR	m_szAngle[200];

	int nDataType ;   // 数据类型； 1 GPS数据，2 海拔卫星情况， 3 定位状态情况
	int bNetData;     // 是否来自网络的数据
	
	CGpsDataInfo& operator =(const CGpsDataInfo& gpsinfo);
	CGpsDataInfo& operator =(const CGpsDataInfo* pGpsinfo);

	bool	ParseGPS(const char* lpszGPS);
	bool	ParseGGA(const char* lpszGPS);
	bool	ParseGSA(const char* lpszGPS);
	//	Get subsring from a string, and the base index is 0
	int	FieldVar(const char *buf, int fldnum, int sep, char *dest);

};

#endif // !defined(AFX_GPSDATAINFO_H__06B1D0EC_DBB0_46E2_B456_72CBD9904616__INCLUDED_)

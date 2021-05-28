// GpsDataInfo.h: interface for the CGpsDataInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPSDATAINFO_H__06B1D0EC_DBB0_46E2_B456_72CBD9904616__INCLUDED_)
#define AFX_GPSDATAINFO_H__06B1D0EC_DBB0_46E2_B456_72CBD9904616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ��¼һ��GPS����״̬
// �����ַ�GPRMC��ʽ��GPS����Ϊ���ָ�ʽ
// �����ָ�ʽ��GPS���ݺϳ�ΪGPRMC��ʽ��GPS�ַ���
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
	int     m_nStCount; // ��������
	float   m_dbHeigth; // ���θ߶�
	int     m_nState;  // ��ǰ״̬ 1���޶�λ��Ϣ��2��2D  3��3D
	SYSTEMTIME m_stTime;
	SYSTEMTIME m_stUTCTime;

	TCHAR	m_szXX[200];
	TCHAR	m_szYY[200];
	TCHAR	m_szSpeed[200];
	TCHAR	m_szAngle[200];

	int nDataType ;   // �������ͣ� 1 GPS���ݣ�2 ������������� 3 ��λ״̬���
	int bNetData;     // �Ƿ��������������
	
	CGpsDataInfo& operator =(const CGpsDataInfo& gpsinfo);
	CGpsDataInfo& operator =(const CGpsDataInfo* pGpsinfo);

	bool	ParseGPS(const char* lpszGPS);
	bool	ParseGGA(const char* lpszGPS);
	bool	ParseGSA(const char* lpszGPS);
	//	Get subsring from a string, and the base index is 0
	int	FieldVar(const char *buf, int fldnum, int sep, char *dest);

};

#endif // !defined(AFX_GPSDATAINFO_H__06B1D0EC_DBB0_46E2_B456_72CBD9904616__INCLUDED_)

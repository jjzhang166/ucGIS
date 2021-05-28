
#ifndef _USERLAYER_H_INCLUDED
#define _USERLAYER_H_INCLUDED
#ifdef USERLAYER_GLOBALES
#define USERLAYER_EXT 

#else 
#define USERLAYER_EXT extern
#endif
#include "gisbase.h"
#include "gpsdatainfo.h"
#include "car.h"
#define STRING_MAX_LEN 200
typedef struct USER
{
	int  userid;
	char nicename[30];
	char email[30];
}USER;
typedef struct FRIEND
{
	int  userid;
	BOOLEAN watch;
}FRIEND;


typedef struct STATE
{
	unsigned char llight:1;         
	unsigned char rlight:1;
	unsigned char slight:1;
	unsigned char speedAlter:1;
	unsigned char emergentAlter:1;
	unsigned char wlAlter:1;
	unsigned char tireAlter:1;
	unsigned char b7:1;     /*a0~a7ȡֵֻ����0��1*/
}STATE;
typedef struct GPSDATA
{
	int carID;
	float xx;
	float yy;
	unsigned short speed;
	unsigned short angle;
	unsigned short heigth;
	INT8S stcount;
	INT8S ststate;
	STATE  state;
	INT8S  other;
	TIME_STRUC gpstime;
}GPSDATA;

typedef enum MESSAGE_TYPE {
	GPS_MESSAGE_TYPE     = (short)0x00101,
	CONNECT_MESSAGE_TYPE = (short)0x00102,
	CONNECT_MESSAGE_RESP = (short)0x10102,
	TEST_MESSAGE_TYPE    = (short)0x00103,
	TEST_MESSAGE_RESP    = (short)0x10103,
	SHORT_MESSAGE_TYPE   = (short)0x00104,
	SHORT_MESSAGE_RESP   = (short)0x10104,
	QQ_SHORT_MESSAGE_TYPE   = (short)0x00105,
	QQ_SHORT_MESSAGE_RESP   = (short)0x10105,
	IMAGE_MESSAGE_TYPE   = (short)0x00106,
	IMAGE_MESSAGE_RESP   = (short)0x10106,
	COMAND_CONFIRM       = (short)0x10107, // 
	REG_NEW_USER_TYPE    = (short)0x00108, // �ͻ���ע��һ���û�
	REG_NEW_USER_RESP    = (short)0x10108, // ע���û��ظ�
	GET_MY_FRIENDS_LIST  = (short)0x00109, // �����ȡ������Ϣ
	GET_MY_FRIENDS_LIST_RESP  = (short)0x10109, // һ��������Ϣ
	DISCONNECT_NET  = (short)0x00110,// ֪ͨ�ͻ����㱻������
	ONLINE_TYPE  = (short)0x00111,  // ֪ͨ��ض�ĳ�û�����
	SERACHUSER_TYPE = (short)0x00112,               // �ͻ�������������û�������Ϣ
	SERACHUSER_RESP = (short)0x10112,               // ���������û����Ͳ��ҽ����Ϣ
	ADD_FRIEND_TYPE = (short)0x00113,              // �ͻ����������һ������
	ADD_FRIEND_RESP = (short)0x10113,              // �������ظ��ͻ����������һ������
}MESSAGE_TYPE;
// ����״̬
typedef enum NET_STATE_TYPE {
	NET_BREAK    = 0,  // ����Ͽ�
	NET_CONNECTING,    // ��������
	NET_CONNECTED,     // �����Ѿ�����
	NET_LOGINED,       // �����Ѿ���¼
	NET_LOGINFEILD,    // �����¼ʧ��
}NET_STATE_TYPE;
// ��Ϣͷ�ṹ
typedef struct MESSAGEHEADER
{
	short nMsgID;
	unsigned short len;
	unsigned short type;
	unsigned short id;
	INT8S  ver;
}MESSAGEHEADER;


// GPS���ݰ�  ������->�ͻ�
// ����Ҫ�ظ�
typedef struct GPSMESSAGE
{
	MESSAGEHEADER header;
	GPSDATA       gps;
}GPSMESSAGE;

// ��������Ϣ
// 
// ��Ҫ�ظ�
typedef enum EM_MEGTYPE
{
	EM_MSG = 0,    // һ����Ϣ
	EM_ALERT,      // ������Ϣ
	EM_WARM,       // ������Ϣ
	EM_BROADCAST,  // �㲥��Ϣ
}EM_MEGTYPE;
typedef struct QQSHORTMESSAGE
{
	MESSAGEHEADER header;
	EM_MEGTYPE messagetype;
	char ch[100];     
}QQSHORTMESSAGE;
typedef struct QQSHORTMESSAGERESP
{
	MESSAGEHEADER header;
	INT8S result;  // 0:��ȷ 1:�д���
}QQSHORTMESSAGERESP;

// �������
typedef struct TEST
{
	MESSAGEHEADER header;
}TEST;
typedef struct TESTRESP
{
	MESSAGEHEADER header;
}TESTRESP;
/*
 �û����
*/
// ��ȡ�����б�
typedef struct GETFRIENDS
{
	MESSAGEHEADER header;
}GETFRIENDS;
// ��ȡ�����б�
typedef struct GETFRIENDSRESP
{
	MESSAGEHEADER header;
	USER user;
}GETFRIENDSRESP;
// ���û���Ų����û�
// �ͻ���->������
typedef struct SERACHUSER
{
	MESSAGEHEADER header;
	int userid;   // �����ҵ��û����
}SERACHUSER;
// �û���ѯ���
// ������->�ͻ���
typedef struct SERACHUSERRESP
{
	MESSAGEHEADER header;
	int userid;   // �����ҵ��û���ţ� ����û����Ϊ0��˵��û���ҵ�
	char  nicename[30]; //�û����ǳ�
}SERACHUSERRESP;
// ���һ������
// �ͻ��ˣ���������
typedef struct ADDFRIEND
{
	MESSAGEHEADER header;
	int userid;   // �����ҵ��û����
}ADDFRIEND;
typedef enum EM_ADD_FRIEND
{
	ADD_FRIEND_OK = 0,
	ADD_FRIEND_ERR,
	ADD_FRIEND_ALREADY,    // �Ѿ���ӣ�����Ҫ�����
	ADD_FRIEND_SELF,       // ��������Լ�Ϊ����
	ADD_FRIEND_NEED_VOL , // ��Ҫ�Է�ͬ��
	ADD_FRIEND_NO_QUXIAN, // û��Ȩ��
	ADD_FRIEND_FIREN_COUT_OVER,  // �û������ﵽ����
}EM_ADD_FRIEND;
// ��Ӻ��ѵĻظ�
// ������->�ͻ���
typedef struct ADDFRIENDRESP
{
	MESSAGEHEADER header;
	EM_ADD_FRIEND result;   // ���
}ADDFRIENDRESP;


// ע�����û���Ϣ�Ļظ�
// �����������Ϳͻ���
typedef struct REGUSERMESSAGE
{
	MESSAGEHEADER header;
	char  nicename[30];
	char  pass[20];
}REGUSERMESSAGE;

// ע�����û���Ϣ
// �ͻ��˷��͸�������
typedef struct REGUSERMESSAGERESP
{
	MESSAGEHEADER header;
	int userID;
}REGUSERMESSAGERESP;
/*
 ͼ�����
*/
typedef struct IMAGE
{
	MESSAGEHEADER header;
	char url[200];
}IMAGE;
typedef struct IMAGERESP
{
	MESSAGEHEADER header;
	INT8S nResult;
}IMAGERESP;
/*
 ��¼���
*/
// �ͻ�����������
typedef enum EM_OS
{
	WIN98 = 0,  
	WIN2000,
	WINXP,
	WINXPEM,
	WINCEPPC2003,
	WINCESP2003,
	MWSP2005,
	MWPPC2005,
	UCOSII
}EM_OS;
// �ͻ���¼
typedef struct CONNECTMESSAGE
{
	MESSAGEHEADER header;
	char user[STRING_MAX_LEN];
	char pass[STRING_MAX_LEN];
	EM_OS os;
	char szVer[20];
}CONNECTMESSAGE;
// �������ظ��ͻ��˵�����
typedef struct CONNECTMESSAGERESP
{
	MESSAGEHEADER header;
	INT8S nResult;  // ���Ϊ0�ǵ�¼�ɹ�
}CONNECTMESSAGERESP;

// �����������Ͽ����ӵ���Ϣ
typedef enum EM_DISCONN
{
	EM_SERVER = 0,  // �����������Ͽ�
	EM_REF,         // �������ܾ�����
	EM_EDGEOUT,     // �û����������ط���¼
	EM_MONEY,       // �û�Ƿ��
}EM_DISCONN;
// �����������Ͽ��ͻ�
typedef struct DISCONN
{
	MESSAGEHEADER header;
	EM_DISCONN nResult;
	char  szMsg[200];
}DISCONN;

// ������Ϣ
typedef struct ONLINE
{
	MESSAGEHEADER header;
	int carID;
	BOOLEAN online;  // true����,false ����
}ONLINE;

typedef struct
{
	INT32U index;					//
	GUI_BITMAP * m_pBmpCenter;              // ����ͼ���ͼ��
	MapCoorTrans* m_mapCoordMap; 
	lpCar m_pNavCar;                // ���ص�����Ŀ��
	GUI_ARRAY   carArray;           // ���泵����map
	int   m_nNetState;
	BOOLEAN m_bShowState;        // ��ʾ״̬��
	BOOLEAN m_bShowMeasureLen;   // ��ʾ���Ȳ�����
	BOOLEAN m_bShowMeasureArea;   // ��ʾ���������
	GISCHAR m_szTraceFilePath[260];
	PFV     m_pSoundFunc;

}UserLayer,*lpUserLayer;

USERLAYER_EXT void UserLayerFree(lpUserLayer puserlayer);
USERLAYER_EXT void UserLayerInitial(lpUserLayer puserlayer);
USERLAYER_EXT void UserLayer_Draw(lpUserLayer puserlayer);
USERLAYER_EXT void GisDrawState(lpUserLayer puserlayer);
USERLAYER_EXT lpCar UserFindCar(GUI_ARRAY *pThis,int ncarid);
USERLAYER_EXT void AddGPSPoint(lpUserLayer puserlayer,GpsDataInfo * pPosition);
USERLAYER_EXT void SetNetState(lpUserLayer puserlayer,int nState);
USERLAYER_EXT void DrawXX(lpUserLayer puserlayer);
USERLAYER_EXT void DrawUserLayerPoint(GpsDataInfo* pPosition,lpUserLayer puserlayer);
USERLAYER_EXT void DrawUserLayerNetState(lpUserLayer puserlayer,int nState);
USERLAYER_EXT BOOLEAN DeleteCar(lpUserLayer puserlayer,INT16U ncarid);


#endif
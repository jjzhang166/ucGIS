
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
	unsigned char b7:1;     /*a0~a7取值只能是0或1*/
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
	REG_NEW_USER_TYPE    = (short)0x00108, // 客户端注册一个用户
	REG_NEW_USER_RESP    = (short)0x10108, // 注册用户回复
	GET_MY_FRIENDS_LIST  = (short)0x00109, // 申请获取好友信息
	GET_MY_FRIENDS_LIST_RESP  = (short)0x10109, // 一个好友信息
	DISCONNECT_NET  = (short)0x00110,// 通知客户：你被迫下线
	ONLINE_TYPE  = (short)0x00111,  // 通知监控端某用户上线
	SERACHUSER_TYPE = (short)0x00112,               // 客户向服务器发送用户查找信息
	SERACHUSER_RESP = (short)0x10112,               // 服务器向用户发送查找结果信息
	ADD_FRIEND_TYPE = (short)0x00113,              // 客户端请求添加一个好友
	ADD_FRIEND_RESP = (short)0x10113,              // 服务器回复客户端请求添加一个好友
}MESSAGE_TYPE;
// 网络状态
typedef enum NET_STATE_TYPE {
	NET_BREAK    = 0,  // 网络断开
	NET_CONNECTING,    // 正在连接
	NET_CONNECTED,     // 网络已经连接
	NET_LOGINED,       // 网络已经登录
	NET_LOGINFEILD,    // 网络登录失败
}NET_STATE_TYPE;
// 信息头结构
typedef struct MESSAGEHEADER
{
	short nMsgID;
	unsigned short len;
	unsigned short type;
	unsigned short id;
	INT8S  ver;
}MESSAGEHEADER;


// GPS数据包  服务器->客户
// 不需要回复
typedef struct GPSMESSAGE
{
	MESSAGEHEADER header;
	GPSDATA       gps;
}GPSMESSAGE;

// 车辆短信息
// 
// 需要回复
typedef enum EM_MEGTYPE
{
	EM_MSG = 0,    // 一般消息
	EM_ALERT,      // 报警消息
	EM_WARM,       // 警告消息
	EM_BROADCAST,  // 广播消息
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
	INT8S result;  // 0:正确 1:有错误
}QQSHORTMESSAGERESP;

// 保活测试
typedef struct TEST
{
	MESSAGEHEADER header;
}TEST;
typedef struct TESTRESP
{
	MESSAGEHEADER header;
}TESTRESP;
/*
 用户相关
*/
// 获取好友列表
typedef struct GETFRIENDS
{
	MESSAGEHEADER header;
}GETFRIENDS;
// 获取好友列表
typedef struct GETFRIENDSRESP
{
	MESSAGEHEADER header;
	USER user;
}GETFRIENDSRESP;
// 按用户编号查找用户
// 客户端->服务器
typedef struct SERACHUSER
{
	MESSAGEHEADER header;
	int userid;   // 待查找的用户标号
}SERACHUSER;
// 用户查询结果
// 服务器->客户端
typedef struct SERACHUSERRESP
{
	MESSAGEHEADER header;
	int userid;   // 待查找的用户标号， 如果用户编号为0，说明没有找到
	char  nicename[30]; //用户的昵称
}SERACHUSERRESP;
// 添加一个好友
// 客户端－》服务器
typedef struct ADDFRIEND
{
	MESSAGEHEADER header;
	int userid;   // 待查找的用户标号
}ADDFRIEND;
typedef enum EM_ADD_FRIEND
{
	ADD_FRIEND_OK = 0,
	ADD_FRIEND_ERR,
	ADD_FRIEND_ALREADY,    // 已经添加，不需要再添加
	ADD_FRIEND_SELF,       // 不能添加自己为好友
	ADD_FRIEND_NEED_VOL , // 需要对方同意
	ADD_FRIEND_NO_QUXIAN, // 没有权限
	ADD_FRIEND_FIREN_COUT_OVER,  // 用户数量达到上线
}EM_ADD_FRIEND;
// 添加好友的回复
// 服务器->客户端
typedef struct ADDFRIENDRESP
{
	MESSAGEHEADER header;
	EM_ADD_FRIEND result;   // 结果
}ADDFRIENDRESP;


// 注册新用户信息的回复
// 给服务器发送客户端
typedef struct REGUSERMESSAGE
{
	MESSAGEHEADER header;
	char  nicename[30];
	char  pass[20];
}REGUSERMESSAGE;

// 注册新用户信息
// 客户端发送给服务器
typedef struct REGUSERMESSAGERESP
{
	MESSAGEHEADER header;
	int userID;
}REGUSERMESSAGERESP;
/*
 图像相关
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
 登录相关
*/
// 客户端请求连接
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
// 客户登录
typedef struct CONNECTMESSAGE
{
	MESSAGEHEADER header;
	char user[STRING_MAX_LEN];
	char pass[STRING_MAX_LEN];
	EM_OS os;
	char szVer[20];
}CONNECTMESSAGE;
// 服务器回复客户端的请求
typedef struct CONNECTMESSAGERESP
{
	MESSAGEHEADER header;
	INT8S nResult;  // 结果为0是登录成功
}CONNECTMESSAGERESP;

// 服务器主动断开连接的信息
typedef enum EM_DISCONN
{
	EM_SERVER = 0,  // 服务器主动断开
	EM_REF,         // 服务器拒绝服务
	EM_EDGEOUT,     // 用户名在其他地方登录
	EM_MONEY,       // 用户欠费
}EM_DISCONN;
// 服务器主动断开客户
typedef struct DISCONN
{
	MESSAGEHEADER header;
	EM_DISCONN nResult;
	char  szMsg[200];
}DISCONN;

// 上线信息
typedef struct ONLINE
{
	MESSAGEHEADER header;
	int carID;
	BOOLEAN online;  // true上线,false 下线
}ONLINE;

typedef struct
{
	INT32U index;					//
	GUI_BITMAP * m_pBmpCenter;              // 中心图标的图像
	MapCoorTrans* m_mapCoordMap; 
	lpCar m_pNavCar;                // 本地当航车目标
	GUI_ARRAY   carArray;           // 保存车辆的map
	int   m_nNetState;
	BOOLEAN m_bShowState;        // 显示状态吗？
	BOOLEAN m_bShowMeasureLen;   // 显示长度测量吗？
	BOOLEAN m_bShowMeasureArea;   // 显示面积测量吗？
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
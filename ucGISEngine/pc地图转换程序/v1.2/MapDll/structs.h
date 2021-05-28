#pragma once
#pragma pack(4)
#define STRING_MAX_LEN 200
#define	UDP_BLOCK_MAX	5000
typedef struct USER
{
	USER()
	{
		memset(this,0,sizeof(*this));
	}
	int  userid;
	char nicename[30];
	char email[30];
}USER;
typedef struct FRIEND
{
	int  userid;
	bool watch;
}FRIEND;


struct STATE
{
	STATE()
	{
		memset(this,0,sizeof(*this));
	}
	unsigned char llight:1;         
	unsigned char rlight:1;
	unsigned char slight:1;
	unsigned char speedAlter:1;
	unsigned char emergentAlter:1;
	unsigned char wlAlter:1;
	unsigned char tireAlter:1;
	unsigned char b7:1;     /*a0~a7取值只能是0或1*/
};
struct GPSDATA
{
	GPSDATA()
	{
		memset(this,0,sizeof(*this));
	}
	int carID;
	float xx;
	float yy;
	unsigned short speed;
	unsigned short angle;
	unsigned short heigth;
	BYTE stcount;
	BYTE ststate;
	STATE  state;
	BYTE  other;
	SYSTEMTIME gpstime;
};

enum MESSAGE_TYPE {
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
};
// 网络状态
enum NET_STATE_TYPE {
	NET_BREAK    = 0,  // 网络断开
	NET_CONNECTING,    // 正在连接
	NET_CONNECTED,     // 网络已经连接
	NET_LOGINED,       // 网络已经登录
	NET_LOGINFEILD,    // 网络登录失败
};
// 信息头结构
struct MESSAGEHEADER
{
	MESSAGEHEADER()
	{
		id = MESSAGEHEADER::nMsgID++;
		memset(this,0,sizeof(*this));
		ver = 1;  // 版本1
	}
	static short nMsgID;
	unsigned short len;
	unsigned short type;
	unsigned short id;
	BYTE  ver;
};


// GPS数据包  服务器->客户
// 不需要回复
struct GPSMESSAGE
{
	GPSMESSAGE()
	{
		header.type = GPS_MESSAGE_TYPE;
		header.len = sizeof(*this);
	}
	MESSAGEHEADER header;
	GPSDATA       gps;
};

// 车辆短信息
// 
// 需要回复
enum EM_MEGTYPE
{
	EM_MSG = 0,    // 一般消息
	EM_ALERT,      // 报警消息
	EM_WARM,       // 警告消息
	EM_BROADCAST,  // 广播消息
};
struct QQSHORTMESSAGE
{
	QQSHORTMESSAGE()
	{
		header.type = QQ_SHORT_MESSAGE_TYPE;
		header.len = sizeof(*this);
		messagetype = EM_MSG;
	}
	MESSAGEHEADER header;
	EM_MEGTYPE messagetype;
	char ch[100];     
};
struct QQSHORTMESSAGERESP
{
	QQSHORTMESSAGERESP()
	{
		header.type = QQ_SHORT_MESSAGE_RESP;
		header.len = sizeof(*this);
	}
	MESSAGEHEADER header;
	BYTE result;  // 0:正确 1:有错误
};

// 保活测试
struct TEST
{
	TEST()
	{
		header.type = TEST_MESSAGE_TYPE;
		header.len = sizeof(*this);
	}
	MESSAGEHEADER header;
};
struct TESTRESP
{
	TESTRESP()
	{
		header.type = TEST_MESSAGE_RESP;
		header.len = sizeof(*this);
	}
	MESSAGEHEADER header;
};
/*
 用户相关
*/
// 获取好友列表
struct GETFRIENDS
{
	GETFRIENDS()
	{
		header.type = GET_MY_FRIENDS_LIST;
		header.len = sizeof(*this);
	}
	MESSAGEHEADER header;
};
// 获取好友列表
struct GETFRIENDSRESP
{
	GETFRIENDSRESP()
	{
		header.type = GET_MY_FRIENDS_LIST_RESP;
		header.len = sizeof(*this);
		memset(&user,0,sizeof(USER));
	}
	MESSAGEHEADER header;
	USER user;
};
// 按用户编号查找用户
// 客户端->服务器
struct SERACHUSER
{
	SERACHUSER()
	{
		header.type = SERACHUSER_TYPE;
		header.len = sizeof(*this);
		userid = 0;
	}
	MESSAGEHEADER header;
	int userid;   // 待查找的用户标号
};
// 用户查询结果
// 服务器->客户端
struct SERACHUSERRESP
{
	SERACHUSERRESP()
	{
		header.type = SERACHUSER_RESP;
		header.len = sizeof(*this);
		userid = 0;
		memset(nicename,0,30);
	}
	MESSAGEHEADER header;
	int userid;   // 待查找的用户标号， 如果用户编号为0，说明没有找到
	char  nicename[30]; //用户的昵称
};
// 添加一个好友
// 客户端－》服务器
struct ADDFRIEND
{
	ADDFRIEND()
	{
		header.type = ADD_FRIEND_TYPE;
		header.len = sizeof(*this);
		userid = 0;
	}
	MESSAGEHEADER header;
	int userid;   // 待查找的用户标号
};
enum EM_ADD_FRIEND
{
	ADD_FRIEND_OK = 0,
	ADD_FRIEND_ERR,
	ADD_FRIEND_ALREADY,    // 已经添加，不需要再添加
	ADD_FRIEND_SELF,       // 不能添加自己为好友
	ADD_FRIEND_NEED_VOL , // 需要对方同意
	ADD_FRIEND_NO_QUXIAN, // 没有权限
	ADD_FRIEND_FIREN_COUT_OVER,  // 用户数量达到上线
};
// 添加好友的回复
// 服务器->客户端
struct ADDFRIENDRESP
{
	ADDFRIENDRESP()
	{
		header.type = ADD_FRIEND_RESP;
		header.len = sizeof(*this);
		result = ADD_FRIEND_OK;
	}
	MESSAGEHEADER header;
	EM_ADD_FRIEND result;   // 结果
};


// 注册新用户信息的回复
// 给服务器发送客户端
struct REGUSERMESSAGE
{
	REGUSERMESSAGE()
	{
		header.type = REG_NEW_USER_TYPE;
		header.len = sizeof(*this);
		memset(nicename,0,30);
		memset(pass,0,20);
	}
	MESSAGEHEADER header;
	char  nicename[30];
	char  pass[20];
};

// 注册新用户信息
// 客户端发送给服务器
struct REGUSERMESSAGERESP
{
	REGUSERMESSAGERESP()
	{
		header.type = REG_NEW_USER_RESP;
		header.len = sizeof(*this);
		userID = 0;
	}
	MESSAGEHEADER header;
	int userID;
};
/*
 图像相关
*/
struct IMAGE
{
	IMAGE()
	{
		header.type = IMAGE_MESSAGE_TYPE;
		header.len = sizeof(*this);
	}
	MESSAGEHEADER header;
	char url[200];
};
struct IMAGERESP
{
	IMAGERESP()
	{
		header.type = IMAGE_MESSAGE_RESP;
		header.len = sizeof(*this);
	}
	MESSAGEHEADER header;
	BYTE nResult;
};
/*
 登录相关
*/
// 客户端请求连接
enum EM_OS
{
	WIN98 = 0,  
	WIN2000,
	WINXP,
	WINXPEM,
	WINCEPPC2003,
	WINCESP2003,
	MWSP2005,
	MWPPC2005,
};
// 客户登录
struct CONNECTMESSAGE
{
	CONNECTMESSAGE()
	{
		header.type = CONNECT_MESSAGE_TYPE;
		header.len = sizeof(*this);
		os = WIN98;
		memset(szVer,0,sizeof(szVer));
	}
	MESSAGEHEADER header;
	char user[STRING_MAX_LEN];
	char pass[STRING_MAX_LEN];
	EM_OS os;
	char szVer[20];
};
// 服务器回复客户端的请求
struct CONNECTMESSAGERESP
{
	CONNECTMESSAGERESP()
	{
		header.type = CONNECT_MESSAGE_RESP;
		header.len = sizeof(*this);
	}
	MESSAGEHEADER header;
	BYTE nResult;  // 结果为0是登录成功
};

// 服务器主动断开连接的信息
enum EM_DISCONN
{
	EM_SERVER = 0,  // 服务器主动断开
	EM_REF,         // 服务器拒绝服务
	EM_EDGEOUT,     // 用户名在其他地方登录
	EM_MONEY,       // 用户欠费
};
// 服务器主动断开客户
struct DISCONN
{
	DISCONN()
	{
		header.type = DISCONNECT_NET;
		header.len = sizeof(*this);
		nResult = EM_SERVER;
		memset(szMsg,0,sizeof(szMsg));
	}
	MESSAGEHEADER header;
	EM_DISCONN nResult;
	char  szMsg[200];
};

// 上线信息
struct ONLINE
{
	ONLINE()
	{
		header.type = ONLINE_TYPE;
		header.len = sizeof(*this);
		online = false;
		carID = 0;
	}
	MESSAGEHEADER header;
	int carID;
	bool online;  // true上线,false 下线
};

#pragma pack()
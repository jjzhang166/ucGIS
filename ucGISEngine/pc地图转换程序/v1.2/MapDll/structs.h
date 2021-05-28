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
	unsigned char b7:1;     /*a0~a7ȡֵֻ����0��1*/
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
};
// ����״̬
enum NET_STATE_TYPE {
	NET_BREAK    = 0,  // ����Ͽ�
	NET_CONNECTING,    // ��������
	NET_CONNECTED,     // �����Ѿ�����
	NET_LOGINED,       // �����Ѿ���¼
	NET_LOGINFEILD,    // �����¼ʧ��
};
// ��Ϣͷ�ṹ
struct MESSAGEHEADER
{
	MESSAGEHEADER()
	{
		id = MESSAGEHEADER::nMsgID++;
		memset(this,0,sizeof(*this));
		ver = 1;  // �汾1
	}
	static short nMsgID;
	unsigned short len;
	unsigned short type;
	unsigned short id;
	BYTE  ver;
};


// GPS���ݰ�  ������->�ͻ�
// ����Ҫ�ظ�
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

// ��������Ϣ
// 
// ��Ҫ�ظ�
enum EM_MEGTYPE
{
	EM_MSG = 0,    // һ����Ϣ
	EM_ALERT,      // ������Ϣ
	EM_WARM,       // ������Ϣ
	EM_BROADCAST,  // �㲥��Ϣ
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
	BYTE result;  // 0:��ȷ 1:�д���
};

// �������
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
 �û����
*/
// ��ȡ�����б�
struct GETFRIENDS
{
	GETFRIENDS()
	{
		header.type = GET_MY_FRIENDS_LIST;
		header.len = sizeof(*this);
	}
	MESSAGEHEADER header;
};
// ��ȡ�����б�
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
// ���û���Ų����û�
// �ͻ���->������
struct SERACHUSER
{
	SERACHUSER()
	{
		header.type = SERACHUSER_TYPE;
		header.len = sizeof(*this);
		userid = 0;
	}
	MESSAGEHEADER header;
	int userid;   // �����ҵ��û����
};
// �û���ѯ���
// ������->�ͻ���
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
	int userid;   // �����ҵ��û���ţ� ����û����Ϊ0��˵��û���ҵ�
	char  nicename[30]; //�û����ǳ�
};
// ���һ������
// �ͻ��ˣ���������
struct ADDFRIEND
{
	ADDFRIEND()
	{
		header.type = ADD_FRIEND_TYPE;
		header.len = sizeof(*this);
		userid = 0;
	}
	MESSAGEHEADER header;
	int userid;   // �����ҵ��û����
};
enum EM_ADD_FRIEND
{
	ADD_FRIEND_OK = 0,
	ADD_FRIEND_ERR,
	ADD_FRIEND_ALREADY,    // �Ѿ���ӣ�����Ҫ�����
	ADD_FRIEND_SELF,       // ��������Լ�Ϊ����
	ADD_FRIEND_NEED_VOL , // ��Ҫ�Է�ͬ��
	ADD_FRIEND_NO_QUXIAN, // û��Ȩ��
	ADD_FRIEND_FIREN_COUT_OVER,  // �û������ﵽ����
};
// ��Ӻ��ѵĻظ�
// ������->�ͻ���
struct ADDFRIENDRESP
{
	ADDFRIENDRESP()
	{
		header.type = ADD_FRIEND_RESP;
		header.len = sizeof(*this);
		result = ADD_FRIEND_OK;
	}
	MESSAGEHEADER header;
	EM_ADD_FRIEND result;   // ���
};


// ע�����û���Ϣ�Ļظ�
// �����������Ϳͻ���
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

// ע�����û���Ϣ
// �ͻ��˷��͸�������
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
 ͼ�����
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
 ��¼���
*/
// �ͻ�����������
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
// �ͻ���¼
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
// �������ظ��ͻ��˵�����
struct CONNECTMESSAGERESP
{
	CONNECTMESSAGERESP()
	{
		header.type = CONNECT_MESSAGE_RESP;
		header.len = sizeof(*this);
	}
	MESSAGEHEADER header;
	BYTE nResult;  // ���Ϊ0�ǵ�¼�ɹ�
};

// �����������Ͽ����ӵ���Ϣ
enum EM_DISCONN
{
	EM_SERVER = 0,  // �����������Ͽ�
	EM_REF,         // �������ܾ�����
	EM_EDGEOUT,     // �û����������ط���¼
	EM_MONEY,       // �û�Ƿ��
};
// �����������Ͽ��ͻ�
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

// ������Ϣ
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
	bool online;  // true����,false ����
};

#pragma pack()
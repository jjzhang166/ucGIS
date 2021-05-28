// PlaySound.cpp: implementation of the CPlaySound class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "resource.h"
#include "PlaySound.h"
#include "Mmsystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define MAKEINTRESOURCE(i)  (LPTSTR) ((DWORD) ((WORD) (i)))
int MunSound[10] = {0};
CPlaySound::CPlaySound()
{
	m_hWnd = NULL;
	m_hKillThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hThreadKilledEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	CreateThread(NULL, 0, PlayThread, this, 0, NULL);
	InitializeCriticalSection(&m_cs);
	//lstrcpy(szSoundFilesPath , TEXT("\\Storage Card\\GPS\\Sound Files"));
	m_bOpenSound = true;

	MunSound[0] = IDR_WAVE_0;
	MunSound[1] = IDR_WAVE_1;
	MunSound[2] = IDR_WAVE_2;
	MunSound[3] = IDR_WAVE_3;
	MunSound[4] = IDR_WAVE_4;
	MunSound[5] = IDR_WAVE_5;
	MunSound[6] = IDR_WAVE_6;
	MunSound[7] = IDR_WAVE_7;
	MunSound[8] = IDR_WAVE_8;
	MunSound[9] = IDR_WAVE_9;
}

CPlaySound::~CPlaySound()
{
	SetEvent(m_hKillThreadEvent);			// 发出关闭子线程的信号
	DeleteCriticalSection(&m_cs);
}
void CPlaySound::Speed(int nSpeed)
{
	Read(nSpeed);
	PushSound(IDR_WAVE_Km);
}
void CPlaySound::Read(int nNum)
{
	int n1000, n100, n10, n;
	n1000 = nNum/1000;
	n100 = (nNum - n1000*1000)/100;
	n10 = (nNum - n100*100)/10;
	n = nNum - n10*10 - n100*100;

	if(n100>0)
	{
		PushSound(MunSound[n100]);
		PushSound(IDR_WAVE_100);
	}

	if(!((n10==0)&&(nNum<100)))  // 只要
	{
		if(!((nNum<100&&n10==1)||(n100>0&&n10==0&&n==0)))
			PushSound(MunSound[n10]);
		if(n10>0)
			PushSound(IDR_WAVE_10);
			
	}

	if(!((n==0)&&(nNum>0)))  // 
	{
		PushSound(MunSound[n]);
	}
}
DWORD WINAPI CPlaySound::PlayThread(LPVOID lParam)
{
	CPlaySound* p =(CPlaySound *)lParam;	// this
	while(true)
	{
		int nCount = (int)p->m_vectorSoundPath.size();

	
		vector<TCHAR*>::iterator pos = p->m_vectorSoundPath.begin();
		for(int i=0;i<nCount;i++)
		{
			pos = p->m_vectorSoundPath.begin();
		//	if(p->m_bOpenSound)

				//PlaySound(p->m_vectorSoundPath[0], NULL, SND_SYNC|SND_NODEFAULT);
				sndPlaySound(p->m_vectorSoundPath[0],SND_SYNC|SND_NODEFAULT);
			delete[] p->m_vectorSoundPath[0];
			EnterCriticalSection(&p->m_cs);
			p->m_vectorSoundPath.erase(pos);
			LeaveCriticalSection(&p->m_cs);
		}
		// 采用资源ID方式的语音
		if(p->m_hWnd!=NULL)
		{
			nCount = (int)p->m_vectorSoundResID.size();
			vector<int>::iterator pos1 = p->m_vectorSoundResID.begin();
			for(int i=0;i<nCount;i++)
			{
				pos1 = p->m_vectorSoundResID.begin();
				if(p->m_bOpenSound)
					PlaySound(MAKEINTRESOURCE(p->m_vectorSoundResID[0]),p->m_hWnd,SND_SYNC|SND_RESOURCE );
				EnterCriticalSection(&p->m_cs);
				p->m_vectorSoundResID.erase(pos1);
				LeaveCriticalSection(&p->m_cs);
			}	
		}
		
		// 检测是否有关闭本线程的信号
		DWORD dwEvent = WaitForSingleObject(p->m_hKillThreadEvent, 500);
		if (dwEvent == WAIT_OBJECT_0) 
			break;
	}
	// 置该线程结束标志
	SetEvent(p->m_hThreadKilledEvent);

	return 9999;
}

void CPlaySound::GPSFixOK(bool ok)
{
	if(ok)
		PushSound(IDR_WAVE_GPS_OK);
	else
		PushSound(IDR_WAVE_GPS_NOOK);
}

void CPlaySound::PushSound(TCHAR *szPath)
{
	int nLen = lstrlen(szPath);
	if(nLen<2) return;

	TCHAR* szTmp = new TCHAR[MAX_PATH];
	_stprintf(szTmp,_T("%s\\%s"),szSoundFilesPath,szPath);
	EnterCriticalSection(&m_cs);
	m_vectorSoundPath.push_back(szTmp);
	LeaveCriticalSection(&m_cs);
}
void CPlaySound::PushSound(int nResID)
{
	if(m_hWnd==NULL) return;
	EnterCriticalSection(&m_cs);
	m_vectorSoundResID.push_back(nResID);
	LeaveCriticalSection(&m_cs);
}

void CPlaySound::Home()
{
	PushSound(_T("home.wav"));
}
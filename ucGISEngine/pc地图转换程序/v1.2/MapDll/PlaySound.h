// PlaySound.h: interface for the CPlaySound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYSOUND_H__C96AE292_F62E_4B9D_929D_E6C8DDCFDD13__INCLUDED_)
#define AFX_PLAYSOUND_H__C96AE292_F62E_4B9D_929D_E6C8DDCFDD13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
using namespace std;
#pragma comment (lib,"winmm.lib")   // ����

class AFX_EXT_CLASS CPlaySound  
{
public:
	void GPSFixOK(bool ok);
	void Speed(int nSpeed);
	
	TCHAR szSoundFilesPath[MAX_PATH];
	bool m_bOpenSound;   //������������
	HINSTANCE m_hWnd;
	void Home();    // ���ŵ��ҵ�����

	CPlaySound();
	virtual ~CPlaySound();
private:
	void PushSound(TCHAR* szPath);
	void PushSound(int nResID);
	static DWORD WINAPI PlayThread(LPVOID lParam);

	vector<TCHAR*>     m_vectorSoundPath;
	vector<int>     m_vectorSoundResID;

	HANDLE m_hKillThreadEvent;		// ֪ͨ���̹߳رյ��¼�
	HANDLE m_hThreadKilledEvent;	// ���߳�����رյ��¼�

	CRITICAL_SECTION m_cs;
	void Read(int nNum);
};

#endif // !defined(AFX_PLAYSOUND_H__C96AE292_F62E_4B9D_929D_E6C8DDCFDD13__INCLUDED_)

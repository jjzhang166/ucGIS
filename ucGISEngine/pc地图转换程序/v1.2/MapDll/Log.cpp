// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "gps.h"
#include "Log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLog::CLog()
{
	
}

CLog::~CLog()
{
	if(hFile != NULL)
		CloseHandle(hFile);

}
void CLog::IntLog()
{
	hFile = NULL;

	InitializeCriticalSection(&m_cs);
}
void CLog::PutLine(const char* lpszOutput)
{
	char szMsg[512],szTime[30];
	memset(szMsg,0,512);
	SYSTEMTIME time;
	GetSystemTime(&time);
	
	sprintf(szTime,"%.2d:%.2d:%.2d",time.wHour,time.wMinute,time.wSecond);
	sprintf(szMsg,"%s\t%s\r\n",szTime,lpszOutput);
 
	if(hFile == NULL)
	{
		TCHAR szFileName[MAX_PATH] = {0};
		GetModuleFileName(NULL,szFileName,MAX_PATH);
		int len = lstrlen(szFileName);
		int nIndex = 0;
		while(len)
		{
			if(szFileName[len-1]==_T('\\'))
			{
				nIndex = len;
				break;
			}
			len--;
		}
		TCHAR szPicPath[MAX_PATH] = {0};
		memcpy(szPicPath,szFileName,len*2);
		lstrcat(szPicPath,TEXT("\\Log.txt"));

		hFile = CreateFile(szPicPath,   // create file
			GENERIC_WRITE,                // open for writing 
			0,                            // do not share 
			NULL,                         // no security 
			OPEN_ALWAYS,                  // overwrite existing 
			FILE_ATTRIBUTE_NORMAL,        // normal file 
			NULL);                        // no attr. template 

	}
	if (hFile != INVALID_HANDLE_VALUE) 
	{ 
		DWORD dwRet = 0;
		EnterCriticalSection(&m_cs);
		SetFilePointer(hFile,0,0,FILE_END);
		WriteFile(hFile,szMsg,strlen(szMsg),&dwRet,NULL);
		LeaveCriticalSection(&m_cs);
		//CloseHandle(hFile);
	} 
	
}

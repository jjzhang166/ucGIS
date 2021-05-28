// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__6233CD85_13A5_4378_A8CD_A70D63CE0F8F__INCLUDED_)
#define AFX_LOG_H__6233CD85_13A5_4378_A8CD_A70D63CE0F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define Log(x) CLog::PutLine(x)
static HANDLE hFile = NULL;
static CRITICAL_SECTION m_cs;
class AFX_EXT_CLASS CLog  
{
private:
	

public:
	static void PutLine(const char* lpszOutput);
	static void IntLog();

	CLog();
	virtual ~CLog();

};

#endif // !defined(AFX_LOG_H__6233CD85_13A5_4378_A8CD_A70D63CE0F8F__INCLUDED_)

// MapTool.h : MapTool Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMapToolApp:
// �йش����ʵ�֣������ MapTool.cpp
//

class CMapToolApp : public CWinApp
{
public:
	CMapToolApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMapToolApp theApp;
// MapToolDoc.cpp : CMapToolDoc 类的实现
//

#include "stdafx.h"
#include "MapTool.h"

#include "MapToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapToolDoc

IMPLEMENT_DYNCREATE(CMapToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CMapToolDoc, CDocument)
END_MESSAGE_MAP()


// CMapToolDoc 构造/析构

CMapToolDoc::CMapToolDoc()
{
	// TODO: 在此添加一次性构造代码

}

CMapToolDoc::~CMapToolDoc()
{
}

BOOL CMapToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CMapToolDoc 序列化

void CMapToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
		//m_Doc.m_UserMap.WriteToFile();
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CMapToolDoc 诊断

#ifdef _DEBUG
void CMapToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMapToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMapToolDoc 命令

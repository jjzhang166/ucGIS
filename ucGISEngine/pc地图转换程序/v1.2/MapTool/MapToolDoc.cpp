// MapToolDoc.cpp : CMapToolDoc ���ʵ��
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


// CMapToolDoc ����/����

CMapToolDoc::CMapToolDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CMapToolDoc::~CMapToolDoc()
{
}

BOOL CMapToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CMapToolDoc ���л�

void CMapToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
		//m_Doc.m_UserMap.WriteToFile();
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CMapToolDoc ���

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


// CMapToolDoc ����

// MapToolView.cpp : CMapToolView ���ʵ��
//

#include "stdafx.h"
#include "MapTool.h"

#include "MapToolDoc.h"
#include "MapToolView.h"
#include "DialogLayerControl.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include<MifToMymap.h>

// CMapToolView

IMPLEMENT_DYNCREATE(CMapToolView, CView)

BEGIN_MESSAGE_MAP(CMapToolView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_OPEN_QM, &CMapToolView::OnOpenQm)
	ON_WM_SIZE()
	ON_COMMAND(ID_ZOOM_IN, &CMapToolView::OnZoomIn)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_MAP_PAN, &CMapToolView::OnMapPan)
	ON_COMMAND(ID_MEASURE_DIS, &CMapToolView::OnMeasureDis)
	ON_COMMAND(ID_MAP_CENTER, &CMapToolView::OnMapCenter)
	ON_COMMAND(ID_ZOOM_OUT, &CMapToolView::OnZoomOut)
	ON_COMMAND(ID_SHOW_ALL_MAP, &CMapToolView::OnShowAllMap)
	ON_COMMAND(ID_OPEN_MIF, &CMapToolView::OnOpenMif)
	ON_COMMAND(ID_LAYER_MAN, &CMapToolView::OnLayerMan)

	ON_WM_MOUSEHWHEEL()
	ON_COMMAND(ID_FILE_SAVE, &CMapToolView::OnFileSave)
	ON_COMMAND(ID_32803, &CMapToolView::OnGridCreate)
	ON_COMMAND(ID_SAVE_MAP_FILE, &CMapToolView::OnSaveMapFile)
	ON_COMMAND(ID_SAVE_MAP_FILE_AS, &CMapToolView::OnSaveMapFileAs)
	ON_COMMAND(ID_32804, &CMapToolView::OnSaveGrid)
	ON_COMMAND(ID_READGRIDINDEX, &CMapToolView::OnReadgridindex)
	ON_COMMAND(ID_SAVEGRIDINDEXINDEX, &CMapToolView::OnSavegridindexindex)
END_MESSAGE_MAP()

// CMapToolView ����/����
extern CString g_strAppDirectory;
CMapToolView::CMapToolView()
{

	m_bNewData = false;

	// ��ȡ�������ڵ�Ŀ¼����������ȫ�ֱ�����
	TCHAR pszCurPath[500];
	int PathLen = GetModuleFileName(NULL,pszCurPath,500);
	while(TRUE)
	{
		if (pszCurPath[PathLen--]=='\\')
			break;
	}
	pszCurPath[PathLen+1] = 0x0;
	g_strAppDirectory = (CString)pszCurPath;

	m_Doc.m_bDrawScale = true;
}

CMapToolView::~CMapToolView()
{
}

BOOL CMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMapToolView ����

void CMapToolView::OnDraw(CDC* pDC)
{
	CMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_Doc.SetCoordMap(&m_mapCoordMap);
	m_Doc.Draw(pDC->m_hDC,0,false,false);
}


// CMapToolView ��ӡ

BOOL CMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CMapToolView ���

#ifdef _DEBUG
void CMapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapToolDoc* CMapToolView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolDoc)));
	return (CMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapToolView ��Ϣ�������

void CMapToolView::OnOpenQm()
{
	// ָ���ļ�·����ͨ�����ļ�����Ի�����
	CFileDialog myDlg(TRUE,_T(".qm2"),_T("*.qm2"),OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,_T("Txt Files (*.qm2)|*.qm2| All Files (*.*)|*.*||"),NULL);
	CString m_strFilePath;
	if (myDlg.DoModal()==IDOK)
		m_strFilePath = myDlg.GetPathName();
	else
		return;	
	delete myDlg;

	if(m_strFilePath.GetLength()>0)
	{
		CString strPath = m_strFilePath;

		m_Doc.m_FileMap.RealseMap();  // �ͷ����򿪵ĵ�ͼ

		m_Doc.SetCoordMap(&m_mapCoordMap); // ���õ�ͼ����״̬
		TCHAR* szPath =  strPath.GetBuffer() ;
		m_Doc.m_FileMap.ReadFromFile(szPath);  // �򿪵�ͼ
		m_mapCoordMap.SetCenter(m_Doc.m_FileMap.m_fCenterX,m_Doc.m_FileMap.m_fCenterY);  // ���õ�ͼ���ĵ����꣬�Ա���ʹ��ͼһ�򿪾ʹ�����Ļ����
		m_mapCoordMap.scale = m_Doc.m_FileMap.scale;  // ���õ�ǰ������Ϊ��ͼ����ı�����
		strPath.ReleaseBuffer();

		CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
		 pFrame->SetWindowText(m_strFilePath);

		Invalidate();
	}
}

void CMapToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_mapCoordMap.wScreen = cx;
	m_mapCoordMap.hScreen = cy;
	m_mapCoordMap.nMapMode = 0;
	m_mapCoordMap.m_bMapChanged = true;
	m_Doc.SetCoordMap(&m_mapCoordMap);
}

void CMapToolView::OnZoomIn()
{
	m_Doc.SetTools(13);
}

void CMapToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case   VK_LEFT: 
		m_Doc.Left();

		Invalidate();
		break ;       
	case   VK_RIGHT:
		m_Doc.Right();

		Invalidate();
		break ;
	case   VK_UP:
		m_Doc.Up();

		Invalidate();
		break ;
	case   VK_DOWN:
		m_Doc.Down();

		Invalidate();
		break ;
	}
	CDC* pClientDC = GetDC();
	m_Doc.OnKeyDown(nChar,nRepCnt,nFlags,pClientDC->m_hDC);
	ReleaseDC(pClientDC);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMapToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDC* pClientDC = GetDC();
	m_Doc.OnLButtonDown(nFlags, point,pClientDC->m_hDC);
	ReleaseDC(pClientDC);

	CView::OnLButtonDown(nFlags, point);
}

void CMapToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDC* pClientDC = GetDC();
	m_Doc.OnLButtonUp(nFlags, point,pClientDC->m_hDC);
	ReleaseDC(pClientDC);


	CView::OnLButtonUp(nFlags, point);
}

void CMapToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	CDC* pClientDC = GetDC();
	m_Doc.OnMouseMove(nFlags, point,pClientDC->m_hDC);
	ReleaseDC(pClientDC);

	CView::OnMouseMove(nFlags, point);
}

void CMapToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CDC* pClientDC = GetDC();
	m_Doc.OnRButtonDown(nFlags, point,pClientDC->m_hDC);
	ReleaseDC(pClientDC);

	CView::OnRButtonDown(nFlags, point);
}

BOOL CMapToolView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(m_Doc.m_nDrawCurrent==16)
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_CROSS));
	else if (m_Doc.m_nDrawCurrent==11)
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOM_OUT));
	else if (m_Doc.m_nDrawCurrent==13)
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOM_IN));
	else if (m_Doc.m_nDrawCurrent==1)
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_SET_CENTER));
	else if (m_Doc.m_nDrawCurrent==2)
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_HAND));
	else if (m_Doc.m_nDrawCurrent==100)
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	else if (m_Doc.m_nDrawCurrent==101)
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ADD_POINT));
	else if (m_Doc.m_nDrawCurrent==102)
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_DELETE_POINT));

	else
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	return FALSE;//CView::OnSetCursor(pWnd, nHitTest, message);
}

void CMapToolView::OnMapPan()
{
	m_Doc.SetTools(2);
}

void CMapToolView::OnMeasureDis()
{
	m_Doc.SetTools(16);
}

void CMapToolView::OnMapCenter()
{
	m_Doc.SetTools(0);	
}
void CMapToolView::OnZoomOut()
{
	m_Doc.SetTools(11);
}

void CMapToolView::OnShowAllMap()
{
	m_Doc.m_FileMap.ShowAllMap();
	Invalidate();
}

void CMapToolView::OnOpenMif()
{
	// ָ���ļ�·����ͨ�����ļ�����Ի�����
	CFileDialog myDlg(TRUE,_T(".mif"),_T("*.mif"),OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,_T("Txt Files (*.mif)|*.mif| All Files (*.*)|*.*||"),NULL);
	CString m_strFilePath;
	if (myDlg.DoModal()==IDOK)
		m_strFilePath = myDlg.GetFileName();
	else
		return;	
	delete myDlg;

	CMifToMymap mifToMap;
	//mifToMap.m_bOneLine = true;
	mifToMap.strMifPath = m_strFilePath;
	mifToMap.strMapPath = "xx.qm2";
	mifToMap.m_bOneLine = false;
	mifToMap.TransformMifData(&m_Doc.m_UserMap);
	m_Doc.m_UserMap.m_bShowAllLabel = true;
	m_Doc.m_UserMap.ShowAllMap();
	Invalidate();
}

void CMapToolView::OnLayerMan()
{
	CDialogLayerControl dog;
	int nCount = 0;
	dog.pState = m_Doc.m_FileMap.GetLayersState(nCount);
	dog.m_nCount = nCount;
	if(dog.DoModal()==IDOK)
	{
		m_Doc.m_FileMap.SetLayersState(dog.pState);
		if(dog.m_bOrderChanged)
		{

		}
	}
	delete dog.pState;
}


void CMapToolView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_mapCoordMap.ZoomIn();
	Invalidate();
	//m_Doc.OnMouseHWheel(nFlags,zDelta,pt);
}

void CMapToolView::OnFileSave()
{
	//m_Doc.m_UserMap.WriteToFile();
	// TODO: �ڴ���������������
}

void CMapToolView::OnGridCreate()
{
	// TODO: �ڴ���������������
	m_Doc.m_FileMap.Oninitgrid();
	m_Doc.m_FileMap.IndexBuid();
}

void CMapToolView::OnSaveMapFile()
{
	// TODO: �ڴ���������������
	//m_Doc.m_UserMap.WriteToFile();
}

void CMapToolView::OnSaveMapFileAs()
{
	// TODO: �ڴ���������������
	CFileDialog myDlg(TRUE,_T(".qm2"),_T("*.qm2"),OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT,_T("Txt Files (*.qm2)|*.qm2| All Files (*.*)|*.*||"),NULL);
	CString m_strFilePath;
	if (myDlg.DoModal()==IDOK)
		m_strFilePath = myDlg.GetPathName();
	else
		return;	
	delete myDlg;

	if(m_strFilePath.GetLength()>0)
	{
		CString strPath = m_strFilePath;
		TCHAR* szPath =  strPath.GetBuffer() ;
	    m_Doc.m_UserMap.WriteToFile(szPath);
	}
}

void CMapToolView::OnSaveGrid()
{
	// TODO: �ڴ���������������
	CFileDialog myDlg(TRUE,_T(".ind"),_T("*.ind"),OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT,_T("Txt Files (*.ind)|*.ind| All Files (*.*)|*.*||"),NULL);
	CString m_strFilePath;
	if (myDlg.DoModal()==IDOK)
		m_strFilePath = myDlg.GetPathName();
	else
		return;	
	delete myDlg;

	if(m_strFilePath.GetLength()>0)
	{
		CString strPath = m_strFilePath;
		TCHAR* szPath =  strPath.GetBuffer() ;
	    m_Doc.m_FileMap.SaveGridIndex(szPath);
	}
}


void CMapToolView::OnReadgridindex()
{
	// TODO: �ڴ���������������
	CFileDialog myDlg(TRUE,_T(".ind"),_T("*.ind"),OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,_T("Txt Files (*.ind)|*.ind| All Files (*.*)|*.*||"),NULL);
	CString m_strFilePath;
	if (myDlg.DoModal()==IDOK)
		m_strFilePath = myDlg.GetPathName();
	else
		return;	
	delete myDlg;

	if(m_strFilePath.GetLength()>0)
	{
		CString strPath = m_strFilePath;
		TCHAR* szPath =  strPath.GetBuffer() ;
	    m_Doc.m_FileMap.ReadGridIndex(szPath);
	}
}

void CMapToolView::OnSavegridindexindex()
{
	// TODO: �ڴ���������������
	CFileDialog myDlg(TRUE,_T(".inx"),_T("*.inx"),OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT,_T("Txt Files (*.inx)|*.inx| All Files (*.*)|*.*||"),NULL);
	CString m_strFilePath;
	if (myDlg.DoModal()==IDOK)
		m_strFilePath = myDlg.GetPathName();
	else
		return;	
	delete myDlg;

	if(m_strFilePath.GetLength()>0)
	{
		CString strPath = m_strFilePath;
		TCHAR* szPath =  strPath.GetBuffer() ;
	    m_Doc.m_FileMap.SaveGridIndexIndex(szPath);
	}
}

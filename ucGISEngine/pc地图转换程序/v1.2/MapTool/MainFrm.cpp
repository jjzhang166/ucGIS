// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "MapTool.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT DrawButtons[]=
{
	ID_DRAW_SELECTION,
	ID_DRAW_POLYLINE,	
	ID_SELECT_LINE,
	ID_DRAW_TEXT,
	ID_ADD_POINT,
	ID_DELETE_POINT,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// 地图编辑工具
	if (!m_wndDrawToolBar.CreateEx(this, TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_SIZE_FIXED |
		CBRS_TOP | CBRS_TOOLTIPS) ||
		!m_wndDrawToolBar.LoadToolBar(IDR_EDIT_TOOLBAR)||
		!m_wndDrawToolBar.SetButtons(DrawButtons,
		sizeof(DrawButtons)/sizeof(UINT)))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;       // fail to create
	}
	m_wndDrawToolBar.SetWindowText(_T("工具箱"));
	m_wndDrawToolBar.EnableDocking(0);

	CPoint pt(GetSystemMetrics(SM_CXSCREEN) - 150,
		GetSystemMetrics(SM_CYSCREEN) / 3);

	m_wndDrawToolBar.SetColumns(2);
	FloatControlBar(&m_wndDrawToolBar, pt);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序




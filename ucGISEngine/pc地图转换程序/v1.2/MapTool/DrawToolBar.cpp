// DrawToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include "DrawToolBar.h"


// CDrawToolBar

IMPLEMENT_DYNAMIC(CDrawToolBar, CToolBar)
CDrawToolBar::CDrawToolBar()
{
	 m_nColumns = 2;
	m_cxLeftBorder = 5;
	m_cyTopBorder = 5;
	m_cxRightBorder = 5;
	m_cyBottomBorder = 5;
}

CDrawToolBar::~CDrawToolBar()
{
}


BEGIN_MESSAGE_MAP(CDrawToolBar, CToolBar)
END_MESSAGE_MAP()



// CDrawToolBar 消息处理程序
void CDrawToolBar::SetColumns(UINT nColumns)
{
	m_nColumns = nColumns;
	int nCount = GetToolBarCtrl().GetButtonCount();

	for (int i = 0; i < nCount; i++)
	{
		UINT nStyle = GetButtonStyle(i);
		BOOL bWrap = (((i + 1) % nColumns) == 0);
		if (bWrap)
			nStyle |= TBBS_WRAPPED;
		else
			nStyle &= ~TBBS_WRAPPED;
		SetButtonStyle(i, nStyle);
	}
	
	Invalidate();
	GetParentFrame()->RecalcLayout();
}


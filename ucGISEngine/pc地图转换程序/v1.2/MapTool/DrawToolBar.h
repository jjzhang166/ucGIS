#pragma once


// CDrawToolBar

class CDrawToolBar : public CToolBar
{
	DECLARE_DYNAMIC(CDrawToolBar)

public:
	CDrawToolBar();

	void SetColumns(UINT nColumns);
	UINT GetColumns() { return m_nColumns; };

	virtual ~CDrawToolBar();
protected:
	UINT m_nColumns;
protected:
	DECLARE_MESSAGE_MAP()
};



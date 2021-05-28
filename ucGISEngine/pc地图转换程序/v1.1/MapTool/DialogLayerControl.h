#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Resource.h"
#include <Draw.h>
// CDialogLayerControl 对话框

class CDialogLayerControl : public CDialog
{
	DECLARE_DYNAMIC(CDialogLayerControl)

public:
	CDialogLayerControl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogLayerControl();

// 对话框数据
	enum { IDD = IDD_DIALOG_LAYER_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSetLayerDisplay();
	afx_msg void OnBnClickedLayerAdd();

	CListCtrl m_ListCtrl;
	CButton m_btnAdd;
	CButton m_btnDelete;
	CButton m_btnDisplay;
	CButton m_btnUp;
	CButton m_btnDown;

	LayerStruct* pState;
	int   m_nCount;
	bool  m_bOrderChanged;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedLayerUp();
protected:
	CImageList m_imagelist;
	CFont m_font;
	int m_nIndex ;
public:
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedLayerDown();
	afx_msg void OnBnClickedOk();
	//CEdit ctrEditBox;
	afx_msg void OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);

	
};

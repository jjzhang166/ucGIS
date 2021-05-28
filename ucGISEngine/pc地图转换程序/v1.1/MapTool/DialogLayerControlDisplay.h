#pragma once
#include "afxwin.h"
#include "Resource.h"

// CDialogLayerControlDisplay 对话框

class CDialogLayerControlDisplay : public CDialog
{
	DECLARE_DYNAMIC(CDialogLayerControlDisplay)

public:
	CDialogLayerControlDisplay(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogLayerControlDisplay();

	CDialogLayerControlDisplay(float fEyeShotMin,float fEyeShotMax,CWnd* pParent = NULL)
		: CDialog(CDialogLayerControlDisplay::IDD, pParent)
	{
		m_fEyeShotMin = fEyeShotMin;
		m_fEyeShotMax = fEyeShotMax;
	};
// 对话框数据
	enum { IDD = IDD_DIALOG_LAYER_CONTROL_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedOk();

	float m_fEyeShotMin;
	float m_fEyeShotMax;

};

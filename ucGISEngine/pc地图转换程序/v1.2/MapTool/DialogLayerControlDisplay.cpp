// DialogLayerControlDisplay.cpp : 实现文件
//

#include "stdafx.h"

#include "DialogLayerControlDisplay.h"
#include ".\dialoglayercontroldisplay.h"
#include "Resource.h"

// CDialogLayerControlDisplay 对话框

IMPLEMENT_DYNAMIC(CDialogLayerControlDisplay, CDialog)
CDialogLayerControlDisplay::CDialogLayerControlDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLayerControlDisplay::IDD, pParent)
	, m_fEyeShotMin(0)
	, m_fEyeShotMax(0)
{
}

CDialogLayerControlDisplay::~CDialogLayerControlDisplay()
{
}

void CDialogLayerControlDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fEyeShotMin);
	DDX_Text(pDX, IDC_EDIT2, m_fEyeShotMax);
}


BEGIN_MESSAGE_MAP(CDialogLayerControlDisplay, CDialog)
	ON_BN_CLICKED(IDC_CHECK5, OnBnClickedCheck5)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDialogLayerControlDisplay 消息处理程序

void CDialogLayerControlDisplay::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDialogLayerControlDisplay::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

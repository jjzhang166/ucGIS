// DialogLayerControlDisplay.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "DialogLayerControlDisplay.h"
#include ".\dialoglayercontroldisplay.h"
#include "Resource.h"

// CDialogLayerControlDisplay �Ի���

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


// CDialogLayerControlDisplay ��Ϣ�������

void CDialogLayerControlDisplay::OnBnClickedCheck5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDialogLayerControlDisplay::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

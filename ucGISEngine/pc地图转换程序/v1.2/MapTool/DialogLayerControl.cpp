// DialogLayerControl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DialogLayerControl.h"
#include "DialogLayerControlDisplay.h"
#include ".\dialoglayercontrol.h"


// CDialogLayerControl �Ի���

IMPLEMENT_DYNAMIC(CDialogLayerControl, CDialog)
CDialogLayerControl::CDialogLayerControl(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLayerControl::IDD, pParent)
{
	m_nCount = 0;
	m_bOrderChanged = false;
}

CDialogLayerControl::~CDialogLayerControl()
{
	m_imagelist.DeleteImageList();
}

void CDialogLayerControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Control(pDX, IDC_LAYER_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_LAYER_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_SET_LAYER_DISPLAY, m_btnDisplay);
	DDX_Control(pDX, IDC_LAYER_UP, m_btnUp);
	DDX_Control(pDX, IDC_LAYER_DOWN, m_btnDown);
}


BEGIN_MESSAGE_MAP(CDialogLayerControl, CDialog)
	ON_BN_CLICKED(IDC_SET_LAYER_DISPLAY, OnBnClickedSetLayerDisplay)
	ON_BN_CLICKED(IDC_LAYER_ADD, OnBnClickedLayerAdd)
	ON_BN_CLICKED(IDC_LAYER_UP, OnBnClickedLayerUp)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_BN_CLICKED(IDC_LAYER_DOWN, OnBnClickedLayerDown)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnNMRclickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
END_MESSAGE_MAP()

bool  WToA(TCHAR *wChar,char *cChar)
{
	int multibytelen=WideCharToMultiByte(  //�����Unicodeת����Ansi����Ҫ���ֽ���
		CP_ACP,  //����ANSI code pageת��
		WC_COMPOSITECHECK | WC_DEFAULTCHAR, //ת��������ȱʡ�ַ�����
		wChar,  //Ҫת�����ַ�����ַ
		lstrlen(wChar),  //Ҫת���ĸ���
		0,  //ת�����ַ������õĵ�ַ
		0,  //���ת���ַ��ĸ�����Ϊ0��ʾ����ת��Unicode����Ҫ���ٸ��ֽ�
		0,  //ȱʡ���ַ�:"\0"
		0   //ȱʡ������
		);
	if(multibytelen > 0)
	{
		WideCharToMultiByte(  //ת��Unicode��Ansi
			CP_ACP,
			WC_COMPOSITECHECK | WC_DEFAULTCHAR,
			wChar,
			lstrlen(wChar),
			(char *)cChar,  //ת������������
			128,  //���128���ֽ�
			0,
			0);
	}
	cChar[multibytelen] = '\0';
	return true;
}
float myatof(CString str)
{
	TCHAR * ls = str.GetBuffer();
	char ch[50];
	WToA(ls,ch);

	return atof(ch);
}

// CDialogLayerControl ��Ϣ�������

void CDialogLayerControl::OnBnClickedSetLayerDisplay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();

	CDialogLayerControlDisplay dog(myatof(m_ListCtrl.GetItemText((int)pos-1,4)),myatof(m_ListCtrl.GetItemText((int)pos-1,5)),this);

	if(dog.DoModal()==IDOK)
	{
		TCHAR str[100];
		_stprintf(str,_T("%f"),dog.m_fEyeShotMin);
		m_ListCtrl.SetItemText((int)pos-1,4,str);

		_stprintf(str,_T("%f"),dog.m_fEyeShotMax);
		m_ListCtrl.SetItemText((int)pos-1,5,str);
	}
}

void CDialogLayerControl::OnBnClickedLayerAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

BOOL CDialogLayerControl::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_nIndex = 0;
	m_bOrderChanged = false;

	m_imagelist.Create(16,16,TRUE,2,2);
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	m_ListCtrl.SetImageList(&m_imagelist,LVSIL_SMALL);

	m_font.CreateFont(12, 0,0,0,FW_NORMAL, 0,0,0,
	DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
	DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("����"));
	m_ListCtrl.SetFont(&m_font);

	/*-----------------------------------------------------------*/ 		
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListCtrl.SetBkColor(RGB(247,247,255));
	m_ListCtrl.SetTextColor(RGB(0,0,255));
	m_ListCtrl.SetTextBkColor(RGB(247,247,255));
	m_ListCtrl.InsertColumn(0, _T("ͼ��"), LVCFMT_LEFT, 170);
	m_ListCtrl.InsertColumn(1, _T("�ɼ�"), LVCFMT_LEFT, 42);
	m_ListCtrl.InsertColumn(2, _T("�༭"), LVCFMT_LEFT, 42);
	m_ListCtrl.InsertColumn(3, _T("��ע"), LVCFMT_LEFT, 42);
	m_ListCtrl.InsertColumn(4, _T("��ҰMin"), LVCFMT_LEFT, 60);
	m_ListCtrl.InsertColumn(5, _T("��ҰMax"), LVCFMT_LEFT, 60);

	for(int i=0;i<m_nCount;i++)
	{
		if(pState[i].Name!=_T(""))
		{
			m_ListCtrl.InsertItem(0,pState[i].Name);
			m_ListCtrl.SetItemText(0,1,pState[i].Visible?_T("Yes"):_T("No"));
			m_ListCtrl.SetItemText(0,2,pState[i].Editable?_T("Yes"):_T("No"));
			m_ListCtrl.SetItemText(0,3,pState[i].AutoLabel?_T("Yes"):_T("No"));
			TCHAR str[100];
			_stprintf(str,_T("%4.0f"),pState[i].nEyeShotMin);
			m_ListCtrl.SetItemText(0,4,str);
			_stprintf(str,_T("%4.0f"),pState[i].nEyeShotMax);
			m_ListCtrl.SetItemText(0,5,str);
		}
	}
  
	// ���ó�ʼ��ʾ״̬
	//m_btnDelete.;
	//m_btnDisplay;
	//m_btnUp;
	//m_btnDown;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDialogLayerControl::OnBnClickedLayerUp()
{
	m_ListCtrl.SetFocus();
	
	if (m_nIndex == -1)	  //���û��ѡ��ͷ���
	{
		MessageBox(_T("��ѡ��һ��������!"),_T("��ʾ"),MB_ICONINFORMATION);
		return;  
	}
	
	if (m_nIndex == 0)  // �ж���ѡ���Ƿ�λ������
	{
		MessageBox(_T("�Ѿ�λ�ڵ�һ��!"),_T("��ʾ"),MB_ICONINFORMATION);
		return;
	}
	
	// ��ȡ��ѡ�б����������
	CString str1, str2, str3, str4,str5,str6;
	str1 = m_ListCtrl.GetItemText(m_nIndex, 0);
	str2 = m_ListCtrl.GetItemText(m_nIndex, 1);
	str3 = m_ListCtrl.GetItemText(m_nIndex, 2);
	str4 = m_ListCtrl.GetItemText(m_nIndex, 3);
	str5 = m_ListCtrl.GetItemText(m_nIndex, 4);
	str6 = m_ListCtrl.GetItemText(m_nIndex, 5);
	
	// ɾ����ѡ�б���
	m_ListCtrl.DeleteItem(m_nIndex);
	
	// ��m_nIndex-1λ�ô�����������ɾ�б���ĸ�������	
	m_ListCtrl.InsertItem(m_nIndex-1, str1);
	m_ListCtrl.SetItemText(m_nIndex-1, 1, str2);
	m_ListCtrl.SetItemText(m_nIndex-1, 2, str3);
	m_ListCtrl.SetItemText(m_nIndex-1, 3, str4);
	m_ListCtrl.SetItemText(m_nIndex-1, 4, str5);
	m_ListCtrl.SetItemText(m_nIndex-1, 5, str6);
	m_nIndex--;
	
	// ʹ��m_nIndex-1λ�ô���Ŀ������ʾ����ý���
	UINT flag = LVIS_SELECTED|LVIS_FOCUSED;
	m_ListCtrl.SetItemState(m_nIndex, flag, flag);

	int nd = m_nCount - m_nIndex;
	LayerStruct ls = pState[nd];
	pState[nd] = pState[nd-1];
	pState[nd-1] = pState[nd];
}

void CDialogLayerControl::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	m_nIndex = m_ListCtrl.GetNextSelectedItem(pos);  // �õ���Ŀ����
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(m_ListCtrl.GetItemText(pNMListView->iItem,pNMListView->iSubItem)==_T("Yes"))
		m_ListCtrl.SetItemText(pNMListView->iItem,pNMListView->iSubItem,_T("No"));
	else if(m_ListCtrl.GetItemText(pNMListView->iItem,pNMListView->iSubItem)==_T("No"))
		m_ListCtrl.SetItemText(pNMListView->iItem,pNMListView->iSubItem,_T("Yes"));
}

void CDialogLayerControl::OnBnClickedLayerDown()
{
	m_ListCtrl.SetFocus();	

	if (m_nIndex == -1)	  //���û��ѡ��ͷ���
	{
		MessageBox(_T("��ѡ��һ��������!"),_T("��ʾ"),MB_ICONINFORMATION);
		return;  
	}

	if (m_nIndex == m_ListCtrl.GetItemCount()-1)  // �ж���ѡ���Ƿ�λ����β
	{
		MessageBox(_T("�Ѿ�λ����ĩ��!"),_T("��ʾ"),MB_ICONINFORMATION);
		return;
	}
	
	CString str1, str2, str3, str4,str5,str6;
	str1 = m_ListCtrl.GetItemText(m_nIndex, 0);
	str2 = m_ListCtrl.GetItemText(m_nIndex, 1);
	str3 = m_ListCtrl.GetItemText(m_nIndex, 2);
	str4 = m_ListCtrl.GetItemText(m_nIndex, 3);
	str5 = m_ListCtrl.GetItemText(m_nIndex, 4);
	str6 = m_ListCtrl.GetItemText(m_nIndex, 5);
	
	// ɾ����ѡ�б���
	m_ListCtrl.DeleteItem(m_nIndex);
	
	// ��m_nIndex-1λ�ô�����������ɾ�б���ĸ�������	
	m_ListCtrl.InsertItem(m_nIndex+1, str1);
	m_ListCtrl.SetItemText(m_nIndex+1, 1, str2);
	m_ListCtrl.SetItemText(m_nIndex+1, 2, str3);
	m_ListCtrl.SetItemText(m_nIndex+1, 3, str4);
	m_ListCtrl.SetItemText(m_nIndex+1, 4, str5);
	m_ListCtrl.SetItemText(m_nIndex+1, 5, str6);
	m_nIndex++;
	
	// ʹ��m_nIndex+1λ�ô���Ŀ������ʾ����ý���
	UINT flag = LVIS_SELECTED|LVIS_FOCUSED;
	m_ListCtrl.SetItemState(m_nIndex, flag, flag);

	int nd = m_nCount - m_nIndex-1;
	LayerStruct ls = pState[nd];
	pState[nd] = pState[nd+1];
	pState[nd+1] = pState[nd];
}

void CDialogLayerControl::OnBnClickedOk()
{
	int n = m_ListCtrl.GetItemCount();
	for(int i=0;i<n;i++)
	{
		pState[n-i-1].Visible = m_ListCtrl.GetItemText(i,1)==_T("Yes")?true:false;
		pState[n-i-1].Editable = m_ListCtrl.GetItemText(i,2)==_T("Yes")?true:false;
		pState[n-i-1].AutoLabel = m_ListCtrl.GetItemText(i,3)==_T("Yes")?true:false;
		pState[n-i-1].nEyeShotMin = myatof(m_ListCtrl.GetItemText(i,4));
		pState[n-i-1].nEyeShotMax = myatof(m_ListCtrl.GetItemText(i,5));
	}
	OnOK();
}

void CDialogLayerControl::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CDialogLayerControl::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

// DialogLayerControl.cpp : 实现文件
//

#include "stdafx.h"
#include "DialogLayerControl.h"
#include "DialogLayerControlDisplay.h"
#include ".\dialoglayercontrol.h"


// CDialogLayerControl 对话框

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
	int multibytelen=WideCharToMultiByte(  //计算从Unicode转换到Ansi后需要的字节数
		CP_ACP,  //根据ANSI code page转换
		WC_COMPOSITECHECK | WC_DEFAULTCHAR, //转换出错用缺省字符代替
		wChar,  //要转换的字符串地址
		lstrlen(wChar),  //要转换的个数
		0,  //转换后字符串放置的地址
		0,  //最多转换字符的个数，为0表示返回转换Unicode后需要多少个字节
		0,  //缺省的字符:"\0"
		0   //缺省的设置
		);
	if(multibytelen > 0)
	{
		WideCharToMultiByte(  //转换Unicode到Ansi
			CP_ACP,
			WC_COMPOSITECHECK | WC_DEFAULTCHAR,
			wChar,
			lstrlen(wChar),
			(char *)cChar,  //转换到缓冲区中
			128,  //最多128个字节
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

// CDialogLayerControl 消息处理程序

void CDialogLayerControl::OnBnClickedSetLayerDisplay()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("宋体"));
	m_ListCtrl.SetFont(&m_font);

	/*-----------------------------------------------------------*/ 		
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListCtrl.SetBkColor(RGB(247,247,255));
	m_ListCtrl.SetTextColor(RGB(0,0,255));
	m_ListCtrl.SetTextBkColor(RGB(247,247,255));
	m_ListCtrl.InsertColumn(0, _T("图层"), LVCFMT_LEFT, 170);
	m_ListCtrl.InsertColumn(1, _T("可见"), LVCFMT_LEFT, 42);
	m_ListCtrl.InsertColumn(2, _T("编辑"), LVCFMT_LEFT, 42);
	m_ListCtrl.InsertColumn(3, _T("标注"), LVCFMT_LEFT, 42);
	m_ListCtrl.InsertColumn(4, _T("视野Min"), LVCFMT_LEFT, 60);
	m_ListCtrl.InsertColumn(5, _T("视野Max"), LVCFMT_LEFT, 60);

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
  
	// 设置初始显示状态
	//m_btnDelete.;
	//m_btnDisplay;
	//m_btnUp;
	//m_btnDown;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDialogLayerControl::OnBnClickedLayerUp()
{
	m_ListCtrl.SetFocus();
	
	if (m_nIndex == -1)	  //如果没有选择就返回
	{
		MessageBox(_T("请选择一项再上移!"),_T("提示"),MB_ICONINFORMATION);
		return;  
	}
	
	if (m_nIndex == 0)  // 判断所选项是否位于行首
	{
		MessageBox(_T("已经位于第一行!"),_T("提示"),MB_ICONINFORMATION);
		return;
	}
	
	// 提取所选列表项各列类容
	CString str1, str2, str3, str4,str5,str6;
	str1 = m_ListCtrl.GetItemText(m_nIndex, 0);
	str2 = m_ListCtrl.GetItemText(m_nIndex, 1);
	str3 = m_ListCtrl.GetItemText(m_nIndex, 2);
	str4 = m_ListCtrl.GetItemText(m_nIndex, 3);
	str5 = m_ListCtrl.GetItemText(m_nIndex, 4);
	str6 = m_ListCtrl.GetItemText(m_nIndex, 5);
	
	// 删除所选列表项
	m_ListCtrl.DeleteItem(m_nIndex);
	
	// 在m_nIndex-1位置处插入上面所删列表项的各列类容	
	m_ListCtrl.InsertItem(m_nIndex-1, str1);
	m_ListCtrl.SetItemText(m_nIndex-1, 1, str2);
	m_ListCtrl.SetItemText(m_nIndex-1, 2, str3);
	m_ListCtrl.SetItemText(m_nIndex-1, 3, str4);
	m_ListCtrl.SetItemText(m_nIndex-1, 4, str5);
	m_ListCtrl.SetItemText(m_nIndex-1, 5, str6);
	m_nIndex--;
	
	// 使得m_nIndex-1位置处项目高亮显示并获得焦点
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
	m_nIndex = m_ListCtrl.GetNextSelectedItem(pos);  // 得到项目索引
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

	if (m_nIndex == -1)	  //如果没有选择就返回
	{
		MessageBox(_T("请选择一项再下移!"),_T("提示"),MB_ICONINFORMATION);
		return;  
	}

	if (m_nIndex == m_ListCtrl.GetItemCount()-1)  // 判断所选项是否位于行尾
	{
		MessageBox(_T("已经位于最末行!"),_T("提示"),MB_ICONINFORMATION);
		return;
	}
	
	CString str1, str2, str3, str4,str5,str6;
	str1 = m_ListCtrl.GetItemText(m_nIndex, 0);
	str2 = m_ListCtrl.GetItemText(m_nIndex, 1);
	str3 = m_ListCtrl.GetItemText(m_nIndex, 2);
	str4 = m_ListCtrl.GetItemText(m_nIndex, 3);
	str5 = m_ListCtrl.GetItemText(m_nIndex, 4);
	str6 = m_ListCtrl.GetItemText(m_nIndex, 5);
	
	// 删除所选列表项
	m_ListCtrl.DeleteItem(m_nIndex);
	
	// 在m_nIndex-1位置处插入上面所删列表项的各列类容	
	m_ListCtrl.InsertItem(m_nIndex+1, str1);
	m_ListCtrl.SetItemText(m_nIndex+1, 1, str2);
	m_ListCtrl.SetItemText(m_nIndex+1, 2, str3);
	m_ListCtrl.SetItemText(m_nIndex+1, 3, str4);
	m_ListCtrl.SetItemText(m_nIndex+1, 4, str5);
	m_ListCtrl.SetItemText(m_nIndex+1, 5, str6);
	m_nIndex++;
	
	// 使得m_nIndex+1位置处项目高亮显示并获得焦点
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
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CDialogLayerControl::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

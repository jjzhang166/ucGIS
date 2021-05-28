#include "StdAfx.h"
#include ".\userlayer.h"
#include "car.h"
#include "log.h"
#include "structs.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUserLayer::CUserLayer(void)
{
	m_mapCoordMap = NULL;
	m_pNavCar = NULL;
	m_pSound = NULL;
	m_bShowState = true;
	m_bShowMeasureLen = false;
	m_bShowMeasureArea = false;
	m_nNetState = 0;
	InitializeCriticalSection(&m_csDraw);

#ifdef UNDER_WINCE
	m_ceDraw = NULL;
	m_pceBmpCenter = NULL;
#endif
}

CUserLayer::~CUserLayer(void)
{
#ifdef UNDER_WINCE
	if(m_pceBmpCenter!=NULL)
		delete m_pceBmpCenter;
	m_pceBmpCenter = NULL;
#endif
	DeleteCriticalSection(&m_csDraw);
	/*
	typedef map <int, CCar*>::const_iterator CARpos;
	for(CARpos p=m_carMap.begin(); p!=m_carMap.end(); ++p)
	{
		delete p->second;
		p->second = NULL;
	}
	*/
}
void CUserLayer::Draw(HDC hDC)
{
	EnterCriticalSection(&m_csDraw);
	if(m_bShowState)
		DrawState(hDC);

	typedef map <int, CCar*>::const_iterator CARpos;

	for(CARpos p=m_carMap.begin(); p!=m_carMap.end(); ++p)
	{
		p->second->Draw(hDC);
	}
	DrawXX();  // ������ͼ��

	LeaveCriticalSection(&m_csDraw);
}
void CUserLayer::DrawState(HDC hDC)
{
#ifdef UNDER_WINCE
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.bottom = 17;
	rect.right = 38;

	// ���Ʊ�����
	CCEPen cePen;
	cePen.CreatePen(1,1, m_ceDraw->ConvertColor(0,0,0 ) ); //
	CCEBrush ceBrush;
	ceBrush.CreateBrush( 1, m_ceDraw->ConvertColor(255,230,255) , 1 );
	CCEFont ceFont;
	ceFont.SetFontColor(RGB(255,255,0));

	m_ceDraw->SetGDIObject( &ceBrush );
	m_ceDraw->SetGDIObject( &cePen );
	CCEFont* oldceFont = (CCEFont*)m_ceDraw->SetGDIObject( &ceFont );
	m_ceDraw->FillRect(rect,1);

	m_ceDraw->DrawLine(2,14,34,14);
	m_ceDraw->DrawLine(2,9,2,14);
	m_ceDraw->DrawLine(35,9,35,14);

	DWORD  now = GetTickCount();
	if(m_pNavCar!=NULL)
	{
		// ���û���յ���Ч��GPS���ݣ���ʾ��ɫ�߿�
		// �������û���յ��������ݣ���ʾ��ɫɫ�߿�
		 if((now-m_pNavCar->m_dwLastComDataTime)>3*1000)
			cePen.CreatePen(1,1, m_ceDraw->ConvertColor(255,255,232 ) ); // ǳɫ
		else if((now-m_pNavCar->m_dwLastValueGpsDataTime)<5*1000)
			cePen.CreatePen(1,1, m_ceDraw->ConvertColor(255,0,0 ) ); // ��ɫ�߿�
		else
			cePen.CreatePen(1,1, m_ceDraw->ConvertColor(255,255,0 ) ); // ��ɫ
	}else
	{
		cePen.CreatePen(1,1, m_ceDraw->ConvertColor(255,0,0 ) ); //
	}
	m_ceDraw->DrawRect(rect);
		// ���������
		int nDistance = m_mapCoordMap->scale*32*100000;

		TCHAR lpszState[30] = {0};
		if(nDistance>999)
		_sntprintf(lpszState,30,_T("%dKm"),
			nDistance/1000
			);
		else
		_sntprintf(lpszState,30,_T("%dM"),
			nDistance
			);

	
		SIZE sz;
		GetTextExtentPoint32(hDC,lpszState,lstrlen(lpszState),&sz);
		POINT pt;
		pt.x = (rect.right - sz.cx)/2;
		pt.y = rect.top;
		m_ceDraw->DrawText(&pt,lpszState, lstrlen(lpszState),0);

	// �ٶ�
		rect.left = rect.right;
		rect.right += 40;

		if(m_pNavCar!=NULL)
		{

			m_ceDraw->FillRect(rect,1);
			m_ceDraw->DrawRect(rect);
			if(m_pNavCar->m_gpsDataInfo.m_nSpeed==-1)
			_sntprintf(lpszState,30,_T("��%s"),
				_T("**")
				);
			else
			_sntprintf(lpszState,30,_T("��%3.0f"),
				m_pNavCar->m_gpsDataInfo.m_nSpeed
				);

		pt.x = rect.left+4;
		pt.y =  rect.top;
		m_ceDraw->DrawText(&pt,lpszState, lstrlen(lpszState),0);
		}

		// ���θ߶�
		rect.left = rect.right;
		rect.right += 42;

		
		if(m_pNavCar!=NULL)
		{
			m_ceDraw->FillRect(rect,1);
			m_ceDraw->DrawRect(rect);
			if(m_pNavCar->m_gpsDataInfo.m_dbHeigth==-1)
			_sntprintf(lpszState,30,_T("��%s"),
				_T("**")
				);
			else
			_sntprintf(lpszState,30,_T("��%4.0f"),
				m_pNavCar->m_gpsDataInfo.m_dbHeigth
				);

			pt.x = rect.left+4;
			pt.y =  rect.top;
			m_ceDraw->DrawText(&pt,lpszState, lstrlen(lpszState),0);
		}

		// ��������
		rect.left = rect.right;
		rect.right += 26;

		if(m_pNavCar!=NULL)
		{
			m_ceDraw->FillRect(rect,1);
			m_ceDraw->DrawRect(rect);
			if(m_pNavCar->m_gpsDataInfo.m_dbHeigth==-1)
			_sntprintf(lpszState,30,_T("��%s"),
				_T("**")
				);
			else
			_sntprintf(lpszState,30,_T("��%d"),
				m_pNavCar->m_gpsDataInfo.m_nStCount
				);
		pt.x = rect.left+3;
		pt.y =  rect.top;
		m_ceDraw->DrawText(&pt,lpszState, lstrlen(lpszState),0);
		}
	// ����״̬
		rect.left = rect.right;
		rect.right += 29;

		m_ceDraw->FillRect(rect,1);
			m_ceDraw->DrawRect(rect);

		switch(m_nNetState)
		{
		case NET_BREAK:
			_sntprintf(lpszState,300,_T("����"),
				m_nNetState
				);
			break;
		case NET_CONNECTING:
			_sntprintf(lpszState,300,_T("��.."),
				m_nNetState
				);
			break;
		case NET_CONNECTED:
			_sntprintf(lpszState,300,_T("��.."),
				m_nNetState
				);
			break;
		case NET_LOGINED:
			_sntprintf(lpszState,300,_T("����"),
				m_nNetState
				);
			break;
		case NET_LOGINFEILD:
			_sntprintf(lpszState,300,_T("�ܴ�"),
				m_nNetState
				);
			break;
		}

		pt.x = rect.left+3;
		pt.y =  rect.top;
		m_ceDraw->DrawText(&pt,lpszState, lstrlen(lpszState),0);

		 m_ceDraw->SetGDIObject( oldceFont );

		 //
		 // ��ʾ���Ȳ������
 		 //
		 if(m_bShowMeasureLen)
		 {
			 
			rect.left = 0;
			rect.top = m_mapCoordMap->hScreen-18; 	// ��ǰ��ͼ�ĸ߶ȺͿ��;
			rect.bottom = 	rect.top+17;
			rect.right = m_mapCoordMap->wScreen/2;
			m_ceDraw->FillRect(rect,1);
			m_ceDraw->DrawRect(rect);

			if(m_pNavCar)
			{
			_sntprintf(lpszState,300,_T("���:%0.3fKm"),
				m_pNavCar->m_fDistance
				);
			}
			else
			_sntprintf(lpszState,300,_T("���:%sKm"),
				_T("****")
				);

			pt.x = rect.left+3;
			pt.y =  rect.top;
			m_ceDraw->DrawText(&pt,lpszState, lstrlen(lpszState),0);
			rect.left = m_mapCoordMap->wScreen/2;
			rect.top = m_mapCoordMap->hScreen-18; 	// ��ǰ��ͼ�ĸ߶ȺͿ��;
			rect.bottom = 	rect.top+17;
			rect.right = m_mapCoordMap->wScreen-1;
			m_ceDraw->FillRect(rect,1);
			m_ceDraw->DrawRect(rect);

			if(m_pNavCar)
			{
				// ��������������1ƽ�����������Ϊƽ��������Ϊ��λ

			_sntprintf(lpszState,300,_T("����:%0.1fKm/h"),
					m_pNavCar->m_averSpeed);
			}
			else
			_sntprintf(lpszState,300,_T("����:%sKm/h"),
				_T("*****")
				);

			pt.x = rect.left+3;
			pt.y =  rect.top;
			m_ceDraw->DrawText(&pt,lpszState, lstrlen(lpszState),0);

		 }
		 //
		 // �������
		 // 
		 if(m_bShowMeasureArea)
		 {
			rect.left = m_mapCoordMap->wScreen/2;
			rect.top = m_mapCoordMap->hScreen-18; 	// ��ǰ��ͼ�ĸ߶ȺͿ��;
			rect.bottom = 	rect.top+17;
			rect.right = m_mapCoordMap->wScreen-1;
			m_ceDraw->FillRect(rect,1);
			m_ceDraw->DrawRect(rect);

			if(m_pNavCar)
			{
				// ��������������1ƽ�����������Ϊƽ��������Ϊ��λ
				if(fabs(m_pNavCar->m_fArea )>1000*1000)
			_sntprintf(lpszState,300,_T("���:%fkm2"),
					fabs(m_pNavCar->m_fArea )/(1000*1000)
				);
				else
			_sntprintf(lpszState,300,_T("���:%0.0fm2"),
					fabs(m_pNavCar->m_fArea )
				);
			}
			else
			_sntprintf(lpszState,300,_T("���:%sm2"),
				_T("****")
				);

			pt.x = rect.left+3;
			pt.y =  rect.top;
			m_ceDraw->DrawText(&pt,lpszState, lstrlen(lpszState),0);
		 }

#else


	// ��״̬��
	HPEN	oldPen,newPen;
	newPen = CreatePen(0,1,RGB(0,0,0));
	oldPen	= (HPEN)SelectObject(hDC,newPen);

	HFONT newFont, oldFont;

	LOGFONT	lf = {0};
	lf.lfHeight = 12;
	lf.lfWidth = 0;
	lf.lfWeight = 0;
	lf.lfCharSet = GB2312_CHARSET;
	lf.lfEscapement=lf.lfOrientation=0;
	memcpy(lf.lfFaceName,TEXT("����"),sizeof(lf.lfFaceName));

	newFont =  (HFONT)CreateFontIndirect(&lf);

	oldFont = (HFONT)SelectObject(hDC,newFont);


	SetBkMode(hDC,TRANSPARENT);

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.bottom = 17;
	rect.right = 39;
	// �����Ҫ�������

		HBRUSH newBrush, oldBrush;
		newBrush = (HBRUSH)CreateSolidBrush(RGB(0,255,255));
		oldBrush = (HBRUSH)SelectObject(hDC,newBrush);
		//FillRect(hDC,&rect,newBrush);
		RoundRect(hDC,rect.left,rect.top,rect.right,rect.bottom,3,3);

		// ���Ʊ�����
		MoveToEx(hDC,2,13,NULL);
		LineTo(hDC,35,13);

		MoveToEx(hDC,2,9,NULL);
		LineTo(hDC,2,15);

		MoveToEx(hDC,35,9,NULL);
		LineTo(hDC,35,15);

		// ���������
		int nDistance = (int)m_mapCoordMap->scale*33*100000;

		TCHAR lpszState[300] = {0};
		if(nDistance>999)
		_sntprintf(lpszState,300,_T("%dKM"),
			nDistance/1000
			);
		else
		_sntprintf(lpszState,300,_T("%dM"),
			nDistance
			);


		SIZE sz;
		GetTextExtentPoint32(hDC,lpszState,lstrlen(lpszState),&sz);
	
		ExtTextOut(hDC, (rect.right - sz.cx)/2, rect.top+1, 0, NULL,(LPCTSTR)(lpszState), 
         lstrlen(lpszState),0);


		// END

		// �ٶ�
		rect.left = rect.right+1;
		rect.right += 38;

		if(m_pNavCar!=NULL)
		{
			RoundRect(hDC,rect.left,rect.top,rect.right,rect.bottom,3,3);

			_sntprintf(lpszState,300,_T("��%3.0f"),
				m_pNavCar->m_gpsDataInfo.m_nSpeed
				);

			ExtTextOut(hDC, rect.left+3, rect.top+2, 0, NULL,(LPCTSTR)(lpszState), 
			 lstrlen(lpszState),0);
		}

		// ���θ߶�
		rect.left = rect.right+1;
		rect.right += 42;

		
		if(m_pNavCar!=NULL)
		{
			RoundRect(hDC,rect.left,rect.top,rect.right,rect.bottom,3,3);
			_sntprintf(lpszState,300,_T("��%4.0f"),
				m_pNavCar->m_gpsDataInfo.m_dbHeigth
				);
			ExtTextOut(hDC, rect.left+4, rect.top+2, 0, NULL, (LPCTSTR)(lpszState), 
			 lstrlen(lpszState),0);
		}

		// ��������
		rect.left = rect.right+1;
		rect.right += 26;

		if(m_pNavCar!=NULL)
		{
			RoundRect(hDC,rect.left,rect.top,rect.right,rect.bottom,3,3);
			_sntprintf(lpszState,300,_T("��%d"),
				m_pNavCar->m_gpsDataInfo.m_nStCount
				);
			ExtTextOut(hDC, rect.left+4, rect.top+2, 0, NULL, (LPCTSTR)(lpszState), 
			 lstrlen(lpszState),0);
		}

		// ����״̬
		rect.left = rect.right+1;
		rect.right += 34;

		RoundRect(hDC,rect.left,rect.top,rect.right,rect.bottom,3,3);

		switch(m_nNetState)
		{
		case NET_BREAK:
			_sntprintf(lpszState,300,_T("����"),
				m_nNetState
				);
			break;
		case NET_CONNECTING:
			_sntprintf(lpszState,300,_T("��.."),
				m_nNetState
				);
			break;
		case NET_CONNECTED:
			_sntprintf(lpszState,300,_T("��.."),
				m_nNetState
				);
			break;
		case NET_LOGINED:
			_sntprintf(lpszState,300,_T("����"),
				m_nNetState
				);
			break;
		case NET_LOGINFEILD:
			_sntprintf(lpszState,300,_T("�ܴ�"),
				m_nNetState
				);
			break;
		}

			ExtTextOut(hDC, rect.left+4, rect.top+2, 0, NULL, (LPCTSTR)(lpszState), 
			 lstrlen(lpszState),0);

		// GPSʱ��
		rect.left = rect.right+1;
		rect.right += 55;
	
		if(m_pNavCar!=NULL)
		{
			RoundRect(hDC,rect.left,rect.top,rect.right,rect.bottom,3,3);
			_sntprintf(lpszState,300,_T("%d:%d:%d"),

				m_pNavCar->m_gpsDataInfo.m_stTime.wHour,
				m_pNavCar->m_gpsDataInfo.m_stTime.wMinute,
				m_pNavCar->m_gpsDataInfo.m_stTime.wSecond

				);
			ExtTextOut(hDC, rect.left+4, rect.top+2, 0, NULL, (LPCTSTR)(lpszState), 
			 lstrlen(lpszState),0);
		}

		// ����״̬
		rect.left = rect.right+1;
		rect.right += 50;
	
		if(m_pNavCar!=NULL)
		{
			RoundRect(hDC,rect.left,rect.top,rect.right,rect.bottom,3,3);
			_sntprintf(lpszState,300,_T("��%0.0f"),
				//m_pNavCar->m_gpsDataInfo.m_stTime.wMinute,
				//m_pNavCar->m_gpsDataInfo.m_stTime.wMinute,
				//m_pNavCar->m_gpsDataInfo.m_stTime.wMinute,
				m_pNavCar->m_gpsDataInfo.m_fAngle

				);
			ExtTextOut(hDC, rect.left+4, rect.top+2, 0, NULL, (LPCTSTR)(lpszState), 
			 lstrlen(lpszState),0);
		}
		SelectObject(hDC,oldBrush);	
		DeleteObject(newBrush);
/*
		MEMORYSTATUS st;
		GlobalMemoryStatus(&st);

		_sntprintf(lpszState,300,_T("�ڴ�:%.2fMb"),st.dwAvailPhys/1024.0/1024.0);

			ExtTextOut(hDC, 5, 65, 0, NULL, lpszState, 
				lstrlen(lpszState),0);	
*/
	SelectObject(hDC,oldPen);
	DeleteObject(newPen);
	SelectObject(hDC,oldFont);
	DeleteObject(newFont);
#endif
}
void CUserLayer::AddGPSPoint(CGpsDataInfo * pPosition,HDC hDC)
{
	ASSERT(pPosition!=NULL);

	typedef map <int, CCar*>::const_iterator CARpos;
	CARpos pos = m_carMap.find(pPosition->m_nCarID);
	CCar* pMyCar = NULL;
	if (pos==m_carMap.end())
	{
		CCar* pCar = new CCar;
		if(pCar==NULL)
		{
			delete pPosition;
			pPosition = NULL;
			return;
		}
		pMyCar = pCar;
		pCar->m_mapCoordMap = m_mapCoordMap;
#ifdef UNDER_WINCE
		pCar->m_ceDraw = m_ceDraw;
#endif
		pCar->m_pSound = m_pSound;
		lstrcpy(pCar->m_szTraceFilePath , m_szTraceFilePath);
		m_carMap[pPosition->m_nCarID] = pCar;
		if(pPosition->m_nCarID==0)
		{
			pCar->m_carType = carMySelf;  //�������IDΪ0����Ϊ�Լ���������
			lstrcpy(pCar->m_szNiceName,TEXT("��"));
			m_pNavCar = pCar;  
		}
		else
		{
			_sntprintf(pCar->m_szNiceName,100,_T("%d"),
			pPosition->m_nCarID
			);
		}
	}
	else
		pMyCar = pos->second;
	
	pMyCar->AddPoint(pPosition,hDC);
}

void CUserLayer::SetNetState(HDC hDC,int nState)
{
	m_nNetState = nState;
	if(m_bShowState)
		DrawState(hDC);
}

void CUserLayer::DrawXX()
{
#ifdef UNDER_WINCE
	if(m_pceBmpCenter==NULL)
	{
		
		TCHAR szFileName[MAX_PATH] = {0};
		GetModuleFileName(NULL,szFileName,MAX_PATH);
		int len = lstrlen(szFileName);
		int nIndex = 0;
		while(len)
		{
			if(szFileName[len-1]==_T('\\'))
			{
				nIndex = len;
				break;
			}
			len--;
		}
		TCHAR szPicPath[MAX_PATH] = {0};
		memcpy(szPicPath,szFileName,len*2);
		lstrcat(szPicPath,TEXT("\\c.dat"));
		
		m_pceBmpCenter = new CCEBitmap;
		m_pceBmpCenter->LoadBitmap( m_ceDraw,szPicPath);//MAKEINTRESOURCE(IDB_BITMAP_CENTER) );
	}
	if(m_pceBmpCenter)
	{
		// �����û�ж�λ������û������Ļ�ϣ�����ʾ��ͼ���α�־
		if(!m_pNavCar)
			m_pceBmpCenter->BitBlt( m_ceDraw, m_mapCoordMap->wScreen/2-15, m_mapCoordMap->hScreen/2-15, 0, 0, 0, 0, SRCCOLORKEY, 0.6f );  // Draw bitmap with alpha blend
		else if(!m_mapCoordMap->IsInScreen(m_pNavCar->m_gpsDataInfo.m_xx,m_pNavCar->m_gpsDataInfo.m_yy))
			m_pceBmpCenter->BitBlt( m_ceDraw, m_mapCoordMap->wScreen/2-15, m_mapCoordMap->hScreen/2-15, 0, 0, 0, 0, SRCCOLORKEY, 0.6f );  // Draw bitmap with alpha blend
			
	}
#endif
}

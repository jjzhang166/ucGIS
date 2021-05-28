#include "StdAfx.h"
#include ".\miftomymap.h"
//#include "MyMapDoc.h"

CMifToMymap::CMifToMymap(void)
: strMifPath(_T(""))
, strMapPath(_T(""))
{
	PointXyz = new mapMAPPOINTSTRUCT[100000];
	posType=-1;posName=-1;posSubType=-1;   // 定义需要的字段的字段位置，以便读取MIF文件

	m_bOneLine = false;
}

CMifToMymap::~CMifToMymap(void)
{
	delete[] PointXyz;
}
short CMifToMymap::GetPen(short pattern,short width)
{
	if (width==0)
		return 5;
	switch(pattern)
	{
		case 0:
			return 5;
			break;
		case 1:
			return 1;
			break;
		case 2:
			return 0;
			break;
		case 22:
			return 4;
			break;
		case 193:
			return 0;
			break;
		default:
			return 1;
			break;
	}	
	return 1;
}

short CMifToMymap::GetWidth(short width)
{
	if (width==0)
		return 0;
	else if(width>=1&&width<=7)
		return  width;
	else
		return (width-10)/10;
}
int	FieldVar(const TCHAR *buf, int fldnum, int sep, TCHAR *dest)
{
	int i;
	const TCHAR *ptr = buf;
	
	for (i=0; i < fldnum; i++)
	{
		if ((ptr = (TCHAR *) wcschr(ptr,sep)))
			ptr++;
		else
			return 0;
	}
	
	for (i=0; (*ptr != sep) && (*ptr) && (i < 4096); i++)
		*dest++ = *ptr++;
	*dest = '\0';
	
	if (*ptr != sep)
		return 0;
	
	return i;
}
void CMifToMymap::ReadMidLine(CLayer* pLayer,CString line,ObjectTypeConstants& nType, ObjectTypeConstants& nSubType,TCHAR* szName)
{
	nType = otNull;
	nSubType = otNull;
	TCHAR* szLine = line.GetBuffer();
	TCHAR szType[100] = {0};
	TCHAR szSubType[100] = {0};
	TCHAR szmyName[255] = {0};

	FieldVar(szLine,posType,',',szType);
	FieldVar(szLine,posSubType,',',szSubType);
	FieldVar(szLine,posName,',',szmyName);
	if((lstrlen(szmyName)*2-4)>0)
		memcpy(szName,szmyName+1,lstrlen(szmyName)*2-4);

	int xxxx = 90;
	// 判断数据类型
	if(lstrcmp(szType,_T("311"))==0)
	{
		pLayer->m_nEyeShotMin = 0;
		pLayer->m_nEyeShotMax = xxxx;

		nType =  otCommunication;
		if(lstrcmp(szSubType,_T("31103"))==0)
			nSubType =  otCommunicationPostOffice;
		else if(lstrcmp(szSubType,_T("31101"))==0)
			nSubType =  otCommunicationTelecom;
	}
	else if(lstrcmp(szType,_T("307"))==0)
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;
		nType =  otEducation;
	}
	else if(lstrcmp(szType,_T("302"))==0)
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;

		nType =  otEnterprise;
	}
	else if(lstrcmp(szType,_T("310"))==0)
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;
		nType =  otFinance;
	}
	else if(lstrcmp(szType,_T("301"))==0)
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;

		nType =  otGOV;
	}	
	else if(lstrcmp(szType,_T("308"))==0)
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;
		nType =  otHospital;
	}
	else if(lstrcmp(szType,_T("303"))==0)
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;
		nType =  otHotel;
	}
	else if(lstrcmp(szType,_T("305"))==0)
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;
		nType =  otMarket;
	}
	else if(lstrcmp(szType,_T("315"))==0)
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;
		nType =  otMedia;
	}
	else if(lstrcmp(szType,_T("314"))==0)
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;
		nType =  otPolice;
	}	
	else if(lstrcmp(szType,_T("315"))==0)
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;
		nType =  otMedia;
	}	
	else if(lstrcmp(szType,_T("319"))==0)
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;
		nType =  otResidential;
	}
	else if(lstrcmp(szType,_T("306"))==0)
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;
		nType =  otRestaurant;
		if(lstrcmp(szSubType,_T("30601"))==0)
			nSubType =  otRestaurantEat;
		else if(lstrcmp(szSubType,_T("30602"))==0)
			nSubType =  otRestaurantTea;
	}
	else if(lstrcmp(szType,_T("317"))==0)
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;
		nType =  otTourSite;
	}
	else if((lstrcmp(szType,_T("309"))==0)||(lstrcmp(szType,_T("318"))==0))
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;

		nType =  otTraffic;
		if(lstrcmp(szSubType,_T("30901"))==0)
			nSubType =  otTrafficPack;
		else if(lstrcmp(szSubType,_T("30903"))==0)
			nSubType =  otTrafficBackShop;
		else if(lstrcmp(szSubType,_T("30902"))==0)
			nSubType =  otTrafficGasStation;
		else if(lstrcmp(szSubType,_T("30604"))==0)
			nSubType =  otTrafficTicketOffice;
		else if(lstrcmp(szSubType,_T("31802"))==0)
			nSubType =  otTrafficStop;
		else if(lstrcmp(szSubType,_T("31803"))==0)
			nSubType =  otTrafficAirport;
		else if(lstrcmp(szSubType,_T("31801"))==0)
			nSubType =  otTrafficRailwayStation;
			
	}	
	else if(lstrcmp(szType,_T("317"))==0)
	{
	pLayer->m_nEyeShotMin = 0;
	pLayer->m_nEyeShotMax = xxxx;
		nType =  otTourSite;
	}
	

	line.ReleaseBuffer();
}
int CMifToMymap:: myatoi(CString str)
{
	TCHAR * ls = str.GetBuffer();
	char ch[50];
	WToA(ls,ch);

	return atoi(ch);
}
float CMifToMymap::myatof(CString str)
{
	TCHAR * ls = str.GetBuffer();
	char ch[50];
	WToA(ls,ch);

	return atof(ch);
}
bool CMifToMymap:: WToA(TCHAR *wChar,char *cChar)
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
void CMifToMymap:: AToW(char *sourceChar,TCHAR *wChar)
{
	char	*p = sourceChar;
	int widecharlen=MultiByteToWideChar( //计算从Ansi转换到Unicode后需要的字节数
		CP_ACP,
		MB_COMPOSITE,
		(char*)p, //要转换的Ansi字符串
		-1, //自动计算长度
		0,
		0
        );

	MultiByteToWideChar( //从Ansi转换到Unicode字符
		CP_ACP,
		MB_COMPOSITE,
		(char*)p,
		-1,
		wChar,	// 转换到 strReceive
		widecharlen // 最多转换widecharlen个Unicode字符
        );

}
// 转化文件
CMyMapDoc* CMifToMymap::TransformMifData(CMyMapDoc *pDoc)
{
	if(m_bOneLine)
		return TransformMifData2(pDoc);
	else
		return TransformMifData1(pDoc);
	
	return NULL;
}
// 转化文件
CMyMapDoc* CMifToMymap::TransformMifData1(CMyMapDoc *pDoc)
{
	//strMifPath = "c:\\四川省-成都市-居民面.MIF";
	//strMapPath = "c:\\xx.qm2";
	strMifPath.Trim();
	strMapPath.Trim();
	if (strMapPath.GetLength()<2)
		return false;

	int PushNumb = 0;

	FILE *pSourFile;			//指向源mif文件
	FILE *pSourFileMid;			//指向源mid文件
	CString sSourLine = TEXT("");		//从源文件中取出的mif一行
	CString sSourLineMid = TEXT("");		//从源文件中取出mid的一行
	char szPath[255] = {0};
	TCHAR* lPath = strMifPath.GetBuffer();
	strMifPath.ReleaseBuffer();
	WToA(lPath,szPath);
	pSourFile = fopen(szPath,"r");
	if(pSourFile==NULL) {
		AfxMessageBox(_T("源文件打开MIF失败！"));
		return false;
	}
	CString sMid = strMifPath.Left(strMifPath.GetLength()-4)+_T(".MID");
	
	TCHAR* lPathMid = sMid.GetBuffer();
	sMid.ReleaseBuffer();
	WToA(lPathMid,szPath);
	pSourFileMid = fopen(szPath,"r");
	if(pSourFileMid==NULL) {
		AfxMessageBox(_T("源文件打开MID失败！"));
		return false;
	}

	strMapPath = _T("c:\\")+strMifPath.Left(strMifPath.GetLength()-4) +_T(".qm2");
	//CMyMapDoc *pDoc = new CMyMapDoc();
	// 创建一个图层
	CLayer* pLayer = new CLayer();
	ASSERT(pLayer!=NULL);
	pLayer->m_bAutoLabel = true;
	CString name = strMifPath.Left(strMifPath.GetLength()-4).Mid(strMifPath.Find(_T("/")));
	lstrcpy(pLayer->m_pszName, name.GetBuffer());
	name.ReleaseBuffer();
	short m_Layer = pDoc->AddLayer(pLayer);

	ObjectTypeConstants nType = otNull, nSubType = otNull;
	CString strFileName = strMifPath.Left(strMifPath.GetLength()-4);
	
	if(strFileName==_T("Buildings"))
		nType = otBuilding;
	else if(strFileName==_T("Communication"))
		nType = otCommunication;
	else if(strFileName==_T("Education"))
		nType = otEducation;
	else if(strFileName==_T("Enterprise"))
		nType = otEnterprise;
	else if(strFileName==_T("Finance"))
		nType = otFinance;
	else if(strFileName==_T("GOV"))
		nType = otGOV;
	else if(strFileName==_T("Hospital"))
		nType = otHospital;
	else if(strFileName==_T("Hotel"))
		nType = otHotel;
	else if(strFileName==_T("Market"))
		nType = otMarket;
	else if(strFileName==_T("Media"))
		nType = otMedia;
	else if(strFileName==_T("Police"))
		nType = otPolice;
	else if(strFileName==_T("Publicsite"))
		nType = otPublic;
	else if(strFileName==_T("Residential"))
		nType = otResidential;
	else if(strFileName==_T("Restaurant"))
		nType = otRestaurant;
	else if((strFileName==_T("Traffic"))||(strFileName==_T("Traffic_center")))
		nType = otTraffic;

	while(!feof(pSourFile)){
		ReadLine(pSourFile, sSourLine);//读一行

		CString s = sSourLine;
		// 读标结构
		if (sSourLine.Find(_T("Columns "),0)==0)
		{
			int nColumnCount = myatoi(sSourLine.Mid(sSourLine.Find(_T(" "))));
			for(int c=0;c<nColumnCount;c++)
			{
				ReadLine(pSourFile, sSourLine);//读一行
				sSourLine.MakeUpper();
				if(sSourLine.Find(_T(" TYPE"),0)>=0)
					posType = c;
				else if(sSourLine.Find(_T(" NAME"),0)>=0)
					posName = c;
				else if(sSourLine.Find(_T(" SUBTYPE"),0)>=0)
				    posSubType = c;
			}
		}
		// 读多边形区域
		if (sSourLine.Find(_T("Region "),0)==0)
		{
			int nRegionCount = myatoi(sSourLine.Mid(sSourLine.Find(_T(" "))));

			int* mulCout;
			mulCout = new int[nRegionCount];
			ASSERT(mulCout!=NULL);
			mapMAPPOINTSTRUCT** ppPoints; 
			ppPoints = new mapMAPPOINTSTRUCT*[nRegionCount];
			if(nRegionCount>1) 
				int dd=3;
			for(int i=0;i<nRegionCount;i++)
			{
				ReadLine(pSourFile, sSourLine);
				int cout = myatoi(sSourLine);
				mulCout[i] = cout;
				ppPoints[i] =  new mapMAPPOINTSTRUCT[mulCout[i]];
				for(int j=0;j<cout;j++)
				{
					ReadLine(pSourFile, sSourLine);

					CString sLongitude = sSourLine.Left(sSourLine.Find(_T(" ")));
					CString sLatitude = sSourLine.Mid((sSourLine.Find(_T(" "))));
					float longitude = myatof(sLongitude);
					float latitude =  myatof(sLatitude);

					ppPoints[i][j].x = longitude;
					ppPoints[i][j].y = latitude;

				}
			}
			int id_only = 0;//pDoc->GetGraphID(2);
			
			// 转换画笔和刷子
			mapBRUSHSTRUCT brush;
			mapPENSTRUCT pen;			
			// 读取Pen
			ReadLine(pSourFile, sSourLine);
			GetPen(sSourLine,pen);
			// 读取Brush
			ReadLine(pSourFile, sSourLine);
			GetBrush(sSourLine,brush);
			// Center
			ReadLine(pSourFile, sSourLine);

			// 读取标注
			ReadLine(pSourFileMid, sSourLineMid);
			TCHAR ch[255] = {0};
			ReadMidLine(pLayer,sSourLineMid,nType , nSubType ,ch);

			for(int i=0;i<nRegionCount;i++)
			{
				pDoc->AddPLineRgn(ch,m_Layer,nType,nSubType,mulCout[i],ppPoints[i],pen,1,1,brush);
				delete[] ppPoints[i];
			}
			delete ppPoints;
			delete[] mulCout;
			PushNumb = 0;
		}

		// 读分段曲线
		if (sSourLine.Find(_T("Pline Multiple "),0)==0)
		{
			//ReadLine(pSourFile, sSourLine);
			PushNumb = 0;
			int cout = myatoi(sSourLine.Mid(sSourLine.Find(_T("le "))+2));
			int* mulCout;
			mulCout = new int[cout];
			ASSERT(mulCout!=NULL);
			mapMAPPOINTSTRUCT** ppPoints; 
			ppPoints = new mapMAPPOINTSTRUCT*[cout];

			for(int i=0;i<cout;i++)
			{
				ReadLine(pSourFile, sSourLine);
				if (sSourLine.Find(_T("  "),0)==0)
				{
					mulCout[i] = myatoi(sSourLine);
					ppPoints[i] =  new mapMAPPOINTSTRUCT[mulCout[i]];
					for(int j=0;j<mulCout[i];j++)
					{
						ReadLine(pSourFile, sSourLine);

						CString sLongitude = sSourLine.Left(sSourLine.Find(_T(" ")));
						CString sLatitude = sSourLine.Mid((sSourLine.Find(_T(" "))));
						float longitude = myatof(sLongitude);
						float latitude =  myatof(sLatitude) ;
						
						ppPoints[i][j].x = longitude;
						ppPoints[i][j].y = latitude;
					}
				}
			}

			int id_only = 0;//pDoc->GetGraphID(2);
			
			ReadLine(pSourFile, sSourLine);
			mapPENSTRUCT pen;
			GetPen(sSourLine,pen);
			// 读取标注
			ReadLine(pSourFileMid, sSourLineMid);
			TCHAR ch[255] = {0};
			ReadMidLine(pLayer,sSourLineMid,nType , nSubType ,ch);
			for(int i=0;i<cout;i++)
			{
				pDoc->AddPLine(ch,m_Layer,nType,nSubType,mulCout[i],ppPoints[i],pen);//->Draw(&ddd,0,0,m_bColor);
				delete ppPoints[i];
			}
			delete ppPoints;
			delete mulCout;
			PushNumb = 0;
		}
	
		// 读曲线
		if (sSourLine.Find(_T("Pline "),0)==0)
		{
			PushNumb = 0;

			int cout = myatoi(sSourLine.Mid(sSourLine.Find(_T(" "))));

			for(int i=0;i<cout;i++)
			{
				ReadLine(pSourFile, sSourLine);

				CString sLongitude = sSourLine.Left(sSourLine.Find(_T(" ")));
				CString sLatitude = sSourLine.Mid((sSourLine.Find(_T(" "))));
				float longitude = myatof(sLongitude);
				float latitude =  myatof(sLatitude);

				PointXyz[i].x = longitude;
				PointXyz[i].y = latitude;
				PushNumb++;
			}

			int id_only = 0;//pDoc->GetGraphID(2);
			ReadLine(pSourFile, sSourLine);
			mapPENSTRUCT pen;
			GetPen(sSourLine,pen);
			// 读取标注
			ReadLine(pSourFileMid, sSourLineMid);
			TCHAR ch[255] = {0};
			ReadMidLine(pLayer,sSourLineMid,nType , nSubType ,ch);
			(pDoc->AddPLine(ch,m_Layer,nType,nSubType,PushNumb,PointXyz,pen))->SetDrawType(false);//->Draw(&ddd,0,0,m_bColor);			
			PushNumb = 0;
		}
		
		// 读点目标
		if (sSourLine.Find(_T("Point "),0)==0)
		{
			CString sTemp = sSourLine.Mid(6);
			CString sLongitude = sTemp.Left(sTemp.Find(_T(" ")));
			CString sLatitude = sTemp.Mid((sTemp.Find(_T(" "))));
			double longitude = myatof(sLongitude);
			double latitude =  myatof(sLatitude);
			// 读取标注
			ReadLine(pSourFileMid, sSourLineMid);
			TCHAR ch[255] = {0};
			ReadMidLine(pLayer,sSourLineMid,nType , nSubType ,ch);
			CString str = sSourLineMid.Mid(1);
			sSourLineMid = str.Mid(0,str.Find(_T("\"")));	
			
			ReadLine(pSourFile, sSourLine);

			// Symbol (53,16754768,5,"Map Symbols",16,0) 
			mapSYMBOLSTRUCT symbol;
			GetSymbol(sSourLine,symbol);
	
			pDoc->AddPoint(ch,m_Layer,nType,nSubType,longitude,latitude);//->Draw(&ddd,0,0,m_bColor);

		}
	}	

	lstrcpy(pDoc->m_pszMapFilePath , strMapPath.GetBuffer());
//	lstrcpy(pDoc->m_lpszMapName , _T("未命名"));
	strMapPath.ReleaseBuffer();
	//pDoc->WriteToFile();
	return pDoc;
}
// 一条线 转化文件
CMyMapDoc* CMifToMymap::TransformMifData2(CMyMapDoc *pDoc)
{
	//strMifPath = "c:\\四川省-成都市-居民面.MIF";
	//strMapPath = "c:\\xx.qm2";
	strMifPath.Trim();
	strMapPath.Trim();
	if (strMapPath.GetLength()<2)
		return false;

	int PushNumb = 0;

	FILE *pSourFile;			//指向源mif文件
	FILE *pSourFileMid;			//指向源mid文件
	CString sSourLine = TEXT("");		//从源文件中取出的mif一行
	CString sSourLineMid = TEXT("");		//从源文件中取出mid的一行
	char szPath[255] = {0};
	TCHAR* lPath = strMifPath.GetBuffer();
	strMifPath.ReleaseBuffer();
	WToA(lPath,szPath);
	pSourFile = fopen(szPath,"r");
	if(pSourFile==NULL) {
		AfxMessageBox(_T("源文件打开MIF失败！"));
		return false;
	}
	CString sMid = strMifPath.Left(strMifPath.GetLength()-4)+_T(".MID");
	
	TCHAR* lPathMid = sMid.GetBuffer();
	sMid.ReleaseBuffer();
	WToA(lPathMid,szPath);
	pSourFileMid = fopen(szPath,"r");
	if(pSourFileMid==NULL) {
		AfxMessageBox(_T("源文件打开MID失败！"));
		return false;
	}

	strMapPath = _T("c:\\")+strMifPath.Left(strMifPath.GetLength()-4) +_T(".qm2");
	//CMyMapDoc *pDoc = new CMyMapDoc();
	// 创建一个图层
	CLayer* pLayer = new CLayer();
	ASSERT(pLayer!=NULL);
	pLayer->m_bAutoLabel = true;
	CString name = strMifPath.Left(strMifPath.GetLength()-4).Mid(strMifPath.Find(_T("/")));
	lstrcpy(pLayer->m_pszName, name.GetBuffer());
	name.ReleaseBuffer();
	short m_Layer = pDoc->AddLayer(pLayer);

	ObjectTypeConstants nType = otNull, nSubType = otNull;
	CString strFileName = strMifPath.Left(strMifPath.GetLength()-4);
	
	if(strFileName==_T("Buildings"))
		nType = otBuilding;
	else if(strFileName==_T("Communication"))
		nType = otCommunication;
	else if(strFileName==_T("Education"))
		nType = otEducation;
	else if(strFileName==_T("Enterprise"))
		nType = otEnterprise;
	else if(strFileName==_T("Finance"))
		nType = otFinance;
	else if(strFileName==_T("GOV"))
		nType = otGOV;
	else if(strFileName==_T("Hospital"))
		nType = otHospital;
	else if(strFileName==_T("Hotel"))
		nType = otHotel;
	else if(strFileName==_T("Market"))
		nType = otMarket;
	else if(strFileName==_T("Media"))
		nType = otMedia;
	else if(strFileName==_T("Police"))
		nType = otPolice;
	else if(strFileName==_T("Publicsite"))
		nType = otPublic;
	else if(strFileName==_T("Residential"))
		nType = otResidential;
	else if(strFileName==_T("Restaurant"))
		nType = otRestaurant;
	else if((strFileName==_T("Traffic"))||(strFileName==_T("Traffic_center")))
		nType = otTraffic;


	nType = otTraffic;

	int nTotalCount = 0;
	while(!feof(pSourFile)){
		ReadLine(pSourFile, sSourLine);//读一行

		CString s = sSourLine;
		// 读标结构
		if (sSourLine.Find(_T("Columns "),0)==0)
		{
			int nColumnCount = myatoi(sSourLine.Mid(sSourLine.Find(_T(" "))));
			for(int c=0;c<nColumnCount;c++)
			{
				ReadLine(pSourFile, sSourLine);//读一行
				sSourLine.MakeUpper();
				if(sSourLine.Find(_T(" TYPE"),0)>=0)
					posType = c;
				else if(sSourLine.Find(_T(" NAME"),0)>=0)
					posName = c;
				else if(sSourLine.Find(_T(" SUBTYPE"),0)>=0)
				    posSubType = c;
			}
		}
		// 读曲线
		if (sSourLine.Find(_T("Pline "),0)==0)
		{
			PushNumb = 0;

			int cout = myatoi(sSourLine.Mid(sSourLine.Find(_T(" "))));

			for(int i=0;i<cout;i++)
			{
				ReadLine(pSourFile, sSourLine);

				CString sLongitude = sSourLine.Left(sSourLine.Find(_T(" ")));
				CString sLatitude = sSourLine.Mid((sSourLine.Find(_T(" "))));
				float longitude = myatof(sLongitude);
				float latitude =  myatof(sLatitude);

				PointXyz[i+nTotalCount].x = longitude;
				PointXyz[i+nTotalCount].y = latitude;
				PushNumb++;
			}

			nTotalCount += cout;
			int id_only = 0;//pDoc->GetGraphID(2);
			ReadLine(pSourFile, sSourLine);
			mapPENSTRUCT pen;
			GetPen(sSourLine,pen);
			// 读取标注
			ReadLine(pSourFileMid, sSourLineMid);
		
			PushNumb = 0;
		}
				// 读点目标
		if (sSourLine.Find(_T("Point "),0)==0)
		{
			CString sTemp = sSourLine.Mid(6);
			CString sLongitude = sTemp.Left(sTemp.Find(_T(" ")));
			CString sLatitude = sTemp.Mid((sTemp.Find(_T(" "))));
			double longitude = myatof(sLongitude);
			double latitude =  myatof(sLatitude);
			// 读取标注
			ReadLine(pSourFileMid, sSourLineMid);
			TCHAR ch[255] = {0};
			ReadMidLine(pLayer,sSourLineMid,nType , nSubType ,ch);
			CString str = sSourLineMid.Mid(1);
			sSourLineMid = str.Mid(0,str.Find(_T("\"")));	
			
			ReadLine(pSourFile, sSourLine);

			// Symbol (53,16754768,5,"Map Symbols",16,0) 
			mapSYMBOLSTRUCT symbol;
			GetSymbol(sSourLine,symbol);
	
			pDoc->AddPoint(ch,m_Layer,otTraffic,otTrafficBusStop,longitude,latitude);//->Draw(&ddd,0,0,m_bColor);

		}
	}	
	if(nTotalCount>0)
	{
	TCHAR ch[255] = {0};
	ReadMidLine(pLayer,sSourLineMid,nType , nSubType ,ch);
	mapPENSTRUCT pen;
	pen.color = RGB(255,0,0);
	pen.pattern = 1;
	pen.width = 2;
	(pDoc->AddPLine(ch,m_Layer,nType,nSubType,nTotalCount,PointXyz,pen))->SetDrawType(false);//->Draw(&ddd,0,0,m_bColor);	
	}

	lstrcpy(pDoc->m_pszMapFilePath , strMapPath.GetBuffer());
//	lstrcpy(pDoc->m_lpszMapName , _T("未命名"));
	strMapPath.ReleaseBuffer();
	//pDoc->WriteToFile();
	return pDoc;
}
void CMifToMymap::ReadLine(FILE* pFile, CString& line)
{
	line="";
	char ch;
	int pos=0;
	char szLine[600] = {0};
	while(!feof(pFile)){
		//Read(&ch,1);
		ch = fgetc(pFile);
		if(ch=='\r'||ch=='\n')break;
		szLine[pos]=ch;
		pos++;
		if(line.Find(_T(""), 0))
		{
			ftell(pFile);
		}
	}
	TCHAR lLine[600] = {0};
	AToW(szLine,lLine);
	line.Format(_T("%s"),lLine);
	line.TrimRight('\r'); 
	line.TrimRight('\n'); 
	line.TrimRight('\r'); 
	line.TrimRight(0xff); //过滤文件结束符
}
void CMifToMymap::GetPen(CString sSourLine, mapPENSTRUCT& pen)
{
	CString width = sSourLine.Mid(sSourLine.Find(_T("("))+1,sSourLine.Find(_T(","))-sSourLine.Find(_T("("))-1);//s.Substring(s.IndexOf("(")+1,(s.IndexOf(",")-s.IndexOf("("))-1);
	CString stmp = sSourLine.Mid(sSourLine.Find(_T(","))+1,sSourLine.GetLength()-sSourLine.Find(_T(","))-1);//s.Substring(s.IndexOf(",")+1,s.Length - s.IndexOf(",")-1);
	CString pattern = stmp.Left(stmp.Find(_T(",")));//Substring(0,stmp.IndexOf(","));
	CString color = stmp.Mid(stmp.Find(_T(","))+1,stmp.GetLength()-stmp.Find(_T(","))-2);//stmp.Substring(stmp.IndexOf(",")+1,stmp.Length-stmp.IndexOf(",")-3);

	short penWidth = myatoi(width);
	short sPattern =  myatoi(pattern);

	pen.color = GetColorFromString(color);
	pen.width = GetWidth(penWidth);
	pen.pattern = GetPen(sPattern,penWidth);
}
void CMifToMymap::GetBrush(CString sSourLine, mapBRUSHSTRUCT& brush)
{
	CString pattern = sSourLine.Mid(sSourLine.Find(_T("("))+1,sSourLine.Find(_T(","))-sSourLine.Find(_T("("))-1); //s.Substring(s.IndexOf("(")+1,(s.IndexOf(",")-s.IndexOf("("))-1);
	CString stmp1 =sSourLine.Mid(sSourLine.Find(_T(","))+1,sSourLine.GetLength()-sSourLine.Find(_T(","))-1);// s.Substring(s.IndexOf(",")+1,s.Length - s.IndexOf(",")-1);
	CString sforecolor = stmp1.Left(stmp1.Find(_T(",")));//stmp.Substring(0,stmp.IndexOf(","));
	CString sbackcolor = stmp1.Mid(stmp1.Find(_T(","))+1,stmp1.GetLength()-stmp1.Find(_T(","))-2);//stmp.Substring(stmp.IndexOf(",")+1,stmp.Length-stmp.IndexOf(",")-2)
	
	short sPattern = myatoi(pattern);

	brush.backcolor = GetColorFromString(sbackcolor);

	brush.forecolor = GetColorFromString(sforecolor);
	brush.pattern = sPattern;
}
// 从图标字符串解析得图标类型
void CMifToMymap::GetSymbol(CString str , mapSYMBOLSTRUCT& symbol)
{
	// Symbol (53,16754768,5,"Map Symbols",16,0) 
	// Symbol ("BANK1-32.BMP",6356832,13,0) 
	CString strTmp = str.Mid(str.Find(_T(" ("))+2);
	if (strTmp.Left(1)!=_T("\""))
	{
		CString  shape = strTmp.Left(strTmp.Find(_T(",")));
		strTmp = strTmp.Mid(strTmp.Find(_T(","))+1);
		CString color =  strTmp.Left(strTmp.Find(_T(",")));

		strTmp = strTmp.Mid(strTmp.Find(_T(","))+1);
		CString size =  strTmp.Left(strTmp.Find(_T(",")));

		strTmp = strTmp.Mid(strTmp.Find(_T(","))+1);
		CString fontname =  strTmp.Left(strTmp.Find(_T(",")));
		fontname = fontname.Mid(1,fontname.GetLength()-2);
		// 如果是SYMBOL (shape, color, size)
		if(fontname.GetLength()==0)
		{
			symbol.isBmp = false;
			symbol.shape = MapInfo3ToMapInfoSymbol(myatoi(shape));
			symbol.color = GetColorFromString(color);
			symbol.size = 12;//atoi(size);
			lstrcpy(symbol.fontname , _T("MapInfo Symbols"));
			symbol.fontstyle = 0;
			symbol.rotation = 0;
		}
		else
		{
			strTmp = strTmp.Mid(strTmp.Find(_T(","))+1);
			CString fontstyle =  strTmp.Left(strTmp.Find(_T(",")));

			strTmp = strTmp.Mid(strTmp.Find(_T(","))+1);
			CString rotation =  strTmp.Left(strTmp.Find(_T(")")));

			symbol.isBmp = false;
			symbol.shape = myatoi(shape);
			symbol.color = GetColorFromString(color);
			symbol.size = myatoi(size);
			lstrcpy(symbol.fontname , fontname);
			symbol.fontstyle = myatoi(fontstyle);
			symbol.rotation = myatoi(rotation);
		}
	}
	else
	{
		CString filename = strTmp.Left(strTmp.Find(_T(",")));
		filename = filename.Mid(1,filename.GetLength()-2);

		strTmp = strTmp.Mid(strTmp.Find(_T(","))+1);
		CString color =  strTmp.Left(strTmp.Find(_T(",")));

		strTmp = strTmp.Mid(strTmp.Find(_T(","))+1);
		CString size =  strTmp.Left(strTmp.Find(_T(",")));

		strTmp = strTmp.Mid(strTmp.Find(_T(","))+1);
		CString customstyle =  strTmp.Left(strTmp.Find(_T(")")));
		symbol.isBmp = true;
		symbol.size = myatoi(size);
		symbol.color = GetColorFromString(color);
		symbol.customstyle = myatoi(customstyle);
		lstrcpy(symbol.filename , filename);

	}
}
// MapINfo3的图标转化为字体
int CMifToMymap::MapInfo3ToMapInfoSymbol(short nShape)
{
	return nShape+1;
}
COLORREF CMifToMymap::GetColorFromString(CString str)
{
	int x = myatoi(str);
	
	char buf[100];
	
	sprintf (buf,"%X",x);
	CString s = (CString)buf;
	int d=0;
	if(s.GetLength()==1)
		s = _T("00000")+s;
	if(s.GetLength()==2)
		s = _T("0000")+s;
	if(s.GetLength()==3)
		s = _T("000")+s;
	if(s.GetLength()==4)
		s = _T("00")+s;
	if(s.GetLength()==5)
		s = _T("0")+s;

	CString sr = s.Left(2);
	CString sg = s.Mid(2,2);
	CString sb = s.Right(2);
	TCHAR* r = sr.GetBuffer();
	TCHAR* g = sg.GetBuffer();
	TCHAR* b = sb.GetBuffer();
	sr.ReleaseBuffer();
	sg.ReleaseBuffer();
	sb.ReleaseBuffer();
	int iR;int iG;int iB;
	char rr[100],gg[100],bb[100];
	WToA(r,rr);
	WToA(g,gg);
	WToA(b,bb);
	
	sscanf(rr,"%X",&iR);
	sscanf(gg,"%X",&iG);
	sscanf(bb,"%X",&iB);

	long l = RGB(iR,iG,iB);
	//long l = RGB(0,255,0);

	return l;
}

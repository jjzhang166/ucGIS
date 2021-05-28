#pragma once
#include "Draw.h"
#include "MyMapDoc.h"
class AFX_EXT_CLASS CMifToMymap
{
public:
	CMifToMymap(void);
	~CMifToMymap(void);
	// mif文件的路径
	CString strMifPath;
	// 转化后文件的存放路径
	CString strMapPath;
	// 转化文件
	CMyMapDoc* TransformMifData(CMyMapDoc *pDoc);
	CMyMapDoc* TransformMifData1(CMyMapDoc *pDoc);
	CMyMapDoc* TransformMifData2(CMyMapDoc *pDoc);
	mapMAPPOINTSTRUCT *PointXyz;

	bool m_bOneLine;  // 把所有线段转化成一条线吗

private:
	void GetSymbol(CString str , mapSYMBOLSTRUCT& symbol); // 把字符串转化为图形点结构
	int  MapInfo3ToMapInfoSymbol(short nShape);         // MapINfo3的图标转化为字体
	COLORREF GetColorFromString(CString str);           // 提取色彩字符串
	void ReadLine(FILE* pFile, CString& line);
	void GetPen(CString sSourLine, mapPENSTRUCT& pen);
	void GetBrush(CString sSourLine, mapBRUSHSTRUCT& brush);  

	short GetWidth(short width);    // 从MapInfo格式的线宽，转化成普通宽度
	short GetPen(short pattern,short width);   // 从MapInfo格式的线类型转化为GDI类型
	void ReadMidLine(CLayer* pLayer,CString line,ObjectTypeConstants& nType, ObjectTypeConstants& nSubType,TCHAR* szName); //读MID，获得类型，名称和子类型

	int posType,posName,posSubType;   // 定义需要的字段的字段位置，以便读取MIF文件


	int  myatoi(CString str);

	float myatof(CString str);
	bool  WToA(TCHAR *wChar,char *cChar);
	void  AToW(char *sourceChar,TCHAR *wChar);
};

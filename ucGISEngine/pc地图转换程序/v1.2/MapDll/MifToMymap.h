#pragma once
#include "Draw.h"
#include "MyMapDoc.h"
class AFX_EXT_CLASS CMifToMymap
{
public:
	CMifToMymap(void);
	~CMifToMymap(void);
	// mif�ļ���·��
	CString strMifPath;
	// ת�����ļ��Ĵ��·��
	CString strMapPath;
	// ת���ļ�
	CMyMapDoc* TransformMifData(CMyMapDoc *pDoc);
	CMyMapDoc* TransformMifData1(CMyMapDoc *pDoc);
	CMyMapDoc* TransformMifData2(CMyMapDoc *pDoc);
	mapMAPPOINTSTRUCT *PointXyz;

	bool m_bOneLine;  // �������߶�ת����һ������

private:
	void GetSymbol(CString str , mapSYMBOLSTRUCT& symbol); // ���ַ���ת��Ϊͼ�ε�ṹ
	int  MapInfo3ToMapInfoSymbol(short nShape);         // MapINfo3��ͼ��ת��Ϊ����
	COLORREF GetColorFromString(CString str);           // ��ȡɫ���ַ���
	void ReadLine(FILE* pFile, CString& line);
	void GetPen(CString sSourLine, mapPENSTRUCT& pen);
	void GetBrush(CString sSourLine, mapBRUSHSTRUCT& brush);  

	short GetWidth(short width);    // ��MapInfo��ʽ���߿�ת������ͨ���
	short GetPen(short pattern,short width);   // ��MapInfo��ʽ��������ת��ΪGDI����
	void ReadMidLine(CLayer* pLayer,CString line,ObjectTypeConstants& nType, ObjectTypeConstants& nSubType,TCHAR* szName); //��MID��������ͣ����ƺ�������

	int posType,posName,posSubType;   // ������Ҫ���ֶε��ֶ�λ�ã��Ա��ȡMIF�ļ�


	int  myatoi(CString str);

	float myatof(CString str);
	bool  WToA(TCHAR *wChar,char *cChar);
	void  AToW(char *sourceChar,TCHAR *wChar);
};

// MapToolDoc.h : CMapToolDoc ��Ľӿ�
//


#pragma once


class CMapToolDoc : public CDocument
{
protected: // �������л�����
	CMapToolDoc();
	DECLARE_DYNCREATE(CMapToolDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CMapToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};



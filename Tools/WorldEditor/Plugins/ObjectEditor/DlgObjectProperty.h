#pragma once
#include "resource.h"

using namespace Ogre;
typedef std::list<PropertyContainer*> ListPropertyContainer;

// ���Ա༭��
class DlgObjectProperty : public CDialog
{
	DECLARE_DYNAMIC(DlgObjectProperty)

public:
	DlgObjectProperty(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgObjectProperty();

// Dialog Data
	enum { IDD = IDD_DIALOG_OBJECT_PROPERTY };


	// ˢ������
	void refreshProperty(PropertyContainer *obj);
	// ˢ������
	void refreshProperty(const ListPropertyContainer &obj);
	// ˢ������
	void refreshProperty();
	// �������
	void clearProperty();
protected:
	// �������
	void addProperty(PropertyContainer *obj , CXTPPropertyGridItem *parent , const PropertyContainer::VTProperty *properties);
	// �������
	void fillProperty(PropertyContainer *obj , CXTPPropertyGridItem *item , const PropertyContainer::Property &property);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	CXTPPropertyGrid m_wndProperty;
	typedef std::map<uint , CXTPPropertyGridItem *> MapGridItem;
	// �����洢����ID��CXTPPropertyGridItem�Ĺ���
	MapGridItem m_mapGridItem;
	// ���Զ���
	ListPropertyContainer m_listObject;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// ��ֹ��Enter��Esc�Ѵ��ڹر���
	virtual void OnOK(){};
	virtual void OnCancel(){};

};

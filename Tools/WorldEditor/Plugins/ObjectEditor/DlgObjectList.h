#pragma once
#include "resource.h"


// DlgObjectList dialog

class DlgObjectList : public CDialog
	, public Ogre::IMapManager::Listener
{
	DECLARE_DYNAMIC(DlgObjectList)

public:
	DlgObjectList(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgObjectList();

// Dialog Data
	enum { IDD = IDD_DIALOG_OBJECT_LIST };
	// ��ֹ��Enter��Esc�Ѵ��ڹر���
	virtual void OnOK(){};
	virtual void OnCancel(){};

	// ������������б�
	void reloadList();
	// ѡ��һ����������
	virtual void onSelectObject(Ogre::ISceneObject *obj);
	// ѡ��һ����������
	virtual void onSelectObject(const Ogre::IMapManager::ListSceneObject &listSceneObject);
	// ��ѡһ����������
	virtual void onDeselectObject(Ogre::ISceneObject *obj);
	// ��ѡһ����������
	virtual void onDeselectObject(const Ogre::IMapManager::ListSceneObject &listSceneObject);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CListCtrl m_list;

	enum ListState
	{
		LS_Free,		// ����״̬
		LS_Processing,	// ���ڴ���״̬
		LS_Selecting,	// ѡ����״̬
	};
	ListState m_listState;
public:
	afx_msg void OnBnClickedButtonRefresh();
	virtual BOOL OnInitDialog();
//	afx_msg void OnHdnItemchangedObjectList(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnHdnItemchangingObjectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangingObjectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedObjectList(NMHDR *pNMHDR, LRESULT *pResult);
};

#pragma once
#include "FileTreeCtrl.h"
#include "resource.h"

// DlgPlaceSceneObject dialog

class DlgPlaceSceneObject : public CDialog , public CFileTreeCtrl::LoadTreeCallback
{
	DECLARE_DYNAMIC(DlgPlaceSceneObject)

public:
	DlgPlaceSceneObject(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgPlaceSceneObject();

// Dialog Data
	enum { IDD = IDD_DIALOG_TREE };
	// ����ص�
	virtual void loadFile(HTREEITEM hItem , CFileFind &file);

	// ����������
	void reloadTree();
protected:

	enum LoadingType
	{
		LT_NONE,		// û����������
		LT_MODEL,		// ��������ģ��
		LT_PARTICLE,	// ������������Ч��
		LT_SOUND,		// ������������Ч��
		LT_SPECIAL,		// �������
	};

	LoadingType m_loadingType;

	void skipToNextCloseBrace(Ogre::DataStreamPtr& stream);
	void skipToNextOpenBrace(Ogre::DataStreamPtr& stream);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CFileTreeCtrl m_fileTree;
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedFiletree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickFiletree(NMHDR *pNMHDR, LRESULT *pResult);

	// ѡ��һ��ģ��
	void selectItem(HTREEITEM hItem);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// ��ֹ��Enter��Esc�Ѵ��ڹر���
	virtual void OnOK(){};
	virtual void OnCancel(){};

};

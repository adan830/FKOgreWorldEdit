#pragma once


// CFileTreeCtrl

class CFileTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CFileTreeCtrl)

public:
	CFileTreeCtrl();
	virtual ~CFileTreeCtrl();

	struct LoadTreeCallback
	{
		virtual void loadFile(HTREEITEM hItem , CFileFind &file) = 0;
	};
	// ��ʼ��Ŀ¼��
	size_t loadTree(HTREEITEM hParent , const CString &strRootPath , const CString &strFileFilter , bool deleteNullDir , LoadTreeCallback *callback);

protected:
	DECLARE_MESSAGE_MAP()
};



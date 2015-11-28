// ============================================================
// �ļ���	:	MainFrm.h
// ������	:	������
// ˵��		:	������ʵ��
// ����ʱ��	:	2008-1-23 8:34:45
// ============================================================
#pragma once
#include "IMainFrame.h"
#include "TListener.h"

namespace Ogre
{
	class WorldEditor;
};
class CChildView;

using namespace Ogre;

class CMainFrame : public CXTPFrameWnd
	, public IMainFrame
	, public TListener<IMainFrame::Listener>
{

public:
	CMainFrame();
protected:
	DECLARE_DYNAMIC(CMainFrame)

	// Attributes
public:

	// Operations
public:

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	// Implementation
public:


	// ���������������
	virtual void addListener(Listener *listener , bool toBack = true)
	{
		TListener<IMainFrame::Listener>::addListener(listener , toBack);
	}
	// ɾ��������������
	virtual void removeListener(Listener *listener)
	{
		TListener<IMainFrame::Listener>::removeListener(listener);
	}

	// ����Ӵ���������
	virtual void addChildViewListener(IChildView::Listener *listener , bool toBack = true)
	{
		ListChildViewListener::iterator iter = m_listChildViewListener.find(listener);
		if(iter != m_listChildViewListener.end())
		{
			m_listChildViewListener.erase(iter);
		}
		m_listChildViewListener[listener] = toBack;
		for(ListChildView::iterator iter = m_ListChildView.begin() ; iter != m_ListChildView.end() ; iter ++)
		{
			(*iter)->addListener(listener , toBack);
		}

	}
	// ɾ���Ӵ���������
	virtual void removeChildViewListener(IChildView::Listener *listener)
	{
		ListChildViewListener::iterator iter = m_listChildViewListener.find(listener);
		if(iter != m_listChildViewListener.end())
		{
			m_listChildViewListener.erase(iter);
		}
		for(ListChildView::iterator iter = m_ListChildView.begin() ; iter != m_ListChildView.end() ; iter ++)
		{
			(*iter)->removeListener(listener);
		}

	}

	// ��������������İ�ť
	virtual int createPluginToolBarButton(const String &strName , IPlugin *plugin);
	// ���ٲ���������İ�ť
	virtual void destroyPluginToolBarButton(const String &strName);

	// ��ô���ָ��
	virtual CWnd* getWnd();
	// ����һ�������
	virtual Camera *createCamera(const String &name);
	// ���һ�������
	virtual Camera *createCamera(CAMERA_TYPE cameraType);
	// ��������ͼ
	virtual IChildView* createChildView(int row , int col , const String &name , Camera *camera);
	// ��������ͼ
	virtual IChildView* createChildView(int row , int col , const String &name , CAMERA_TYPE cameraType);

	// ��ס���������
	virtual void lockMouse(bool bLock);
	// ����Ƿ��Ѿ���ס��
	virtual bool isMouseLocked();
	// ��֤�������������ʱ��λ��
	virtual void keepLockMouse();
	// ����Ӵ����б�
	virtual ChildViewIterator getChildViewIterator();

	// �������bars�������������������˵��Ľӿ�
	virtual CXTPCommandBars* getCommandBars();
	// ���״̬���ӿ�
	virtual CXTPStatusBar* getStatusBar();
	// ��ø������ڹ�����
	virtual CXTPDockingPaneManager* getDockingPaneManager();

	// �� �� �� : createPane
	// ע    �� : ����һ����������
	// �������� : virtual CXTPDockingPane* 
	// ��    �� : UINT nID									������ԴID
	// ��    �� : CRect rc									����
	// ��    �� : XTPDockingPaneDirection direction			��������
	// ��    �� : CXTPDockingPaneBase* pNeighbour = NULL	�����Ĵ���
	virtual CXTPDockingPane* createPane(UINT nID, const String &title , CRect rc, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour = NULL);
	// ���ٸ�������
	virtual void destroyPane(UINT nID);
	// �� �� �� : createSubMenu
	// ע    �� : ����һ���Ӳ˵�
	// �������� : virtual CXTPMenuBar* 
	// ��    �� : UINT nID				�˵���ԴID
	// ��    �� : UINT nSubMenu = 0		�ڼ����Ӳ˵�
	// ��    �� : UINT nBefore = -1		�Ƿ���֮ǰ����
	virtual CXTPMenuBar* createSubMenu(UINT nID , UINT nSubMenu = 0 , UINT nBefore = -1);
	// �����Ӳ˵�
	virtual void destroySubMenu(CXTPMenuBar* pMenuBar , UINT nSubMenu = 0);

	// �� �� �� : *createToolBar
	// ע    �� : ����������
	// �������� : virtual CXTPToolBar 
	// ��    �� : UINT nID									����������ԴID
	// ��    �� : const CString &strText					�������ı���
	// ��    �� : XTPBarPosition barPosition = xtpBarTop	��������λ��
	virtual CXTPToolBar *createToolBar(UINT nID , const CString &strText , XTPBarPosition barPosition = xtpBarTop);
	// ���ٹ�����
	virtual void destroyToolBar(UINT nID);

	// ��ʾ���ļ��Ի���
	virtual String showOpenDialog(bool openFileDialog , const String &title, const String &filter , const String &defaultExt);
	// ��ʾ��ͼƬ�ļ��Ի���
	virtual String showOpenImageDialog(const String &title);

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CXTPStatusBar  m_wndStatusBar;
	CXTPDockingPaneManager m_paneManager;
	CXTSplitterWnd  m_wndSplitter;
	CXTPMenuBar *m_menuBar;
	CXTPToolBar *m_toolBar;
	// ��ͼ�б�
	ListChildView m_ListChildView;
	// �༭��ϵͳ
	WorldEditor *m_worldEditor;
	// �����ļ���
	String m_sceneFileName;


	// ����Ƿ��Ѿ���ס
	bool m_mouseLocked;
	// ��ס��λ��
	POINT m_lockMousePos;

	typedef std::map<IChildView::Listener* , bool> ListChildViewListener;
	// �������б�
	ListChildViewListener m_listChildViewListener;

	typedef std::map<UINT , IPlugin*> PluginButtonList;
	// �����ť�б�
	PluginButtonList m_pluginButtonList;

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnClose();
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnViewSetting();
	afx_msg void OnViewPluginManager();
	afx_msg void OnMapNew();
	afx_msg void OnMapOpen();
	afx_msg void OnMapSave();
	afx_msg void OnMapSaveAs();
	afx_msg void OnPluginButton(UINT nID);
	afx_msg void OnUpdatePluginButton(CCmdUI* pCmdUI);
};


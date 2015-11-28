// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "WorldEditor.h"

#include "resource.h"
#include "MainFrm.h"
#include "ChildView.h"
#include "SettingManager.h"
#include "PluginManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame
#define WM_TIMER_RENDER (WM_USER + 10000)

#define PLUGIN_BUTTON_START (WM_USER + 1000)
#define PLUGIN_BUTTON_END (PLUGIN_BUTTON_START + 100)

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_VIEW_SETTING, &CMainFrame::OnViewSetting)
	ON_COMMAND(ID_VIEW_PLUGIN_MANAGER, &CMainFrame::OnViewPluginManager)
	ON_COMMAND(ID_MAP_NEW, &CMainFrame::OnMapNew)
	ON_COMMAND(ID_MAP_OPEN, &CMainFrame::OnMapOpen)
	ON_COMMAND(ID_MAP_SAVE, &CMainFrame::OnMapSave)
	ON_COMMAND(ID_MAP_SAVE_AS, &CMainFrame::OnMapSaveAs)
	ON_COMMAND_RANGE(PLUGIN_BUTTON_START , PLUGIN_BUTTON_END , &CMainFrame::OnPluginButton)
	ON_UPDATE_COMMAND_UI_RANGE(PLUGIN_BUTTON_START , PLUGIN_BUTTON_END , &CMainFrame::OnUpdatePluginButton)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT uHideCmds[] =
{
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
: m_worldEditor(0)
, m_mouseLocked(false)
, m_menuBar(0)
, m_toolBar(0)
{
	m_worldEditor = new Ogre::WorldEditor(this);
}

CMainFrame::~CMainFrame()
{
	OGRE_DELETE(m_worldEditor);
}

// ����һ�������
Camera *CMainFrame::createCamera(const String &name)
{
	// ������ڣ���ֱ�ӷ��ش����ֵ������
	if(m_worldEditor->getDisplaySystem()->getMainSceneMgr()->hasCamera(name))
	{
		return m_worldEditor->getDisplaySystem()->getMainSceneMgr()->getCamera(name);
	}
	Camera *camera = m_worldEditor->getDisplaySystem()->getMainSceneMgr()->createCamera(name);
	camera->setNearClipDistance(0.1);
	camera->setFarClipDistance(1000);
	return camera;
}

// ����һ�������
Camera *CMainFrame::createCamera(CAMERA_TYPE cameraType)
{
	if(cameraType >= CT_MAX)
		return 0;
	String name("CameraType_" + StringConverter::toString(cameraType));
	Camera *camera = createCamera(name);

	Vector3 cameraPos(500,500,500);
	Vector3 cameraDir(0,0,0);

	switch(cameraType)
	{
	case CT_TOP :			// ����ͼ�����
		cameraDir.y = -1;
		break;
	case CT_BOTTOM :		// ����ͼ�����
		cameraDir.y = 1;
		break;
	case CT_LEFT :			// ����ͼ�����
		cameraDir.x = 1;
		break;
	case CT_RIGHT :			// ����ͼ�����
		cameraDir.x = -1;
		break;
	case CT_FRONT :			// ǰ��ͼ�����
		cameraDir.z = -1;
		break;
	case CT_BACK :			// ����ͼ�����
		cameraDir.z = 1;
		break;
	case CT_PERSPECTIVE :	// ��׼͸�������
		camera->setPosition(10,20,30);
		camera->lookAt(0,0,0);
		return camera;
		break;
	}
	camera->setPosition(cameraPos * -cameraDir);
	camera->setOrientation(MathEx::DirectionToQuaternion(cameraDir));
	camera->setProjectionType(PT_ORTHOGRAPHIC);
	camera->setOrthoWindow(10,10);

	return camera;
}

// ��������ͼ
IChildView* CMainFrame::createChildView(int row , int col , const String &name , Camera *camera)
{
	CChildView *view;
	view = (CChildView *)m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(row , col));
	if(view)
	{
		m_ListChildView.remove(view);
		m_wndSplitter.DeleteView(row , col);
		view = 0;
	}
	// ��������ͼ
	if (!m_wndSplitter.CreateView(row , col , RUNTIME_CLASS(CChildView),
		CSize(0, 0), 0))
	{
		TRACE0("Failed to create CChildView\n");
		return false;
	}
	view = (CChildView*)m_wndSplitter.GetPane(row , col);
	view->SetWindowText(name.c_str());
	m_ListChildView.push_back(view);
	view->setCamera(camera);
	// �������������
	for(ListChildViewListener::iterator iter = m_listChildViewListener.begin() ; iter != m_listChildViewListener.end() ; iter ++)
	{
		view->addListener(iter->first , iter->second);
	}
	return view;
}

// ��������ͼ
IChildView* CMainFrame::createChildView(int row , int col , const String &name , CAMERA_TYPE cameraType)
{
	// ���������
	Camera *camera = createCamera(cameraType);
	if(camera == 0)
		return false;
	// ������ͼ
	IChildView *view = createChildView(row , col , name , camera);
	if(view == 0)
	{
		return false;
	}
	static_cast<CChildView*>(view)->setCameraType(cameraType);
	return view;
}

// ��ô���ָ��
CWnd* CMainFrame::getWnd()
{
	return this;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{


	return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Initialize the command bars
	if (!InitCommandBars())
		return -1;

	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}

	// Add the menu bar
	m_menuBar = pCommandBars->SetMenu(
		_T("�˵���"), IDR_MAINFRAME);
	if(m_menuBar == NULL)
	{
		TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}

	// Create ToolBar
	m_toolBar = (CXTPToolBar*)
		pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!m_toolBar || !m_toolBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Set Office 2003 Theme
	CXTPPaintManager::SetTheme(xtpThemeOffice2003);

	// Hide array of commands
	pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));

	// Set "Always Show Full Menus" option to the FALSE
	pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);

	// Load the previous state for toolbars and menus.
	//LoadCommandBars(_T("CommandBars"));

	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);
	// Set Office 2003 Theme
	m_paneManager.SetTheme(xtpPaneThemeOffice2003);


	// Load the previous state for docking panes.
	//CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	//if (layoutNormal.Load(_T("NormalLayout")))
	//{
	//	m_paneManager.SetLayout(&layoutNormal);
	//}

	// �ڵ�һ��Paint��ʱ��ʹ���Ogreϵͳ
	m_worldEditor->init(GetSafeHwnd());
	// �������ӿ�
	m_worldEditor->getDisplaySystem()->getMainWindow()->setActive(false);

	// ��Ⱦ��ʱ��
	SetTimer(WM_TIMER_RENDER , 1 , 0);

	const int xMax = 2;
	const int yMax = 2;
	// ����2��2��
	if (!m_wndSplitter.CreateStatic(this, xMax, yMax))
	{
		TRACE0("Failed to create splitter window\n");
		return FALSE;
	}

	// ��������ͼ
	createChildView(0 , 0 , "����ͼ" , CT_TOP);
	createChildView(0 , 1 , "ǰ��ͼ" , CT_FRONT);
	createChildView(1 , 0 , "����ͼ" , CT_LEFT);
	createChildView(1 , 1 , "͸��" , CT_PERSPECTIVE);

	// �Զ��ָ�
	CRect r;
	GetClientRect(&r);
	m_wndSplitter.SetRowInfo( 0, r.Height()/2, 0 );
	m_wndSplitter.SetColumnInfo( 0, r.Width()/2, 0 );

	// ����ֻ��ʾ͸����ͼ
	m_wndSplitter.HideRow(0);
	m_wndSplitter.HideColumn(0);

	// ִ������������
	ExecuteListener1(IMainFrame::Listener , onCreate , lpCreateStruct);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	for(ListChildView::iterator iter = m_ListChildView.begin() ; iter != m_ListChildView.end() ; iter ++)
	{
		((CChildView*)*iter)->SetFocus();
	}
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ִ������������
	for(size_t i = 0 ; i < m_listListener.size() ; i ++)
	{
		if(m_listListener[i]->onCmdMsg(nID , nCode , pExtra , pHandlerInfo))
			return TRUE;
	}

	ListChildView listChildView(m_ListChildView);
	// �ص����Ӵ���
	for(ListChildView::iterator iter = listChildView.begin() ; iter != listChildView.end() ; iter ++)
	{
		if (((CChildView*)*iter)->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::OnClose()
{

	// ִ������������
	ExecuteListener(IMainFrame::Listener , onClose);

	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));

	// Save the current state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	m_paneManager.GetLayout(&layoutNormal);
	layoutNormal.Save(_T("NormalLayout"));

	CFrameWnd::OnClose();
}


void CMainFrame::OnCustomize()
{
	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars != NULL)
	{
		// Instanciate the customize dialog object.
		CXTPCustomizeSheet dlg(pCommandBars);

		// Add the keyboard page to the customize dialog.
		CXTPCustomizeKeyboardPage pageKeyboard(&dlg);
		dlg.AddPage(&pageKeyboard);
		pageKeyboard.AddCategories(IDR_MAINFRAME);

		// Add the options page to the customize dialog.
		CXTPCustomizeOptionsPage pageOptions(&dlg);
		dlg.AddPage(&pageOptions);

		// Add the commands page to the customize dialog.
		CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
		pCommands->AddCategories(IDR_MAINFRAME);

		// Use the command bar manager to initialize the
		// customize dialog.
		pCommands->InsertAllCommandsCategory();
		pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
		pCommands->InsertNewMenuCategory();

		// Dispaly the dialog.
		dlg.DoModal();
	}
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	// ִ������������
	ExecuteListener2(IMainFrame::Listener , onDockingPaneNotify , wParam , lParam);

	return FALSE;
}

BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// ��������Ogre����
	try
	{
		// ִ������������
		for(size_t i = 0 ; i < m_listListener.size() ; i ++)
		{
			if(!m_listListener[i]->onWndMsg(message , wParam , lParam , pResult))
				return TRUE;
		}


		return CXTPFrameWnd::OnWndMsg(message , wParam , lParam , pResult);
	}
	catch( Exception& e )
	{
		MessageBox(e.getFullDescription().c_str(), "�׳���һ���쳣", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		return FALSE;
	}
}

void CMainFrame::OnDestroy()
{
	CXTPFrameWnd::OnDestroy();
	// �����������Ӵ���
	for(ListChildView::iterator iter = m_ListChildView.begin() ; iter != m_ListChildView.end() ; iter ++)
	{
		((CChildView*)*iter)->DestroyWindow();
	}
	m_ListChildView.clear();
	m_wndSplitter.DestroyWindow();
	// ������ʾ��
	m_worldEditor->shutdown();
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == WM_TIMER_RENDER)
	{
		if(m_worldEditor->isInited())
		{
			m_worldEditor->getDisplaySystem()->renderOneFrame();
		}
		return;
	}
	CXTPFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	keepLockMouse();
	__super::OnMouseMove(nFlags, point);
}
// ��֤�������������ʱ��λ��
void CMainFrame::keepLockMouse()
{
	// ����Ѿ������������������λ��
	if(isMouseLocked())
	{
		SetCursorPos(m_lockMousePos.x , m_lockMousePos.y);
	}
}
// ��ס���������
void CMainFrame::lockMouse(bool bLock)
{
	if(m_mouseLocked == bLock)
	{
		return;
	}
	m_mouseLocked = bLock;
	if(m_mouseLocked)
	{
		ShowCursor(false);
		GetCursorPos(&m_lockMousePos);
	}
	else
	{
		SetCursorPos(m_lockMousePos.x , m_lockMousePos.y);
		ShowCursor(true);
	}
}

// ����Ƿ��Ѿ���ס��
bool CMainFrame::isMouseLocked()
{
	return m_mouseLocked;
};
// ����Ӵ����б�
IMainFrame::ChildViewIterator CMainFrame::getChildViewIterator()
{
	return ChildViewIterator(m_ListChildView.begin() , m_ListChildView.end());
}


// �������bars�������������������˵��Ľӿ�
CXTPCommandBars* CMainFrame::getCommandBars()
{
	return GetCommandBars();
}

// ���״̬���ӿ�
CXTPStatusBar* CMainFrame::getStatusBar()
{
	return &m_wndStatusBar;
}

// ��ø������ڹ�����
CXTPDockingPaneManager* CMainFrame::getDockingPaneManager()
{
	return &m_paneManager;
}


// �� �� �� : createPane
// ע    �� : ����һ����������
// �������� : CXTPDockingPane* 
// ��    �� : UINT nID									������ԴID
// ��    �� : CRect rc									����
// ��    �� : XTPDockingPaneDirection direction			��������
// ��    �� : CXTPDockingPaneBase* pNeighbour = NULL	�����Ĵ���
CXTPDockingPane* CMainFrame::createPane(UINT nID, const String &title , CRect rc, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour)
{

	CXTPDockingPaneBase* pDockPane = pNeighbour;
	//if(pDockPane == 0)
	//{
	//	// ���ӵ���һ��pane
	//	CXTPDockingPaneInfoList& paneList = m_paneManager.GetPaneList();
	//	if(paneList.GetCount())
	//	{
	//		pDockPane = paneList.GetHead();
	//	}
	//}
	// �������
	CXTPDockingPane* pPane = m_paneManager.CreatePane(nID, rc, direction, pDockPane);
	if(pPane)
	{
		pPane->SetTitle(title.c_str());
		//if(pDockPane)
		//{
		//	m_paneManager.DockPane(pPane , xtpPaneDockBottom , pDockPane);
		//}
		//CString strTitle;
		//strTitle.LoadString(nID);
		//// ������˵�
		//CXTPControl *pControl = GetViewPaneControls()->Add(xtpControlButton, ID_VIEW_PANE_FIRST + GetViewPaneControls()->GetCount(), _T(""));
		//// ��������
		//pControl->SetCaption(strTitle);
		//pControl->SetIconId(nID);
	}
	return pPane;
}
// ���ٸ�������
void CMainFrame::destroyPane(UINT nID)
{
	m_paneManager.DestroyPane(nID);
}

// �� �� �� : createSubMenu
// ע    �� : ����һ���Ӳ˵�
// �������� : CXTPMenuBar* 
// ��    �� : UINT nID				�˵���ԴID
// ��    �� : UINT nSubMenu = 0		�ڼ����Ӳ˵�
// ��    �� : UINT nBefore = -1		�Ƿ���֮ǰ����
CXTPMenuBar* CMainFrame::createSubMenu(UINT nID , UINT nSubMenu , UINT nBefore)
{
	// �½��˵�
	CXTPMenuBar* pMenuBar(static_cast<CXTPMenuBar*>(CXTPMenuBar::CreateObject()));
	pMenuBar->LoadMenuBar(nID);

	// ��Ӳ˵�
	CXTPControl *pControl = pMenuBar->GetControl(nSubMenu);
	pControl = m_menuBar->GetControls()->Add(pControl , 0 , "" , nBefore);
	// ���ò˵�
	pControl->SetEnabled(TRUE);

	return pMenuBar;
}

// �����Ӳ˵�
void CMainFrame::destroySubMenu(CXTPMenuBar* pMenuBar , UINT nSubMenu)
{
	CXTPControl *pControl = pMenuBar->GetControl(nSubMenu);
	if(pControl)
	{
		//getMenuBar()->GetControls()->Remove(pControl);
		// add by ������ 2007-8-22
		// ������pMenuBar���ڴ棿
	}

}

// �� �� �� : *createToolBar
// ע    �� : ����������
// �������� : CXTPToolBar 
// ��    �� : UINT nID									����������ԴID
// ��    �� : const CString &strText					�������ı���
// ��    �� : XTPBarPosition barPosition = xtpBarTop	��������λ��
CXTPToolBar *CMainFrame::createToolBar(UINT nID , const CString &strText , XTPBarPosition barPosition)
{
	// ����������
	CXTPToolBar* pObjectBar = (CXTPToolBar*)getCommandBars()->Add(strText, xtpBarTop);
	if (!pObjectBar ||
		!pObjectBar->LoadToolBar(nID))
	{
		TRACE0("Failed to create toolbar\n");
		return 0;
	}
	CXTPCommandBars* pCommandBars = GetCommandBars();
	CXTPCommandBar *bar = pCommandBars->GetAt(pCommandBars->GetCount() - 1);
	DockRightOf(pObjectBar , (CXTPToolBar*)bar);
	return pObjectBar;
}
// ���ٹ�����
void CMainFrame::destroyToolBar(UINT nID)
{

}

void CMainFrame::OnViewSetting()
{
	SettingManager::getSingleton().showSettingDlg();
}

void CMainFrame::OnViewPluginManager()
{
	PluginManager::getSingleton().showPluginManagerDlg();
}


// ��ʾ���ļ��Ի���
String CMainFrame::showOpenDialog(bool openFileDialog , const String &title, const String &filter , const String &defaultExt)
{
	String filePathName;
	CFileDialog dlgFile(openFileDialog , defaultExt.c_str() , NULL , OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT , filter.c_str() , this);

	CString fileName;
	dlgFile.GetOFN().lpstrFile = fileName.GetBuffer(1024 * 1024);
	dlgFile.GetOFN().nMaxFile = 1024 * 1024;
	char szDirectoryp[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH , szDirectoryp);

	INT_PTR nResult = dlgFile.DoModal();
	if(nResult == IDOK)
	{
		//����ѡȡ���ļ����򷵻��ļ�����
		filePathName = dlgFile.GetPathName();
	}
	fileName.ReleaseBuffer();
	::SetCurrentDirectory(szDirectoryp);
	return filePathName;
}

// ��ʾ��ͼƬ�ļ��Ի���
String CMainFrame::showOpenImageDialog(const String &title)
{
	return showOpenDialog(true , title , "*.png;*.bmp;*.jpg;*.dds;*.tga|*.png;*.bmp;*.jpg;*.dds;*.tga|.*|*.*||" , ".tga");
}

void CMainFrame::OnMapNew()
{
	WorldEditor::getSingleton().getMapManager()->newScene();
	m_sceneFileName.clear();
}

void CMainFrame::OnMapOpen()
{
	String fileName = showOpenDialog(true , "�򿪳���" , "*.osm;*.sne;*.xml|*.osm;*.sne;*.xml||" , ".sne");
	if(!fileName.empty())
	{
		String outBaseName , outPath;
		StringUtil::splitFilename(fileName , outBaseName , outPath);
		WorldEditor::getSingleton().getMapManager()->loadScene(outBaseName);
		m_sceneFileName = fileName;
	}
}

void CMainFrame::OnMapSave()
{
	if(!m_sceneFileName.empty())
	{
		WorldEditor::getSingleton().getMapManager()->saveScene(m_sceneFileName);
	}
	else
	{
		OnMapSaveAs();
	}
}

void CMainFrame::OnMapSaveAs()
{
	String fileName = showOpenDialog(false , "���泡��" , "*.sne|*.sne|*.xml|*.xml||" , ".sne");
	if(!fileName.empty())
	{
		if(WorldEditor::getSingleton().getMapManager()->saveScene(fileName))
		{
			m_sceneFileName = fileName;
		}
	}
}


// ��������������İ�ť
int CMainFrame::createPluginToolBarButton(const String &strName , IPlugin *plugin)
{
	CXTPControl *control = m_toolBar->GetControls()->Add(xtpControlCheckBox , m_toolBar->GetControls()->GetCount() + PLUGIN_BUTTON_START);
	control->SetCaption(strName.c_str());
	m_pluginButtonList[control->GetID()] = plugin;
	return control->GetID();
}

// ���ٲ���������İ�ť
void CMainFrame::destroyPluginToolBarButton(const String &strName)
{
	CXTPControls *controls = m_toolBar->GetControls();
	for(int i = 0 ; i < controls->GetCount() ; i ++)
	{
		if(String(controls->GetAt(i)->GetCaption()) == strName)
		{
			m_pluginButtonList.erase(m_pluginButtonList.find(controls->GetAt(i)->GetID()));
			controls->Remove(i);
			return;
		}
	}
}

void CMainFrame::OnPluginButton(UINT nID)
{
	PluginButtonList::iterator iter = m_pluginButtonList.find(nID);
	if(iter != m_pluginButtonList.end())
	{
		IPlugin *plugin = iter->second;
		for(PluginButtonList::iterator iter = m_pluginButtonList.begin(); iter != m_pluginButtonList.end() ; iter ++)
		{
			// ֻ����ѡ�е�
			if(iter->second == plugin && !iter->second->isActivate())
			{
				iter->second->activate();
			}
			else
			{
				iter->second->deactivate();
			}
		}
	}
}

void CMainFrame::OnUpdatePluginButton(CCmdUI* pCmdUI)
{
	PluginButtonList::iterator iter = m_pluginButtonList.find(pCmdUI->m_nID);
	if(iter != m_pluginButtonList.end())
	{
		IPlugin *plugin = iter->second;
		pCmdUI->SetCheck(plugin->isActivate());
	}
}

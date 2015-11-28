// ============================================================
// �ļ���	:	IMainFrame.h
// ������	:	������
// ˵��		:	�����ڹ�����
// ����ʱ��	:	2008-1-23 8:31:03
// ============================================================

#pragma once
#include "IChildView.h"
namespace Ogre
{

	class IChildView;
	class IPlugin;


	// �����ڹ�����
	class IMainFrame
	{
	public:

		IMainFrame(void)
		{
		}

		virtual ~IMainFrame(void)
		{
		}

		// �����ڵ��¼�������
		struct Listener
		{
			// ��������
			virtual void onCreate(LPCREATESTRUCT lpCreateStruct){};
			// ����������
			virtual void onClose(){};
			// ������Ϣ������true���ʾ��������������Ϣ����Ӧ��ť�ͻᱻ���ã����Ҳ�ִ�к������������false�������Ӧ��ť
			virtual bool onCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo){return false;};
			// ��������Ϣ������false��ִ�к����������
			virtual bool onWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult){return true;};
			// ��������֪ͨ
			virtual LRESULT onDockingPaneNotify(WPARAM wParam, LPARAM lParam){return 0;};
		};

		// ���������������
		virtual void addListener(Listener *listener , bool toBack = true) = 0;
		// ɾ��������������
		virtual void removeListener(Listener *listener) = 0;

		// ����Ӵ���������
		virtual void addChildViewListener(IChildView::Listener *listener , bool toBack = true) = 0;
		// ɾ���Ӵ���������
		virtual void removeChildViewListener(IChildView::Listener *listener) = 0;

		// ��ô���ָ��
		virtual CWnd* getWnd() = 0;

		// ����һ�������
		virtual Camera *createCamera(const String &name) = 0;
		// ���һ�������
		virtual Camera *createCamera(CAMERA_TYPE cameraType) = 0;
		// ��������ͼ
		virtual IChildView* createChildView(int row , int col , const String &name , Camera *camera) = 0;
		// ��������ͼ
		virtual IChildView* createChildView(int row , int col , const String &name , CAMERA_TYPE cameraType) = 0;

		// ��ס���������
		virtual void lockMouse(bool bLock) = 0;
		// ����Ƿ��Ѿ���ס��
		virtual bool isMouseLocked() = 0;
		// ��֤�������������ʱ��λ��
		virtual void keepLockMouse() = 0;

		// �� �� �� : createPane
		// ע    �� : ����һ����������
		// �������� : virtual CXTPDockingPane* 
		// ��    �� : UINT nID									������ԴID
		// ��    �� : CRect rc									����
		// ��    �� : XTPDockingPaneDirection direction			��������
		// ��    �� : CXTPDockingPaneBase* pNeighbour = NULL	�����Ĵ���
		virtual CXTPDockingPane* createPane(UINT nID, const String &title , CRect rc, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour = NULL) = 0;
		// ���ٸ�������
		virtual void destroyPane(UINT nID) = 0;

		// �� �� �� : createSubMenu
		// ע    �� : ����һ���Ӳ˵�
		// �������� : virtual CXTPMenuBar* 
		// ��    �� : UINT nID				�˵���ԴID
		// ��    �� : UINT nSubMenu = 0		�ڼ����Ӳ˵�
		// ��    �� : UINT nBefore = -1		�Ƿ���֮ǰ����
		virtual CXTPMenuBar* createSubMenu(UINT nID , UINT nSubMenu = 0 , UINT nBefore = -1) = 0;
		// �����Ӳ˵�
		virtual void destroySubMenu(CXTPMenuBar* pMenuBar , UINT nSubMenu = 0) = 0;

		// �� �� �� : *createToolBar
		// ע    �� : ����������
		// �������� : virtual CXTPToolBar 
		// ��    �� : UINT nID									����������ԴID
		// ��    �� : const CString &strText					�������ı���
		// ��    �� : XTPBarPosition barPosition = xtpBarTop	��������λ��
		virtual CXTPToolBar *createToolBar(UINT nID , const CString &strText , XTPBarPosition barPosition = xtpBarTop) = 0;
		// ���ٹ�����
		virtual void destroyToolBar(UINT nID) = 0;

		// ��������������İ�ť
		virtual int createPluginToolBarButton(const String &strName , IPlugin *plugin) = 0;
		// ���ٲ���������İ�ť
		virtual void destroyPluginToolBarButton(const String &strName) = 0;

		// �������bars�������������������˵��Ľӿ�
		virtual CXTPCommandBars* getCommandBars() = 0;
		// ���״̬���ӿ�
		virtual CXTPStatusBar* getStatusBar() = 0;
		// ��ø������ڹ�����
		virtual CXTPDockingPaneManager* getDockingPaneManager() = 0;

		typedef std::list<IChildView*> ListChildView;
		typedef VectorIterator<ListChildView> ChildViewIterator;
		// ����Ӵ����б�
		virtual ChildViewIterator getChildViewIterator() = 0;

		// ��ʾ���ļ��Ի���
		virtual String showOpenDialog(bool openFileDialog , const String &title, const String &filter , const String &defaultExt) = 0;
		// ��ʾ��ͼƬ�ļ��Ի���
		virtual String showOpenImageDialog(const String &title) = 0;

	};
};
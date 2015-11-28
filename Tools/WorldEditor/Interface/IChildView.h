// ============================================================
// �ļ���	:	IChildView.h
// ������	:	������
// ˵��		:	���ӿڹ�����
// ����ʱ��	:	2008-1-23 8:31:03
// ============================================================

#pragma once
#include "IMainFrame.h"
namespace Ogre
{
	// ���������
	enum CAMERA_TYPE
	{
		CT_TOP,			// ����ͼ�����
		CT_BOTTOM,		// ����ͼ�����
		CT_LEFT,		// ����ͼ�����
		CT_RIGHT,		// ����ͼ�����
		CT_FRONT,		// ǰ��ͼ�����
		CT_BACK,		// ����ͼ�����
		CT_PERSPECTIVE,	// ��׼͸�������
		CT_USER,		// �û��Զ��������
		CT_MAX,			// �������������
	};
	// ���ӿ�
	class IChildView
	{
	public:

		IChildView(void)
		{
		}

		virtual ~IChildView(void)
		{
		}

		// ���ӿڵ��¼�������
		struct Listener
		{
			// ��ʼ��
			virtual void onInit(IChildView *view){}
			// ����
			virtual void onDestroy(IChildView *view){}
			// ������Ϣ������true���ʾ��������������Ϣ����Ӧ��ť�ͻᱻ���ã����Ҳ�ִ�к������������false�������Ӧ��ť
			virtual bool onCmdMsg(IChildView *view , UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo){return false;};
			// ���ӿ���Ϣ������false��ִ�к����������
			virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult){return true;};
			// ����ƶ�������false��ִ�к����������
			virtual bool onMouseMove(IChildView *view , const CPoint &moveDistance , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult){return true;};

			// ׼�����´��ڣ�����false��ִ�к����������
			virtual bool preViewportUpdate(IChildView *view , const RenderTargetViewportEvent& evt){return true;};
			// ������ϴ��ڣ�����false��ִ�к����������
			virtual bool postViewportUpdate(IChildView *view , const RenderTargetViewportEvent& evt){return true;};
		};

		// ������ӿ�������
		virtual void addListener(Listener *listener , bool toBack = true) = 0;
		// ɾ�����ӿ�������
		virtual void removeListener(Listener *listener) = 0;


		// ��ô���ָ��
		virtual CWnd* getWnd() = 0;
		// ��ô��ڴ�С
		virtual CRect getWndRect() = 0;

		// ��������ӿڵ������
		virtual void setCamera(Camera *camera) = 0;
		// �������ӿڵ������
		virtual Camera *getCamera()const = 0;

		// ���������ҽӵĽڵ�
		virtual SceneNode *getMoveCameraSceneNode()const = 0;
		// ���������ҽӵĽڵ�
		virtual SceneNode *getRotateCameraSceneNode()const = 0;
		// �����Ⱦ����
		virtual RenderWindow *getRenderWindow()const = 0;
		// ����ӿ�
		virtual Viewport *getViewport()const = 0;

		// ������������
		virtual CAMERA_TYPE getCameraType()const = 0;

		// ������һ��WM_MOUSEMOVEʱ����ƶ��˶��پ���
		virtual const CPoint &getMouseMoveDistance() = 0;


	};
};
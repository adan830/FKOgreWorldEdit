// ============================================================
// �ļ���	:	CameraControllerPlugin.h
// ������	:	������
// ˵��		:	�����������ʵ��
// ����ʱ��	:	2008-1-24 19:22:49
// ============================================================
#pragma once
#include "IPlugin.h"
#include "OgreTextAreaOverlayElement.h"
namespace Ogre
{
	class CameraControllerPlugin : public IPlugin 
		, public IChildView::Listener
		, public Singleton<CameraControllerPlugin>
	{
	public:
		CameraControllerPlugin(HINSTANCE hInstance);
		virtual ~CameraControllerPlugin(void);
		// ����ģʽ�ӿ�
		static CameraControllerPlugin& getSingleton(void);
		static CameraControllerPlugin* getSingletonPtr(void);


		// ��ʼ���������������worldEditor������༭���Ľӿ�
		virtual bool init(IWorldEditor *worldEditor);
		// �رղ��
		virtual void shutdown();
		// �Ƿ��Ѿ���ʼ��
		virtual bool isInited()const;

		// ������
		virtual void activate();
		// ��������
		virtual void deactivate();
		// �Ƿ��Ѿ�����
		virtual bool isActivate()const;

		// �ӿڸ���
		virtual void preViewportUpdate(const RenderTargetViewportEvent& evt);
		virtual void postViewportUpdate(const RenderTargetViewportEvent& evt);

		// ��ò������
		virtual const Description &getDescription()const;

		// ��ѯһЩ���ݣ�Ȼ�󷵻ص�result��
		virtual bool query(const String &name , const NameValuePairList *params , void *result);
		// ��������Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		// ����ƶ�
		virtual bool onMouseMove(IChildView *view , const CPoint &moveDistance , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

		// �������༭���ӿ�
		IWorldEditor *getWorldEditor()
		{
			return m_worldEditor;
		}
		// �ƶ������
		void moveCameraRel(IChildView *view , float x , float y , float z);
		// ��ת�����������ĳ����ת
		void rotateCameraAroundPosition(IChildView *view , const Vector3 &aroundPosition , const Radian &x , const Radian &y);
		// ��ת����������ӿ�������ת
		void rotateCameraAroundViewportCenter(IChildView *view , const Radian &x , const Radian &y);
	protected:

		bool m_inited;
		bool m_activated;
		
		// �������
		Description m_description;

		// ����༭���ӿ�
		IWorldEditor *m_worldEditor;
	};

};
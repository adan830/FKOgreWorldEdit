// ============================================================
// �ļ���	:	StrollPlugin.h
// ������	:	������
// ˵��		:	�����������ʵ��
// ����ʱ��	:	2008-1-24 19:22:49
// ============================================================
#pragma once
#include "IPlugin.h"
namespace Ogre
{
	class StrollPlugin : public IPlugin 
		, public IChildView::Listener
		, public FrameListener
		, public CharacterCallback
		, public IMapManager::Listener
		, public Singleton<StrollPlugin>
	{
	public:
		StrollPlugin(HINSTANCE hInstance);
		virtual ~StrollPlugin(void);
		// ����ģʽ�ӿ�
		static StrollPlugin& getSingleton(void);
		static StrollPlugin* getSingletonPtr(void);


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
		// �������༭���ӿ�
		IWorldEditor *getWorldEditor()
		{
			return m_worldEditor;
		}


		// ÿ֡����������·״̬
		bool frameStarted( const FrameEvent& evt );

		// ������һ��������
		// @triggerObject ������Ĵ�����
		// @player ��������
		virtual void onEnterTrigger(TriggerObject *triggerObject , CharacterObject *player);

		// ��һ����������ȥ
		// @triggerObject ��Ҵ������������ȥ
		// @player ��ȥ�����
		virtual void onLeaveTrigger(TriggerObject *triggerObject , CharacterObject *player);

		// ���ٳ���
		virtual void onDestroyScene(IScene *scene);
	protected:

		bool m_inited;
		bool m_activated;
		
		// �������
		Description m_description;


		// ����༭���ӿ�
		IWorldEditor *m_worldEditor;

		// �������γ��������
		CharacterObject *m_character;
		// ��Һ���������
		CameraObject *m_camera;

		// �Զ����������������
		bool m_autoPlaceFloor;


		enum CharacterState
		{
			CS_TurnLeft = 1 << 1,
			CS_TurnRight = 1 << 2,
			CS_Forward = 1 << 3,
			CS_Backward = 1 << 4,
		};
		// ���״̬�����״̬
		uint m_state;
	};

};
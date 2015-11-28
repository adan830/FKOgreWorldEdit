// ============================================================
// �ļ���	:	AxisGridPlugin.h
// ������	:	������
// ˵��		:	������ʵ��
// ����ʱ��	:	2008-1-24 19:22:49
// ============================================================
#pragma once
#include "IPlugin.h"
#include "AxisRTT.h"
#include "OgreTextAreaOverlayElement.h"
namespace Ogre
{
	class AxisGridPlugin : public IPlugin 
		, public IChildView::Listener
		, public Singleton<AxisGridPlugin>
	{
	public:
		AxisGridPlugin(HINSTANCE hInstance);
		virtual ~AxisGridPlugin(void);
		// ����ģʽ�ӿ�
		static AxisGridPlugin& getSingleton(void);
		static AxisGridPlugin* getSingletonPtr(void);


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

	protected:

		// ����һ���������
		ManualObject* createGrid(float width , float height , float interval);
		// �����������ʶ
		ManualObject* createAxis();
		// �������������
		void createAxisOverlay();
		// ����ͳ�ƽ���
		void createStatOverlay();

		bool m_inited;
		bool m_activated;
		
		// �������
		ManualObject *m_gridObject;
		SceneNode *m_gridNode;
		// ���������
		Overlay* m_axisOverlay;
		OverlayElement* m_axisElement;
		AxisRTT * m_axisRTT;
		// ͳ����
		TextAreaOverlayElement *m_statElement;

		// �������
		Description m_description;


		// ����༭���ӿ�
		IWorldEditor *m_worldEditor;
	};

};
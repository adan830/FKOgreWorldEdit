#pragma once
#include "NullObjectVisible.h"
#include "AnimationTrackController.h"
namespace Ogre
{
	class AnimationTrackObject;
	class IScene;
	// ��������Ŀɼ����������ڱ༭����ѡ��
	class DISPLAYLAYER_API AnimationTrackObjectVisible : public NullObjectVisible
		, public FrameListener
	{
	public:
		AnimationTrackObjectVisible(AnimationTrackObject *obj);
		virtual ~AnimationTrackObjectVisible(void);

		// ��ʾ�߿�
		virtual void showBorder(bool show);

		// ��ʾ�ؼ���
		void showKeyNode(bool show);

		// �Ƿ���ʾ�ؼ���
		bool isShowKeyNode()const
		{
			return m_showKeyNode;
		}
		// �ؽ��ɼ�����
		virtual void _notifyRebuild();

		// ÿ֡����Ƿ���Ҫ�ؽ��ɼ�����
		virtual bool frameStarted(const FrameEvent& evt)
		{
			rebuild();
			return true;
		}
	protected:
		// ���ٹ켣��
		void destroyTrackLines();
		// �ؽ��ɼ�����
		void rebuild();
		// �켣��
		ManualObject *m_trackLines;
		// �켣�ڵ�
		SceneNode *m_trackNode;
		typedef std::list<Entity*> ListKeyNode;
		// �켣�ؼ����б�
		ListKeyNode m_listKeyNode;
		// �켣�ؼ���Ľڵ�
		SceneNode *m_trackKeyNode;
		// �Ƿ���ʾ�ؼ���
		bool m_showKeyNode;
		// ��Ҫ�ؽ�
		bool m_needRebuild;
		// ��������켣�ߵĿ�����
		AnimationTrackController* m_controller;
	};
};
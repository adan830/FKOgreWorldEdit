#pragma once
#include "ITriggerWorld.h"

namespace Ogre
{
	class OctreeTriggerWorld : public ITriggerWorld
		, public FrameListener
	{
	public:
		OctreeTriggerWorld(const String &name);
		virtual ~OctreeTriggerWorld(void);


		// �������
		virtual const String &getName()const
		{
			return m_name;
		}


		// �������Ӵ�����ʵ��
		virtual IBoxTriggerEntity* createBoxTriggerEntity(const String &name = StringUtil::BLANK);
		// �������崥����ʵ��
		virtual ISphereTriggerEntity* createSphereTriggerEntity(const String &name = StringUtil::BLANK);
		// �������ʵ��
		virtual ITriggerPlayer * createTriggerPlayer(const String &name = StringUtil::BLANK);
		// ���ٴ�����ʵ��
		virtual void destroyTriggerEntity(ITriggerEntity *entity);
		// ���ٴ�����ʵ��
		virtual void destroyTriggerEntity(const String &name , const String &triggerType);
		// ���һ��������ʵ��
		virtual ITriggerEntity* getTriggerEntity(const String &name , const String &triggerType)const;
		// �Ƿ������������ʵ��
		virtual bool hasTriggerEntity(const String &name , const String &triggerType)const;

		// �������д�����ʵ��
		virtual void destroyAllTriggerEntity();

		// ������ж�����б�
		virtual TriggerEntityIterator getTriggerEntityIterator(const String &triggerType);

		// ���Ogre����������
		SceneManager *getSceneManager()const
		{
			return m_sceneMgr;
		}
		typedef std::map<String , ITriggerPlayer*> MapTriggerPlayer;
		typedef MapIterator<MapTriggerPlayer> TriggerPlayerIterator;
		// ���������ҵ��б�
		virtual TriggerPlayerIterator getTriggerPlayerIterator();

		// ÿ֡���³���������
		virtual bool frameStarted(const FrameEvent& evt);

	protected:
		String m_name;
		// Ogre����������
		SceneManager *m_sceneMgr;
	};

};
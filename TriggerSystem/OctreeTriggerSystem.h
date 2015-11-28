#pragma once
#include "ITriggerSystem.h"

namespace Ogre
{
	// ������ϵͳʵ��
	class OctreeTriggerSystem : public ITriggerSystem
	{
	public:
		OctreeTriggerSystem(void);
		virtual ~OctreeTriggerSystem(void);


		// ����һ������������
		virtual ITriggerWorld *createWorld(const String &name);
		// ���һ������������
		virtual ITriggerWorld *getWorld(const String &name);
		// �Ƿ���������ƵĴ���������
		virtual bool hasWorld(const String &name);
		// ����һ������������
		virtual void destroyWorld(const String &name);
		// ����һ������������
		virtual void destroyWorld(ITriggerWorld *world);

	protected:
		typedef std::map<String , ITriggerWorld*> TriggerWorldMap;
		TriggerWorldMap m_worldMap;

	};
};
#pragma once
#include <TListener.h>
#include "ITriggerEntity.h"
namespace Ogre
{


	class IBoxTriggerEntity;
	class ISphereTriggerEntity;
	// ����������
	class ITriggerWorld
		: public TListener<TriggerListener>
	{
	public:
		ITriggerWorld(){};
		virtual ~ITriggerWorld(void){};

		// �������
		virtual const String &getName()const = 0;


		// �������Ӵ�����ʵ��
		virtual IBoxTriggerEntity* createBoxTriggerEntity(const String &name = StringUtil::BLANK) = 0;
		// �������崥����ʵ��
		virtual ISphereTriggerEntity* createSphereTriggerEntity(const String &name = StringUtil::BLANK) = 0;
		// �������ʵ��
		virtual ITriggerPlayer * createTriggerPlayer(const String &name = StringUtil::BLANK) = 0;
		// ���ٴ�����ʵ��
		virtual void destroyTriggerEntity(ITriggerEntity *entity)= 0;
		// ���ٴ�����ʵ��
		virtual void destroyTriggerEntity(const String &name , const String &triggerType)= 0;
		// ���һ��������ʵ��
		virtual ITriggerEntity* getTriggerEntity(const String &name , const String &triggerType)const = 0;
		// �Ƿ������������ʵ��
		virtual bool hasTriggerEntity(const String &name , const String &triggerType)const = 0;

		// �������д�����ʵ��
		virtual void destroyAllTriggerEntity() = 0;

		typedef std::map<String , ITriggerEntity*> MapTriggerEntity;
		typedef MapIterator<MapTriggerEntity> TriggerEntityIterator;
		// ������д��������б�
		virtual TriggerEntityIterator getTriggerEntityIterator(const String &triggerType) = 0;
	};
};
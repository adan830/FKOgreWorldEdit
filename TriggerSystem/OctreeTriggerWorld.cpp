#include "StdAfx.h"
#include "OctreeTriggerWorld.h"
#include "OctreeBoxTriggerEntity.h"
#include "OctreeSphereTriggerEntity.h"
#include "OctreeTriggerPlayer.h"

namespace Ogre
{

	OctreeTriggerWorld::OctreeTriggerWorld(const String &name)
		: m_name(name)
		, m_sceneMgr(0)
	{
#ifdef _DEBUG
		m_sceneMgr = Root::getSingleton().createSceneManager("DefaultSceneManager" , m_name);
#else
		m_sceneMgr = Root::getSingleton().createSceneManager("OctreeSceneManager" , m_name);
#endif
		Root::getSingleton().addFrameListener(this);
	}

	OctreeTriggerWorld::~OctreeTriggerWorld(void)
	{
		Root::getSingleton().removeFrameListener(this);
		destroyAllTriggerEntity();
		Root::getSingleton().destroySceneManager(m_sceneMgr);
	}

	// ÿ֡���³���������
	bool OctreeTriggerWorld::frameStarted(const FrameEvent& evt)
	{
		// Process queued needUpdate calls 
		Node::processQueuedUpdates();

		// Cascade down the graph updating transforms & world bounds
		// In this implementation, just update from the root
		// Smarter SceneManager subclasses may choose to update only
		//   certain scene graph branches
		m_sceneMgr->getRootSceneNode()->_update(true, false);

		return true;

	}


	// �������Ӵ�����ʵ��
	IBoxTriggerEntity* OctreeTriggerWorld::createBoxTriggerEntity(const String &name)
	{
		if(hasTriggerEntity(name , OctreeBoxTriggerEntityFactory::FACTORY_TYPE_NAME))
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"����������Ϊ '" + name + "' �Ĵ������Ѿ�����", 
				"OctreeTriggerWorld::createBoxTriggerEntity");
			return 0;
		}
		NameValuePairList params;
		params["world"] = StringConverter::toString((long)this);
		OctreeBoxTriggerEntity *obj = static_cast<OctreeBoxTriggerEntity*>(m_sceneMgr->createMovableObject(name , OctreeBoxTriggerEntityFactory::FACTORY_TYPE_NAME , &params));
		return obj;

	}
	// �������崥����ʵ��
	ISphereTriggerEntity* OctreeTriggerWorld::createSphereTriggerEntity(const String &name)
	{
		if(hasTriggerEntity(name , OctreeSphereTriggerEntityFactory::FACTORY_TYPE_NAME))
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"����������Ϊ '" + name + "' �Ĵ������Ѿ�����", 
				"OctreeTriggerWorld::createSphereTriggerEntity");
			return 0;
		}
		NameValuePairList params;
		params["world"] = StringConverter::toString((long)this);
		OctreeSphereTriggerEntity *obj = static_cast<OctreeSphereTriggerEntity*>(m_sceneMgr->createMovableObject(name , OctreeSphereTriggerEntityFactory::FACTORY_TYPE_NAME , &params));
		return obj;
	}
	// �������ʵ��
	ITriggerPlayer * OctreeTriggerWorld::createTriggerPlayer(const String &name)
	{
		if(hasTriggerEntity(name , OctreeTriggerPlayerFactory::FACTORY_TYPE_NAME))
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"����������Ϊ '" + name + "' �Ĵ������Ѿ�����", 
				"OctreeTriggerWorld::createTriggerPlayer");
			return 0;
		}
		NameValuePairList params;
		params["world"] = StringConverter::toString((long)this);
		OctreeTriggerPlayer *obj = static_cast<OctreeTriggerPlayer*>(m_sceneMgr->createMovableObject(name , OctreeTriggerPlayerFactory::FACTORY_TYPE_NAME , &params));
		return obj;
	}

	// �������д�����ʵ��
	void OctreeTriggerWorld::destroyAllTriggerEntity()
	{
		m_sceneMgr->destroyAllMovableObjectsByType(OctreeBoxTriggerEntityFactory::FACTORY_TYPE_NAME);
		m_sceneMgr->destroyAllMovableObjectsByType(OctreeSphereTriggerEntityFactory::FACTORY_TYPE_NAME);
		m_sceneMgr->destroyAllMovableObjectsByType(OctreeTriggerPlayerFactory::FACTORY_TYPE_NAME);
	}

	// ���ٴ�����
	void OctreeTriggerWorld::destroyTriggerEntity(const String &name , const String &triggerType)
	{
		m_sceneMgr->destroyMovableObject(name , triggerType);
	}

	void OctreeTriggerWorld::destroyTriggerEntity(ITriggerEntity *obj)
	{
		destroyTriggerEntity(obj->getName() , obj->getType());
	}

	// ���һ��������
	ITriggerEntity *OctreeTriggerWorld::getTriggerEntity(const String &name , const String &triggerType)const
	{
		MovableObject *obj = m_sceneMgr->getMovableObject(name , triggerType);
		return (ITriggerEntity*)obj;
		//if(!obj)
		//{
		//	return 0;
		//}
		//if(obj->getMovableType() == OctreeBoxTriggerEntityFactory::FACTORY_TYPE_NAME)
		//	return static_cast<OctreeBoxTriggerEntity*>(obj);
		//if(obj->getMovableType() == OctreeSphereTriggerEntityFactory::FACTORY_TYPE_NAME)
		//	return static_cast<OctreeSphereTriggerEntity*>(obj);
		//if(obj->getMovableType() == OctreeTriggerPlayerFactory::FACTORY_TYPE_NAME)
		//	return static_cast<OctreeTriggerPlayer*>(obj);
		//return 0;
	}
	// �Ƿ����ָ�����ֵĴ�����
	bool OctreeTriggerWorld::hasTriggerEntity(const String &name , const String &triggerType)const
	{
		return m_sceneMgr->hasMovableObject(name , triggerType);
	}	

	// ������ж�����б�
	OctreeTriggerWorld::TriggerEntityIterator OctreeTriggerWorld::getTriggerEntityIterator(const String &triggerType)
	{
		SceneManager::MovableObjectIterator iter = m_sceneMgr->getMovableObjectIterator(triggerType);
		return (TriggerEntityIterator&)iter;
	}


	// ���������ҵ��б�
	OctreeTriggerWorld::TriggerPlayerIterator OctreeTriggerWorld::getTriggerPlayerIterator()
	{
		SceneManager::MovableObjectIterator iter = m_sceneMgr->getMovableObjectIterator(OctreeTriggerPlayerFactory::FACTORY_TYPE_NAME);
		return (TriggerPlayerIterator&)iter;
	}
};
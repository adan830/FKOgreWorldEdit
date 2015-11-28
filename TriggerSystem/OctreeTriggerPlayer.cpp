#include "StdAfx.h"
#include "OctreeTriggerPlayer.h"
#include "OctreeTriggerWorld.h"

namespace Ogre
{
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	String OctreeTriggerPlayerFactory::FACTORY_TYPE_NAME = "OctreeTriggerPlayer";
	//-----------------------------------------------------------------------
	const String& OctreeTriggerPlayerFactory::getType(void) const
	{
		return FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------
	MovableObject* OctreeTriggerPlayerFactory::createInstanceImpl( const String& name,
		const NameValuePairList* params)
	{
		OctreeTriggerWorld *world = 0;
		if (params != 0)
		{
			NameValuePairList::const_iterator ni = params->find("world");
			if (ni != params->end())
			{
				world = (OctreeTriggerWorld*)StringConverter::parseLong(ni->second);
			}

		}
		if (world == 0)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"'world' parameter required when constructing an OctreeTriggerPlayer.",
				"OctreeTriggerPlayerFactory::createInstance");
		}

		return new OctreeTriggerPlayer(name, world);
	}
	//-----------------------------------------------------------------------
	void OctreeTriggerPlayerFactory::destroyInstance( MovableObject* obj)
	{
		delete obj;
	}
	const String& OctreeTriggerPlayer::getMovableType(void) const
	{
		return OctreeTriggerPlayerFactory::FACTORY_TYPE_NAME;
	}

	OctreeTriggerPlayer::OctreeTriggerPlayer(const String &name , OctreeTriggerWorld *world)
		: m_name(name)
		, m_world(world)
		, m_sceneMgr(0)
		, m_node(0)
		, m_radius(1)
		, m_query(0)
		, m_aabb(-0.5 , -0.5 , -0.5 , 0.5 , 0.5 , 0.5)
		, MovableObject(name)
	{
		m_sceneMgr = world->getSceneManager();
		m_node = m_sceneMgr->getRootSceneNode()->createChildSceneNode(name);
		m_node->attachObject(this);
		// ��Ҳ��μ���ײ����
		setQueryFlags(0);
		MovableObject::setUserAny(Any(static_cast<ITriggerEntity*>(this)));

		// ����һ��ȱʡ�İ�Χ�в�ѯ��
		m_query = m_sceneMgr->createAABBQuery(AxisAlignedBox());
	}

	OctreeTriggerPlayer::~OctreeTriggerPlayer(void)
	{
		// ����ǰҲҪ����
		updateTrigger();
		if(m_node)
		{
			m_node->detachObject(this);
			m_sceneMgr->getRootSceneNode()->removeAndDestroyChild(m_node->getName());
			m_node = 0;
		}
		if(m_query)
		{
			m_sceneMgr->destroyQuery(m_query);
			m_query = 0;
		}
	}
	// ��ô���������
	ITriggerWorld *OctreeTriggerPlayer::getTriggerWorld()const
	{
		return m_world;
	}

	// �����������
	void OctreeTriggerPlayer::setPosition(const Vector3 &pos)
	{
		m_node->setPosition(pos);
	}
	// ����������
	const Vector3 &OctreeTriggerPlayer::getPosition()const
	{
		return m_node->getPosition();
	}

	// �����������
	void OctreeTriggerPlayer::setScale(const Vector3 &scale)
	{
		m_node->setScale(scale);
	}
	// ����������
	const Vector3 &OctreeTriggerPlayer::getScale()const
	{
		return m_node->getScale();
	}

	// ������Է�λ
	void OctreeTriggerPlayer::setOrientation(const Quaternion &orientation)
	{
		m_node->setOrientation(orientation);
	}
	// ����������
	const Quaternion &OctreeTriggerPlayer::getOrientation()const
	{
		return m_node->getOrientation();
	}

	// ���´��������ݣ���������ᵼ��һ�α�����ѯ�����ص�
	void OctreeTriggerPlayer::updateTrigger()
	{
		if(m_aabb.isNull())
			return;
		// ���������Χ����Ϣ
		m_obb.setCenter(getPosition());
		m_obb.setExtents(getBoundingBox().getHalfSize() * getScale());
		m_obb.setOrientation(getOrientation());

		m_query->setBox(getWorldBoundingBox(true));
		SceneQueryResult& result = m_query->execute();

		// ���ڷ�������ײ���б�
		MapCollideTriggerEntity mapCollideTriggerEntity;

		// �������
		for(SceneQueryResultMovableList::iterator iter = result.movables.begin() ; iter != result.movables.end() ; iter ++)
		{
			// ��ת����ITriggerEntity
			ITriggerEntity *entity = trigger_cast((*iter)->getUserAny());
			if(entity)
			{
				// ����ײ���㣬���������ײ��
				if(entity->collide(this))
				{				
					// ��ӽ�ȥ
					mapCollideTriggerEntity[entity->getName()] = entity;
				}
			}
		}

		// ����������ײ�Ľ��
		for(MapCollideTriggerEntity::iterator iter = mapCollideTriggerEntity.begin() ; iter != mapCollideTriggerEntity.end() ; iter ++)
		{
			// ����������棬��˵����ȥ��
			if(m_mapCollideTriggerEntity.find(iter->second->getName()) == m_mapCollideTriggerEntity.end())
			{
				// �ص������
				const ITriggerPlayer::ListListener &listPlayer = getListenerList();
				for(size_t i = 0 ; i < listPlayer.size() ; i ++)
				{
					listPlayer[i]->onEnter(iter->second , this);
				}
				// �ص���������
				const ITriggerEntity::ListListener &listEntity = iter->second->getListenerList();
				for(size_t i = 0 ; i < listEntity.size() ; i ++)
				{
					listEntity[i]->onEnter(iter->second , this);
				}
				// �ص�������
				const ITriggerWorld::ListListener &listWorld = m_world->getListenerList();
				for(size_t i = 0 ; i < listWorld.size() ; i ++)
				{
					listWorld[i]->onEnter(iter->second , this);
				}
			}
		}

		// ����δ������ײ�Ľ��
		for(MapCollideTriggerEntity::iterator iter = m_mapCollideTriggerEntity.begin() ; iter != m_mapCollideTriggerEntity.end() ; iter ++)
		{
			// ����������棬��˵����ȥ��
			if(mapCollideTriggerEntity.find(iter->second->getName()) == mapCollideTriggerEntity.end())
			{
				// �ص������
				const ITriggerPlayer::ListListener &listPlayer = getListenerList();
				for(size_t i = 0 ; i < listPlayer.size() ; i ++)
				{
					listPlayer[i]->onLeave(iter->second , this);
				}
				// �ص���������
				const ITriggerEntity::ListListener &listEntity = iter->second->getListenerList();
				for(size_t i = 0 ; i < listEntity.size() ; i ++)
				{
					listEntity[i]->onLeave(iter->second , this);
				}
				// �ص�������
				const ITriggerWorld::ListListener &listWorld = m_world->getListenerList();
				for(size_t i = 0 ; i < listWorld.size() ; i ++)
				{
					listWorld[i]->onLeave(iter->second , this);
				}
			}
		}

		// �����µĽ��
		m_mapCollideTriggerEntity = mapCollideTriggerEntity;
	}

};
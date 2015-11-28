#include "StdAfx.h"
#include "OctreeBoxTriggerEntity.h"
#include "OctreeTriggerWorld.h"
#include "ITriggerPlayer.h"
#include "OctreeTriggerPlayer.h"

namespace Ogre
{

	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	String OctreeBoxTriggerEntityFactory::FACTORY_TYPE_NAME = "OctreeBoxTriggerEntity";
	//-----------------------------------------------------------------------
	const String& OctreeBoxTriggerEntityFactory::getType(void) const
	{
		return FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------
	MovableObject* OctreeBoxTriggerEntityFactory::createInstanceImpl( const String& name,
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
				"'world' parameter required when constructing an OctreeBoxTriggerEntity.",
				"OctreeBoxTriggerEntityFactory::createInstance");
		}

		return new OctreeBoxTriggerEntity(name, world);
	}
	//-----------------------------------------------------------------------
	void OctreeBoxTriggerEntityFactory::destroyInstance( MovableObject* obj)
	{
		delete obj;
	}
	const String& OctreeBoxTriggerEntity::getMovableType(void) const
	{
		return OctreeBoxTriggerEntityFactory::FACTORY_TYPE_NAME;
	}


	OctreeBoxTriggerEntity::OctreeBoxTriggerEntity(const String &name , OctreeTriggerWorld *world)
		: m_name(name)
		, m_world(world)
		, m_sceneMgr(0)
		, m_node(0)
		, m_radius(0)
		, m_aabb(-0.5 , -0.5 , -0.5 , 0.5 , 0.5 , 0.5)
		, MovableObject(name)
	{
		m_sceneMgr = world->getSceneManager();
		m_node = m_sceneMgr->getRootSceneNode()->createChildSceneNode(name);
		m_node->attachObject(this);
		MovableObject::setUserAny(Any(static_cast<ITriggerEntity*>(this)));
	}

	OctreeBoxTriggerEntity::~OctreeBoxTriggerEntity(void)
	{
		if(m_node)
		{
			m_node->detachObject(this);
			m_sceneMgr->getRootSceneNode()->removeAndDestroyChild(m_node->getName());
			m_node = 0;
		}
	}
	// ��ô���������
	ITriggerWorld *OctreeBoxTriggerEntity::getTriggerWorld()const
	{
		return m_world;
	}

	// �����������
	void OctreeBoxTriggerEntity::setPosition(const Vector3 &pos)
	{
		m_node->setPosition(pos);
		m_obb.setCenter(pos);
	}
	// ����������
	const Vector3 &OctreeBoxTriggerEntity::getPosition()const
	{
		return m_node->getPosition();
	}

	// �����������
	void OctreeBoxTriggerEntity::setScale(const Vector3 &scale)
	{
		m_node->setScale(scale);
		m_obb.setExtents(m_aabb.getHalfSize() * scale);
	}
	// ����������
	const Vector3 &OctreeBoxTriggerEntity::getScale()const
	{
		return m_node->getScale();
	}
	// ���ú���
	void OctreeBoxTriggerEntity::setBox(const AxisAlignedBox &box)
	{
		Vector3 size = m_aabb.getHalfSize();
		m_radius = std::max(std::max(size.x , size.z) , size.y);
		m_aabb = box;
		m_obb.setCenter(getPosition());
		m_obb.setExtents(getBoundingBox().getHalfSize() * getScale());
		m_obb.setOrientation(getOrientation());
		m_node->needUpdate();
	}
	// ������Է�λ
	void OctreeBoxTriggerEntity::setOrientation(const Quaternion &orientation)
	{
		m_node->setOrientation(orientation);
		m_obb.setOrientation(orientation);
	}
	// ����������
	const Quaternion &OctreeBoxTriggerEntity::getOrientation()const
	{
		return m_node->getOrientation();
	}

	// �ƶ�ʱ�����������
	void OctreeBoxTriggerEntity::_notifyMoved(void)
	{
		OctreeTriggerWorld::TriggerPlayerIterator iter = m_world->getTriggerPlayerIterator();
		while(iter.hasMoreElements())
		{
			ITriggerPlayer *player = iter.getNext();
			player->updateTrigger();
		}
	}

	// �Ƿ�����ҷ�������ײ
	bool OctreeBoxTriggerEntity::collide(ITriggerPlayer *player)
	{
		const OBB &obbPlayer = static_cast<OctreeTriggerPlayer*>(player)->getOBB();
		return obbPlayer.intersects(m_obb);
	}
};
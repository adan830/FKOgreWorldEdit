#include "StdAfx.h"
#include "OctreeSphereTriggerEntity.h"
#include "OctreeTriggerWorld.h"
#include "ITriggerPlayer.h"
#include "OctreeTriggerPlayer.h"

namespace Ogre
{

	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	String OctreeSphereTriggerEntityFactory::FACTORY_TYPE_NAME = "OctreeSphereTriggerEntity";
	//-----------------------------------------------------------------------
	const String& OctreeSphereTriggerEntityFactory::getType(void) const
	{
		return FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------
	MovableObject* OctreeSphereTriggerEntityFactory::createInstanceImpl( const String& name,
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
				"'world' parameter required when constructing an OctreeSphereTriggerEntity.",
				"OctreeSphereTriggerEntityFactory::createInstance");
		}

		return new OctreeSphereTriggerEntity(name, world);
	}
	//-----------------------------------------------------------------------
	void OctreeSphereTriggerEntityFactory::destroyInstance( MovableObject* obj)
	{
		delete obj;
	}
	const String& OctreeSphereTriggerEntity::getMovableType(void) const
	{
		return OctreeSphereTriggerEntityFactory::FACTORY_TYPE_NAME;
	}

	OctreeSphereTriggerEntity::OctreeSphereTriggerEntity(const String &name , OctreeTriggerWorld *world)
		: m_name(name)
		, m_world(world)
		, m_sceneMgr(0)
		, m_node(0)
		, m_radius(1.0f)
		, MovableObject(name)
	{
		m_sceneMgr = world->getSceneManager();
		m_node = m_sceneMgr->getRootSceneNode()->createChildSceneNode(name);
		m_node->attachObject(this);
		MovableObject::setUserAny(Any(static_cast<ITriggerEntity*>(this)));
	}

	OctreeSphereTriggerEntity::~OctreeSphereTriggerEntity(void)
	{
		if(m_node)
		{
			m_node->detachObject(this);
			m_sceneMgr->getRootSceneNode()->removeAndDestroyChild(m_node->getName());
			m_node = 0;
		}
	}
	// ��ô���������
	ITriggerWorld *OctreeSphereTriggerEntity::getTriggerWorld()const
	{
		return m_world;
	}

	// �����������
	void OctreeSphereTriggerEntity::setPosition(const Vector3 &pos)
	{
		m_node->setPosition(pos);
	}
	// ����������
	const Vector3 &OctreeSphereTriggerEntity::getPosition()const
	{
		return m_node->getPosition();
	}

	// �����������
	void OctreeSphereTriggerEntity::setScale(const Vector3 &scale)
	{
		m_node->setScale(scale);
	}
	// ����������
	const Vector3 &OctreeSphereTriggerEntity::getScale()const
	{
		return m_node->getScale();
	}

	// ������Է�λ
	void OctreeSphereTriggerEntity::setOrientation(const Quaternion &orientation)
	{
	}
	// ����������
	const Quaternion &OctreeSphereTriggerEntity::getOrientation()const
	{
		return Quaternion::IDENTITY;
	}

	// ������İ뾶
	void OctreeSphereTriggerEntity::setRadius(Real radius)
	{
		m_radius = abs(radius);
		m_aabb.setExtents(-m_radius , -m_radius , -m_radius , m_radius , m_radius , m_radius);
		m_node->needUpdate();
	}
	// �����İ뾶
	Real OctreeSphereTriggerEntity::getRadius()const
	{
		return m_radius;
	}
	// �ƶ�ʱ�����������
	void OctreeSphereTriggerEntity::_notifyMoved(void)
	{
		OctreeTriggerWorld::TriggerPlayerIterator iter = m_world->getTriggerPlayerIterator();
		while(iter.hasMoreElements())
		{
			ITriggerPlayer *player = iter.getNext();
			player->updateTrigger();
		}
	}
	// �Ƿ�����ҷ�������ײ
	bool OctreeSphereTriggerEntity::collide(ITriggerPlayer *player)
	{
		const OBB &obbPlayer(static_cast<OctreeTriggerPlayer*>(player)->getOBB());
		return obbPlayer.intersects(Sphere(getPosition() , m_radius * getScale().x));
	}
};
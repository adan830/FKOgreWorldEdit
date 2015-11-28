#include "StdAfx.h"
#include "OpcodeCollisionEntity.h"
#include "ICollisionSystem.h"
#include "ICollisionWorld.h"

namespace Ogre
{
	OpcodeCollisionEntity::OpcodeCollisionEntity(ICollisionWorld *world)
		: m_entity(0)
		, m_world(world)
	{

	}
	OpcodeCollisionEntity::~OpcodeCollisionEntity(void)
	{
		destroy();
	}

	// ��ʼ��
	bool OpcodeCollisionEntity::init(Entity *entity , CollisionShapeType type , const String &shapeName)
	{
		destroy();

		m_entity = entity;
		loadCollisionShape(type , shapeName);

		return true;
	}
	// ����
	void OpcodeCollisionEntity::destroy()
	{
		unloadCollisionShape();
		m_entity = 0;
	}

	// ����ray�ͱ�ʵ��֮�����ײ��������ײ��Ϣ������true������ײ
	bool OpcodeCollisionEntity::collide(const Ray& ray , RayCollisionInfo& collisionInfo)
	{
		if(!m_entity || !m_entity->getParentSceneNode())
			return false;
		Matrix4 mat;
		m_entity->getParentSceneNode()->getWorldTransforms(&mat);
		return m_world->collide(ray , m_shape , mat , collisionInfo);
	}


	// ������ײģ������
	void OpcodeCollisionEntity::setCollisionShapeType(CollisionShapeType type)
	{
		loadCollisionShape(type , m_collisionName);
	}

	// ������ײģ��
	void OpcodeCollisionEntity::setCollisionShapeName(const String &name)
	{
		loadCollisionShape(m_collisionType , name);
	}

	// ������ײģ��
	void OpcodeCollisionEntity::loadCollisionShape(CollisionShapeType type , const String &name)
	{
		if(m_collisionType == type && m_collisionName == name)
			return;
		unloadCollisionShape();

		switch(type)
		{
		case CT_SELF_MESH:
			m_shape = ICollisionSystem::getSingleton().createMeshCollisionShape(m_entity->getMesh());
			break;
		case CT_SELF_SUBMESH:
			m_shape = ICollisionSystem::getSingleton().createSubMeshCollisionShape(m_entity->getMesh() , name);
			break;
		case CT_OTHER_MESH:
			m_shape = ICollisionSystem::getSingleton().createMeshCollisionShape(name);
			break;
		}

		// ��������ֵ
		m_collisionType = type;
		m_collisionName = name;
	}

	// ������ײģ��
	void OpcodeCollisionEntity::unloadCollisionShape()
	{
		if(!m_shape.isNull())
		{
			ICollisionSystem::getSingleton().destroyCollisionShape(m_shape);
			m_collisionType = CT_NONE;
			m_collisionName.clear();
		}
	}

};
#include "StdAfx.h"
#include "OpcodeCollisionSystem.h"
#include "OpcodeCollisionMeshShape.h"
#include "OpcodeCollisionWorld.h"
#include "OpcodeCollisionSubMeshShape.h"
#include "OpcodeCollisionVertexShape.h"
namespace Ogre
{
	OpcodeCollisionSystem::OpcodeCollisionSystem(void)
	{
	}

	OpcodeCollisionSystem::~OpcodeCollisionSystem(void)
	{
		m_collisionShapeMap.clear();
		for(CollisionWorldMap::iterator iter = m_worldMap.begin() ; iter != m_worldMap.end() ; iter ++)
		{
			delete iter->second;
		}
		m_worldMap.clear();
	}

	// ���һ����ײģ�͵���ײϵͳ��
	ICollisionShapePtr OpcodeCollisionSystem::getCollisionShape(const String &type , const String& name)
	{
		CollisionShapeMapMap::iterator itr = m_collisionShapeMap.find(type);
		if(itr != m_collisionShapeMap.end())
		{
			CollisionShapeMap::iterator iter = itr->second.find(name);
			if(iter != itr->second.end())
			{
				return iter->second;
			}
			else
			{
				return ICollisionShapePtr();
			}
		}
		else
		{
			return ICollisionShapePtr();
		}
	}

	// �������ƻ����ײģ��
	void OpcodeCollisionSystem::addCollisionShape(ICollisionShapePtr& shape)
	{
		m_collisionShapeMap[shape->getType()][shape->getName()] = shape;
	}


	// ��һ��Mesh������ײģ��
	ICollisionShapePtr OpcodeCollisionSystem::createMeshCollisionShape(const String& meshName , const Vector3& position, const Quaternion& orient, const Vector3 &scale)
	{
		//check if a shape with this name already exists
		ICollisionShapePtr shape = getCollisionShape(OpcodeCollisionMeshShape::SHAPE_TYPE , meshName);

		if(shape.isNull())
		{
			shape.bind(new OpcodeCollisionMeshShape());
			if(!static_cast<OpcodeCollisionMeshShape*>(shape.getPointer())->init(meshName , position , orient , scale))
			{
				shape.setNull();
				return ICollisionShapePtr();
			}
			addCollisionShape(shape);
		}

		return shape;
	}


	// ��һ��Mesh������ײģ��
	ICollisionShapePtr OpcodeCollisionSystem::createMeshCollisionShape(const MeshPtr &mesh , const Vector3& position, const Quaternion& orient, const Vector3 &scale)
	{
		return createMeshCollisionShape(mesh->getName() , position , orient , scale);
	}


	// ��һ��Mesh������ײģ��
	ICollisionShapePtr OpcodeCollisionSystem::createSubMeshCollisionShape(const MeshPtr &mesh , const String &subMeshName , const Vector3& position, const Quaternion& orient, const Vector3 &scale)
	{
		return createSubMeshCollisionShape(mesh->getName() , subMeshName , position , orient , scale);
	}

	// ��һ��Mesh������ײģ��
	ICollisionShapePtr OpcodeCollisionSystem::createSubMeshCollisionShape(const String &meshName , const String &subMeshName , const Vector3& position, const Quaternion& orient, const Vector3 &scale)
	{
		//check if a shape with this name already exists
		ICollisionShapePtr shape = getCollisionShape(OpcodeCollisionSubMeshShape::SHAPE_TYPE , meshName + "_" + subMeshName);

		if(shape.isNull())
		{
			shape.bind(new OpcodeCollisionSubMeshShape());
			if(!static_cast<OpcodeCollisionSubMeshShape*>(shape.getPointer())->init(meshName , subMeshName , position , orient , scale))
			{
				shape.setNull();
				return ICollisionShapePtr();
			}
			addCollisionShape(shape);
		}

		return shape;
	}

	// ��һ�����������������������ײģ��
	ICollisionShapePtr OpcodeCollisionSystem::createVertexCollisionShape(const String &name , const Vector3 *vertexBuffer , size_t vertexCount , bool autoDeleteVertex , const uint16 *indexBuffer , size_t indexCount , bool autoDeleteIndex)
	{
		//check if a shape with this name already exists
		ICollisionShapePtr shape = getCollisionShape(OpcodeCollisionVertexShape::SHAPE_TYPE , name);

		if(shape.isNull())
		{
			shape.bind(new OpcodeCollisionVertexShape());
			if(!static_cast<OpcodeCollisionVertexShape*>(shape.getPointer())->init(name , vertexBuffer , vertexCount , autoDeleteVertex , indexBuffer , indexCount , autoDeleteVertex))
			{
				shape.setNull();
				return ICollisionShapePtr();
			}
			addCollisionShape(shape);
		}

		return shape;
	}

	// ������ײģ��
	void OpcodeCollisionSystem::destroyCollisionShape(ICollisionShapePtr &shape)
	{
		CollisionShapeMapMap::iterator itr = m_collisionShapeMap.find(shape->getType());
		if(itr != m_collisionShapeMap.end())
		{
			CollisionShapeMap::iterator iter = itr->second.find(shape->getName());
			if(iter != itr->second.end())
			{
				shape.setNull();
				// ���һ������ɾ��֮
				if(iter->second.unique())
				{
					itr->second.erase(iter);
				}
			}
		}
	}



	// ����һ����ײ����
	ICollisionWorld *OpcodeCollisionSystem::createWorld(const String &name)
	{
		if(hasWorld(name))
		{
			return 0;
		}
		ICollisionWorld *world = new OpcodeCollisionWorld(name);
		m_worldMap[name] = world;
		return world;
	}

	// ���һ����ײ����
	ICollisionWorld *OpcodeCollisionSystem::getWorld(const String &name)
	{
		CollisionWorldMap::iterator iter = m_worldMap.find(name);
		if(iter != m_worldMap.end())
		{
			return iter->second;
		}
		else
		{
			return 0;
		}
	}

	// �Ƿ���������Ƶ���ײ����
	bool OpcodeCollisionSystem::hasWorld(const String &name)
	{
		return m_worldMap.find(name) != m_worldMap.end();
	}

	// ����һ����ײ����
	void OpcodeCollisionSystem::destroyWorld(const String &name)
	{
		CollisionWorldMap::iterator iter = m_worldMap.find(name);
		if(iter != m_worldMap.end())
		{
			delete iter->second;
			m_worldMap.erase(iter);
		}
	}

	// ����һ����ײ����
	void OpcodeCollisionSystem::destroyWorld(ICollisionWorld *world)
	{
		destroyWorld(world->getName());
	}

};
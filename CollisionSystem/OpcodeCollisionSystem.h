#pragma once
#include "ICollisionSystem.h"

namespace Ogre
{
	// ��ײϵͳʵ��
	class OpcodeCollisionSystem : public ICollisionSystem
	{
	public:
		OpcodeCollisionSystem(void);
		virtual ~OpcodeCollisionSystem(void);


		// ��һ��Mesh������ײģ��
		virtual ICollisionShapePtr createMeshCollisionShape(const String& meshName, const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE);
		// ��һ��Mesh������ײģ��
		virtual ICollisionShapePtr createMeshCollisionShape(const MeshPtr &mesh, const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE);
		// ��һ��SubMesh������ײģ��
		virtual ICollisionShapePtr createSubMeshCollisionShape(const MeshPtr &mesh , const String &subMeshName , const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE);
		// ��һ��SubMesh������ײģ��
		virtual ICollisionShapePtr createSubMeshCollisionShape(const String& meshName, const String &subMeshName , const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE);
		// ��һ�����������������������ײģ��
		virtual ICollisionShapePtr createVertexCollisionShape(const String &name , const Vector3 *vertexBuffer , size_t vertexCount , bool autoDeleteVertex , const uint16 *indexBuffer , size_t indexCount , bool autoDeleteIndex);

		// ������ײģ��
		virtual void destroyCollisionShape(ICollisionShapePtr& shape);

		// ����һ����ײ����
		virtual ICollisionWorld *createWorld(const String &name);
		// ���һ����ײ����
		virtual ICollisionWorld *getWorld(const String &name);
		// �Ƿ���������Ƶ���ײ����
		virtual bool hasWorld(const String &name);
		// ����һ����ײ����
		virtual void destroyWorld(const String &name);
		// ����һ����ײ����
		virtual void destroyWorld(ICollisionWorld *world);

	protected:

		// �������ƻ����ײģ��
		ICollisionShapePtr getCollisionShape(const String &type , const String& name);
		// ���һ����ײģ�͵���ײϵͳ��
		void addCollisionShape(ICollisionShapePtr& shape);

		typedef std::map<String, ICollisionShapePtr> CollisionShapeMap;
		typedef std::map<String, CollisionShapeMap> CollisionShapeMapMap;
		CollisionShapeMapMap m_collisionShapeMap;


		typedef std::map<String , ICollisionWorld*> CollisionWorldMap;
		CollisionWorldMap m_worldMap;

	};
};
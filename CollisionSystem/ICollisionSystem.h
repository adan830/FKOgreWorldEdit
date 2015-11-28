#pragma once
#include "ICollisionShape.h"
namespace Ogre
{
	class ICollisionWorld;
	// ��ײϵͳ�ӿ�
	class COLLISIONSYSTEM_API ICollisionSystem : public Singleton<ICollisionSystem>
	{
	public:
		ICollisionSystem(void){};
		virtual~ICollisionSystem(void){};

		// ����ģʽ�ӿ�
		static ICollisionSystem& getSingleton(void);
		static ICollisionSystem* getSingletonPtr(void);

		// ��ʼ����ײϵͳ
		static void init(SceneManager *sceneMgr);
		// ������ײϵͳ
		static void shutdown();

		// ��һ��Mesh������ײģ��
		virtual ICollisionShapePtr createMeshCollisionShape(const String& meshName, const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE) = 0;
		// ��һ��Mesh������ײģ��
		virtual ICollisionShapePtr createMeshCollisionShape(const MeshPtr &mesh, const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE) = 0;
		// ��һ��SubMesh������ײģ��
		virtual ICollisionShapePtr createSubMeshCollisionShape(const MeshPtr &mesh , const String &subMeshName , const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE) = 0;
		// ��һ��SubMesh������ײģ��
		virtual ICollisionShapePtr createSubMeshCollisionShape(const String& meshName, const String &subMeshName , const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE) = 0;
		// ��һ�����������������������ײģ��
		virtual ICollisionShapePtr createVertexCollisionShape(const String &name , const Vector3 *vertexBuffer , size_t vertexCount , bool autoDeleteVertex , const uint16 *indexBuffer , size_t indexCount , bool autoDeleteIndex) = 0;

		// ������ײģ��
		virtual void destroyCollisionShape(ICollisionShapePtr &shape) = 0;

		// ����һ����ײ����
		virtual ICollisionWorld *createWorld(const String &name) = 0;
		// ���һ����ײ����
		virtual ICollisionWorld *getWorld(const String &name) = 0;
		// �Ƿ���������Ƶ���ײ����
		virtual bool hasWorld(const String &name) = 0;
		// ����һ����ײ����
		virtual void destroyWorld(const String &name) = 0;
		// ����һ����ײ����
		virtual void destroyWorld(ICollisionWorld *world) = 0;
	};
};
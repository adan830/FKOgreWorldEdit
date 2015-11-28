#pragma once
#include "ICollisionShape.h"
namespace Ogre
{
	// ��ײģ������
	enum CollisionShapeType
	{
		CT_NONE = 0,		// û����ײģ��
		CT_SELF_MESH,		// ��Entity��Mesh
		CT_SELF_SUBMESH,	// ��Entity��Mesh�е�һ��SubMesh
		CT_OTHER_MESH,		// �ⲿMesh

		CT_UNKNOWN = 0xFFFF,// δ֪����
	};
	class ICollisionWorld;
	// ��ײʵ��
	class COLLISIONSYSTEM_API ICollisionEntity
	{
	public:
		ICollisionEntity(void){}
		virtual ~ICollisionEntity(void){}

		// �Ƿ��Ѿ���ʼ��
		virtual bool isInited()const = 0;

		// �����ײģ��
		virtual ICollisionShapePtr &getCollisionShape() = 0;

		// ����ray�ͱ�ʵ��֮�����ײ��������ײ��Ϣ������true������ײ
		virtual bool collide(const Ray& ray , RayCollisionInfo& collisionInfo) = 0;

		// �����ײ����
		virtual ICollisionWorld *getCollisionWorld()const = 0;

		// ������ײģ������
		virtual void setCollisionShapeType(CollisionShapeType type) = 0;
		// �����ײģ������
		virtual CollisionShapeType getCollisionShapeType()const = 0;
		// ������ײģ��
		virtual void setCollisionShapeName(const String &name) = 0;
		// �����ײģ������
		virtual const String &getCollisionShapeName()const = 0;

	};
};
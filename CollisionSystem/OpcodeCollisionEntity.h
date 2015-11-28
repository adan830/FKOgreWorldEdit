#pragma once
#include "ICollisionEntity.h"
namespace Ogre
{

	// ��ײʵ���ʵ��
	class OpcodeCollisionEntity : public ICollisionEntity
	{
	public:
		OpcodeCollisionEntity(ICollisionWorld *world);
		virtual ~OpcodeCollisionEntity(void);

		// ��ʼ��
		bool init(Entity *entity , CollisionShapeType type , const String &shapeName);
		// ����
		void destroy();

		// �Ƿ��Ѿ���ʼ��
		bool isInited()const
		{
			return m_entity && m_entity->getParentSceneNode() && !m_shape.isNull();
		}


		// �����ײģ��
		virtual ICollisionShapePtr &getCollisionShape()
		{
			return m_shape;
		}

		// ����ray�ͱ�ʵ��֮�����ײ��������ײ��Ϣ������true������ײ
		virtual bool collide(const Ray& ray , RayCollisionInfo& collisionInfo);

		// �����ײ����
		virtual ICollisionWorld *getCollisionWorld()const
		{
			return m_world;
		};

		// ������ײģ������
		void setCollisionShapeType(CollisionShapeType type);
		// �����ײģ������
		CollisionShapeType getCollisionShapeType()const
		{
			return m_collisionType;
		}
		// ������ײģ��
		void setCollisionShapeName(const String &name);
		// �����ײģ������
		const String &getCollisionShapeName()const
		{
			return m_collisionName;
		}
	protected:
		// ������ײģ��
		void loadCollisionShape(CollisionShapeType type , const String &name);
		// ������ײģ��
		void unloadCollisionShape();

		Entity *m_entity;
		ICollisionShapePtr m_shape;
		ICollisionWorld *m_world;
		// ��ײ������
		CollisionShapeType m_collisionType;
		// ��ײģ������
		String m_collisionName;

	};

};
#pragma once
#include "ICollisionWorld.h"
namespace Opcode
{
	class AABBTreeCollider;
	struct BVTCache;
	class RayCollider;
	class CollisionFaces;
}

namespace IceMaths
{
	class Matrix4x4;
	class Point;
}

namespace Ogre
{
	class OpcodeCollisionWorld : public ICollisionWorld
	{
	public:
		OpcodeCollisionWorld(const String &name);
		virtual ~OpcodeCollisionWorld(void);


		// �������
		virtual const String &getName()const
		{
			return m_name;
		}

		// ����ray��col֮�����ײ��������ײ��Ϣ������true������ײ
		virtual bool collide(const Ray& ray
			, const ICollisionShapePtr& col
			, const Matrix4& worldTrans
			, RayCollisionInfo& collisionInfo
			);

		// ��һ��Entity������ײʵ��
		virtual ICollisionEntity* createEntityCollisionEntity(Entity *entity , CollisionShapeType type = CT_NONE , const String &shapeName = StringUtil::BLANK);
		// ������ײʵ��
		virtual void destroyCollisionEntity(ICollisionEntity *entity);

	protected:
		String m_name;
		// ��Ogre����ת����OpCode����
		IceMaths::Matrix4x4* getIceMathsMatrix(const Matrix4& matrix) const;

		Opcode::AABBTreeCollider* mCollider; //opcode collider
		Opcode::BVTCache* mColCache; //opcode cache
		Opcode::RayCollider* mRayCollider; //ray collider
		Opcode::CollisionFaces* mRayColfaces;	//ray collision faces

	};

};
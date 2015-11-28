#pragma once
#include "ICollisionShape.h"
#include "ICollisionEntity.h"

namespace Ogre
{
	class ICollisionWorld
	{
	public:
		ICollisionWorld(){};
		virtual ~ICollisionWorld(void){};

		// �������
		virtual const String &getName()const = 0;

		// ����ray��col֮�����ײ��������ײ��Ϣ������true������ײ
		virtual bool collide(const Ray& ray
			, const ICollisionShapePtr& col
			, const Matrix4& worldTrans
			, RayCollisionInfo& collisionInfo
			) = 0;

		// ��һ��Entity������ײʵ��
		virtual ICollisionEntity* createEntityCollisionEntity(Entity *entity , CollisionShapeType type = CT_NONE , const String &shapeName = StringUtil::BLANK) = 0;
		// ������ײʵ��
		virtual void destroyCollisionEntity(ICollisionEntity *entity) = 0;
	};
};
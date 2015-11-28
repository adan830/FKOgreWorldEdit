#pragma once

namespace Ogre
{
	// ��ײ��Ϣ
	struct RayCollisionInfo
	{
		RayCollisionInfo()
			: distance(0.0f)
			, a1(Vector3::ZERO)
			, b1(Vector3::ZERO)
			, c1(Vector3::ZERO)
			, a2(Vector3::ZERO)
			, b2(Vector3::ZERO)
			, c2(Vector3::ZERO)
		{

		}
		float distance;				// �����ߵ���ײ����
		Vector3 a1, b1, c1;			// ��һ����ײ������
		Vector3 a2, b2, c2;			// �ڶ�����ײ������
	};

	// ��ײģ��
	class COLLISIONSYSTEM_API ICollisionShape
	{
	public:
		ICollisionShape(void){};
		virtual ~ICollisionShape(void){};

		// �������
		virtual const String &getName()const = 0;
		// �������
		virtual const String &getType()const = 0;

	};
	typedef SharedPtr<ICollisionShape> ICollisionShapePtr;
};
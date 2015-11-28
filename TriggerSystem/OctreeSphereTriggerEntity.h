#pragma once
#include "ISphereTriggerEntity.h"

namespace Ogre
{
	class OctreeTriggerWorld;
	// ���崥������ʵ��
	class OctreeSphereTriggerEntity : public ISphereTriggerEntity
		, public MovableObject
	{
	public:
		OctreeSphereTriggerEntity(const String &name , OctreeTriggerWorld *world);
		virtual ~OctreeSphereTriggerEntity(void);

		// �������
		virtual const String &getName()const
		{
			return m_name;
		}
		// �������
		virtual const String &getType()const
		{
			return getMovableType();
		};
		// ��ô���������
		virtual ITriggerWorld *getTriggerWorld()const;

		// �����������
		virtual void setPosition(const Vector3 &pos);
		// ����������
		virtual const Vector3 &getPosition()const;

		// �����������
		virtual void setScale(const Vector3 &scale);
		// ����������
		virtual const Vector3 &getScale()const;


		// ������Է�λ
		virtual void setOrientation(const Quaternion &orientation);
		// ����������
		virtual const Quaternion &getOrientation()const;

		// ������İ뾶
		virtual void setRadius(Real radius);
		// �����İ뾶
		virtual Real getRadius()const;


		virtual const String& getMovableType(void) const;
		virtual const AxisAlignedBox& getBoundingBox(void) const
		{
			return m_aabb;
		}
		virtual Real getBoundingRadius(void) const
		{
			return m_radius;
		}
		virtual void _updateRenderQueue(RenderQueue* queue){};
		// �ƶ�ʱ�����������
		virtual void _notifyMoved(void);
		// �Ƿ�����ҷ�������ײ
		virtual bool collide(ITriggerPlayer *player);

	protected:
		// ����
		String m_name;
		// �˲�������
		OctreeTriggerWorld *m_world;
		// ����������
		SceneManager *m_sceneMgr;

		// �˲����ڵ�
		SceneNode *m_node;
		// ��Χ��
		AxisAlignedBox m_aabb;
		// ����뾶
		float m_radius;
	};

	/** Factory object for creating Entity instances */
	class OctreeSphereTriggerEntityFactory : public MovableObjectFactory
	{
	protected:
		MovableObject* createInstanceImpl( const String& name, const NameValuePairList* params);
	public:
		OctreeSphereTriggerEntityFactory() {}
		virtual ~OctreeSphereTriggerEntityFactory() {}

		static String FACTORY_TYPE_NAME;

		const String& getType(void) const;
		void destroyInstance( MovableObject* obj);

	};
};
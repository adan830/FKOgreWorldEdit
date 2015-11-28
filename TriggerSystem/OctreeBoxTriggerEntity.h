#pragma once
#include "IBoxTriggerEntity.h"

namespace Ogre
{
	class OctreeTriggerWorld;
	// ���Ӵ�������ʵ��
	class OctreeBoxTriggerEntity : public IBoxTriggerEntity
		, public MovableObject
	{
	public:
		OctreeBoxTriggerEntity(const String &name , OctreeTriggerWorld *world);
		virtual ~OctreeBoxTriggerEntity(void);

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

		// ���ú���
		virtual void setBox(const AxisAlignedBox &box);
		// ��ú���
		virtual const AxisAlignedBox &getBox()
		{
			return m_aabb;
		}

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

		// ��Χ�뾶
		Real m_radius;
		// ��Χ��
		AxisAlignedBox m_aabb;

		// �����Χ��
		OBB m_obb;
	};

	/** Factory object for creating Entity instances */
	class OctreeBoxTriggerEntityFactory : public MovableObjectFactory
	{
	protected:
		MovableObject* createInstanceImpl( const String& name, const NameValuePairList* params);
	public:
		OctreeBoxTriggerEntityFactory() {}
		virtual ~OctreeBoxTriggerEntityFactory() {}

		static String FACTORY_TYPE_NAME;

		const String& getType(void) const;
		void destroyInstance( MovableObject* obj);

	};
};
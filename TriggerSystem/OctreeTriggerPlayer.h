#pragma once
#include "ITriggerPlayer.h"

namespace Ogre
{
	class OctreeTriggerWorld;
	// ��ҵ�ʵ��
	class OctreeTriggerPlayer : public ITriggerPlayer
		, public MovableObject
	{
	public:
		OctreeTriggerPlayer(const String &name , OctreeTriggerWorld *world);
		virtual ~OctreeTriggerPlayer(void);

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
		virtual void setBox(const AxisAlignedBox &box)
		{
			Vector3 size = m_aabb.getHalfSize();
			m_radius = std::max(std::max(size.x , size.z) , size.y);
			m_aabb = box;
		}
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

		// ���´��������ݣ���������ᵼ��һ�α�����ѯ�����ص�
		virtual void updateTrigger();


		// ���������֮������ײ�Ĵ�����
		virtual CollideTriggerEntityIterator getCollideTriggerEntityIterator()
		{
			return CollideTriggerEntityIterator(m_mapCollideTriggerEntity.begin() , m_mapCollideTriggerEntity.end());
		}


		// �Ƿ�����ҷ�������ײ
		virtual bool collide(ITriggerPlayer *player)
		{
			return true;
		}

		// ���������Χ��
		const OBB &getOBB()const
		{
			return m_obb;
		}

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
		// ��֮������ײ�Ĵ������б�
		MapCollideTriggerEntity m_mapCollideTriggerEntity;

		// �����Χ��
		OBB m_obb;

		// ��Χ�в�ѯ
		AxisAlignedBoxSceneQuery* m_query;
	};


	/** Factory object for creating Entity instances */
	class OctreeTriggerPlayerFactory : public MovableObjectFactory
	{
	protected:
		MovableObject* createInstanceImpl( const String& name, const NameValuePairList* params);
	public:
		OctreeTriggerPlayerFactory() {}
		virtual ~OctreeTriggerPlayerFactory() {}

		static String FACTORY_TYPE_NAME;

		const String& getType(void) const;
		void destroyInstance( MovableObject* obj);

	};
};
/******************************************************************
** �ļ���:	DecalSystem.h
** ��  Ȩ:	(C)  
** ������:	FreeKnight(������)
** ��  ��:	2008-1-10 9:53:30
** ��  ��:
******************************************************************/
#pragma once
#include "IDecalSystem.h"
namespace Ogre
{
// �������У��ڵ���֮�����֮ǰ
#define RENDER_QUEUE_DECAL (RENDER_QUEUE_3)

	class SimpleDecal : public IDecal
	{
	public:
		SimpleDecal(SceneManager *mgr , const String &name);
		virtual ~SimpleDecal();

		// ����������������
		virtual const String &getName()const;

		// ��ó���������
		virtual SceneManager* getSceneManager()const
		{
			return m_sceneMgr;
		}

		// ����������������
		virtual void setMaterialName(const String &materialName);
	
		// ���������������
		virtual const String &getMaterialName()const;

		// ������������λ��
		virtual void setPosition(const Vector3 &pos);
		// ��ȡ��������λ��
		virtual const Vector3 &getPosition()const;

		// ����������С
		virtual void setSize(const Vector2 &size);
		// ��ȡ������С
		virtual const Vector2 &getSize()const;

		// ��ȡ�����İ�Χ�У��������꣩
		virtual const AxisAlignedBox &getBoundingBox()const;

		// ��ȡ�����������Χ�У��������꣩
		virtual const AxisAlignedBox &getWorldBoundingBox()const;

		// ������������ȣ�ָ�������ĸ߶�
		virtual void setDepth(float depth);
		// ��ȡ�������
		virtual float getDepth()const;

		// ����������Y�����ת��
		virtual void setRotate(const Radian &angle);
		// ��ȡ������Y�����ת��
		virtual const Radian &getRotate()const;

		// ����������������
		virtual void setVisible(bool visible);
		// ��������Ƿ���ʾ
		virtual bool isVisible()const;


		// ���ùҽ��������ƫ��
		virtual void setAttachObjectsOffset(const Vector3 &offsetPosition = Vector3::ZERO
			, const Quaternion &offsetOrientation = Quaternion::IDENTITY
			, const Vector3 &offsetScale = Vector3(1.0f , 1.0f , 1.0f)
			);
		// ��ùҽ����������ת
		virtual const Quaternion &getOffsetOrientation()const;
		// ��ùҽ��������λ��
		virtual const Vector3 &getOffsetPosition()const;
		// ��ùҽ������������
		virtual const Vector3 &getOffsetScale()const;

		// �ҽ�һ��������������ĵ���
		virtual void attachObject(MovableObject *obj);
		// ȡ���ҽ����
		virtual void dettachObject(MovableObject *obj);
		// ����������ȡ���ҽ����
		virtual void dettachObject(ushort index);
		// ȡ���ҽ��������
		virtual void detachAllObjects();

		// ���������Ż�ùҽ��˵����
		virtual MovableObject* getAttachObject(ushort index)const;
		// ����ѹҽӵ����ö��
		virtual SceneNode::ObjectIterator getAttachedObjectIterator(void);

		// ÿ֡��������
		virtual void update(float timeSinceLastFrame);

		// ��������ڵ�
		virtual SceneNode* getSceneNode()
		{
			return m_node;
		}
		// ���������������Ľڵ�
		virtual SceneNode* getAttachObjectSceneNode()
		{
			return m_nodeAttachObject;
		}
	protected:
		// �����Ըı��ˣ���Ҫ�ع�������ʱ��
		virtual void updateDecal() = 0;

		// ��������
		String m_name;

		// ��������Ⱦ�ڵ�
		SceneNode *m_node;
		// �����ҽ�����Ľڵ�
		SceneNode *m_nodeAttachObject;
		// ʹ�õĳ���������
		SceneManager *m_sceneMgr;

		// ��������
		String m_materialName;
		// ��������λ��
		Vector3 m_position;
		// ������С
		Vector2 m_size;
		// ��������ȣ�ָ�������ĸ߶�
		float m_depth;
		// ������Y�����ת��
		Radian m_rotate;
		// ������������
		bool m_visible;

		// �ҽ����������ת
		Quaternion m_offsetOrientation;
		// �ҽ��������λ��
		Vector3 m_offsetPosition;
		// �ҽ������������
		Vector3 m_offsetScale;

		// �漰�����������Ա��ı���
		enum DIRTY_FLAGS
		{
			DIRTY_MATERIAL_NAME = 1 << 0,
			DIRTY_POSITION		= 1 << 1,
			DIRTY_SIZE			= 1 << 2,
			DIRTY_DEPTH			= 1 << 3,
			DIRTY_ROTATE		= 1 << 4,
			DIRTY_VISIBLE		= 1 << 5,
			DIRTY_OFFSET		= 1 << 6,
			DIRTY_MAX_COUNT		= 7,
		};
		// �������Ըı䣬DIRTY_FLAGS��Ǽ�
		ulong m_dirty;

		// ���ذ�Χ��
		mutable AxisAlignedBox m_aabb;
		mutable bool m_aabbNeedUpdate;
		// �����Χ��
		mutable AxisAlignedBox m_worldAABB;
		mutable bool m_worldAABBNeedUpdate;
	};

};
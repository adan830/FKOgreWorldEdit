/******************************************************************
** �ļ���:	DecalSystem.cpp
** ��  Ȩ:	(C)  
** ������:	FreeKnight(������)
** ��  ��:	2008-1-10 9:53:29
** ��  ��:
******************************************************************/
#include "stdafx.h"
#include "SimpleDecal.h"

 
namespace Ogre
{
	SimpleDecal::SimpleDecal(SceneManager *mgr , const String &name)
		: m_name(name)
		, m_sceneMgr(mgr)
		, m_node(0)
		, m_nodeAttachObject(0)
		, m_position(Vector3::ZERO)
		, m_size(Vector2::ZERO)
		, m_depth(1)
		, m_rotate(0)
		, m_offsetOrientation(Quaternion::IDENTITY)
		, m_offsetPosition(Vector3::ZERO)
		, m_offsetScale(1.0f , 1.0f , 1.0f)
		, m_visible(false)
		, m_dirty(0xFFFFFFFF)
		, m_aabbNeedUpdate(true)
		, m_worldAABBNeedUpdate(true)
	{
		m_node = m_sceneMgr->getRootSceneNode()->createChildSceneNode(name);
	}

	SimpleDecal::~SimpleDecal()
	{
		if(m_nodeAttachObject)
		{
			m_sceneMgr->destroySceneNode(m_nodeAttachObject->getName());
		}
		if(m_node)
		{
			m_sceneMgr->destroySceneNode(m_node->getName());
		}
	}

	// ����������������
	const String &SimpleDecal::getName()const
	{
		return m_name;
	}

	// ����������������
	void SimpleDecal::setMaterialName(const String &materialName)
	{
		if(m_materialName != materialName)
		{
			m_dirty |= DIRTY_MATERIAL_NAME;
		}
		m_materialName = materialName;
	}

	// ���������������
	const String &SimpleDecal::getMaterialName()const
	{
		return m_materialName;
	}

	// ������������λ��
	void SimpleDecal::setPosition(const Vector3 &pos)
	{
		if(m_position != pos)
		{
			m_dirty |= DIRTY_POSITION;
			m_aabbNeedUpdate = true;
		}
		m_position = pos;
	}

	// ��ȡ��������λ��
	const Vector3 &SimpleDecal::getPosition()const
	{
		return m_position;
	}


	// ����������С
	void SimpleDecal::setSize(const Vector2 &size)
	{
		if(m_size != size)
		{
			m_dirty |= DIRTY_SIZE;
			m_aabbNeedUpdate = true;
			m_worldAABBNeedUpdate = true;
		}
		m_size = size;
	}

	// ��ȡ������С
	const Vector2 &SimpleDecal::getSize()const
	{
		return m_size;
	}


	// ������������ȣ�ָ�������ĸ߶�
	void SimpleDecal::setDepth(float depth)
	{
		if(m_depth != depth)
		{
			m_dirty |= DIRTY_DEPTH;
			m_aabbNeedUpdate = true;
			m_worldAABBNeedUpdate = true;
		}
		m_depth = depth;
	}

	// ��ȡ�������
	float SimpleDecal::getDepth()const
	{
		return m_depth;
	}


	// ����������Y�����ת��
	void SimpleDecal::setRotate(const Radian &angle)
	{
		if(m_rotate != angle)
		{
			m_dirty |= DIRTY_ROTATE;
			m_aabbNeedUpdate = true;
			m_worldAABBNeedUpdate = true;
		}
		m_rotate = angle;
	}

	// ��ȡ������Y�����ת��
	const Radian &SimpleDecal::getRotate()const
	{
		return m_rotate;
	}


	// ����������������
	void SimpleDecal::setVisible(bool visible)
	{
		if(m_visible != visible)
		{
			m_dirty |= DIRTY_VISIBLE;
		}
		m_visible = visible;
	}
 
	// ��������Ƿ���ʾ
	bool SimpleDecal::isVisible()const
	{
		return m_visible;
	}


	// �ҽ�һ��������������ĵ���
	void SimpleDecal::attachObject(MovableObject *obj)
	{
		if(m_nodeAttachObject == 0)
		{
			m_nodeAttachObject = m_node->createChildSceneNode(m_name + "_Attach");
		}
		m_nodeAttachObject->attachObject(obj);
	}

	// ȡ���ҽ����
	void SimpleDecal::dettachObject(MovableObject *obj)
	{
		if(m_nodeAttachObject)
		{
			m_nodeAttachObject->detachObject(obj);
		}
	}

	// ����������ȡ���ҽ����
	void SimpleDecal::dettachObject(ushort index)
	{
		if(m_nodeAttachObject)
		{
			m_nodeAttachObject->detachObject(index);
		}
	}
	// ȡ���ҽ��������
	void SimpleDecal::detachAllObjects()
	{
		if(m_nodeAttachObject)
		{
			m_nodeAttachObject->detachAllObjects();
		}
	}

	// ���������Ż�ùҽ��˵����
	MovableObject* SimpleDecal::getAttachObject(ushort index)const
	{
		return m_nodeAttachObject->getAttachedObject(index);
	}

	// ����ѹҽӵ����ö��
	SceneNode::ObjectIterator SimpleDecal::getAttachedObjectIterator(void)
	{
		return m_nodeAttachObject->getAttachedObjectIterator();
	}

	// ÿ֡��������
	void SimpleDecal::update(float timeSinceLastFrame)
	{
		if(m_dirty)
		{
			updateDecal();
			m_dirty = false;
		}
	}


	// ���ùҽ��������ƫ��
	void SimpleDecal::setAttachObjectsOffset(const Vector3 &offsetPosition
		, const Quaternion &offsetOrientation
		, const Vector3 &offsetScale
		)
	{	
		if(m_offsetOrientation != offsetOrientation
			|| m_offsetPosition != offsetPosition
			|| m_offsetScale != offsetScale
			)
		{
			m_dirty |= DIRTY_OFFSET;
		}
		m_offsetOrientation = offsetOrientation;
		m_offsetPosition = offsetPosition;
		m_offsetScale = offsetScale;
	}
	// ��ùҽ����������ת
	const Quaternion &SimpleDecal::getOffsetOrientation()const
	{
		return m_offsetOrientation;
	}
	// ��ùҽ��������λ��
	const Vector3 &SimpleDecal::getOffsetPosition()const
	{
		return m_offsetPosition;
	}
	// ��ùҽ������������
	const Vector3 &SimpleDecal::getOffsetScale()const
	{
		return m_offsetScale;
	}

	// ��ȡ�����İ�Χ�У��������꣩
	const AxisAlignedBox &SimpleDecal::getBoundingBox()const
	{
		if(m_aabbNeedUpdate)
		{
			Vector3 halfSize(m_size.x / 2.0f , m_depth , m_size.y / 2.0f);
			m_aabb.setExtents(-halfSize , +halfSize);
			m_aabbNeedUpdate = false;
		}
		return m_aabb;
	}

	// ��ȡ�����������Χ�У��������꣩
	const AxisAlignedBox &SimpleDecal::getWorldBoundingBox()const
	{
		if(m_worldAABBNeedUpdate)
		{
			m_worldAABB = getBoundingBox();
			Matrix4 mat;
			mat.makeTrans(m_position);
			m_worldAABB.transformAffine(mat);
			m_worldAABBNeedUpdate = false;
		}
		return m_worldAABB;
	}

};
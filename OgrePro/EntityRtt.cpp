/******************************************************************
** �ļ���:	EntityRtt.cpp
** ������:	FreeKnight(������)
** ��  ��:	2008-1-13 15:17:27
** ��  ��:
******************************************************************/

#include "stdafx.h"
#include "EntityRtt.h"
namespace Ogre
{

	void EntityRTTListener::preRenderTargetUpdate(const RenderTargetEvent& evt)
	{
		m_rtt->m_node->setVisible(true);
	}

	void EntityRTTListener::postRenderTargetUpdate(const RenderTargetEvent& evt)
	{
		m_rtt->m_node->setVisible(false);
	}

	//���������
	EntityRTT::EntityRTT(const String &name)
		: m_name(name)
		, m_inited(false)
		, m_createdTexture(false)
		, m_setedEntity(false)
		, m_enabled(false)
		, m_sceneMgr(0)
		, m_textureWidth(0)
		, m_textureHeight(0)
		, m_textureFormat(PF_UNKNOWN) 
		, m_bgColor(ColourValue::ZERO)
		, m_nearclipDist(0)
		, m_farclipDist(0)
		, m_fovy(0)
		, m_ratio(0)
		, m_entity(0)
		, m_cloneEntity(0)
		, m_viewport(0)
		, m_camera(0)
		, m_target(0)
		, m_node(0)
		, m_nodeEntity(0)
		, m_listener(0)
		, m_animationController(0)
	{
		// Ĭ�ϵ�listener
		m_listener = new EntityRTTListener(this);
		m_animationController = new EntityAnimationController;
	}

	EntityRTT::~EntityRTT()
	{
		destroy();
		OGRE_DELETE(m_animationController);
	}

	// ��ʼ��RTT
	void EntityRTT::init(SceneManager *sceneMgr
		, ushort textureWidth
		, ushort textureHeight
		, PixelFormat textureFormat
		, const ColourValue& bgColor
		, float nearclipDist
		, float farclipDist
		, const Radian &fovy
		, float ratio)
	{
		if(m_inited && m_createdTexture)
			return;
		m_sceneMgr = sceneMgr;
		m_textureWidth = textureWidth;
		m_textureHeight = textureHeight;
		m_textureFormat = textureFormat;
		m_nearclipDist = nearclipDist;
		m_farclipDist = farclipDist;
		m_fovy = fovy;
		m_ratio = ratio;
		m_inited = true;
	}
	// ����RTT������ӿ�
	void EntityRTT::createViewport()
	{
		if(m_viewport)
			return;
		// ��������
		m_rtt = TextureManager::getSingleton().createManual(m_name + "_RTT", 
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 
			m_textureWidth, m_textureHeight, 0, m_textureFormat, TU_RENDERTARGET );
		m_target = m_rtt->getBuffer()->getRenderTarget();
		// Ĭ�ϲ�RTT
		m_target->setActive(false);
		m_enabled = false;
		// ������
		if(m_listener)
		{
			m_target->addListener(m_listener);
		}
		// ���������
		m_camera = m_sceneMgr->createCamera(m_name + "_camera");
		m_camera->setFOVy(m_fovy);
		m_camera->setNearClipDistance(m_nearclipDist);
		m_camera->setFarClipDistance(m_farclipDist);
		m_camera->setAspectRatio(m_ratio);

		// �����ӿ�
		m_viewport = m_target->addViewport(m_camera);
		m_viewport->setOverlaysEnabled(false);
		m_viewport->setBackgroundColour(m_bgColor);
		m_viewport->setClearEveryFrame(true);
	}

	// �ӳٳ�ʼ��
	void EntityRTT::delayInit()
	{
		if(m_createdTexture || !m_inited)
			return;
		// ����RTT������ӿ�
		createViewport();
		// �����ڵ�
		m_node = m_sceneMgr->getRootSceneNode()->createChildSceneNode(m_name);
		m_nodeEntity = m_node->createChildSceneNode(m_name + "_Entity");

		// RTT����Ⱦ������
		m_node->setPosition(0 , -10000 , 0);

		m_createdTexture = true;
	}

	// ����RTT
	void EntityRTT::destroy()
	{
		if(!m_createdTexture)
			return;
		destroyEntity();
		destroyViewport();
		OGRE_DELETE(m_listener);
		m_sceneMgr->destroySceneNode(m_nodeEntity->getName());
		m_sceneMgr->destroySceneNode(m_node->getName());
		m_inited = false;
		m_createdTexture = false;
	}

	// �����ӿ�
	void EntityRTT::destroyViewport()
	{
		// ����Viewport
		if(m_viewport)
		{
			m_target->setActive(false);
			m_target->removeAllListeners();
			m_target->removeAllViewports();
			m_sceneMgr->destroyCamera(m_camera);
			if(!m_rtt.isNull())
			{
				//TextureManager::getSingleton().unload(m_rtt->getName());
				//TextureManager::getSingleton().remove(m_rtt->getName());
				m_rtt.setNull();
			}
			m_target = 0;
			m_viewport = 0;
			m_camera = 0;
		}
	}

	// ���һ�����ظ���ʵ������
	String EntityRTT::getUniqueEntityName(const String &preName)
	{
		int index = 0;
		String name;
		do
		{
			name = preName + "_" + StringConverter::toString(index++);
		}while(m_sceneMgr->hasEntity(name));
		return name;
	}

	// ����Ҫ��Ⱦ��ʵ��
	void EntityRTT::setEntity(Entity *entity , const String &defaultAnimationName)
	{
		destroyEntity();
		// �ӳٴ���RTT����
		delayInit();
		m_entity = entity;
		// ��¡һ������
		m_cloneEntity = m_entity->clone(getUniqueEntityName(entity->getName() + "_Clone"));

		// ���ӵ��ڵ�
		m_nodeEntity->attachObject(m_cloneEntity);
		// ����ת��
		m_nodeEntity->setOrientation(m_rotate);
		// ����Ĭ�϶���
		static_cast<EntityAnimationController*>(m_animationController)->create(m_cloneEntity);
		if(!defaultAnimationName.empty())
		{
			m_animationController->play(defaultAnimationName);
		}
		// �����������ʵ����ǰ
		adjustCamera();

		Root::getSingleton().addFrameListener(this);

		m_setedEntity = true;
	}

	bool EntityRTT::frameStarted(const FrameEvent& evt)
	{
		if(m_animationController)
		{
			m_animationController->update(evt.timeSinceLastFrame);
		}
		return true;
	}

	// �����������ʵ����ǰ
	void EntityRTT::adjustCamera()
	{
		m_nodeEntity->_updateBounds();
		AxisAlignedBox box = m_nodeEntity->_getWorldAABB();

		Vector3 halfSize(box.getHalfSize());
		Vector3 offset(box.getCenter());
		float radius = std::max(halfSize.y , halfSize.z);
		// ���������������ǰ�����������fovy�������Χ�еĸ߶�������
		offset.x += radius / tan(m_camera->getFOVy().valueRadians() * 0.5f) + halfSize.x;
		m_camera->setPosition(offset);

		m_camera->lookAt(box.getCenter());
	}

	// ����ʵ��
	void EntityRTT::destroyEntity()
	{
		if(!m_setedEntity)
			return;
		static_cast<EntityAnimationController*>(m_animationController)->destroy();
		if(m_entity)
		{
			m_entity = 0;
		}
		if(m_cloneEntity)
		{
			m_sceneMgr->destroyEntity(m_cloneEntity);
			m_cloneEntity = 0;
		}
		Root::getSingleton().removeFrameListener(this);
		m_setedEntity = false;

	}

	// ���RTT����
	TexturePtr &EntityRTT::getRTT()
	{
		return m_rtt;
	}

	// ����RTT��Ⱦ
	void EntityRTT::setEnabled(bool enable)
	{
		m_enabled = enable;
		if(m_target)
		{
			m_target->setActive(m_enabled);
		}
	}

	// �Ƿ��Ѿ�����RTT��Ⱦ
	bool EntityRTT::isEnabled()const
	{
		return m_enabled;
	}
	// ������ת��
	void EntityRTT::setEntityRotate(const Quaternion &rotate)
	{
		m_rotate = rotate;
		if(m_nodeEntity)
		{
			m_nodeEntity->setOrientation(m_rotate);
		}
	}
	// ������ת
	void EntityRTT::rotateEntity(const Quaternion &rotate)
	{
		m_rotate = m_rotate * rotate;
		if(m_nodeEntity)
		{
			m_nodeEntity->setOrientation(m_rotate);
		}
	}

	// �����ת��
	const Quaternion &EntityRTT::getEntityRotate()const
	{
		return m_rotate;
	}

	// ����RTT������Ⱦ����
	void EntityRTT::setRatio(float ratio)
	{
		m_ratio = ratio;
		if(m_camera)
		{
			m_camera->setAspectRatio(m_ratio);
		}
	}
	// ���ô��ڴ�С������������С�Զ����������С�����������
	void EntityRTT::setWindowSize(ushort width , ushort height)
	{
		// ���ñ���
		setRatio((float)width / (float)height);
		// ���������С
		setTextureSize(width , height);
	}

	// ����RTT�����С
	void EntityRTT::setTextureSize(ushort width , ushort height)
	{
		// �������2����
		ushort widthPower = 1;
		while(width > pow(2.0f , widthPower))
		{
			widthPower ++;
		};
		ushort realWidth = pow(2.0f , widthPower);

		// �������2����
		ushort heightPower = 1;
		while(height > pow(2.0f , heightPower))
		{
			heightPower ++;
		};
		ushort realHeight = pow(2.0f , heightPower);

		if(m_textureWidth == realWidth && m_textureHeight == realHeight)
			return;
		m_textureWidth = realWidth;
		m_textureHeight = realHeight;
		if(m_viewport)
		{
			bool enabled = isEnabled();
			// ���´���RTT������ӿ�
			destroyViewport();
			createViewport();
			setEnabled(enabled);
		}
	}
};
#include "StdAfx.h"
#include "NullObjectVisible.h"
#include "IScene.h"
#include "DisplaySystem.h"
#include "NullObject.h"

namespace Ogre
{

	NullObjectVisible::NullObjectVisible(NullObject *obj)
		: m_object(obj)
		, m_showBorder(false)
		, m_showEntity(false)
		, m_showPlacingEntity(false)
		, m_entitySphere(0)
		, m_sphereRadius(0.5f)
		, m_scene(0)
		, m_alphaController(0)
		, m_collisionEntity(0)
	{
		m_scene = m_object->getScene();
		m_sphereColor = ColourValue::Green;
		createVisibleSphere();
		showEntity(false);
	}

	NullObjectVisible::~NullObjectVisible(void)
	{
		destroyVisibleSphere();
	}
	// ����Բ��
	void NullObjectVisible::destroyVisibleSphere()
	{
		if(m_entitySphere)
		{
			m_scene->getCollisionWorld()->destroyCollisionEntity(m_collisionEntity);
			m_collisionEntity = 0;
			OGRE_DELETE(m_alphaController);
			SceneNode *node = m_entitySphere->getParentSceneNode();
			node->detachObject(m_entitySphere);
			m_scene->getOgreSceneManager()->destroySceneNode(node->getName());
			m_scene->getOgreSceneManager()->destroyMovableObject(m_entitySphere);
			m_entitySphere = 0;
		}
	}
	// ����һ��Բ��������SceneNode��
	void NullObjectVisible::createVisibleSphere()
	{
		destroyVisibleSphere();
		if(m_sphereRadius == 0.0f)
			return;
		m_entitySphere = m_scene->getOgreSceneManager()->createEntity(m_object->getFullName() + "_Circle" , SceneManager::PT_SPHERE);
		m_entitySphere->setUserAny(Any(static_cast<ISceneObject*>(m_object)));
		m_entitySphere->setMaterialName(DisplaySystem::getSingleton().getColorMaterial(m_sphereColor));
		SceneNode *node = m_object->getOgreSceneNode()->createChildSceneNode();
		node->attachObject(m_entitySphere);
		node->setScale(0.01f * m_sphereRadius * 2.0f , 0.01f * m_sphereRadius * 2.0f , 0.01f * m_sphereRadius * 2.0f);

		m_alphaController = new EntityMaterialInstance(m_entitySphere);

		m_collisionEntity = m_scene->getCollisionWorld()->createEntityCollisionEntity(m_entitySphere , CT_SELF_MESH);
	}

	// ��ʾ�߿�
	void NullObjectVisible::showBorder(bool show)
	{
		m_showBorder = show;
		SceneNode *node = m_entitySphere->getParentSceneNode();
		node->showBoundingBox(show);
	}
	// �Ƿ���ʾ�߿�
	bool NullObjectVisible::isShowBorder() const
	{
		return m_showBorder;
	}

	// ��ʾʵ��
	void NullObjectVisible::showEntity(bool show)
	{
		m_showEntity = show;
		m_entitySphere->setVisible(show);
		m_alphaController->setTransparency(1.0f);
	}
	// �Ƿ���ʾʵ��
	bool NullObjectVisible::isShowEntity() const
	{
		return m_showEntity;
	}

	// ��ó�������
	ISceneObject *NullObjectVisible::getSceneObject()const
	{
		return m_object;
	}


	// ��ʾ���ڰڷŵ�ģ��
	void NullObjectVisible::showPlacingEntity(bool show)
	{
		m_showPlacingEntity = show;
		if(show)
		{
			m_entitySphere->setVisible(true);
			m_alphaController->setTransparency(0.5f);
		}
		else
		{
			showEntity(isShowEntity());
		}
	}
	// �Ƿ���ʾ���ڰڷŵ�ģ��
	bool NullObjectVisible::isShowPlacingEntity() const
	{
		return m_showPlacingEntity;
	}

	// ��ÿɼ��������ײʵ��
	ICollisionEntity *NullObjectVisible::getCollisionEntity()const
	{
		return m_collisionEntity;
	}
};
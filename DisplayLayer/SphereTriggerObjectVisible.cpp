#include "StdAfx.h"
#include "SphereTriggerObjectVisible.h"
#include "IScene.h"
#include "DisplaySystem.h"
#include "SphereTriggerObject.h"
#include "ISphereTriggerEntity.h"

namespace Ogre
{

	SphereTriggerObjectVisible::SphereTriggerObjectVisible(SphereTriggerObject *obj)
		: m_object(obj)
		, m_showBorder(false)
		, m_showEntity(false)
		, m_showPlacingEntity(false)
		, m_scene(0)
		, m_entity(0)
		, m_entityNode(0)
	{
		m_scene = m_object->getScene();
		createEntity();
	}
	// ����ģ��ʵ��
	void SphereTriggerObjectVisible::createEntity()
	{
		destroyEntity();
		m_entity = m_scene->getOgreSceneManager()->createEntity(m_object->getFullName() + "_VisibleObject" , SceneManager::PT_SPHERE);
		m_entityNode = m_object->getOgreSceneNode()->createChildSceneNode();
		m_entityNode->attachObject(m_entity);
		ISphereTriggerEntity *entity = static_cast<ISphereTriggerEntity*>(m_object->getTriggerEntity());
		m_entityNode->setScale(0.02f * entity->getRadius()
			, 0.02f * entity->getRadius()
			, 0.02f * entity->getRadius()
			);
		m_entity->setVisible(false);
		m_entity->setUserAny(Any(static_cast<ISceneObject*>(m_object)));
		m_entity->setMaterialName(DisplaySystem::getSingleton().getColorMaterial(ColourValue(1 , 1 , 0)));
		showEntity(isShowEntity());
	}
	// ����ģ��ʵ��
	void SphereTriggerObjectVisible::destroyEntity()
	{
		if(m_entityNode)
		{
			m_object->getOgreSceneNode()->removeAndDestroyChild(m_entityNode->getName());
			m_entityNode = 0;
		}
		if(m_entity)
		{
			m_scene->getOgreSceneManager()->destroyEntity(m_entity);
			m_entity = 0;
		}
	}

	SphereTriggerObjectVisible::~SphereTriggerObjectVisible(void)
	{
		if(m_entityNode)
		{
			m_object->getOgreSceneNode()->removeAndDestroyChild(m_entityNode->getName());
			m_entityNode = 0;
		}
		m_scene->getOgreSceneManager()->destroyEntity(m_entity);
	}

	// ��ʾ�߿�
	void SphereTriggerObjectVisible::showBorder(bool show)
	{
		m_showBorder = show;
		m_entityNode->showBoundingBox(show);
	}
	// �Ƿ���ʾ�߿�
	bool SphereTriggerObjectVisible::isShowBorder() const
	{
		return m_showBorder;
	}

	// ��ʾʵ��
	void SphereTriggerObjectVisible::showEntity(bool show)
	{
		m_showEntity = show;
		m_entity->setVisible(show);
	}
	// �Ƿ���ʾʵ��
	bool SphereTriggerObjectVisible::isShowEntity() const
	{
		return m_showEntity;
	}

	// ��ó�������
	ISceneObject *SphereTriggerObjectVisible::getSceneObject()const
	{
		return m_object;
	}


	// ��ʾ���ڰڷŵ�ģ��
	void SphereTriggerObjectVisible::showPlacingEntity(bool show)
	{
		m_showPlacingEntity = show;
		m_entity->setVisible(show);
	}
	// �Ƿ���ʾ���ڰڷŵ�ģ��
	bool SphereTriggerObjectVisible::isShowPlacingEntity() const
	{
		return m_showPlacingEntity;
	}

	// ��ÿɼ��������ײʵ��
	ICollisionEntity *SphereTriggerObjectVisible::getCollisionEntity()const
	{
		return 0;
	}
};
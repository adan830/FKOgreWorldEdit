#include "StdAfx.h"
#include "ParticleObjectVisible.h"
#include "IScene.h"
#include "DisplaySystem.h"
#include "ParticleObject.h"

namespace Ogre
{

	ParticleObjectVisible::ParticleObjectVisible(ParticleObject *obj)
		: m_object(obj)
		, m_showBorder(false)
		, m_showEntity(false)
		, m_showPlacingEntity(false)
		, m_scene(0)
	{
		m_scene = m_object->getScene();
	}

	ParticleObjectVisible::~ParticleObjectVisible(void)
	{
	}

	// ��ʾ�߿�
	void ParticleObjectVisible::showBorder(bool show)
	{
		m_showBorder = show;
		m_object->getOgreSceneNode()->showBoundingBox(show);
	}

	// �Ƿ���ʾ�߿�
	bool ParticleObjectVisible::isShowBorder() const
	{
		return m_showBorder;
	}

	// ��ʾʵ��
	void ParticleObjectVisible::showEntity(bool show)
	{
		m_showEntity = show;
		if(m_object->getOgreParticleSystem())
		{
			m_object->getOgreParticleSystem()->setVisible(show);
		}
	}
	// �Ƿ���ʾʵ��
	bool ParticleObjectVisible::isShowEntity() const
	{
		return m_showEntity;
	}

	// ��ó�������
	ISceneObject *ParticleObjectVisible::getSceneObject()const
	{
		return m_object;
	}

	// ��ʾ���ڰڷŵ�ģ��
	void ParticleObjectVisible::showPlacingEntity(bool show)
	{
		m_showPlacingEntity = show;
		if(m_object->getOgreParticleSystem())
		{
			m_object->getOgreParticleSystem()->setVisible(show);
		}
	}
	// �Ƿ���ʾ���ڰڷŵ�ģ��
	bool ParticleObjectVisible::isShowPlacingEntity() const
	{
		return m_showPlacingEntity;
	}

	// ��ÿɼ��������ײʵ��
	ICollisionEntity *ParticleObjectVisible::getCollisionEntity()const
	{
		return 0;
	}

};
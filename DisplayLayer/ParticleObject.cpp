#include "StdAfx.h"
#include "ParticleObject.h"
#include "IScene.h"
#include "ParticleObjectVisible.h"
namespace Ogre
{

	const String ParticleObjectFactory::FACTORY_TYPE = "ParticleObject";

	// ��øù�������������
	const String &ParticleObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// ��������Ĵ�������
	ISceneObject *ParticleObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		ParticleObject *obj = new ParticleObject(name);
		obj->create(scene , params);
		return obj;
	}

	// ������������ٷ���
	void ParticleObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	ParticleObject::ParticleObject(const String &name)
		: NullObject(name)
		, m_particleSystem(0)
	{
	}

	ParticleObject::~ParticleObject(void)
	{
		destroy();
	}

	// ��������ʵ��
	void ParticleObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		if(params && params->find("templateName") != params->end())
		{
			setParticleTemplate(params->find("templateName")->second);
		}
	}

	// ���ٶ���ʵ��
	void ParticleObject::destroy()
	{
		destroyParticle();
		NullObject::destroy();
	}
	// ��������ģ��
	void ParticleObject::setParticleTemplate(const String &templateName)
	{
		destroyParticle();
		// ����ʵ��
		m_particleSystem = m_scene->getOgreSceneManager()->createParticleSystem(getFullName() , templateName);
		m_particleSystem->setUserAny(Any(static_cast<ISceneObject*>(this)));
		// �ҽ���ȥ
		m_node->attachObject(m_particleSystem);

		m_particleTemplateName = templateName;
	}

	// ��������ϵͳ
	void ParticleObject::destroyParticle()
	{
		if(m_particleSystem)
		{
			// ����ģ��ʵ��
			m_scene->getOgreSceneManager()->destroyMovableObject(m_particleSystem);
			m_particleSystem = 0;
			m_particleTemplateName.clear();
		}
	}



	// ö���������ԣ���������ʽ
	void ParticleObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_Particle , PT_GROUP , "��������" , "��������" , true , false , false));
		properties.back().addChild(Property(ID_ParticleTemplate , PT_STRING , "����ģ��" , "����ģ��" , true , true , true));
	}

	// ��������
	bool ParticleObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);
		switch(id)
		{
		case ID_Particle:		// һ������
			{
				return true;
			}
			break;
		case ID_ParticleTemplate:	// ģ���ļ�
			{
				setParticleTemplate(any_cast<String>(value));
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// ��ȡ����ֵ
	Any ParticleObject::getProperty(uint id)const
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::getProperty(id);
		switch(id)
		{
		case ID_Particle:		// ģ������
			{
				return Any();
			}
			break;
		case ID_ParticleTemplate:	// ģ���ļ�
			{
				return Any(getParticleTemplate());
			}
			break;
		default:
			return Any();
			break;
		}
	}


	// ����������ԣ���������ʽ
	const PropertyContainer::VTProperty* ParticleObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// ����������ԣ����б����ʽ
	const PropertyContainer::MapProperty* ParticleObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

	// ��ó�������Ŀɼ����������ڱ༭����ѡ��
	ISceneObjectVisible *ParticleObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new ParticleObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}
};
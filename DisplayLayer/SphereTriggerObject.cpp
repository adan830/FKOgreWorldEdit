#include "StdAfx.h"
#include "SphereTriggerObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
#include "SphereTriggerObjectVisible.h"
#include "ISphereTriggerEntity.h"
namespace Ogre
{

	const String SphereTriggerObjectFactory::FACTORY_TYPE = "SphereTriggerObject";

	// ��øù�������������
	const String &SphereTriggerObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// ��������Ĵ�������
	ISceneObject *SphereTriggerObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		SphereTriggerObject *obj = new SphereTriggerObject(name);
		obj->create(scene , params);
		return obj;
	}

	// ������������ٷ���
	void SphereTriggerObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	SphereTriggerObject::SphereTriggerObject(const String &name)
		: TriggerObject(name)
		, m_entity(0)
	{
	}

	SphereTriggerObject::~SphereTriggerObject(void)
	{
		destroy();
	}
	ITriggerEntity *SphereTriggerObject::getTriggerEntity()const
	{
		return m_entity;
	}
	// ��������ʵ��
	void SphereTriggerObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		// ����ʵ��
		m_entity = m_scene->getTriggerWorld()->createSphereTriggerEntity(getFullName());
		m_entity->setUserAny(Any(static_cast<ISceneObject*>(this)));
		m_node->setListener(this);
	}

	// ���ٶ���ʵ��
	void SphereTriggerObject::destroy()
	{
		if(m_entity)
		{
			// ����ģ��ʵ��
			m_scene->getTriggerWorld()->destroyTriggerEntity(m_entity);
			m_entity = 0;
		}
		NullObject::destroy();
	}

	// �ڵ����ʱ˳����´�����
	void SphereTriggerObject::nodeUpdated(const Node *node)
	{
		m_entity->setPosition(getWorldPosition());
		m_entity->setScale(getWorldScale());
		m_entity->setOrientation(getWorldOrientation());
	}

	// ö���������ԣ���������ʽ
	void SphereTriggerObject::enumPropertiesTree(VTProperty &properties)const
	{
		TriggerObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_SphereTrigger , PT_GROUP , "���δ���������" , "���δ���������" , true , false , false));
		properties.back().addChild(Property( ID_SphereRadius , PT_REAL , "����뾶" , "����뾶" , true , true , true));
	}

	// ��������
	bool SphereTriggerObject::setProperty(uint id , const Any &value)
	{
		if((id > ID_TriggerObject_Begin && id < ID_TriggerObject_End)
			|| (id > ID_NullObject_Begin && id < ID_NullObject_End))
			return TriggerObject::setProperty(id , value);
		switch(id)
		{
		case ID_SphereTrigger:							// ����������
			{
				return true;
			}
			break;
		case ID_SphereRadius:							// ����뾶
			{
				m_entity->setRadius(any_cast<Real>(value));
				// �ؽ��ɼ�����
				if(m_sceneObjectVisible)
				{
					m_sceneObjectVisible->_notifyRebuild();
				}
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// ��ȡ����ֵ
	Any SphereTriggerObject::getProperty(uint id)const
	{
		if((id > ID_TriggerObject_Begin && id < ID_TriggerObject_End)
			|| (id > ID_NullObject_Begin && id < ID_NullObject_End))
			return TriggerObject::getProperty(id);
		switch(id)
		{
		case ID_SphereTrigger:							// ����������
			{
				return Any();
			}
			break;
		case ID_SphereRadius:							// ����뾶
			{
				return Any(m_entity->getRadius());
			}
			break;
		default:
			return Any();
			break;
		}
	}

	// ����������ԣ���������ʽ
	const PropertyContainer::VTProperty* SphereTriggerObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// ����������ԣ����б����ʽ
	const PropertyContainer::MapProperty* SphereTriggerObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

	// ��ó�������Ŀɼ����������ڱ༭����ѡ��
	ISceneObjectVisible *SphereTriggerObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new SphereTriggerObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}

	// ���MovableObject
	MovableObject *SphereTriggerObject::getOgreMovableObject()const
	{
		return 0;
	}


	// �����������
	void SphereTriggerObject::setScale(const Vector3 &scale)
	{
		TriggerObject::setScale(Vector3(scale.x , scale.x , scale.x));
	}
	// ���ݵ�ǰ����ֵ����
	void SphereTriggerObject::scale(const Vector3& scale)
	{
		TriggerObject::scale(Vector3(scale.x , scale.x , scale.x));
	}
};
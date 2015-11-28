#include "StdAfx.h"
#include "BoxTriggerObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
#include "BoxTriggerObjectVisible.h"
#include "IBoxTriggerEntity.h"
namespace Ogre
{

	const String BoxTriggerObjectFactory::FACTORY_TYPE = "BoxTriggerObject";

	// ��øù�������������
	const String &BoxTriggerObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// ��������Ĵ�������
	ISceneObject *BoxTriggerObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		BoxTriggerObject *obj = new BoxTriggerObject(name);
		obj->create(scene , params);
		return obj;
	}

	// ������������ٷ���
	void BoxTriggerObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	BoxTriggerObject::BoxTriggerObject(const String &name)
		: TriggerObject(name)
		, m_entity(0)
	{
	}

	BoxTriggerObject::~BoxTriggerObject(void)
	{
		destroy();
	}

	// ��������ʵ��
	void BoxTriggerObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		// ����ʵ��
		m_entity = m_scene->getTriggerWorld()->createBoxTriggerEntity(getFullName());
		m_entity->setUserAny(Any(static_cast<ISceneObject*>(this)));
		m_node->setListener(this);
	}

	// ���ٶ���ʵ��
	void BoxTriggerObject::destroy()
	{
		if(m_entity)
		{
			// ����ģ��ʵ��
			m_scene->getTriggerWorld()->destroyTriggerEntity(m_entity);
			m_entity = 0;
		}
		NullObject::destroy();
	}
	ITriggerEntity *BoxTriggerObject::getTriggerEntity()const
	{
		return m_entity;
	}
	// �ڵ����ʱ˳����´�����
	void BoxTriggerObject::nodeUpdated(const Node *node)
	{
		m_entity->setPosition(getWorldPosition());
		m_entity->setScale(getWorldScale());
		m_entity->setOrientation(getWorldOrientation());
	}

	// ö���������ԣ���������ʽ
	void BoxTriggerObject::enumPropertiesTree(VTProperty &properties)const
	{
		TriggerObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_BoxTrigger , PT_GROUP , "���Ӵ���������" , "���Ӵ���������" , true , false , false));
		properties.back().addChild(Property( ID_BoxTriggerAxisAlignedBox , PT_AXISALIGNEDBOX , "��Χ��" , "���Ӵ������İ�Χ��" , true , true , true));
	}

	// ��������
	bool BoxTriggerObject::setProperty(uint id , const Any &value)
	{
		if((id > ID_TriggerObject_Begin && id < ID_TriggerObject_End)
			|| (id > ID_NullObject_Begin && id < ID_NullObject_End))
			return TriggerObject::setProperty(id , value);
		switch(id)
		{
		case ID_BoxTrigger:							// ����������
			{
				return true;
			}
			break;
		case ID_BoxTriggerAxisAlignedBox:							// ��Χ�е�minֵ
			{
				m_entity->setBox(any_cast<AxisAlignedBox>(value));
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
	Any BoxTriggerObject::getProperty(uint id)const
	{
		if((id > ID_TriggerObject_Begin && id < ID_TriggerObject_End)
			|| (id > ID_NullObject_Begin && id < ID_NullObject_End))
			return TriggerObject::getProperty(id);
		switch(id)
		{
		case ID_BoxTrigger:							// ����������
			{
				return Any();
			}
			break;
		case ID_BoxTriggerAxisAlignedBox:							// ��Χ�е�minֵ
			{
				return Any(m_entity->getBox());
			}
			break;
		default:
			return Any();
			break;
		}
	}

	// ����������ԣ���������ʽ
	const PropertyContainer::VTProperty* BoxTriggerObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// ����������ԣ����б����ʽ
	const PropertyContainer::MapProperty* BoxTriggerObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

	// ��ó�������Ŀɼ����������ڱ༭����ѡ��
	ISceneObjectVisible *BoxTriggerObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new BoxTriggerObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}

	// ���MovableObject
	MovableObject *BoxTriggerObject::getOgreMovableObject()const
	{
		return 0;
	}
};
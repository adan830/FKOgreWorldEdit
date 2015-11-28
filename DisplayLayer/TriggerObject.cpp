#include "StdAfx.h"
#include "TriggerObject.h"

namespace Ogre
{
	TriggerObject::TriggerObject(const String &name)
		: NullObject(name)
		, m_condition(true)
	{
	}

	TriggerObject::~TriggerObject(void)
	{
	}

	// ö���������ԣ���������ʽ
	void TriggerObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_Trigger , PT_GROUP , "����������" , "����������" , true , false , false));
		properties.back().addChild(Property( ID_TriggerCondition , PT_BOOL , "��������" , "�Ƿ��Ѿ������������������������ֻ�е���������ʱ�ſ��Դ���" , true , true , true));
		properties.back().addChild(Property( ID_TriggerOnEnterEvent , PT_STRING , "�����¼�" , "��������������봥�������ͻᴥ������¼�" , true , true , true));
		properties.back().addChild(Property( ID_TriggerOnLeaveEvent , PT_STRING , "�˳��¼�" , "�������˽����¼����˳����������ͻᴥ������¼�" , true , true , true));
	}

	// ��������
	bool TriggerObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);
		switch(id)
		{
		case ID_Trigger:							// ����������
			{
				return true;
			}
			break;
		case ID_TriggerCondition:							// ��������
			{
				setCondition(any_cast<bool>(value));
				return true;
			}
			break;
		case ID_TriggerOnEnterEvent:							// �����¼�
			{
				setOnEnterEvent(any_cast<String>(value));
				return true;
			}
			break;
		case ID_TriggerOnLeaveEvent:							// �˳��¼�
			{
				setOnLeaveEvent(any_cast<String>(value));
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// ��ȡ����ֵ
	Any TriggerObject::getProperty(uint id)const
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::getProperty(id);
		switch(id)
		{
		case ID_Trigger:							// ����������
			{
				return Any();
			}
			break;
		case ID_TriggerCondition:							// ��������
			{
				return Any(getCondition());
			}
			break;
		case ID_TriggerOnEnterEvent:							// �����¼�
			{
				return Any(getOnEnterEvent());
			}
			break;
		case ID_TriggerOnLeaveEvent:							// �˳��¼�
			{
				return Any(getOnLeaveEvent());
			}
			break;
		default:
			return Any();
			break;
		}
	}

	// ����������ԣ���������ʽ
	const PropertyContainer::VTProperty* TriggerObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// ����������ԣ����б����ʽ
	const PropertyContainer::MapProperty* TriggerObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

};
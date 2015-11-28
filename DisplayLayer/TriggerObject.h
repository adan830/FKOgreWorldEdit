#pragma once
#include "NullObject.h"

namespace Ogre
{

	// �������������
	class DISPLAYLAYER_API TriggerObject : public NullObject
	{
	public:

		TriggerObject(const String &name);

		virtual ~TriggerObject(void);

		// ��ô�����ʵ��
		virtual ITriggerEntity *getTriggerEntity()const = 0;


		// �����Ƿ���������
		virtual void setCondition(bool value)
		{
			m_condition = value;
		}
		// �����Ƿ���������
		virtual bool getCondition()const
		{
			return m_condition;
		}

		// ���ý��봥�����󴥷����¼�
		virtual void setOnEnterEvent(const String &event)
		{
			m_onEnterEvent = event;
		}
		// ���ؽ��봥�����󴥷����¼�
		virtual const String &getOnEnterEvent()const
		{
			return m_onEnterEvent;
		}

		// �����뿪�������󴥷����¼�
		virtual void setOnLeaveEvent(const String &event)
		{
			m_onLeaveEvent = event;
		}
		// �����뿪�������󴥷����¼�
		virtual const String &getOnLeaveEvent()const
		{
			return m_onLeaveEvent;
		}

		// ��������
		virtual bool setProperty(uint id , const Any &value);
		// ��ȡ����ֵ
		virtual Any getProperty(uint id)const;
		// ����������ԣ���������ʽ
		virtual const VTProperty* getPropertiesTree()const;
		// ����������ԣ����б����ʽ
		virtual const MapProperty* getPropertiesList()const;
	protected:
		// ö���������ԣ���������ʽ
		virtual void enumPropertiesTree(VTProperty &properties)const;

		// �Ƿ���������
		bool m_condition;
		// ���봥�����󴥷����¼�
		String m_onEnterEvent;
		// �뿪�������󴥷����¼�
		String m_onLeaveEvent;
	};

};
#pragma once
#include "TriggerObject.h"
namespace Ogre
{
	// ��������Ķ��󹤳�
	class DISPLAYLAYER_API SphereTriggerObjectFactory : public ISceneObjectFactory
	{
	public:
		static const String FACTORY_TYPE;
		// ��øù�������������
		virtual const String &getType() const;
		// ��������Ĵ�������
		virtual ISceneObject *create(const String &name , IScene *scene , const NameValuePairList* params);
		// ������������ٷ���
		virtual void destroy(ISceneObject *obj);
	};

	// ģ��ʵ�����
	class DISPLAYLAYER_API SphereTriggerObject : public TriggerObject
		, public Node::Listener
	{
	public:
		SphereTriggerObject(const String &name);
		virtual ~SphereTriggerObject(void);
		// ��ö�������
		virtual const String &getType()const
		{
			return SphereTriggerObjectFactory::FACTORY_TYPE;
		}
		ITriggerEntity *getTriggerEntity()const;
		// ��������ʵ��
		virtual void create(IScene *scene , const NameValuePairList* params);
		// ���ٶ���ʵ��
		virtual void destroy();
		// ���MovableObject
		virtual MovableObject *getOgreMovableObject()const;

		// ��������
		virtual bool setProperty(uint id , const Any &value);
		// ��ȡ����ֵ
		virtual Any getProperty(uint id)const;
		// ����������ԣ���������ʽ
		virtual const VTProperty* getPropertiesTree()const;
		// ����������ԣ����б����ʽ
		virtual const MapProperty* getPropertiesList()const;

		// ��ó�������Ŀɼ����������ڱ༭����ѡ��
		virtual ISceneObjectVisible *getSceneObjectVisible();

		// �ڵ����ʱ˳����´�����
		virtual void nodeUpdated(const Node *node);

		// �����������
		virtual void setScale(const Vector3 &scale);
		// ���ݵ�ǰ����ֵ����
		virtual void scale(const Vector3& scale);

		// ������Է�λ
		virtual void setOrientation(const Quaternion &orientation){};
		// ����ĳ���������꣬������������ڲ��������緽λ
		virtual void lookAt(const Vector3& targetPoint){};
		// ���ݵ�ǰ��λ��ת
		virtual void rotate(const Quaternion& rotate){};

	protected:
		// ö���������ԣ���������ʽ
		virtual void enumPropertiesTree(VTProperty &properties)const;
		// ���Ӵ�����ʵ��
		ISphereTriggerEntity *m_entity;
	};


};
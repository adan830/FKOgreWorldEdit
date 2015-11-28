#pragma once
#include "NullObject.h"
namespace Ogre
{
	// ��������Ķ��󹤳�
	class DISPLAYLAYER_API ParticleObjectFactory : public ISceneObjectFactory
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
	class DISPLAYLAYER_API ParticleObject : public NullObject
	{
	public:
		ParticleObject(const String &name);
		virtual ~ParticleObject(void);
		// ��ö�������
		virtual const String &getType()const
		{
			return ParticleObjectFactory::FACTORY_TYPE;
		}
		// ��������ʵ��
		virtual void create(IScene *scene , const NameValuePairList* params);
		// ���ٶ���ʵ��
		virtual void destroy();
		// ���MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return m_particleSystem;
		}
		// ���ParticleSystem
		virtual ParticleSystem *getOgreParticleSystem()const
		{
			return m_particleSystem;
		}

		// ��������ģ��
		void setParticleTemplate(const String &templateName);
		// �������ģ������
		const String &getParticleTemplate()const
		{
			return m_particleTemplateName;
		}

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

	protected:
		// ö���������ԣ���������ʽ
		virtual void enumPropertiesTree(VTProperty &properties)const;
		// ��������ϵͳ
		void destroyParticle();
		// Ogre����ϵͳ
		ParticleSystem *m_particleSystem;
		// ����ģ������
		String m_particleTemplateName;
	};


};
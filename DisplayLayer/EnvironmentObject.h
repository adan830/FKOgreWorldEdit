#pragma once
#include "NullObject.h"
namespace Ogre
{
	// ��������Ķ��󹤳�
	class DISPLAYLAYER_API EnvironmentObjectFactory : public ISceneObjectFactory
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

	// ��������
	class DISPLAYLAYER_API EnvironmentObject : public NullObject
	{
	public:
		EnvironmentObject(const String &name);
		virtual ~EnvironmentObject(void);
		// ��ö�������
		virtual const String &getType()const
		{
			return EnvironmentObjectFactory::FACTORY_TYPE;
		}
		// ��������ʵ��
		virtual void create(IScene *scene , const NameValuePairList* params);
		// ���ٶ���ʵ��
		virtual void destroy();
		// ���MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return 0;
		}

		// ��������
		virtual bool setProperty(uint id , const Any &value);
		// ��ȡ����ֵ
		virtual Any getProperty(uint id)const;
		// ����������ԣ���������ʽ
		virtual const VTProperty* getPropertiesTree()const;
		// ����������ԣ����б����ʽ
		virtual const MapProperty* getPropertiesList()const;

		// �����Ƿ���ʾ
		virtual void setVisible(bool visible);


		// ��ó�������Ŀɼ����������ڱ༭����ѡ��
		virtual ISceneObjectVisible *getSceneObjectVisible();
	protected:
		// ö���������ԣ���������ʽ
		virtual void enumPropertiesTree(VTProperty &properties)const;

		// �����Ƿ��ʼ��
		bool m_environmentInited;

		// �ӳ�����������ȡ��������
		void _getEnvironment();
		// ���û���
		void _setEnvironment();
		// ���û���
		void _disableEnvironment();
		// �������
		void _setSky();
		// ������
		void _setFog();
		// ���ù���
		void _setLighting();

		// ��պ�����
		bool m_skyBoxEnabled;					// �Ƿ�����պ�
		String m_skyBoxMaterialName;			// ��պ�����
		Real m_skyBoxRenderDistance;			// ��Ⱦ����

		// ������
		FogMode m_fogMode;						// ����ʽ
		ColourValue m_fogColor;					// ����ɫ
		Real m_fogDensity;						// ���ܶ�
		Real m_fogStart;						// ����ʼ����
		Real m_fogEnd;							// ����������

		// ��������
		ColourValue m_ambientLight;				// ��������ɫ

	};


};
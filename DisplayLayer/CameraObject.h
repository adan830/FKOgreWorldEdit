#pragma once
#include "NullObject.h"
namespace Ogre
{
	// ��������Ķ��󹤳�
	class DISPLAYLAYER_API CameraObjectFactory : public ISceneObjectFactory
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
	class DISPLAYLAYER_API CameraObject : public NullObject
	{
	public:
		CameraObject(const String &name);
		virtual ~CameraObject(void);
		// ��ö�������
		virtual const String &getType()const
		{
			return CameraObjectFactory::FACTORY_TYPE;
		}
		Camera *getOgreCamera()const
		{
			return m_camera;
		}
		// ��������ʵ��
		virtual void create(IScene *scene , const NameValuePairList* params);
		// ���ٶ���ʵ��
		virtual void destroy();
		// ���MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return m_camera;
		}

		// ���������
		// @x ���x��Ļ���꣨0~1��
		// @y ���y��Ļ���꣨0~1��
		// @pos ����ཻ������
		// @return �Ƿ��ཻ��
		virtual bool hitTest(float x , float y , Vector3 &pos);


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
		// Ogreʵ��
		Camera *m_camera;
	};


};
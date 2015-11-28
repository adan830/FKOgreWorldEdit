#pragma once
#include "TriggerObject.h"
namespace Ogre
{
	// ��������Ķ��󹤳�
	class TrackKeyFrameObjectFactory : public ISceneObjectFactory
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

	// �켣�ؼ�֡����
	class TrackKeyFrameObject : public EntityObject
	{
	public:
		TrackKeyFrameObject(const String &name);
		virtual ~TrackKeyFrameObject(void);
		// ��ö�������
		virtual const String &getType()const
		{
			return TrackKeyFrameObjectFactory::FACTORY_TYPE;
		}
		// ��������ʵ��
		virtual void create(IScene *scene , const NameValuePairList* params);
		// ���ٶ���ʵ��
		virtual void destroy();

		// ��������
		virtual bool setProperty(uint id , const Any &value);
		// ��ȡ����ֵ
		virtual Any getProperty(uint id)const;
		// ����������ԣ���������ʽ
		virtual const VTProperty* getPropertiesTree()const;
		// ����������ԣ����б����ʽ
		virtual const MapProperty* getPropertiesList()const;



		// �����������
		virtual void setPosition(const Vector3 &pos);

		// �����������
		virtual void setScale(const Vector3 &scale);
		// ����������
		virtual const Vector3 &getScale()const;

		// ������Է�λ
		virtual void setOrientation(const Quaternion &orientation);

		// ����ĳ���������꣬������������ڲ��������緽λ
		virtual void lookAt(const Vector3& targetPoint);
		// ���ݵ�ǰ����ֵ����
		virtual void scale(const Vector3& scale);
		// ���ݵ�ǰλ���ƶ�
		virtual void translate(const Vector3& distance);
		// ���ݵ�ǰ��λ��ת
		virtual void rotate(const Quaternion& rotate);

		// �����ݸ��µ��ؼ�֡������ȥ
		void updateToKeyFrame();
		// �����ݴӹؼ�֡��������µ�����
		void updateFromKeyFrame();
	protected:
		// ��������
		float m_length;
		// ���ű���
		Vector3 m_scale;
		// ö���������ԣ���������ʽ
		virtual void enumPropertiesTree(VTProperty &properties)const;
	};


};
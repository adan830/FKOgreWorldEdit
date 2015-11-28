#pragma once
#include "NullObject.h"
namespace Ogre
{
	// ��������Ķ��󹤳�
	class DISPLAYLAYER_API BillboardSetObjectFactory : public ISceneObjectFactory
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
	class DISPLAYLAYER_API BillboardSetObject : public NullObject
		, private MovableObject::Listener
	{
	public:
		BillboardSetObject(const String &name);
		virtual ~BillboardSetObject(void);
		// ��ö�������
		virtual const String &getType()const
		{
			return BillboardSetObjectFactory::FACTORY_TYPE;
		}
		BillboardSet *getOgreBillboardSet()const
		{
			return m_billboardSet;
		}
		// ��������ʵ��
		virtual void create(IScene *scene , const NameValuePairList* params);
		// ���ٶ���ʵ��
		virtual void destroy();
		// ���MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return m_billboardSet;
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
		// Ogreʵ��
		BillboardSet *m_billboardSet;

		// ��ת��Ϣ����ʾ��һ���Ƕȼн���ʹ��һ��ָ��������
		struct RotateInfo
		{
			// ��ʼ�Ƕ�
			Radian m_begin;
			// �����Ƕ�
			Radian m_end;
			// ʹ���������
			String m_materialName;
			// ��������
			FloatRect m_textureCoord;
		};
		typedef std::vector<RotateInfo> ListRotateInfo;
		ListRotateInfo m_listRotateInfo;
	private:
		// �������������ı�billboard������
		virtual bool objectRendering(const MovableObject *mo, const Camera *camera);
	};


};
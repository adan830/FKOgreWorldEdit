#include "StdAfx.h"
#include "BillboardSetObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
namespace Ogre
{

	const String BillboardSetObjectFactory::FACTORY_TYPE = "BillboardSetObject";

	// ��øù�������������
	const String &BillboardSetObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// ��������Ĵ�������
	ISceneObject *BillboardSetObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		BillboardSetObject *obj = new BillboardSetObject(name);
		obj->create(scene , params);
		return obj;
	}

	// ������������ٷ���
	void BillboardSetObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	BillboardSetObject::BillboardSetObject(const String &name)
		: NullObject(name)
		, m_billboardSet(0)
	{
	}

	BillboardSetObject::~BillboardSetObject(void)
	{
		destroy();
	}

	// ��������ʵ��
	void BillboardSetObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		// ����ʵ��
		m_billboardSet = static_cast<BillboardSet*>(m_scene->getOgreSceneManager()->createBillboardSet(getFullName()));
		m_billboardSet->setUserAny(Any(static_cast<ISceneObject*>(this)));
		m_billboardSet->setListener(this);
		// �ҽ���ȥ
		m_node->attachObject(m_billboardSet);
	}

	// ���ٶ���ʵ��
	void BillboardSetObject::destroy()
	{
		if(m_billboardSet)
		{
			// ����ģ��ʵ��
			m_scene->getOgreSceneManager()->destroyBillboardSet(m_billboardSet);
			m_billboardSet = 0;
		}
		NullObject::destroy();
	}
	// �������������ı�billboard������
	bool BillboardSetObject::objectRendering(const MovableObject *mo, const Camera *camera)
	{
		const Quaternion &camQ = m_billboardSet->getCamQ();
		Vector3 eular(MathEx::QuaternionToEular(camQ));
		

		return true;
	}


	// ö���������ԣ���������ʽ
	void BillboardSetObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
	}

	// ��������
	bool BillboardSetObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);

		return false;
	}
	// ��ȡ����ֵ
	Any BillboardSetObject::getProperty(uint id)const
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::getProperty(id);
		return Any();
	}

	// ����������ԣ���������ʽ
	const PropertyContainer::VTProperty* BillboardSetObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// ����������ԣ����б����ʽ
	const PropertyContainer::MapProperty* BillboardSetObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

	// ��ó�������Ŀɼ����������ڱ༭����ѡ��
	ISceneObjectVisible *BillboardSetObject::getSceneObjectVisible()
	{
		//if(!m_sceneObjectVisible)
		//{
		//	m_sceneObjectVisible = new BillboardSetObjectVisible(this);
		//}
		return 0;
	}

};
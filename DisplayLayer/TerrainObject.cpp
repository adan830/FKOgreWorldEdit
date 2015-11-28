#include "StdAfx.h"
#include "TerrainObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
#include "ETTerrain.h"
namespace Ogre
{

	const String TerrainObjectFactory::FACTORY_TYPE = "TerrainObject";

	// ��øù�������������
	const String &TerrainObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// ��������Ĵ�������
	ISceneObject *TerrainObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		TerrainObject *obj = new TerrainObject(name);
		obj->create(scene , params);
		return obj;
	}

	// ������������ٷ���
	void TerrainObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	TerrainObject::TerrainObject(const String &name)
		: NullObject(name)
		, m_terrain(0)
	{
	}

	TerrainObject::~TerrainObject(void)
	{
		destroy();
	}

	// ��������ʵ��
	void TerrainObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		// ����ʵ��
		m_terrain = new ETTerrain();
		m_terrain->init(scene->getOgreSceneManager() , m_scene->getCollisionWorld() , getName());
		m_terrain->setUserAny(Any(static_cast<ISceneObject*>(this)));
		m_terrain->setBaseDir("../Data/ET/");
		// �ҽ���ȥ
		m_node->attachObject(m_terrain->getTerrainImpl());
	}
	// ���MovableObject
	MovableObject *TerrainObject::getOgreMovableObject()const
	{
		return m_terrain->getTerrainImpl();
	}

	// ���ٶ���ʵ��
	void TerrainObject::destroy()
	{
		OGRE_DELETE(m_terrain);
		NullObject::destroy();
	}

	// ������ε��ļ�
	void TerrainObject::save(const String &fileName)
	{
		m_fileName = fileName;
		if(m_terrain)
		{
			m_terrain->saveTerrain(m_fileName);
		}
	}

	// ö���������ԣ���������ʽ
	void TerrainObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_Terrain , PT_GROUP , "��������" , "��������" , true , false , false));
		properties.push_back(Property( ID_TerrainFile , PT_STRING , "�����ļ�" , "�����ļ�" , true , true , true));
	}

	// ��������
	bool TerrainObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);
		switch(id)
		{
		case ID_Terrain:		// ��������
			{
				return true;
			}
			break;
		case ID_TerrainFile:		// �����ļ�
			{
				m_fileName = any_cast<String>(value);
				m_terrain->loadTerrain(m_fileName);
				
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// ��ȡ����ֵ
	Any TerrainObject::getProperty(uint id)const
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::getProperty(id);
		switch(id)
		{
		case ID_Terrain:		// ��������
			{
				return Any();
			}
			break;
		case ID_TerrainFile:		// �����ļ�
			{
				return Any(m_fileName);
			}
			break;
		default:
			return Any();
			break;
		}
	}


	// ����������ԣ���������ʽ
	const PropertyContainer::VTProperty* TerrainObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// ����������ԣ����б����ʽ
	const PropertyContainer::MapProperty* TerrainObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}
};
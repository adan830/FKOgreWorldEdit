#include "StdAfx.h"
#include "EntityObject.h"
#include "IScene.h"
#include "EntityMaterialInstance.h"
#include "EntityObjectVisible.h"
namespace Ogre
{

	const String EntityObjectFactory::FACTORY_TYPE = "EntityObject";

	// ��øù�������������
	const String &EntityObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// ��������Ĵ�������
	ISceneObject *EntityObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		EntityObject *obj = new EntityObject(name);
		obj->create(scene , params);
		return obj;
	}

	// ������������ٷ���
	void EntityObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	EntityObject::EntityObject(const String &name)
		: NullObject(name)
		, m_entity(0)
		, m_alpha(1.0f)
		, m_alphaController(0)
		, m_collisionEntity(0)
		, m_collisionType(CT_NONE)
	{
	}

	EntityObject::~EntityObject(void)
	{
		destroy();
	}

	// ��������ʵ��
	void EntityObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		if(params && params->find("mesh") != params->end())
		{
			setMeshFile(params->find("mesh")->second);
		}
	}

	// ���ٶ���ʵ��
	void EntityObject::destroy()
	{
		destroyMesh();
		NullObject::destroy();
	}

	// ֡��������ÿִ֡��
	bool EntityObject::frameStarted( const FrameEvent& evt )
	{
		// TODO:����Ҫ�Ż���������frameStartedÿ֡��ȥ���¶���״̬
		// ��Ϊ����������������Object��ʱ���û��Ҫ���¹��������ˡ�
		// ���¶���������
		if(!m_animationController.isNull())
		{
			m_animationController.update(evt.timeSinceLastFrame);
		}
		return true;
	}

	// �����Ƿ���ʾ
	void EntityObject::setVisible(bool visible)
	{
		NullObject::setVisible(visible);
		if(m_sceneObjectVisible)
		{
			m_sceneObjectVisible->showBorder(m_sceneObjectVisible->isShowBorder());
		}
	}
	// ������ײģ������
	void EntityObject::setCollisionShapeType(CollisionShapeType type)
	{
		m_collisionEntity->setCollisionShapeType(type);
	}

	// ������ײģ��
	void EntityObject::setCollisionShapeName(const String &name)
	{
		m_collisionEntity->setCollisionShapeName(name);
	}

	// ����͸����
	void EntityObject::setAlpha(float alpha)
	{
		m_alpha = std::max(std::min(alpha , 1.0f) , 0.0f);
		if(m_alpha == 1.0f)
		{
			if(m_alphaController)
			{
				m_alphaController->setTransparency(1.0f);
				OGRE_DELETE(m_alphaController);
			}
			return;
		}
		if(m_alphaController == 0)
		{
			m_alphaController = new EntityMaterialInstance(m_entity);
		}
		// ����͸����
		m_alphaController->setTransparency(m_alpha);
	}
	// ���͸����
	float EntityObject::getAlpha()const
	{
		return m_alpha;
	}

	// ��һ����������ҽӵ���ͷ��
	void EntityObject::attachObjectToBone(const String &boneName,
		ISceneObject *obj,
		const Quaternion &offsetOrientation,
		const Vector3 &offsetPosition)
	{
		if(obj->getParent())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"�������� '" + obj->getName() + "'�ѹҽ���'" + obj->getParent()->getName() + "'��", 
				"EntityObject::attachObjectToBone");
		}
		// �ӽڵ����ҽ�
		static_cast<NullObject*>(obj)->getOgreSceneNode()->detachObject(obj->getOgreMovableObject());
		// �ҽӵ���ͷ
		TagPoint* tagPoint = m_entity->attachObjectToBone(boneName
			, obj->getOgreMovableObject()
			, offsetOrientation
			, offsetPosition);
		// ���浽�б�
		m_childObjectList[obj->getName()] = obj;
	}
	// ��һ����������ӹ�ͷȡ���ҽ�
	void EntityObject::detachObjectFromBone(ISceneObject *obj)
	{
		detachObjectFromBone(obj->getName());
	}
	void EntityObject::detachObjectFromBone(const String &objName)
	{
		ChildObjectList::iterator iter = m_childObjectList.find(objName);
		if(iter == m_childObjectList.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"�������� '" + objName + "'û�ҽ���'" + getName() + "'��", 
				"EntityObject::detachObjectFromBone");
		}
		m_entity->detachObjectFromBone(objName);
		// �ҽӻ��Լ��Ľڵ�
		static_cast<NullObject*>(iter->second)->getOgreSceneNode()->attachObject(iter->second->getOgreMovableObject());
		m_childObjectList.erase(iter);
	}
	// �����г�������ӹ�ͷȡ���ҽ�
	void EntityObject::detachAllObjectsFromBone()
	{
		while(m_childObjectList.size() != 0)
		{
			detachObjectFromBone(m_childObjectList.begin()->first);
		}
	}
	// ����ѹҽӵ���ͷ�ĳ������
	EntityObject::ChildObjectListIterator EntityObject::getAttachedObjectIterator(void)
	{
		return ChildObjectListIterator(m_childObjectList.begin() , m_childObjectList.end());
	}

	// ʹ�����Mesh���ع�Entity
	bool EntityObject::setMeshFile(const String &meshFileName)
	{
		if(m_meshFileName == meshFileName)
			return true;
		destroyMesh();
		// ����ʵ��
		m_entity = m_scene->getOgreSceneManager()->createEntity(getFullName() , meshFileName);
		
		m_entity->setUserAny(Any(static_cast<ISceneObject*>(this)));
		// �ҽ���ȥ
		m_node->attachObject(m_entity);
		// ��������������
		m_animationController.create(m_entity);
		Root::getSingleton().addFrameListener(this);
		m_meshFileName = meshFileName;

		if(m_scene->getCollisionWorld())
		{
			m_collisionEntity = m_scene->getCollisionWorld()->createEntityCollisionEntity(m_entity);
		}

		return true;
	}
	// ���Mesh�ļ���
	const String &EntityObject::getMeshFile()const
	{
		return m_meshFileName;
	}
	// ����mesh
	void EntityObject::destroyMesh()
	{
		OGRE_DELETE(m_sceneObjectVisible);
		// ������ײʵ��
		if(m_collisionEntity)
		{
			m_scene->getCollisionWorld()->destroyCollisionEntity(m_collisionEntity);
			m_collisionEntity = 0;
		}
		// ����ģ��ʵ��
		if(m_entity)
		{
			detachAllObjectsFromBone();
			Root::getSingleton().removeFrameListener(this);
			// ����͸��������
			OGRE_DELETE(m_alphaController);
			// ���ٶ���������
			m_animationController.destroy();
			// ����ģ��ʵ��
			m_scene->getOgreSceneManager()->destroyEntity(m_entity);
			m_entity = 0;
			m_meshFileName.clear();
		}
	}


	// ö���������ԣ���������ʽ
	void EntityObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_Entity , PT_GROUP , "ģ������" , "ģ������" , true , false , false));
		properties.back().addChild(Property(ID_MeshFile , PT_STRING , "ģ���ļ�" , "ģ���ļ�" , true , true , true));
		properties.back().addChild(Property(ID_Alpha , PT_REAL , "͸����" , "͸����" , true , true , true));
		properties.back().addChild(Property(ID_CollisionShapeType , PT_UINT , "��ײģ������" , "��ײģ������" , true , true , true));
		properties.back().addChild(Property(ID_CollisionShapeName , PT_STRING , "��ײģ������" , "��ײģ������" , true , true , true));
	}

	// ��������
	bool EntityObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);
		switch(id)
		{
		case ID_Entity:		// һ������
			{
				return true;
			}
			break;
		case ID_MeshFile:	// ģ���ļ�
			{
				setMeshFile(any_cast<String>(value));
				return true;
			}
			break;
		case ID_Alpha:	// ͸����
			{
				setAlpha(any_cast<Real>(value));
				return true;
			}
			break;
		case ID_CollisionShapeType:	// ��ײģ������
			{
				setCollisionShapeType((CollisionShapeType)any_cast<uint>(value));
				return true;
			}
			break;
		case ID_CollisionShapeName:	// ��ײģ������
			{
				setCollisionShapeName(any_cast<String>(value));
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// ��ȡ����ֵ
	Any EntityObject::getProperty(uint id)const
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::getProperty(id);
		switch(id)
		{
		case ID_Entity:		// ģ������
			{
				return Any();
			}
			break;
		case ID_MeshFile:	// ģ���ļ�
			{
				return Any(getMeshFile());
			}
			break;
		case ID_Alpha:		// ͸����
			{
				return Any((Real)getAlpha());
			}
			break;
		case ID_CollisionShapeType:	// ��ײģ������
			{
				return Any((uint)getCollisionShapeType());
			}
			break;
		case ID_CollisionShapeName:	// ��ײģ������
			{
				return Any(getCollisionShapeName());
			}
			break;
	default:
			return Any();
			break;
		}
	}

	// ����������ԣ���������ʽ
	const PropertyContainer::VTProperty* EntityObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// ����������ԣ����б����ʽ
	const PropertyContainer::MapProperty* EntityObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}


	// ��ó�������Ŀɼ����������ڱ༭����ѡ��
	ISceneObjectVisible *EntityObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new EntityObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}
};

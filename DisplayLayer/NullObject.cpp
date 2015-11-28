#include "StdAfx.h"
#include "NullObject.h"
#include "IScene.h"
#include "DisplaySystem.h"
#include "Scene.h"
#include "NullObjectVisible.h"
namespace Ogre
{

	const String NullObjectFactory::FACTORY_TYPE = "NullObject";

	// ��øù�������������
	const String &NullObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// ��������Ĵ�������
	ISceneObject *NullObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		ISceneObject *obj = new NullObject(name);
		obj->create(scene , params);
		return obj;
	}
	// ������������ٷ���
	void NullObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}


	NullObject::NullObject(const String &name)
		: m_name(name)
		, m_node(0)
		, m_scene(0)
		, m_parent(0)
		, m_visible(true)
		, m_autoDeleteNode(false)
		, m_canCollide(true)
		, m_sceneObjectVisible(0)
	{
	}

	NullObject::~NullObject(void)
	{
		destroy();
	}

	// ��������ʵ��
	void NullObject::create(IScene *scene , const NameValuePairList* params)
	{
		m_scene = scene;

		if(params)
		{
			m_creationParams = *params;
			// ���ָ���˳����ڵ����ƣ���ֱ��������ڵ�
			NameValuePairList::const_iterator iter = params->find("isRootNode");
			if(iter != params->end())
			{
				m_node = m_scene->getOgreSceneManager()->getRootSceneNode();
				m_autoDeleteNode = false;
			}
		}
		// ���û��ָ���ڵ����ƣ���ȥ����һ����������ʱ���ҽ�
		if(!m_node)
		{
			m_node = m_scene->getOgreSceneManager()->createSceneNode(getFullName());
			m_autoDeleteNode = true;
		}
	}

	// ���ٶ���ʵ��
	void NullObject::destroy()
	{
		OGRE_DELETE(m_sceneObjectVisible);
		if(m_parent)
		{
			m_parent->removeChildObject(this);
		}
		while(m_mapChildObject.size() != 0)
		{
			removeChildObject(m_mapChildObject.begin()->second);
		}
		if(m_node && m_autoDeleteNode)
		{
			m_scene->getOgreSceneManager()->destroySceneNode(m_node->getName());
			m_node = 0;
		}
		m_creationParams.clear();
	}



	// ��ô������ĳ���
	IScene *NullObject::getScene()const
	{
		return m_scene;
	}

	// ����������
	bool NullObject::setName(const String &name)
	{
		if(m_scene->hasSceneObject(name))
			return false;
		static_cast<Scene*>(m_scene)->renameSceneObject(m_name , name);
		m_name = name;
		return true;
	}
	// ��ö�������
	const String &NullObject::getName()const
	{
		return m_name;
	}
	// �����Ogre�еĶ�������
	String NullObject::getFullName()const
	{
		return m_scene->getName() + m_name;
	}

	// �����Ƿ���ʾ
	void NullObject::setVisible(bool visible)
	{
		m_node->setVisible(visible);
		m_visible = visible;
	}
	// �Ƿ���ʾ
	bool NullObject::isVisible()const
	{
		return m_visible;
	}

	// �����������
	void NullObject::setPosition(const Vector3 &pos)
	{
		m_node->setPosition(pos);
	}

	// ����������
	const Vector3 &NullObject::getPosition()const
	{
		return m_node->getPosition();
	}

	// ������������
	void NullObject::setWorldPosition(const Vector3 &pos)
	{
		m_node->setPosition(pos - m_node->getWorldPosition() + m_node->getPosition());
	}

	// �����������
	const Vector3 &NullObject::getWorldPosition()const
	{
		return m_node->getWorldPosition();
	}


	// �����������
	void NullObject::setScale(const Vector3 &scale)
	{
		m_node->setScale(scale);
	}

	// ����������
	const Vector3 &NullObject::getScale()const
	{
		return m_node->getScale();
	}

	// ������������
	void NullObject::setWorldScale(const Vector3 &scale)
	{
		const Vector3 &worldScale = m_node->_getDerivedScale();
		const Vector3 &localScale = m_node->getScale();
		Vector3 scaleResult;
		if(worldScale.x == 0.0f)
			scaleResult.x = 0.0f;
		else
			scaleResult.x = scale.x / worldScale.x * localScale.x;
		if(worldScale.y == 0.0f)
			scaleResult.y = 0.0f;
		else
			scaleResult.y = scale.y / worldScale.y * localScale.y;
		if(worldScale.z == 0.0f)
			scaleResult.z = 0.0f;
		else
			scaleResult.z = scale.z / worldScale.z * localScale.z;

		m_node->setScale(scaleResult);
	}

	// �����������
	const Vector3 &NullObject::getWorldScale()const
	{
		return m_node->_getDerivedScale();
	}


	// ������Է�λ
	void NullObject::setOrientation(const Quaternion &orientation)
	{
		m_node->setOrientation(orientation);
	}

	// ����������
	const Quaternion &NullObject::getOrientation()const
	{
		return m_node->getOrientation();
	}

	// �������緽λ
	void NullObject::setWorldOrientation(const Quaternion &orientation)
	{
		// ����������ת�������orientation���õ��Ľ���Ǵ�������ת��orientation�Ľ���תƫ��
		m_node->setOrientation(m_node->getOrientation() * (m_node->getWorldOrientation().Inverse() * orientation));
	}

	// ������緽λ
	const Quaternion &NullObject::getWorldOrientation()const
	{
		return m_node->getWorldOrientation();
	}


	// ���ø�����
	void NullObject::setParent(ISceneObject *parent)
	{
		removeFromParentObject();
		parent->addChildObject(this);
	}
	// ��ø�����
	ISceneObject* NullObject::getParent()const
	{
		return m_parent;
	}


	// ����ĳ���������꣬������������ڲ��������緽λ
	void NullObject::lookAt(const Vector3& targetPoint)
	{
		m_node->lookAt(targetPoint , Node::TS_WORLD);
	}

	// ���ݵ�ǰ����ֵ����
	void NullObject::scale(const Vector3& scale)
	{
		m_node->scale(scale);
	}

	// ���ݵ�ǰλ���ƶ�
	void NullObject::translate(const Vector3& distance)
	{
		m_node->translate(distance);
	}

	// ���ݵ�ǰ��λ��ת
	void NullObject::rotate(const Quaternion& rotate)
	{
		m_node->rotate(rotate);
	}

	// ��һ����������ҽӵ�����
	void NullObject::addChildObject(ISceneObject *obj)
	{
		if(obj->getParent())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"�������� '" + obj->getName() + "'�ѹҽ���'" + obj->getParent()->getName() + "'��", 
				"NullObject::addChildObject");
			return;
		}
		m_mapChildObject[obj->getName()] = obj;

		SceneNode *node = static_cast<NullObject*>(obj)->getOgreSceneNode();
		if(node->getParentSceneNode())
		{
			node->getParentSceneNode()->removeChild(node);
		}
		m_node->addChild(node);
		static_cast<NullObject*>(obj)->m_parent = this;
	}
	// ��һ����������ӵ�����ȡ���ҽ�
	void NullObject::removeChildObject(ISceneObject *obj)
	{
		if(!hasChildObject(obj->getName()))
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"�������� '" + obj->getName() + "'�ҽ���'" + obj->getParent()->getName() + "'�ϣ������ǹҽ��ڱ��ڵ�'" + getName() + "'��", 
				"NullObject::removeChildObject");
			return;
		}
		m_mapChildObject.erase(m_mapChildObject.find(obj->getName()));

		SceneNode *node = static_cast<NullObject*>(obj)->getOgreSceneNode();
		if(node->getParentSceneNode())
		{
			node->getParentSceneNode()->removeChild(node);
		}
		static_cast<NullObject*>(obj)->m_parent = 0;
	}

	// �������������Ӹ���������ȡ���ҽ�
	void NullObject::removeFromParentObject()
	{
		if(m_parent)
		{
			m_parent->removeChildObject(this);
			m_parent = 0;
		}
	}

	// ����Ѹ��ӵĳ������ö��
	ISceneObject::ChildObjectIterator NullObject::getChildObjectIterator()
	{
		return ChildObjectIterator(m_mapChildObject.begin() , m_mapChildObject.end());
	}

	// �������ͺ����ƴ���һ���˳���������ӳ�������
	ISceneObject *NullObject::createChildObject(const String &name , const String &type, const NameValuePairList* params)
	{
		ISceneObject *obj = m_scene->createSceneObject(name , type , params);
		addChildObject(obj);
		return obj;
	}

	// �������ͺ����ƴ���һ���˳���������ӳ�������
	ISceneObject *NullObject::createChildObject(const String &type, const NameValuePairList* params)
	{
		ISceneObject *obj = m_scene->createSceneObject(type , params);
		addChildObject(obj);
		return obj;
	}

	// ����һ���˳���������ӳ�������
	void NullObject::destroyChildObject(const String &name)
	{
		MapChildObject::iterator iter = m_mapChildObject.find(name);
		if(iter == m_mapChildObject.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"�Ҳ�������Ϊ '" + name + "' ���ӳ�������", 
				"NullObject::destroyChildObject");
			return;
		}
		m_scene->destroySceneObject(name);
	}

	void NullObject::destroyChildObject(ISceneObject *obj)
	{
		destroyChildObject(obj->getName());
	}

	// ���һ���˳���������ӳ�������
	ISceneObject *NullObject::getChildObject(const String &name)
	{
		MapChildObject::iterator iter = m_mapChildObject.find(name);
		if(iter == m_mapChildObject.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"�Ҳ�������Ϊ '" + name + "' ���ӳ�������", 
				"NullObject::getChildObject");
			return 0;
		}
		return iter->second;
	}

	// �Ƿ����ָ�����ֵĴ˳���������ӳ�������
	bool NullObject::hasChildObject(const String &name)const
	{
		return m_mapChildObject.find(name) != m_mapChildObject.end();
	}
	// ��newName��Ϊ���ƿ�¡һ���µĶ���
	ISceneObject *NullObject::clone(const String &newName , IScene *newScene)
	{
		ISceneObject *obj = (newScene ? newScene : m_scene)->createSceneObject(newName , getType() , &m_creationParams);
		cloneData(obj);
		return obj;
	}
	// ���Զ����ɵ�������Ϊ���ƿ�¡һ���µĶ���
	ISceneObject *NullObject::clone(IScene *newScene)
	{
		return clone((newScene ? newScene : m_scene)->getUniqueName() , newScene);
	}

	// �ѵ�ǰ�����е����ݿ�¡��target��
	void NullObject::cloneData(ISceneObject *target)
	{
		if(target->getType() != getType())
		{
			OGRE_EXCEPT(Exception::ERR_INVALID_STATE, 
				"target�������Ͳ�һ�� '" + target->getType() + "' ���ǵ�ǰ��������� '" + getType() + "'", 
				"NullObject::cloneData");
			return;
		}
		cloneProperties(target);
	}

	// ����������ԣ���������ʽ
	const PropertyContainer::VTProperty* NullObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// ����������ԣ����б����ʽ
	const PropertyContainer::MapProperty* NullObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

	// ö���������ԣ���������ʽ
	void NullObject::enumPropertiesTree(VTProperty &properties)const
	{
		properties.push_back(Property( ID_General , PT_GROUP , "һ������" , "һ������" , true , false , false));
		properties.back().addChild(Property(ID_Type , PT_STRING , "����" , "����" , true , false , false));
		properties.back().addChild(Property(ID_Name , PT_STRING , "����" , "����" , true , true , false));
		properties.back().addChild(Property(ID_Position , PT_VECTOR3 , "λ��" , "λ��" , true , true , true));
		properties.back().addChild(Property(ID_Rotate , PT_VECTOR3 , "��ת" , "��ת" , true , true , true));
		properties.back().addChild(Property(ID_Scale , PT_VECTOR3 , "����" , "����" , true , true , true));
		properties.back().addChild(Property(ID_Visible , PT_BOOL , "�ɼ�" , "�ڵ��Ƿ�ɼ�" , true , true , true));
		properties.back().addChild(Property(ID_Parent , PT_STRING , "���ڵ�" , "���ڵ�" , false , false , true));
		properties.back().addChild(Property(ID_CanCollide , PT_BOOL , "��ײ����" , "��ײ����" , true , true , true));
	}

	// �����Ƿ�μ���ײ
	void NullObject::setCanCollide(bool can)
	{
		m_canCollide = can;
	}

	// �����Ƿ�μ���ײ
	bool NullObject::getCanCollide()const
	{
		return m_canCollide;
	}

	// ��������
	bool NullObject::setProperty(uint id , const Any &value)
	{
		switch(id)
		{
		case ID_General:		// һ������
			{
				return true;
			}
			break;
		case ID_Name:	// ����
			{
				setName(any_cast<String>(value));
				return true;
			}
			break;
		case ID_Position:	// λ��
			{
				setPosition(any_cast<Vector3>(value));
				return true;
			}
			break;
		case ID_Rotate:		// ��ת
			{
				Vector3 rotate(any_cast<Vector3>(value));
				rotate.x = Math::DegreesToRadians(rotate.x);
				rotate.y = Math::DegreesToRadians(rotate.y);
				rotate.z = Math::DegreesToRadians(rotate.z);
				setOrientation(MathEx::EularToQuaternion(rotate));
				return true;
			}
			break;
		case ID_Scale:		// ����
			{
				setScale(any_cast<Vector3>(value));
				return true;
			}
			break;
		case ID_Visible:		// �ɼ�
			{
				setVisible(any_cast<bool>(value));
				return true;
			}
			break;
		case ID_Parent:		// ���ڵ�
			{
				String parentName(any_cast<String>(value));
				if(parentName.empty())
					removeFromParentObject();
				else
					setParent(m_scene->getSceneObject(parentName));
				return true;
			}
			break;
		case ID_CanCollide:		// ��ײ����
			{
				m_canCollide = any_cast<bool>(value);
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// ��ȡ����ֵ
	Any NullObject::getProperty(uint id)const
	{
		switch(id)
		{
		case ID_General:		// һ������
			{
				return Any();
			}
			break;
		case ID_Name:		// ����
			{
				return Any(getName());
			}
			break;
		case ID_Type:		// λ��
			{
				return Any(getType());
			}
			break;
		case ID_Position:	// λ��
			{
				return Any(getPosition());
			}
			break;
		case ID_Rotate:		// ��ת
			{
				Vector3 rotate(MathEx::QuaternionToEular(getOrientation()));
				rotate.x = Math::RadiansToDegrees(rotate.x);
				rotate.y = Math::RadiansToDegrees(rotate.y);
				rotate.z = Math::RadiansToDegrees(rotate.z);
				return Any(rotate);
			}
			break;
		case ID_Scale:		// ����
			{
				return Any(getScale());
			}
			break;
		case ID_Visible:		// �ɼ�
			{
				return Any(isVisible());
			}
			break;
		case ID_Parent:		// ���ڵ�
			{
				return Any(m_parent ? m_parent->getName() : StringUtil::BLANK);
			}
			break;
		case ID_CanCollide:		// ��ײ����
			{
				return Any(m_canCollide);
			}
			break;
		default:
			return Any();
			break;
		}
	}

	// ��ó�������Ŀɼ����������ڱ༭����ѡ��
	ISceneObjectVisible *NullObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new NullObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}

};
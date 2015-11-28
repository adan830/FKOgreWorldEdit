#include "StdAfx.h"
#include "XMLSceneSerializer.h"
#include "IScene.h"
namespace Ogre
{

	XMLSceneSerializer::XMLSceneSerializer(void)
	{
	}

	XMLSceneSerializer::~XMLSceneSerializer(void)
	{
	}


	// ���볡��
	bool XMLSceneSerializer::importScene(DataStreamPtr& stream, IScene* scene)
	{
		m_scene = scene;
		// ��XML�ļ�
		if(!openXML(stream))
			return false;
		// ��������Scene
		for (TiXmlElement* element = m_xmlDoc->RootElement()->FirstChildElement("Scene");
			element != 0; element = element->NextSiblingElement("Scene"))
		{
			readScene(element);
		}
		return true;
	}


	// ��ȡ����
	void XMLSceneSerializer::readScene(TiXmlElement *parent)
	{
		ISceneObject *obj = m_scene->getRootObject();
		readSceneObjectChilds(parent , obj);
	}
	// ��ȡ�����Ӷ���
	void XMLSceneSerializer::readSceneObjectChilds(TiXmlElement *element , ISceneObject *parent)
	{
		// ��������SceneObject
		for (TiXmlElement* childElement = element->FirstChildElement("SceneObject");
			childElement != 0; childElement = childElement->NextSiblingElement("SceneObject"))
		{
			readSceneObject(childElement , parent);
		}
	}

	// ��ȡ��������
	void XMLSceneSerializer::readSceneObject(TiXmlElement *element , ISceneObject *parent)
	{
		String name = getAttribute(element , "Name");
		String type = getAttribute(element , "Type");
		NameValuePairList params(StringConverterEx::parseNameValuePairList(getAttribute(element , "CreationParams")));
		// ������������
		ISceneObject *obj = parent->createChildObject(name , type , &params);
		// ������������
		for (TiXmlElement* propertyElement = element->FirstChildElement("Property");
			propertyElement != 0; propertyElement = propertyElement->NextSiblingElement("Property"))
		{
			// ��ȡֵ
			uint propertyId = StringConverter::parseUnsignedInt(getAttribute(propertyElement , "Id"));
			PropertyContainer::PropertyType propertyType = (PropertyContainer::PropertyType)StringConverter::parseUnsignedInt(getAttribute(propertyElement , "Type"));
			String propertyValue = getAttribute(propertyElement , "Value");
			if(propertyId)
			{
				obj->setProperty(propertyId , PropertyContainer::toAny(propertyValue , propertyType));
			}
		}
		// ��ȡ�����Ӷ���
		TiXmlElement *childsElement = element->FirstChildElement("Childs");
		if(childsElement)
		{
			readSceneObjectChilds(childsElement , obj);
		}
	}

	// ��������
	bool XMLSceneSerializer::exportScene(IScene* scene, const String& filename)
	{
		m_scene = scene;

		LogManager::getSingleton().logMessage("XMLSceneSerializer writing XMLScene data to " + filename + "...");

		// �½�xml�ļ�
		if(!newXML("Scenes"))
		{
			return false;
		}
		TiXmlElement *scenesElement = m_xmlDoc->RootElement();

		LogManager::getSingleton().logMessage("Writing XMLScene data...");
		writeScene(scenesElement);
		LogManager::getSingleton().logMessage("XMLScene data exported.");

		saveXML(filename);
		LogManager::getSingleton().logMessage("XMLSceneSerializer export successful.");

		closeXML();

		return true;
	}
	// д�볡��
	void XMLSceneSerializer::writeScene(TiXmlElement *parent)
	{
		// ��ʼ��
		TiXmlElement *sceneElement = newElement(parent , "Scene");
		ISceneObject *obj = m_scene->getRootObject();
		writeSceneObjectChilds(sceneElement , obj);
	}
	// д�볡������
	void XMLSceneSerializer::writeSceneObject(TiXmlElement *parent , ISceneObject *obj)
	{
		const PropertyContainer::MapProperty *properties;
		properties = obj->getPropertiesList();
		// ��ʼ��
		TiXmlElement *element = newElement(parent , "SceneObject");
		// д������
		setAttribute(element , "Name" , obj->getName());
		// д������
		setAttribute(element , "Type" , obj->getType());
		// д�봴������
		setAttribute(element , "CreationParams" , obj->getCreationParams());

		// д����������
		for(PropertyContainer::MapProperty::const_iterator iter = properties->begin() ; iter != properties->end() ; iter ++)
		{
			if(iter->second.saveable)
			{
				TiXmlElement *propertyElement = newElement(element , "Property");
				setAttribute(propertyElement , "Name" , iter->second.name);
				setAttribute(propertyElement , "Id" , StringConverter::toString(iter->second.id));
				setAttribute(propertyElement , "Type" , StringConverter::toString(iter->second.type));
				setAttribute(propertyElement , "Value" , PropertyContainer::toString(obj->getProperty(iter->second.id) , iter->second.type));
			}
		}
		// д�������Ӷ���
		TiXmlElement *childs = newElement(element , "Childs");
		writeSceneObjectChilds(childs , obj);
	}

	// д���ӳ�������
	void XMLSceneSerializer::writeSceneObjectChilds(TiXmlElement *parent , ISceneObject *obj)
	{
		ISceneObject::ChildObjectIterator iter = obj->getChildObjectIterator();
		while(iter.hasMoreElements())
		{
			ISceneObject *objChild = iter.getNext();
			writeSceneObject(parent , objChild);
		}
	}
};
#include "StdAfx.h"
#include "SneSceneSerializer.h"
#include "IScene.h"
namespace Ogre
{
	SneSceneSerializer::SneSceneSerializer(void)
		: m_scene(0)
	{
	}

	SneSceneSerializer::~SneSceneSerializer(void)
	{
	}


	// ���볡��
	bool SneSceneSerializer::importScene(DataStreamPtr& stream, IScene* scene)
	{
		m_scene = scene;
		// Determine endianness (must be the first thing we do!)
		determineEndianness(stream);
		// Check header
		readFileHeader(stream);

		SneSceneChunkID streamID;
		while(!stream->eof())
		{
			streamID = (SneSceneChunkID)readChunk(stream);
			switch (streamID)
			{
			case S_SCENE:
				readScene(stream);
				break;
			default:
				// ����ʶ���������������
				skipCurrentChunk(stream);
				break;
			}
		}
		return true;
	}

	// ��ȡ����
	void SneSceneSerializer::readScene(DataStreamPtr& stream)
	{
		ISceneObject *obj = m_scene->getRootObject();
		SneSceneChunkID streamID;
		while(!stream->eof())
		{
			streamID = (SneSceneChunkID)readChunk(stream);
			if(streamID == S_SCENE_OBJECT)
			{
				readSceneObject(stream, obj);
			}
			else if(streamID == S_SCENE_END)
			{
				return;
			}
			else
			{
				// ����ʶ���������������
				skipCurrentChunk(stream);
			}
		}
	}
	// ��ȡ��������
	void SneSceneSerializer::readSceneObject(DataStreamPtr& stream , ISceneObject *parent)
	{
		String name = readString(stream);
		String type = readString(stream);
		NameValuePairList params;
		readNameValuePairList(stream , params);
		// ������������
		ISceneObject *obj = parent->createChildObject(name , type , &params);
		uint32 size;
		readInts(stream , &size , 1);
		for(uint32 i = 0 ; i < size ; i ++)
		{
			uint32 id , type;
			readInts(stream , &id , 1);
			readInts(stream , &type , 1);
			// ��ȡֵ
			Any value = readAny(stream , (PropertyContainer::PropertyType)type);
			if(!value.isEmpty())
			{
				obj->setProperty(id , value);
			}
		}
		// ��������
		uint16 streamID = readChunk(stream);
		if(streamID != S_SCENE_OBJECT_END)
		{
			// ����ʶ���������������
			skipCurrentChunk(stream);
			LogManager::getSingleton().logMessage("��ƥ���SceneObject�������� Begin "
				+ StringConverter::toString(S_SCENE_OBJECT) + " , End " + StringConverter::toString(streamID));
		}
	}

	// ��������
	bool SneSceneSerializer::exportScene(IScene* scene, const String& filename)
	{
		m_scene = scene;

		LogManager::getSingleton().logMessage("SneSceneSerializer writing SneScene data to " + filename + "...");

		// Decide on endian mode
		determineEndianness(ENDIAN_NATIVE);
		mpfFile = fopen(filename.c_str(), "wb");
		if (!mpfFile)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"Unable to open file " + filename + " for writing",
				"SneSceneSerializer::exportScene");
			return false;
		}

		writeFileHeader();
		LogManager::getSingleton().logMessage("File header written.");

		LogManager::getSingleton().logMessage("Writing SneScene data...");
		writeScene();
		LogManager::getSingleton().logMessage("SneScene data exported.");

		fclose(mpfFile);
		LogManager::getSingleton().logMessage("SneSceneSerializer export successful.");


		return true;
	}
	// д�볡��
	void SneSceneSerializer::writeScene()
	{
		// ��ʼ��
		beginWriteChunk(S_SCENE , S_SCENE_END);

		ISceneObject *obj = m_scene->getRootObject();
		writeSceneObjectChilds(obj);
		// ������
		endWriteChunk(S_SCENE , S_SCENE_END);
	}
	// д�볡������
	void SneSceneSerializer::writeSceneObject(ISceneObject *obj)
	{
		const PropertyContainer::MapProperty *properties;
		properties = obj->getPropertiesList();
		// ��ʼ��
		beginWriteChunk(S_SCENE_OBJECT , S_SCENE_OBJECT_END);
		// д������
		writeString(obj->getName());
		// д������
		writeString(obj->getType());
		// д�봴������
		writeNameValuePairList(obj->getCreationParams());
	
		// д����������
		uint32 size = 0;
		// ������Ҫд�����������
		for(PropertyContainer::MapProperty::const_iterator iter = properties->begin() ; iter != properties->end() ; iter ++)
		{
			if(iter->second.saveable)
			{
				size++;
			}
		}
		// д������	
		writeInts(&size , 1);
		// д����������
		for(PropertyContainer::MapProperty::const_iterator iter = properties->begin() ; iter != properties->end() ; iter ++)
		{
			if(iter->second.saveable)
			{
				writeInts(&iter->second.id , 1);
				writeInts(&(uint32&)iter->second.type , 1);
				writeAny(obj->getProperty(iter->second.id) , iter->second.type);
			}
		}
		// ������
		endWriteChunk(S_SCENE_OBJECT , S_SCENE_OBJECT_END);
		// д�������Ӷ���
		writeSceneObjectChilds(obj);
	}

	// д���ӳ�������
	void SneSceneSerializer::writeSceneObjectChilds(ISceneObject *obj)
	{
		ISceneObject::ChildObjectIterator iter = obj->getChildObjectIterator();
		while(iter.hasMoreElements())
		{
			ISceneObject *objChild = iter.getNext();
			writeSceneObject(objChild);
		}
	}
};
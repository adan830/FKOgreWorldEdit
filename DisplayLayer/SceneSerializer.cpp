#include "StdAfx.h"
#include "SceneSerializer.h"
#include "IScene.h"
#include "OsmSceneSerializer.h"
#include "SneSceneSerializer.h"
#include "XMLSceneSerializer.h"
namespace Ogre
{

	SceneSerializer::SceneSerializer(void)
		: m_serializer(0)
	{
	}

	SceneSerializer::~SceneSerializer(void)
	{
		OGRE_DELETE(m_serializer);
	}

	// ���볡��
	bool SceneSerializer::importScene(const String& filename, IScene* scene)
	{
		// ���ļ�
		DataStreamPtr stream = ResourceGroupManager::getSingleton().openResource(filename);
		if(stream.isNull())
			return false;
		// ��ô��л���
		ISceneSerializer *serializer = getSerializer(filename);
		if(serializer == 0)
			return false;
		scene->_notifyOrigin(filename);
		// ���볡��
		return serializer->importScene(stream , scene);
	}
	// ��������
	bool SceneSerializer::exportScene(IScene* scene, const String& filename)
	{
		// ��ô��л���
		ISceneSerializer *serializer = getSerializer(filename);
		if(serializer == 0)
			return false;
		// ��������
		return serializer->exportScene(scene , filename);
	}

	// �����ļ���չ���Զ��ֱ���������ĸ�ʽ����ô��л���
	ISceneSerializer *SceneSerializer::getSerializer(const String& filename)
	{
		// �����ļ���
		String outFilePath , outFileName , outBaseName , outExtention;
		StringUtil::splitFilename(filename , outFileName , outFilePath);
		StringUtil::splitBaseFilename(outFileName , outBaseName , outExtention);
		StringUtil::toLowerCase(outExtention);
		OGRE_DELETE(m_serializer);
		// ʹ��Osm���л���������
		if(outExtention == "osm")
		{
			m_serializer = new OsmSceneSerializer;
			return m_serializer;
		}
		// ʹ��SNE���л���������
		if(outExtention == "sne")
		{
			m_serializer = new SneSceneSerializer;
			return m_serializer;
		}
		// ʹ��xml���л���������
		if(outExtention == "xml")
		{
			m_serializer = new XMLSceneSerializer;
			return m_serializer;
		}
		return 0;
	}

};
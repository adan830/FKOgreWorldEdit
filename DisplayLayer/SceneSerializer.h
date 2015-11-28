#pragma once
#include "SneSceneSerializer.h"
#include "OsmSceneSerializer.h"
namespace Ogre
{
	class IScene;
	class ISceneSerializer;
	// �������л�ʵ��
	class SceneSerializer
	{
	public:
		SceneSerializer(void);
		virtual ~SceneSerializer(void);

		// ���볡����������ļ���չ���Զ��ֱ���������ĸ�ʽ��������Xml��ʽ����sne��ʽ
		bool importScene(const String& filename, IScene* scene);
		// ����������������ļ���չ���Զ��ֱ���������ĸ�ʽ��������Xml��ʽ����sne��ʽ
		bool exportScene(IScene* scene, const String& filename);

		// �����ļ���չ���Զ��ֱ���������ĸ�ʽ����ô��л���
		ISceneSerializer *getSerializer(const String& filename);
	protected:
		ISceneSerializer *m_serializer;
	};

};
#pragma once
#include "ISceneSerializer.h"
#include "SerializerEx.h"
#include "ISceneObject.h"
namespace Ogre
{

#define CHUNK_END 1

	enum SneSceneChunkID
	{
		// �ļ�ͷ
		S_HEADER = 0x1000,

		// ������һ��С�����ļ������ж������
		S_SCENE = 0x2000,

			// ��������
			S_SCENE_OBJECT = 0x3000,

			S_SCENE_OBJECT_END = S_SCENE_OBJECT + CHUNK_END,

		S_SCENE_END = S_SCENE + CHUNK_END,
	};

	// �����Ƹ�ʽ�ĳ����ļ����л���
	class SneSceneSerializer : public ISceneSerializer
		, public SerializerEx
	{
	public:
		SneSceneSerializer(void);
		virtual ~SneSceneSerializer(void);

		// ���볡��
		virtual bool importScene(DataStreamPtr& stream, IScene* scene);
		// ��������
		virtual bool exportScene(IScene* scene, const String& filename);

	protected:
		IScene *m_scene;

		// ��ȡ����
		void readScene(DataStreamPtr& stream);
		// ��ȡ��������
		void readSceneObject(DataStreamPtr& stream , ISceneObject *parent);


		// д�볡��
		void writeScene();

		// д�볡������
		void writeSceneObject(ISceneObject *obj);
		// д���ӳ�������
		void writeSceneObjectChilds(ISceneObject *obj);

	};
};
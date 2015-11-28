#pragma once
#include "XMLSerializer.h"
#include "ISceneSerializer.h"
#include "ISceneObject.h"

namespace Ogre
{

	// ʹ��XML��ʽ��ŵĳ������л�
	class XMLSceneSerializer : public ISceneSerializer
		, public XMLSerializer
	{
	public:
		XMLSceneSerializer(void);
		virtual ~XMLSceneSerializer(void);

		// ���볡��
		virtual bool importScene(DataStreamPtr& stream, IScene* scene);
		// ��������
		virtual bool exportScene(IScene* scene, const String& filename);

	protected:
		IScene *m_scene;

		// ��ȡ����
		void readScene(TiXmlElement *parent);
		// ��ȡ��������
		void readSceneObject(TiXmlElement *element , ISceneObject *parent);
		// ��ȡ�����Ӷ���
		void readSceneObjectChilds(TiXmlElement *element , ISceneObject *parent);


		// д�볡��
		void writeScene(TiXmlElement *parent);

		// д�볡������
		void writeSceneObject(TiXmlElement *parent , ISceneObject *obj);
		// д���ӳ�������
		void writeSceneObjectChilds(TiXmlElement *parent , ISceneObject *obj);

	};

};
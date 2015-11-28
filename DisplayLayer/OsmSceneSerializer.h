#pragma once
#include "TinyXMLInclude.h"
#include "ISceneSerializer.h"

namespace Ogre
{
	class NullObject;
	// XML��ʽ�ĳ����ļ����л���������oFusion��OSM��ʽ
	class OsmSceneSerializer : public ISceneSerializer
	{
	public:
		OsmSceneSerializer(void);
		virtual ~OsmSceneSerializer(void);

		// ���볡��
		virtual bool importScene(DataStreamPtr& stream, IScene* scene);
		// ��������
		virtual bool exportScene(IScene* scene, const String& filename);

	protected:
		// XML�ĵ�
		TiXmlDocumentPtr mXMLDoc;
		// ��ǰ�����ĳ���
		IScene *m_scene;
		// Ogre����������
		SceneManager *m_sceneMgr;
		// ��ȡ��������
		void readScene(TiXmlElement *parentElement);
		// ��ȡNullObject
		void readNullObject(NullObject *obj , TiXmlElement *parentElement);
		// ��ȡEntities
		void readEntities(TiXmlElement *parentElement);
		// ��ȡEntity
		void readEntity(TiXmlElement *parentElement);
		// ��ȡParticles
		void readParticles(TiXmlElement *parentElement);
		// ��ȡParticle
		void readParticle(TiXmlElement *parentElement);
		// ��ȡRibbonTrails
		void readRibbonTrails(TiXmlElement *parentElement);
		// ��ȡRibbonTrail
		void readRibbonTrail(TiXmlElement *parentElement);
		// ��ȡ������
		void readAnimations(NullObject *entity , TiXmlElement *parentElement);
		// ��ȡ����
		void readAnimation(NullObject *entity , TiXmlElement *parentElement);
		// ��ȡ����֡
		void readKeyframe(NodeAnimationTrack *track , TiXmlElement *parentElement);

		// �����ɫ���ԣ���Ӧxml�ڵ����r��g��b���ԣ�0~1��ʾ�������
		ColourValue getColorAttribute(TiXmlElement *element);
		// ���Vector3����Ӧxml�ڵ��x/y/z����
		Vector3 getVector3Attribute(TiXmlElement *element);
		// ���Quatation����Ӧxml�ڵ��x/y/z/w����
		Quaternion getQuaternionAttribute(TiXmlElement *element);
		// ����ַ�������
		String getAttribute(TiXmlElement *element , const char *name);
	};
};
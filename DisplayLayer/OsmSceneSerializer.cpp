#include "StdAfx.h"
#include "OsmSceneSerializer.h"
#include "IScene.h"
#include "EntityObject.h"
#include "ParticleObject.h"
#include "RibbonTrailObject.h"
#include "IAnimationStateController.h"
namespace Ogre
{
	OsmSceneSerializer::OsmSceneSerializer(void)
		: m_scene(0)
	{
	}

	OsmSceneSerializer::~OsmSceneSerializer(void)
	{
	}

	// ���볡��
	bool OsmSceneSerializer::importScene(DataStreamPtr& stream, IScene* scene)
	{
		mXMLDoc.bind(new TiXmlDocument());

		m_scene = scene;
		m_sceneMgr = m_scene->getOgreSceneManager();

		// ����XML�ļ�
		size_t iSize = stream->size();
		char *pBuf = new char[iSize+1];
		memset(pBuf, 0, iSize+1);
		stream->read(pBuf, iSize);
		stream.setNull();
		mXMLDoc->Parse(pBuf);
		OGRE_DELETE_ARRAY(pBuf);

		// ���XML�ļ��Ƿ��ȡ�ɹ�
		if(mXMLDoc->Error())
		{
			mXMLDoc.setNull();

			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"���볡��ʧ�� : " + scene->getOrigin(),
				"OsmSceneSerializer::importScene");
			return false;
		}
		// ��ȡ��������
		readScene(mXMLDoc->RootElement());
		return true;
	}
	// ��������
	bool OsmSceneSerializer::exportScene(IScene* scene, const String& filename)
	{
		m_scene = scene;
		m_sceneMgr = m_scene->getOgreSceneManager();
		return true;
	}
	// �����ɫ���ԣ���Ӧxml�ڵ����r��g��b���ԣ�0~1��ʾ�������
	ColourValue OsmSceneSerializer::getColorAttribute(TiXmlElement *element)
	{
		return ColourValue(StringConverter::parseReal(getAttribute(element , "r"))
			, StringConverter::parseReal(getAttribute(element , "g"))
			, StringConverter::parseReal(getAttribute(element , "b"))
			, element->Attribute("a") ? StringConverter::parseReal(getAttribute(element , "a")) : 1.0f
			);
	}
	// ���Vector3����Ӧxml�ڵ��x/y/z����
	Vector3 OsmSceneSerializer::getVector3Attribute(TiXmlElement *element)
	{
		return Vector3(StringConverter::parseReal(getAttribute(element , "x"))
			, StringConverter::parseReal(getAttribute(element , "y"))
			, StringConverter::parseReal(getAttribute(element , "z"))
			);
	}
	// ���Quatation����Ӧxml�ڵ��x/y/z/w����
	Quaternion OsmSceneSerializer::getQuaternionAttribute(TiXmlElement *element)
	{
		return Quaternion(StringConverter::parseReal(getAttribute(element , "w"))
			, StringConverter::parseReal(getAttribute(element , "x"))
			, StringConverter::parseReal(getAttribute(element , "y"))
			, StringConverter::parseReal(getAttribute(element , "z"))
			);
	}
	// ����ַ�������
	String OsmSceneSerializer::getAttribute(TiXmlElement *element , const char *name)
	{
		if(!element)
			return StringUtil::BLANK;
		const char *value = element->Attribute(name);
		if(!value)
			return StringUtil::BLANK;
		else
			return value;
	}
	// ��ȡ��������
	void OsmSceneSerializer::readScene(TiXmlElement *parentElement)
	{
		TiXmlElement *element;
		// ������
		element = parentElement->FirstChildElement("lightColor");
		m_sceneMgr->setAmbientLight(getColorAttribute(element));
		// ����Entityʵ��
		element = parentElement->FirstChildElement("entities");
		if(element)
		{
			readEntities(element);
		}
		// ����Particleʵ��
		element = parentElement->FirstChildElement("particles");
		if(element)
		{
			readParticles(element);
		}
		// ����RibbonTrailʵ��
		element = parentElement->FirstChildElement("ribbonTrails");
		if(element)
		{
			readRibbonTrails(element);
		}
		// �Զ����Ŷ���
		if(m_scene->hasAnimationStateController("autoplay"))
		{
			IAnimationStateController* ani = m_scene->getAnimationStateController("autoplay");
			if(ani)
			{
				ani->play();
			}
		}
	}
	// ��ȡNullObject
	void OsmSceneSerializer::readNullObject(NullObject *obj , TiXmlElement *parentElement)
	{
		// �����Ƿ�ɼ�
		obj->setVisible(!StringConverter::parseBool(getAttribute(parentElement , "hidden")));
		// λ��
		obj->setPosition(getVector3Attribute(parentElement->FirstChildElement("position")));
		// ��ת
		obj->setOrientation(getQuaternionAttribute(parentElement->FirstChildElement("rotation")));
		// ����
		obj->setScale(getVector3Attribute(parentElement->FirstChildElement("scale")));
		// ֡����
		TiXmlElement *element = parentElement->FirstChildElement("animations");
		if(element)
		{
			readAnimations(obj , element);
		}

	}

	// ��ȡEntities
	void OsmSceneSerializer::readEntities(TiXmlElement *parentElement)
	{
		// ��������Entity
		for (TiXmlElement* element = parentElement->FirstChildElement("entity");
			element != 0; element = element->NextSiblingElement("entity"))
		{
			readEntity(element);
		}
	}
	// ��ȡEntity
	void OsmSceneSerializer::readEntity(TiXmlElement *parentElement)
	{
		NameValuePairList params;
		params["mesh"] = getAttribute(parentElement , "filename");
		// ����ʵ��
		EntityObject *entity = static_cast<EntityObject*>(m_scene->getRootObject()->createChildObject(getAttribute(parentElement , "name")
			, "EntityObject"
			, &params
			));
		// �Ƿ������Ӱ
		entity->getOgreEntity()->setCastShadows(StringConverter::parseBool(getAttribute(parentElement , "CastShadows")));
		// ��ȡ�ڵ�����
		readNullObject(entity , parentElement);
	}


	// ��ȡParticles
	void OsmSceneSerializer::readParticles(TiXmlElement *parentElement)
	{
		// ��������Entity
		for (TiXmlElement* element = parentElement->FirstChildElement("particle");
			element != 0; element = element->NextSiblingElement("particle"))
		{
			readParticle(element);
		}
	}

	// ��ȡParticle
	void OsmSceneSerializer::readParticle(TiXmlElement *parentElement)
	{
		NameValuePairList params;
		params["templateName"] = getAttribute(parentElement , "templatename");
		// ����ʵ��
		ParticleObject *particle = static_cast<ParticleObject*>(m_scene->getRootObject()->createChildObject(getAttribute(parentElement , "name")
			, "ParticleObject"
			, &params
			));
		// ��ȡ�ڵ�����
		readNullObject(particle , parentElement);
	}

	// ��ȡRibbonTrails
	void OsmSceneSerializer::readRibbonTrails(TiXmlElement *parentElement)
	{
		// ��������Entity
		for (TiXmlElement* element = parentElement->FirstChildElement("particle");
			element != 0; element = element->NextSiblingElement("particle"))
		{
			readRibbonTrail(element);
		}
	}

	// ��ȡRibbonTrail
	void OsmSceneSerializer::readRibbonTrail(TiXmlElement *parentElement)
	{
		NameValuePairList params;
		// ����ʵ��
		RibbonTrailObject *ribbonTrail = static_cast<RibbonTrailObject*>(m_scene->getRootObject()->createChildObject(getAttribute(parentElement , "name")
			, "RibbonTrailObject"
			, &params
			));
		// ��ȡ�ڵ�����
		readNullObject(ribbonTrail , parentElement);
	}


	// ��ȡ������
	void OsmSceneSerializer::readAnimations(NullObject *entity , TiXmlElement *parentElement)
	{
		// ��������animation
		for (TiXmlElement* element = parentElement->FirstChildElement("animation");
			element != 0; element = element->NextSiblingElement("animation"))
		{
			readAnimation(entity , element);
		}
	}
	// ��ȡ����
	void OsmSceneSerializer::readAnimation(NullObject *entity , TiXmlElement *parentElement)
	{
		Animation *animation;
		String name(getAttribute(parentElement , "name"));
		if(!m_sceneMgr->hasAnimation(name))
		{
			// ��������
			animation = m_sceneMgr->createAnimation(name
				, StringConverter::parseReal(getAttribute(parentElement , "length"))
				);
			// ��������������
			m_scene->createAnimationStateController(name , name);
		}
		else
		{
			// ���֮ǰ�Ķ���
			animation = m_sceneMgr->getAnimation(name);
		}
		// �����ڵ����ж���
		NodeAnimationTrack *track = animation->createNodeTrack(animation->getNumNodeTracks() + 1
			, entity->getOgreSceneNode()
			);
		// ��������animation
		for (TiXmlElement* element = parentElement->FirstChildElement("keyframe");
			element != 0; element = element->NextSiblingElement("keyframe"))
		{
			readKeyframe(track , element);
		}
	}
	// ��ȡ����֡
	void OsmSceneSerializer::readKeyframe(NodeAnimationTrack *track , TiXmlElement *parentElement)
	{
		// �����ؼ�֡
		TransformKeyFrame* keyframe = track->createNodeKeyFrame(StringConverter::parseReal(getAttribute(parentElement , "time")));
		// �ؼ�֡λ��
		keyframe->setTranslate(getVector3Attribute(parentElement->FirstChildElement("position")));
		// �ؼ�֡��ת
		keyframe->setRotation(getQuaternionAttribute(parentElement->FirstChildElement("rotation")));
		// �ؼ�֡����
		keyframe->setScale(getVector3Attribute(parentElement->FirstChildElement("scale")));
	}
};
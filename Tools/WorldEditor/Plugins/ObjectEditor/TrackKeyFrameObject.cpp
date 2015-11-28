#include "StdAfx.h"
#include "TrackKeyFrameObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
#include "DlgTrackTools.h"
namespace Ogre
{

#define ID_KeyFrameLength (ID_EntityObject_End - 1)
	const String TrackKeyFrameObjectFactory::FACTORY_TYPE = "TrackKeyFrameObject";

	// ��øù�������������
	const String &TrackKeyFrameObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// ��������Ĵ�������
	ISceneObject *TrackKeyFrameObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		TrackKeyFrameObject *obj = new TrackKeyFrameObject(name);
		obj->create(scene , params);
		return obj;
	}

	// ������������ٷ���
	void TrackKeyFrameObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	TrackKeyFrameObject::TrackKeyFrameObject(const String &name)
		: EntityObject(name)
		, m_length(1.0f)
		, m_scale(Vector3::UNIT_SCALE)
	{
	}

	TrackKeyFrameObject::~TrackKeyFrameObject(void)
	{
		destroy();
	}

	// ��������ʵ��
	void TrackKeyFrameObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		EntityObject::create(scene , params);

		setScale(m_scale);

	}

	// ���ٶ���ʵ��
	void TrackKeyFrameObject::destroy()
	{

		EntityObject::destroy();
	}

	// �����ݸ��µ��ؼ�֡������ȥ
	void TrackKeyFrameObject::updateToKeyFrame()
	{
		// ֻ�б����Ŷ���ָ��Ĳſ���
		KeyObjectData data = safe_cast<KeyObjectData>(getUserAny());
		if(!data.isNull())
		{
			const AnimationTrackObject::KeyFrame &keyFrame = data.m_object->getKeyFrame(data.m_index);
			data.m_object->modifyKeyFrame(data.m_index , m_length , getPosition() , getScale() , getOrientation());
		}
	}

	// �����ݴӹؼ�֡��������µ�����
	void TrackKeyFrameObject::updateFromKeyFrame()
	{
		// ֻ�б����Ŷ���ָ��Ĳſ���
		KeyObjectData data = safe_cast<KeyObjectData>(getUserAny());
		if(!data.isNull())
		{
			const AnimationTrackObject::KeyFrame &keyFrame = data.m_object->getKeyFrame(data.m_index);
			m_node->setPosition(keyFrame.m_translate);
			m_scale = keyFrame.m_scale;
			EntityObject::setScale(m_scale * (1.0f / m_entity->getMesh()->getBoundingSphereRadius()));
			m_node->setOrientation(keyFrame.m_rotate);
			m_length = keyFrame.m_length;
		}
	}

	// �����������
	void TrackKeyFrameObject::setPosition(const Vector3 &pos)
	{
		EntityObject::setPosition(pos);
		updateToKeyFrame();
	}

	// �����������
	void TrackKeyFrameObject::setScale(const Vector3 &scale)
	{
		m_scale = scale;
		EntityObject::setScale(scale * (1.0f / m_entity->getMesh()->getBoundingSphereRadius()));
		updateToKeyFrame();
	}

	// ����������
	const Vector3 &TrackKeyFrameObject::getScale()const
	{
		return m_scale;
	}

	// ������Է�λ
	void TrackKeyFrameObject::setOrientation(const Quaternion &orientation)
	{
		EntityObject::setOrientation(orientation);
		updateToKeyFrame();
	}

	// ����ĳ���������꣬������������ڲ��������緽λ
	void TrackKeyFrameObject::lookAt(const Vector3& targetPoint)
	{
		EntityObject::lookAt(targetPoint);
		updateToKeyFrame();
	}
	// ���ݵ�ǰ����ֵ����
	void TrackKeyFrameObject::scale(const Vector3& scale)
	{
		EntityObject::scale(scale);
		m_scale *= scale;
		updateToKeyFrame();
	}
	// ���ݵ�ǰλ���ƶ�
	void TrackKeyFrameObject::translate(const Vector3& distance)
	{
		EntityObject::translate(distance);
		updateToKeyFrame();
	}
	// ���ݵ�ǰ��λ��ת
	void TrackKeyFrameObject::rotate(const Quaternion& rotate)
	{
		EntityObject::rotate(rotate);
		updateToKeyFrame();
	}

	// ö���������ԣ���������ʽ
	void TrackKeyFrameObject::enumPropertiesTree(VTProperty &properties)const
	{
		EntityObject::enumPropertiesTree(properties);
		properties.front().addChild(Property( ID_KeyFrameLength , PT_REAL , "�ؼ�֡����" , "�ؼ�֡���ȣ���λ����" , true , true , true));
	}

	// ��������
	bool TrackKeyFrameObject::setProperty(uint id , const Any &value)
	{
		switch(id)
		{
		case ID_KeyFrameLength:							// �ؼ�֡����
			{
				m_length = any_cast<Real>(value);
				updateToKeyFrame();
				return true;
			}
			break;
		default:
			return EntityObject::setProperty(id , value);
			break;
		}
	}
	// ��ȡ����ֵ
	Any TrackKeyFrameObject::getProperty(uint id)const
	{
		switch(id)
		{
		case ID_KeyFrameLength:							// �ؼ�֡����
			{
				return Any(m_length);
			}
			break;
		default:
			return EntityObject::getProperty(id);
			break;
		}
	}

	// ����������ԣ���������ʽ
	const PropertyContainer::VTProperty* TrackKeyFrameObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// ����������ԣ����б����ʽ
	const PropertyContainer::MapProperty* TrackKeyFrameObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

};
#include "StdAfx.h"
#include "AnimationTrackObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
#include "AnimationTrackObjectVisible.h"
#include "AnimationTrackController.h"
namespace Ogre
{

	const String AnimationTrackObjectFactory::FACTORY_TYPE = "AnimationTrackObject";

	// ��øù�������������
	const String &AnimationTrackObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// ��������Ĵ�������
	ISceneObject *AnimationTrackObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		AnimationTrackObject *obj = new AnimationTrackObject(name);
		obj->create(scene , params);
		return obj;
	}

	// ������������ٷ���
	void AnimationTrackObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	AnimationTrackObject::AnimationTrackObject(const String &name)
		: NullObject(name)
		, m_loop(false)
		, m_interpolationMode(IM_LINEAR)
		, m_rotationInterpolationMode(RIM_LINEAR)
	{
	}

	AnimationTrackObject::~AnimationTrackObject(void)
	{
		destroy();
	}

	// ��������ʵ��
	void AnimationTrackObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
	}

	// ���ٶ���ʵ��
	void AnimationTrackObject::destroy()
	{
		NullObject::destroy();
		destroyAllControllers();
	}


	// ö���������ԣ���������ʽ
	void AnimationTrackObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_AnimationTrack , PT_GROUP , "�����켣" , "�����켣" , true , false , false));
		properties.back().addChild(Property( ID_AnimationLoop , PT_BOOL , "�Ƿ�ѭ��" , "�Ƿ�ѭ��" , true , true , true));
		properties.back().addChild(Property( ID_AnimationInterpolationMode , PT_UINT , "��ֵģʽ" , "��ֵģʽ" , true , true , true));
		properties.back().addChild(Property( ID_AnimationRotationInterpolationMode , PT_UINT , "��ת��ֵģʽ" , "��ת��ֵģʽ" , true , true , true));
		properties.back().addChild(Property( ID_AnimationKeyFrames , PT_GROUP , "�ؼ�֡�б�" , "�ؼ�֡�б�" , false , false , false));
		VTProperty &keyFrames = properties.back().childs.back().childs;

		uint count = 0;
		// ������йؼ�֡��ȥ
		for(ListKeyFrame::const_iterator iter = m_listKeyFrame.begin() ; iter != m_listKeyFrame.end() ; iter ++)
		{
			
			keyFrames.push_back(Property( ID_AnimationKeyFrames_Begin + count , PT_GROUP , "�ؼ�֡" + StringConverter::toString(count / 5 + 1) , "�ؼ�֡" + StringConverter::toString(count / 5 + 1) , false , false , false));
			count ++;
			keyFrames.back().addChild(Property( ID_AnimationKeyFrames_Begin + count, PT_REAL , "֡��������" + StringConverter::toString(count / 5 + 1) , "֡�������ȣ���λ����" , false , true , true));
			count ++;
			keyFrames.back().addChild(Property( ID_AnimationKeyFrames_Begin + count, PT_VECTOR3 , "֡λ��" + StringConverter::toString(count / 5 + 1) , "֡λ��" , false , true , true));
			count ++;
			keyFrames.back().addChild(Property( ID_AnimationKeyFrames_Begin + count, PT_VECTOR3 , "֡����" + StringConverter::toString(count / 5 + 1) , "֡����" , false , true , true));
			count ++;
			keyFrames.back().addChild(Property( ID_AnimationKeyFrames_Begin + count, PT_VECTOR3 , "֡��ת" + StringConverter::toString(count / 5 + 1) , "֡��ת" , false , true , true));
			count ++;
		}
	}

	// ��������
	bool AnimationTrackObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);
		switch(id)
		{
		case ID_AnimationTrack:							// �����켣
			{
				return true;
			}
			break;
		case ID_AnimationKeyFrames:							// �ؼ�֡�б�
			{
				return true;
			}
			break;
		case ID_AnimationLoop:							// �Ƿ�ѭ��
			{
				setLoop(any_cast<bool>(value));
				return true;
			}
			break;
		case ID_AnimationInterpolationMode:							// ��ֵģʽ
			{
				setInterpolationMode((InterpolationMode)any_cast<uint>(value));
				return true;
			}
			break;
		case ID_AnimationRotationInterpolationMode:							// ��ת��ֵģʽ
			{
				setRotationInterpolationMode((RotationInterpolationMode)any_cast<uint>(value));
				return true;
			}
			break;
		default:
			// �ؼ�֡
			if(id >= ID_AnimationKeyFrames_Begin && id <= ID_AnimationKeyFrames_End)
			{
				size_t index = (id - ID_AnimationKeyFrames_Begin) / 5;
				KeyFrame *keyFrame = 0;
				// ��������˴�С���ʹ���һ���µ�
				if(index >= m_listKeyFrame.size())
				{
					KeyFrame frame;
					m_listKeyFrame.push_back(frame);
					keyFrame = &m_listKeyFrame.back();
				}
				// �����ȡһ���ɵ�
				else
				{
					keyFrame = &m_listKeyFrame[index];
				}
				switch((id - ID_AnimationKeyFrames_Begin) % 5)
				{
					// �ؼ�֡
				case 0:
					return true;
					break;
					// ��������
				case 1:
					keyFrame->m_length = any_cast<Real>(value);
					break;
					// λ��
				case 2:
					keyFrame->m_translate = any_cast<Vector3>(value);
					break;
					// ����
				case 3:
					keyFrame->m_scale = any_cast<Vector3>(value);
					break;
					// ��ת
				case 4:
					Vector3 rotate(any_cast<Vector3>(value));
					rotate.x = Math::DegreesToRadians(rotate.x);
					rotate.y = Math::DegreesToRadians(rotate.y);
					rotate.z = Math::DegreesToRadians(rotate.z);
					keyFrame->m_rotate = MathEx::EularToQuaternion(rotate);
					break;

				}
				// ֪ͨ�ؽ�
				_notifyRebuild();
				return true;
			}
			return false;
			break;
		}
	}
	// ��ȡ����ֵ
	Any AnimationTrackObject::getProperty(uint id)const
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::getProperty(id);
		switch(id)
		{
		case ID_AnimationTrack:							// �����켣
			{
				return Any();
			}
			break;
		case ID_AnimationKeyFrames:							// �ؼ�֡�б�
			{
				return Any();
			}
			break;
		case ID_AnimationLoop:							// �Ƿ�ѭ��
			{
				return Any(getLoop());
			}
			break;
		case ID_AnimationInterpolationMode:							// ��ֵģʽ
			{
				return Any((uint)getInterpolationMode());
			}
			break;
		case ID_AnimationRotationInterpolationMode:							// ��ת��ֵģʽ
			{
				return Any((uint)getRotationInterpolationMode());
			}
			break;
		default:
			// �ؼ�֡
			if(id >= ID_AnimationKeyFrames_Begin && id <= ID_AnimationKeyFrames_End)
			{
				size_t index = (id - ID_AnimationKeyFrames_Begin) / 5;
				const KeyFrame *keyFrame = 0;
				// ��������˴�С���ʹ���һ���µ�
				if(index >= m_listKeyFrame.size())
				{
					return Any();
				}
				// �����ȡһ���ɵ�
				else
				{
					keyFrame = &m_listKeyFrame[index];
				}
				switch((id - ID_AnimationKeyFrames_Begin) % 5)
				{
					// �ؼ�֡
				case 0:
					return Any();
					break;
					// ��������
				case 1:
					return Any(keyFrame->m_length);
					break;
					// λ��
				case 2:
					return Any(keyFrame->m_translate);
					break;
					// ����
				case 3:
					return Any(keyFrame->m_scale);
					break;
					// ��ת
				case 4:
					Vector3 rotate(MathEx::QuaternionToEular(keyFrame->m_rotate));
					rotate.x = Math::RadiansToDegrees(rotate.x);
					rotate.y = Math::RadiansToDegrees(rotate.y);
					rotate.z = Math::RadiansToDegrees(rotate.z);
					return Any(rotate);
					break;
				}
				return Any();
			}
			return Any();
			break;
		}
	}

	// ����������ԣ���������ʽ
	const PropertyContainer::VTProperty* AnimationTrackObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		s_properties.clear();
		enumPropertiesTree(s_properties);
		return &s_properties;
	}
	// ����������ԣ����б����ʽ
	const PropertyContainer::MapProperty* AnimationTrackObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		s_properties.clear();
		enumPropertiesList(s_properties);
		return &s_properties;
	}

	// ��ó�������Ŀɼ����������ڱ༭����ѡ��
	ISceneObjectVisible *AnimationTrackObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new AnimationTrackObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}
	// ���һ���ؼ�֡����󣬷��عؼ�֡������
	size_t AnimationTrackObject::addKeyFrameToBack(float length , const Vector3 &translate , const Vector3 &scale , const Quaternion &rotate)
	{
		KeyFrame keyFrame;
		keyFrame.m_length = length;
		keyFrame.m_translate = translate;
		keyFrame.m_scale = scale;
		keyFrame.m_rotate = rotate;
		m_listKeyFrame.push_back(keyFrame);
		_notifyRebuild();
		return m_listKeyFrame.size() - 1;
	}

	// ���һ���ؼ�֡����ǰ�����عؼ�֡������
	size_t AnimationTrackObject::addKeyFrameToFront(float length , const Vector3 &translate , const Vector3 &scale , const Quaternion &rotate)
	{
		KeyFrame keyFrame;
		keyFrame.m_length = length;
		keyFrame.m_translate = translate;
		keyFrame.m_scale = scale;
		keyFrame.m_rotate = rotate;
		m_listKeyFrame.push_front(keyFrame);
		_notifyRebuild();
		return 0;
	}

	// ��һ���ؼ�֮֡�����һ���ؼ�֡
	size_t AnimationTrackObject::insertKeyFrame(size_t index , float length , const Vector3 &translate , const Vector3 &scale , const Quaternion &rotate)
	{
		KeyFrame keyFrame;
		keyFrame.m_length = length;
		keyFrame.m_translate = translate;
		keyFrame.m_scale = scale;
		keyFrame.m_rotate = rotate;
		m_listKeyFrame.insert(m_listKeyFrame.begin() + index + 1 , keyFrame);
		_notifyRebuild();
		return index + 1;
	}

	// �޸�һ���ؼ�֡
	void AnimationTrackObject::modifyKeyFrame(size_t index , float length , const Vector3 &translate , const Vector3 &scale , const Quaternion &rotate)
	{
		KeyFrame &keyFrame = m_listKeyFrame[index];
		keyFrame.m_length = length;
		keyFrame.m_translate = translate;
		keyFrame.m_scale = scale;
		keyFrame.m_rotate = rotate;
		_notifyRebuild();
	}

	// ��������ɾ��һ���ؼ�֡
	void AnimationTrackObject::removeKeyFrame(size_t index)
	{
		m_listKeyFrame.erase(m_listKeyFrame.begin() + index);
		_notifyRebuild();
	}

	// ��ùؼ�֡������
	size_t AnimationTrackObject::getKeyFrameCount()const
	{
		return m_listKeyFrame.size();
	}

	// �����������һ���ؼ�֡
	const AnimationTrackObject::KeyFrame &AnimationTrackObject::getKeyFrame(size_t index)const
	{
		return m_listKeyFrame[index];
	}

	// ��һ�����������������켣�ܶ������ع켣������
	AnimationTrackObject::IAnimationTrackController* AnimationTrackObject::createTrack(ISceneObject *trackObject)
	{
		AnimationTrackController *controller = new AnimationTrackController(this , trackObject);
		m_controllers.push_back(controller);
		return controller;
	}
	// ���ٸ��ٹ켣
	void AnimationTrackObject::destroyTrack(IAnimationTrackController *controller)
	{
		m_controllers.remove(controller);
		delete controller;
	}

	// ֪ͨ���ؽ����ж����켣������
	void AnimationTrackObject::_notifyRebuild()
	{
		for(ListAnimationTrackController::iterator iter = m_controllers.begin() ; iter != m_controllers.end() ; iter ++)
		{
			static_cast<AnimationTrackController*>(*iter)->_notifyRebuild();
		}
		// �ؽ��ɼ�����
		if(m_sceneObjectVisible)
		{
			m_sceneObjectVisible->_notifyRebuild();
		}
	}
	// �������й켣������
	void AnimationTrackObject::destroyAllControllers()
	{
		for(ListAnimationTrackController::iterator iter = m_controllers.begin() ; iter != m_controllers.end() ; iter ++)
		{
			delete (*iter);
		}
		m_controllers.clear();
	}

	// �����Ƿ�ѭ��
	void AnimationTrackObject::setLoop(bool loop)
	{
		for(ListAnimationTrackController::iterator iter = m_controllers.begin() ; iter != m_controllers.end() ; iter ++)
		{
			(*iter)->setLoop(loop);
		}
		m_loop = loop;
		// �ؽ��ɼ�����
		if(m_sceneObjectVisible)
		{
			m_sceneObjectVisible->_notifyRebuild();
		}
	}
	// ��ȡ�Ƿ�ѭ��
	bool AnimationTrackObject::getLoop()const
	{
		return m_loop;
	}

	// ���ò�ֵ��ʽ
	void AnimationTrackObject::setInterpolationMode(InterpolationMode im)
	{
		for(ListAnimationTrackController::iterator iter = m_controllers.begin() ; iter != m_controllers.end() ; iter ++)
		{
			(*iter)->setInterpolationMode(im);
		}
		m_interpolationMode = im;
		// �ؽ��ɼ�����
		if(m_sceneObjectVisible)
		{
			m_sceneObjectVisible->_notifyRebuild();
		}
	}
	// ��ò�ֵ��ʽ
	AnimationTrackObject::InterpolationMode AnimationTrackObject::getInterpolationMode(void) const
	{
		return m_interpolationMode;
	}

	// ������ת��ֵ��ʽ
	void AnimationTrackObject::setRotationInterpolationMode(RotationInterpolationMode im)
	{
		for(ListAnimationTrackController::iterator iter = m_controllers.begin() ; iter != m_controllers.end() ; iter ++)
		{
			(*iter)->setRotationInterpolationMode(im);
		}
		m_rotationInterpolationMode = im;
		// �ؽ��ɼ�����
		if(m_sceneObjectVisible)
		{
			m_sceneObjectVisible->_notifyRebuild();
		}
	}
	// �����ת��ֵ��ʽ
	AnimationTrackObject::RotationInterpolationMode AnimationTrackObject::getRotationInterpolationMode(void) const
	{
		return m_rotationInterpolationMode;
	}

};
#include "StdAfx.h"
#include "AnimationTrackController.h"
#include "IScene.h"
namespace Ogre
{
	AnimationTrackController::AnimationTrackController(AnimationTrackObject *animationTrackObject , ISceneObject *trackObject)
		: m_speed(1.0f)
		, m_paused(true)
		, m_animationTrackObject(animationTrackObject)
		, m_trackObject(trackObject)
		, m_animation(0)
		, m_animationTrack(0)
		, m_animationState(0)
		, m_sceneMgr(0)
		, m_startTime(0.0f)
		, m_endTime(0.0f)
		, m_needRebuild(true)
		, m_loop(false)
		, m_length(0.0f)
		, m_interpolationMode(AnimationTrackObject::IM_LINEAR)
		, m_rotationInterpolationMode(AnimationTrackObject::RIM_LINEAR)
	{
		m_loop = m_animationTrackObject->getLoop();
		m_interpolationMode = m_animationTrackObject->getInterpolationMode();
		m_rotationInterpolationMode = m_animationTrackObject->getRotationInterpolationMode();

		m_sceneMgr = m_animationTrackObject->getScene()->getOgreSceneManager();
		Root::getSingleton().addFrameListener(this);
		// һ��ʼ���ؽ�����
		_rebuild();
	}
	AnimationTrackController::~AnimationTrackController()
	{
		Root::getSingleton().removeFrameListener(this);
		destroy();
	}
	// ���ع켣��������
	AnimationTrackObject *AnimationTrackController::getAnimationTrackObject()const
	{
		return m_animationTrackObject;
	}
	// ���ع켣�ϵ��������
	ISceneObject *AnimationTrackController::getTrackObject()const
	{
		return m_trackObject;
	}

	// ���Ź켣���������ÿ�ʼ�������Ķ����ؼ�֡����
	void AnimationTrackController::playByIndex(size_t startIndex , size_t endIndex)
	{
		float startTime = getIndexTimePosition(startIndex);
		float endTime = getIndexTimePosition(endIndex);
		playByTime(startTime , endTime);
	}
	// ���Ź켣���������ÿ�ʼ�������Ķ���ʱ��
	void AnimationTrackController::playByTime(float startTime , float endTime)
	{
		m_startTime = startTime;
		m_endTime = endTime;
		m_animationState->setTimePosition(m_startTime);
		m_animationState->setEnabled(true);
		m_paused = false;
	}
	// ������������Ĺؼ�֡����ڿ�ʼʱ���ƫ��
	float AnimationTrackController::getIndexTimePosition(size_t index)
	{
		float aniLength = 0.0f;
		AnimationTrackObject::ConstKeyFrameIterator iterator = m_animationTrackObject->getConstKeyFrameIterator();
		size_t indexCount = 0;
		while(iterator.hasMoreElements())
		{
			const AnimationTrackObject::KeyFrame& keyFrame = iterator.getNext();
			if(indexCount >= index)
				break;
			aniLength += keyFrame.m_length;
		}
		return aniLength;
	}

	// �Ƿ��Ѿ��������
	bool AnimationTrackController::hasEnded() const
	{
		return m_animationState->getTimePosition() == m_endTime && !m_loop;
	}

	// ��ͣ
	void AnimationTrackController::setPaused(bool pause)
	{
		m_paused = pause;
	}
	// �Ƿ���ͣ
	bool AnimationTrackController::getPaused()const
	{
		return m_paused;
	}

	// ���õ�ǰ����λ�ã�ʹ��ʱ��������
	void AnimationTrackController::setPlayPosByTime(float timePos)
	{

	}
	// ���õ�ǰ����λ�ã�ʹ�ùؼ�֡����������
	void AnimationTrackController::setPlayPosByIndex(size_t index)
	{

	}

	// ��õ�ǰ���ŵ�λ�ã���λΪ��
	float AnimationTrackController::getTimePosition()const
	{
		return m_animationState ? m_animationState->getTimePosition() : 0.0f;
	}

	// ���ò����ٶ�
	void AnimationTrackController::setSpeed(float speed)
	{
		m_speed = speed;
	}
	// ��ò����ٶ�
	float AnimationTrackController::getSpeed()const
	{
		return m_speed;
	}

	// �ؽ���������������
	void AnimationTrackController::_rebuild()
	{
		if(!m_needRebuild)
			return;
		destroy();

		// ���㶯������
		m_length = 0.0f;
		{
			AnimationTrackObject::ConstKeyFrameIterator iterator = m_animationTrackObject->getConstKeyFrameIterator();
			while(iterator.hasMoreElements())
			{
				const AnimationTrackObject::KeyFrame& keyFrame = iterator.getNext();
				m_length += keyFrame.m_length;
			}
		}
		// ��������
		m_animation = m_sceneMgr->createAnimation(m_animationTrackObject->getFullName() + "_" + m_trackObject->getScene()->getName() + m_trackObject->getName()
			, m_length
			);
		// ʹ����������
		m_animation->setInterpolationMode((Animation::InterpolationMode)m_interpolationMode);
		m_animation->setRotationInterpolationMode((Animation::RotationInterpolationMode)m_rotationInterpolationMode);
		// �����ڵ�켣����
		m_animationTrack = m_animation->createNodeTrack(0 , m_trackObject->getOgreSceneNode());
		// �������йؼ�֡
		{
			float timePos = 0.0f;
			AnimationTrackObject::ConstKeyFrameIterator iterator = m_animationTrackObject->getConstKeyFrameIterator();
			while(iterator.hasMoreElements())
			{
				const AnimationTrackObject::KeyFrame& keyFrame = iterator.getNext();
				timePos += keyFrame.m_length;
				TransformKeyFrame *frame = m_animationTrack->createNodeKeyFrame(timePos);
				frame->setTranslate(keyFrame.m_translate);
				frame->setScale(keyFrame.m_scale);
				frame->setRotation(keyFrame.m_rotate);
			}
		}
		// ��������״̬
		m_animationState = m_sceneMgr->createAnimationState(m_animation->getName());
		// ���ǲ�ʹ��ѭ��ϵͳ
		m_animationState->setLoop(false);
		
		// �ؽ����
		m_needRebuild = false;
	}

	// ���ٶ���������
	void AnimationTrackController::destroy()
	{
		// ���ٶ���״̬
		if(m_animationState)
		{
			m_sceneMgr->destroyAnimationState(m_animationState->getAnimationName());
			m_animationState = 0;
		}
		// ���ٶ���
		if(m_animation)
		{
			m_sceneMgr->destroyAnimation(m_animation->getName());
			m_animation = 0;
			m_animationTrack = 0;
		}
	}

	// ÿ֡���¶���
	bool AnimationTrackController::frameStarted(const FrameEvent& evt)
	{
		// ���Ƿ���Ҫ�ؽ�����
		_rebuild();
		// �����ͣ��δ��ʼ�����򲻲���
		if(m_paused
			|| m_animationState == 0
			|| !m_animationState->getEnabled())
		{
			return true;
		}
		// ����ʱ��
		m_animationState->addTime(evt.timeSinceLastFrame * m_speed);
		// ���������������ʱ�䣬�����õ����ȥ
		if(m_animationState->getTimePosition() >= m_endTime)
		{
			if(!m_loop)
			{
				m_animationState->setTimePosition(m_endTime);
				m_animationState->setEnabled(false);
			}
			else
			{
				m_animationState->setTimePosition(m_startTime);
			}
		}
		// ���𲥷Ż�����������
		else if(m_animationState->getTimePosition() <= m_startTime)
		{
			if(!m_loop)
			{
				m_animationState->setTimePosition(m_startTime);
				m_animationState->setEnabled(false);
			}
			else
			{
				m_animationState->setTimePosition(m_endTime);
			}
		}
		return true;
	}


	// �����Ƿ�ѭ��
	void AnimationTrackController::setLoop(bool loop)
	{
		m_loop = loop;
	}
	// ��ȡ�Ƿ�ѭ��
	bool AnimationTrackController::getLoop()const
	{
		return m_loop;
	}

	// ���ò�ֵ��ʽ
	void AnimationTrackController::setInterpolationMode(AnimationTrackObject::InterpolationMode im)
	{
		m_animation->setInterpolationMode((Animation::InterpolationMode)im);
		m_interpolationMode = im;
	}
	// ��ò�ֵ��ʽ
	AnimationTrackObject::InterpolationMode AnimationTrackController::getInterpolationMode(void) const
	{
		return m_interpolationMode;
	}

	// ������ת��ֵ��ʽ
	void AnimationTrackController::setRotationInterpolationMode(AnimationTrackObject::RotationInterpolationMode im)
	{
		m_animation->setRotationInterpolationMode((Animation::RotationInterpolationMode)im);
		m_rotationInterpolationMode = im;
	}
	// �����ת��ֵ��ʽ
	AnimationTrackObject::RotationInterpolationMode AnimationTrackController::getRotationInterpolationMode(void) const
	{
		return m_rotationInterpolationMode;
	}
};
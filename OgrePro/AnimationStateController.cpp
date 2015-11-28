#include "StdAfx.h"
#include "AnimationStateController.h"

namespace Ogre
{
	AnimationStateController::AnimationStateController()
		: m_state(0)
		, m_speed(1.0f)
	{
	}

	AnimationStateController::~AnimationStateController(void)
	{
		destroy();
	}

	// ����
	void AnimationStateController::create(const String &name , AnimationState* state)
	{
		destroy();
		m_state = state;
		m_name = name;
	}

	// ����
	void AnimationStateController::create(AnimationState* state)
	{
		create(StringUtil::BLANK , state);
	}
	// ����
	void AnimationStateController::destroy()
	{
	}


	// ��ö�������������
	const String& AnimationStateController::getName() const
	{
		return m_name;
	}
	// ��ö�������
	const String& AnimationStateController::getAnimationName() const
	{
		return m_state->getAnimationName();
	}

	// ������ڲ��ŵ�λ��
	float AnimationStateController::getTimePosition(void) const
	{
		return m_state->getTimePosition();
	}

	// ���ò���λ��
	void AnimationStateController::setTimePosition(float timePos)
	{
		m_state->setTimePosition(timePos);
	}

	// ��ö�������
	float AnimationStateController::getLength() const
	{
		return m_state->getLength();
	}

	// ����Ƿ񲥷Ž�����
	bool AnimationStateController::hasEnded(void) const
	{
		return m_state->hasEnded();
	}


	// ����Ƿ����ڲ���
	bool AnimationStateController::isPlaying(void) const
	{
		return m_state->getEnabled();
	}

	// ����
	void AnimationStateController::play()
	{
		m_state->setEnabled(true);
	}

	// ��ͣ
	void AnimationStateController::pause()
	{
		m_state->setEnabled(false);
	}

	// ֹͣ
	void AnimationStateController::stop()
	{
		m_state->setEnabled(false);
		m_state->setTimePosition(0.0f);
	}

	// �����Ƿ�ѭ��
	void AnimationStateController::setLoop(bool loop)
	{
		m_state->setLoop(loop);
	}

	// ����Ƿ�ѭ��
	bool AnimationStateController::getLoop(void) const
	{
		return m_state->getLoop();
	}

	// ���¶���״̬
	void AnimationStateController::update(float timeSinceLastFrame)
	{
		m_state->addTime(timeSinceLastFrame * m_speed);
	}
	// ���ò����ٶ�
	void AnimationStateController::setSpeed(float speed)
	{
		m_speed = speed;
	}
	// ��ò����ٶ�
	float AnimationStateController::getSpeed()const
	{
		return m_speed;
	}

};
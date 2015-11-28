#pragma once
#include "IAnimationStateController.h"
namespace Ogre
{
	// ����״̬��������ʵ�֣�ֻ�ܶԵ�������������
	class OGREPRO_API AnimationStateController : public IAnimationStateController
	{
	public:
		AnimationStateController();
		virtual ~AnimationStateController(void);

		// ����
		void create(const String &name , AnimationState* state);
		void create(AnimationState* state);
		// ����
		void destroy();

		// ��ö�������������
		virtual const String& getName() const;
		// ��ö�������
		virtual const String& getAnimationName() const;
		// ������ڲ��ŵ�λ��
		virtual float getTimePosition(void) const;
		// ���ò���λ��
		virtual void setTimePosition(float timePos);
		// ��ö�������
		virtual float getLength() const;

		// ����Ƿ񲥷Ž�����
		virtual bool hasEnded(void) const;

		// ����Ƿ����ڲ���
		virtual bool isPlaying(void) const;
		// ����
		virtual void play();
		// ��ͣ
		virtual void pause();
		// ֹͣ
		virtual void stop();
		// �����Ƿ�ѭ��
		virtual void setLoop(bool loop);
		// ����Ƿ�ѭ��
		virtual bool getLoop(void) const;

		// ���ò����ٶ�
		virtual void setSpeed(float speed);
		// ��ò����ٶ�
		virtual float getSpeed()const;

		// ���¶���״̬
		virtual void update(float timeSinceLastFrame);
		// �Ƿ��Ѿ���ʼ��
		virtual bool isNull()const
		{
			return m_state == 0;
		}
	protected:
		// ����״̬
		AnimationState *m_state;
		// �����ٶ�
		float m_speed;
		// ����������
		String m_name;
	};
};
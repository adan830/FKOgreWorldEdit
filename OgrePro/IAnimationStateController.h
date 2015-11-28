#pragma once

namespace Ogre
{
	class IScene;
	// ����״̬��������ֻ�ܶԵ�������������
	class IAnimationStateController
	{
	public:

		IAnimationStateController(void)
		{
		}

		virtual ~IAnimationStateController(void)
		{
		}
		// ��ö�������������
		virtual const String& getName() const = 0;
		// ��ö�������
		virtual const String& getAnimationName() const = 0;
		// ������ڲ��ŵ�λ��
		virtual float getTimePosition(void) const = 0;
		// ���ò���λ��
		virtual void setTimePosition(float timePos) = 0;
		// ��ö�������
		virtual float getLength() const = 0;

		// ����Ƿ񲥷Ž�����
		virtual bool hasEnded(void) const = 0;

		// ����Ƿ����ڲ���
		virtual bool isPlaying(void) const = 0;
		// ����
		virtual void play() = 0;
		// ��ͣ
		virtual void pause() = 0;
		// ֹͣ
		virtual void stop() = 0;
		// �����Ƿ�ѭ��
		virtual void setLoop(bool loop) = 0;
		// ����Ƿ�ѭ��
		virtual bool getLoop(void) const = 0;

		// ���ò����ٶ�
		virtual void setSpeed(float speed) = 0;
		// ��ò����ٶ�
		virtual float getSpeed()const = 0;

		// ���¶���״̬
		virtual void update(float timeSinceLastFrame) = 0;

		// �Ƿ��Ѿ���ʼ��
		virtual bool isNull()const = 0;

	};
};
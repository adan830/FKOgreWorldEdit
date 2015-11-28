#pragma once
#include "AnimationTrackObject.h"
namespace Ogre
{
	// �����켣������
	class DISPLAYLAYER_API AnimationTrackController : public AnimationTrackObject::IAnimationTrackController
		, public FrameListener
	{
	public:
		AnimationTrackController(AnimationTrackObject *animationTrackObject , ISceneObject *trackObject);
		virtual ~AnimationTrackController();
		// ���ع켣��������
		virtual AnimationTrackObject *getAnimationTrackObject()const;
		// ���ع켣�ϵ��������
		virtual ISceneObject *getTrackObject()const;

		// ���Ź켣���������ÿ�ʼ�������Ķ����ؼ�֡����
		virtual void playByIndex(size_t startIndex = 0 , size_t endIndex = 0xFFFFFFFF);
		// ���Ź켣���������ÿ�ʼ�������Ķ���ʱ��
		virtual void playByTime(float startTime = 0.0f , float endTime = FLT_MAX);

		// ��ͣ
		virtual void setPaused(bool pause);
		// �Ƿ���ͣ
		virtual bool getPaused()const;

		// �����Ƿ�ѭ��
		virtual void setLoop(bool loop);
		// ��ȡ�Ƿ�ѭ��
		virtual bool getLoop()const;

		// ��ö�������
		virtual float getLength()const
		{
			return m_length;
		}

		// ���ò�ֵ��ʽ
		virtual void setInterpolationMode(AnimationTrackObject::InterpolationMode im);
		// ��ò�ֵ��ʽ
		virtual AnimationTrackObject::InterpolationMode getInterpolationMode(void) const;
		
		// ������ת��ֵ��ʽ
		virtual void setRotationInterpolationMode(AnimationTrackObject::RotationInterpolationMode im);
		// �����ת��ֵ��ʽ
		virtual AnimationTrackObject::RotationInterpolationMode getRotationInterpolationMode(void) const;

		// ���õ�ǰ����λ�ã�ʹ��ʱ��������
		virtual void setPlayPosByTime(float timePos);
		// ���õ�ǰ����λ�ã�ʹ�ùؼ�֡����������
		virtual void setPlayPosByIndex(size_t index);

		// ��õ�ǰ���ŵ�λ�ã���λΪ��
		virtual float getTimePosition()const;

		// �Ƿ��Ѿ��������
		virtual bool hasEnded() const;

		// ���ò����ٶ�
		virtual void setSpeed(float speed);
		// ��ò����ٶ�
		virtual float getSpeed()const;

		// ֪ͨ���ؽ����������켣������
		void _notifyRebuild()
		{
			m_needRebuild = true;
		}
		// �ؽ����������켣������
		void _rebuild();

		// ÿ֡���¶���
		virtual bool frameStarted(const FrameEvent& evt);

		// ��ȡOgre����
		Animation *getAnimation()const
		{
			return m_animation;
		}
		// ��ȡOgre�����켣
		NodeAnimationTrack *getNodeAnimationTrack()const
		{
			return m_animationTrack;
		}
		// ��ȡOgre����״̬
		AnimationState *getAnimationState()const
		{
			return m_animationState;
		}
	protected:
		// ������������Ĺؼ�֡����ڿ�ʼʱ���ƫ��
		float getIndexTimePosition(size_t index);
		// ���ٶ���������
		void destroy();
		// �켣��������
		AnimationTrackObject *m_animationTrackObject;
		// �켣�ϵ��������
		ISceneObject *m_trackObject;
		// �����ٶ�
		float m_speed;
		// �Ƿ���ͣ״̬
		bool m_paused;
		// �Ƿ���Ҫ�ؽ����������켣������
		bool m_needRebuild;
		// ��ʼ���ŵ�ʱ��
		float m_startTime;
		// �������ŵ�ʱ��
		float m_endTime;
		// ��������
		float m_length;

		// �Ƿ�ѭ��
		bool m_loop;
		// ��ֵ��ʽ
		AnimationTrackObject::InterpolationMode m_interpolationMode;
		// ��ת��ֵ��ʽ
		AnimationTrackObject::RotationInterpolationMode m_rotationInterpolationMode;

		// Ogre����
		Animation *m_animation;
		// Ogre�����켣
		NodeAnimationTrack *m_animationTrack;
		// Ogre����״̬
		AnimationState *m_animationState;
		// Ogre����������
		SceneManager *m_sceneMgr;
	};
};
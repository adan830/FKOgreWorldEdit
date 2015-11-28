#pragma once
#include "IEntityAnimationController.h"
namespace Ogre
{
	// ���ڲ��ŵĶ�����Ϣ
	class OGREPRO_API PlayingAnimationInfo
	{
	public:
		PlayingAnimationInfo(AnimationState *state , const AnimationInfo &info);
		virtual ~PlayingAnimationInfo();
		// ���������Ϣ
		void clear();
		// ���¶���
		void update(float timeSinceLastFrame);
		// �Ƿ��Ѿ���϶������
		bool hasBlendEnded()
		{
			return m_aniState->hasEnded() || (m_aniState->getWeight() == m_destWeight || m_destBlendTime == 0.0f);
		}
		// �Ƿ��Ѿ��������
		bool hasEnded()
		{
			// �ﵽ�����������
			return (m_aniInfo.playCount > 0 && m_playedCount >= m_aniInfo.playCount);
		}

		// ����Ŀ��Ȩ��
		void setDestWeight(float weight , float blendTime)
		{
			m_sourceWeight = m_aniState->getWeight();
			m_destWeight = weight;
			m_destBlendTime = blendTime;
			m_destBlendTimeLeft = blendTime;
			if(blendTime == 0.0f)
			{
				m_aniState->setWeight(m_destWeight);
				//printf("����%s : %.2f \n" , m_aniState->getAnimationName().c_str() , m_aniState->getWeight());
			}
		}
		// ǿ����ɻ��
		void forceFinishBlend()
		{
			m_aniState->setEnabled(true);
			m_aniState->setWeight(m_destWeight);
			m_destBlendTime = 0.0f;
		}
		// ���ûص�
		virtual void setCallback(IEntityAnimationCallback *callback)
		{
			m_callback = callback;
		}
		// ��ȡ�ص�
		virtual IEntityAnimationCallback* getCallback()
		{
			return m_callback;
		}

		// ��ȡ�л���������������ʱ��
		float getBlendTime()
		{
			return m_aniInfo.blendTime;
		}

		// ��ö�������
		float getAnimationLength()
		{
			return m_aniState->getLength();
		}

		// ��õ�ǰ����λ�ã���λ����
		float getTimePosition()
		{
			return m_aniState->getTimePosition();
		}

		// ��������
		const String &getAnimationName()const
		{
			return m_aniInfo.aniName;
		}

		// ���ö������ʱ�ص�
		void setAnimationStateCallback(Animation::AnimationCallback *callback)
		{
			m_aniState->setCallback(callback);
		}

		// ��ö�����Ϣ
		const AnimationInfo& getAnimationInfo()const
		{
			return m_aniInfo;
		}

		// ��ö������ʱ�ص�
		Animation::AnimationCallback* getAnimationStateCallback() const { return m_aniState->getCallback(); } 

	protected:
		// ��ǰ���ŵĶ���
		AnimationState *m_aniState;
		// Ŀ��Ȩ��
		float m_destWeight;
		// ��ʼȨ��
		float m_sourceWeight;
		// ������Ϣ
		AnimationInfo m_aniInfo;
		// ʣ�µ�ת��ʱ��
		float m_destBlendTimeLeft;
		// ������Ȩ��ת����Ŀ��Ȩ�������ʱ��
		float m_destBlendTime;
		// �Ѿ����ŵĴ���
		uint m_playedCount;
		// ���Żص�
		IEntityAnimationCallback *m_callback;

	};

};
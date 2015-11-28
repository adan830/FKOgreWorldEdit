#include "StdAfx.h"
#include "PlayingAnimationInfo.h"
namespace Ogre
{
	PlayingAnimationInfo::PlayingAnimationInfo(AnimationState *state , const AnimationInfo &info)
		: m_aniState(state)
		, m_destWeight(0.0f)
		, m_destBlendTime(info.blendTime)
		, m_aniInfo(info)
		, m_callback(0)
		, m_sourceWeight(0.0f)
		, m_playedCount(0)
	{
		m_aniState->setLoop(false);
		// ȫ��ת������Ϊ��λ������׼��������ʱ�䷶Χ��
		m_aniInfo.positionStart = std::max<float>(m_aniInfo.getPositionStartAsSecond(m_aniState) , 0.0f);
		m_aniInfo.positionEnd = std::min<float>(m_aniInfo.getPositionEndAsSecond(m_aniState) , m_aniState->getLength());
		m_aniInfo.positionFormatStart = PF_SECOND;
		m_aniInfo.positionFormatEnd = PF_SECOND;

		// ��ͷ����
		// ���򲥷�
		if(m_aniInfo.playSpeed > 0)
		{
			m_aniState->setTimePosition(m_aniInfo.positionStart);
		}
		// ���򲥷�
		else if(m_aniInfo.playSpeed < 0)
		{
			m_aniState->setTimePosition(m_aniInfo.positionEnd);
		}
		m_aniState->setEnabled(true);
		m_aniState->setWeight(0.0f);
	}

	PlayingAnimationInfo::~PlayingAnimationInfo()
	{
		clear();
	};
	// ���������Ϣ
	void PlayingAnimationInfo::clear()
	{
		if(m_callback)
		{
			m_callback->onPlayEnd(m_aniInfo);
		}
		m_aniState->setEnabled(false);
		m_aniState->setWeight(0.0f);
	}

	// ���¶���
	void PlayingAnimationInfo::update(float timeSinceLastFrame)
	{
		// ����Ȩ��
		float weight = m_aniState->getWeight();
		// ���ʱ�仹û������ϣ��Ͳ���
		if(m_destBlendTime > 0.0f)
		{
			// ʱ����������
			m_destBlendTime -= timeSinceLastFrame;
			// ����
			if(weight > m_destWeight)
			{
				weight -= (m_sourceWeight - m_destWeight) * (timeSinceLastFrame / m_destBlendTimeLeft);
				weight = std::max(weight , 0.0f);
			}
			else if(weight < m_destWeight)
			{
				weight += (m_destWeight - m_sourceWeight) * (timeSinceLastFrame / m_destBlendTimeLeft);
				weight = std::min(weight , 1.0f);
			}
			m_aniState->setWeight(weight);

			// printf("%s : %.2f \n" , m_aniState->getAnimationName().c_str() , m_aniState->getWeight());
		}
		else if(weight != m_destWeight)
		{
			weight = m_destWeight;
			m_destBlendTime = 0.0f;
			m_aniState->setWeight(weight);

			// printf("����Ȩ�� %s : %.2f \n" , m_aniState->getAnimationName().c_str() , m_aniState->getWeight());
		}
		// ����׼��ҪaddTime��ʱ��
		float timePass = timeSinceLastFrame * m_aniInfo.playSpeed;
		// ����Ѿ�������ϣ��Ͳ�����ʱ����
		if(timePass == 0 || hasEnded())
		{
			return;
		}

		// ���¶���ʱ��
		m_aniState->addTime(timePass);

		Real pos = m_aniState->getTimePosition();
		// ���򲥷�
		if(m_aniInfo.playSpeed > 0)
		{
			// ������һ��
			if(pos >= m_aniInfo.positionEnd)
			{
				m_playedCount += (uint)(abs(timePass / (m_aniInfo.positionEnd - m_aniInfo.positionStart)) + 0.9999f);
				// �ص�һ��
				if(m_callback)
				{
					m_callback->onPlayOnce(m_aniInfo);
				}
				if(hasEnded())
					return;
				m_aniState->setTimePosition(m_aniInfo.positionStart);
			}
		}
		// ���򲥷�
		else if(m_aniInfo.playSpeed < 0)
		{
			// ��������һ��
			if(pos <= m_aniInfo.positionStart)
			{
				m_playedCount += (uint)(abs(timePass / (m_aniInfo.positionEnd - m_aniInfo.positionStart)) + 0.9999f);
				// �ص�һ��
				if(m_callback)
				{
					m_callback->onPlayOnce(m_aniInfo);
				}
				if(hasEnded())
					return;
				m_aniState->setTimePosition(m_aniInfo.positionEnd);
			}
		}
	}
};
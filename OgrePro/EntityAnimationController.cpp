#include "StdAfx.h"
#include "EntityAnimationController.h"
#include "PlayingAnimationInfo.h"
namespace Ogre
{
	// ��Ϲ���ѡ����
	class AllBlendBoneSelector : public BoneSelector
	{
	public:
		AllBlendBoneSelector(EntityAnimationController *controller)
			: BoneSelector(ALL_BLEND)
			, m_controller(controller)
		{
		}
		// ��apply������ʱ��ѡ���Ƿ�apply�������
		// ��ִ�еĹ����У����Զ�̬�޸Ĳ���
		virtual bool applyBone(Bone *&bone , Real &weight , Real &scale)
		{
			const String &name = bone->getName();
			EntityAnimationController::PlayingAnimationIterator iter = m_controller->getPlayingAnimationIterator();
			// Ӱ�챾��ͷ�Ķ�������
			size_t numAnimation = 1;
			while(iter.hasMoreElements())
			{
				PlayingAnimation &ani = iter.getNext();
				// ������Լ�����ֹ��ѭ��
				if(ani.selector)
				{
					if(ani.selector->getBlendType() == m_blendType)
					{
						continue;
					}
					// ������κ�һ���Ӷ����õ�����������Ͳ�����ȫ��������
					if(!ani.aniBlending.isNull() || !ani.aniDest.isNull())
					{
						if(ani.selector->isApplyBone(bone))
						{
							return false;
						}
						// ͳ���ж��ٸ��Ӷ����ڲ���
						numAnimation++;
					}
				}
			}
			// ������Ӷ����ڲ��ţ��͵���Ȩ�ز���
			if(numAnimation > 1)
			{
				weight *= numAnimation;
			}
			return true;
		}
		// �Ƿ�Ӱ�������ͷ
		virtual bool isApplyBone(Bone *bone)
		{
			return true;
		}

	protected:
		EntityAnimationController *m_controller;
	};

	// �Ƿ�Ӱ�������ͷ
	bool NameBoneSelector::isApplyBone(Bone *bone)
	{
		const String &name = bone->getName();
		return StringUtil::startsWith(name , m_preName , false);
	}

	// ��apply������ʱ��ѡ���Ƿ�apply�������
	// ��ִ�еĹ����У����Զ�̬�޸Ĳ���
	bool NameBoneSelector::applyBone(Bone *&bone , Real &weight , Real &scale)
	{
		if(isApplyBone(bone))
		{
			weight = 1.0f;
			return true;
		}
		else
		{
			return false;
		}
	}


	EntityAnimationController::EntityAnimationController()
		: m_entity(0)
		, m_speed(1.0f)
		, m_paused(true)
		, m_callback(0)
	{
	}

	EntityAnimationController::~EntityAnimationController(void)
	{
		destroy();
	}

	// �������й���ѡ����
	void EntityAnimationController::destroyAllBoneSelector()
	{
		for(MapPlayingAnimation::iterator iter = m_playing.begin() ; iter != m_playing.end() ; iter ++)
		{
			iter->second.setNull();
			delete iter->second.selector;
		}
		m_playing.clear();
	}

	// ����
	void EntityAnimationController::create(Entity *entity)
	{
		// ������
		destroy();
		// ע��Ĭ�ϵĻ����
		registerBoneSelector(new AllBlendBoneSelector(this));

		// ����ʵ��
		m_entity = entity;
		// �����������Ȩ�غͲ���״̬
		AnimationStateSet *sets = m_entity->getAllAnimationStates();
		if(sets)
		{
			AnimationStateIterator it = sets->getAnimationStateIterator();
			while(it.hasMoreElements())
			{
				AnimationState *anim = it.getNext();
				anim->setEnabled(false);
				anim->setWeight(0);
				anim->setTimePosition(0);
			}
		}
	}

	// ����
	void EntityAnimationController::destroy()
	{
		destroyAllBoneSelector();
	}

	// ������в���״̬
	void EntityAnimationController::clearState()
	{
		for(MapPlayingAnimation::iterator iter = m_playing.begin() ; iter != m_playing.end() ; iter ++)
		{
			iter->second.setNull();
		}
	}

	// ���һ������״̬
	void EntityAnimationController::clearState(const AnimationInfo &aniInfo)
	{
		switch(aniInfo.blendType)
		{
		case ALL_REPLACE:
			{
				clearState();
			}
			break;
		case ALL_BLEND:
			{
				PlayingAnimation &ani = *getPlayingAnimation(ALL_BLEND);
				if(!ani.aniBlending.isNull())
				{
					ani.aniDest = ani.aniBlending;
					// ǿ����ɻ��
					ani.aniDest->forceFinishBlend();
					ani.aniBlending.setNull();
				}
			}
			break;
		default:
			{
				getPlayingAnimation(aniInfo.blendType)->setNull();
			}
			break;
		}
	}

	// ��ö���
	PlayingAnimation *EntityAnimationController::getPlayingAnimation(uint blendType)
	{
		MapPlayingAnimation::iterator iter = m_playing.find(blendType);
		if(iter == m_playing.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"�Ҳ���BlendTypeΪ '" + StringConverter::toString(blendType) + "' �Ļ������", 
				"EntityAnimationController::getPlayingAnimation");
			return 0;
		}
		return &iter->second;
	}

	// ��ö���
	const PlayingAnimation *EntityAnimationController::getPlayingAnimation(uint blendType)const
	{
		MapPlayingAnimation::const_iterator iter = m_playing.find(blendType);
		if(iter == m_playing.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"�Ҳ���BlendTypeΪ '" + StringConverter::toString(blendType) + "' �Ļ������", 
				"EntityAnimationController::getPlayingAnimation");
			return 0;
		}
		return &iter->second;
	}
	// ����һ����������
	void EntityAnimationController::play(const AnimationInfo &aniInfo)
	{
		// ɾ��֮ǰ�Ķ���
		clearState(aniInfo);
		// ��ö���״̬
		AnimationState *state = m_entity->getAnimationState(aniInfo.aniName);
		if(state == 0)
			return;
		PlayingAnimationInfoPtr info(new PlayingAnimationInfo(state , aniInfo));
		// ���ò��Żص�
		info->setCallback(m_callback);

		PlayingAnimation *ani = 0;
		if(aniInfo.blendType == ALL_REPLACE)
		{
			ani = getPlayingAnimation(ALL_BLEND);
		}
		else
		{
			ani = getPlayingAnimation(aniInfo.blendType);
		}

		if(!ani->aniDest.isNull())
		{
			ani->aniDest->setDestWeight(0.0f , info->getBlendTime());
		}
		if(!ani->aniBlending.isNull())
		{
			ani->aniDest = ani->aniBlending;
		}

		if(ani->aniDest.isNull())
		{
			ani->aniDest = info;
			ani->aniDest->setDestWeight(1.0f , 0.0f);
			// ʹ���Լ���ѡ����
			ani->aniDest->setAnimationStateCallback(ani->selector);
		}
		else
		{
			ani->aniBlending = info;
			ani->aniBlending->setDestWeight(1.0f , info->getBlendTime());
			// ʹ���Լ���ѡ����
			ani->aniBlending->setAnimationStateCallback(ani->selector);
		}

		m_paused = false;
	}

	// ���ò����ٶ�
	void EntityAnimationController::setSpeed(float speed)
	{
		m_speed = speed;
	}

	// ��ò����ٶ�
	float EntityAnimationController::getSpeed()const
	{
		return m_speed;
	}

	// �����Ƿ���ͣ
	void EntityAnimationController::setPaused(bool pause)
	{
		m_paused = pause;
	}

	// �Ƿ���ͣ
	bool EntityAnimationController::isPaused()
	{
		return m_paused;
	}

	// ���¶���״̬
	void EntityAnimationController::update(float timeSinceLastFrame)
	{
		if(m_paused)
			return;
		// �����ٶȼ���ʱ��
		float timeSince = timeSinceLastFrame * m_speed;
		for(MapPlayingAnimation::iterator iter = m_playing.begin() ; iter != m_playing.end() ; iter ++)
		{
			PlayingAnimation &ani = iter->second;
			// ����ȫ����̬
			if(!ani.aniDest.isNull())
			{
				ani.aniDest->update(timeSince);
				if(ani.aniDest->hasEnded())
				{
					// ��ȫ����������ϣ���ֹͣ����֮
					if(iter->first != ALL_BLEND)
					{
						ani.aniDest.setNull();
					}
					// ȫ�������͵���ֹͣ�ص����Ժ�Ͳ��ص���
					else
					{
						if(ani.aniDest->getCallback())
						{
							ani.aniDest->getCallback()->onPlayEnd(ani.aniDest->getAnimationInfo());
							ani.aniDest->setCallback(0);
						}
					}
				}

			}
			// ���¹�����̬
			if(!ani.aniBlending.isNull())
			{
				ani.aniBlending->update(timeSince);
				// ȫ����ȶ���������ϣ����л���ȫ����̬
				if(ani.aniBlending->hasBlendEnded())
				{
					ani.aniDest = ani.aniBlending;
					ani.aniBlending.setNull();
					ani.aniDest->forceFinishBlend();
				}
			}
		}
	}

	// ���ĳ�������Ƿ񲥷Ž�����
	bool EntityAnimationController::hasEnded(const AnimationInfo &aniInfo) const
	{
		const PlayingAnimation &ani = *getPlayingAnimation(aniInfo.blendType);
		PlayingAnimationInfoPtr aniPtr(ani.aniBlending.isNull() ? ani.aniDest : ani.aniBlending);
		if(aniPtr.isNull())
			return true;
		if(aniPtr->getAnimationName() == aniInfo.aniName)
		{
			return aniPtr->hasEnded();
		}
		else
		{
			return true;
		}
	}


	// ֹͣһ����������
	void EntityAnimationController::stop(const AnimationInfo &aniInfo)
	{
		
	}
	// �������ƻ�����ڲ��ŵĶ���
	PlayingAnimationInfoPtr EntityAnimationController::getPlayingAnimation(const AnimationInfo &aniInfo)
	{
		MapPlayingAnimation::iterator iter = m_playing.find(aniInfo.blendType);
		if(iter == m_playing.end())
		{
			return PlayingAnimationInfoPtr((PlayingAnimationInfo*)0);
		}
		if(!iter->second.aniBlending.isNull())
		{
			if(iter->second.aniBlending->getAnimationName() == aniInfo.aniName)
			{
				return iter->second.aniBlending;
			}
		}
		if(!iter->second.aniDest.isNull())
		{
			if(iter->second.aniDest->getAnimationName() == aniInfo.aniName)
			{
				return iter->second.aniDest;
			}
		}
		return PlayingAnimationInfoPtr((PlayingAnimationInfo*)0);
	}

	// ��õ�λΪ���λ��
	inline float convertPositionToSecond(const AnimationState *aniState , uchar format , float pos)
	{
		float posSecond;
		switch(format & 0xF)
		{
		case PF_SECOND:
			posSecond = pos;
			break;
		case PF_MILLISECOND:
			posSecond = pos / 1000.0f;
			break;
		case PF_PERCENT:
			posSecond = aniState->getLength() * pos;
			break;
		default:
			posSecond = pos;
			break;
		}

		switch(format & 0xF0)
		{
		case PF_BEGINPOS:
			return posSecond;
			break;
		case PF_CURPOS:
			return aniState->getTimePosition() + posSecond;
			break;
		case PF_ENDPOS:
			return aniState->getLength() + posSecond;
			break;
		default:
			return posSecond;
			break;
		}
	}
};
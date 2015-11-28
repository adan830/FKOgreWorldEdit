#include "StdAfx.h"
#include "CharacterObject.h"
#include "IScene.h"
#include "ITriggerPlayer.h"
#include "TriggerObject.h"
namespace Ogre
{

	const String CharacterObjectFactory::FACTORY_TYPE = "CharacterObject";

	// ��øù�������������
	const String &CharacterObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// ��������Ĵ�������
	ISceneObject *CharacterObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		ISceneObject *obj = new CharacterObject(name);
		obj->create(scene , params);
		return obj;
	}

	// ������������ٷ���
	void CharacterObjectFactory::destroy(ISceneObject *obj)
	{
		obj->destroy();
		OGRE_DELETE(obj);
	}

	CharacterObject::CharacterObject(const String &name)
		: EntityObject(name)
		, m_movingTargetDistance(FLT_MAX)
		, m_moveStartPosition(Vector3::ZERO)
		, m_movingTarget(Vector3::ZERO)
		, m_movingVelocity(0)
		, m_fadeSpeed(0)
		, m_callback(0)
		, m_triggerPlayer(0)
	{
		m_canCollide = false;
		m_mapAnimationInfo["idle"] = AnimationInfo("idle" , ALL_BLEND , 0 , 0.1f);
		m_mapAnimationInfo["walk"] = AnimationInfo("walk" , ALL_BLEND , 0 , 0.1f);
		m_mapAnimationInfo["run"] = AnimationInfo("run" , ALL_BLEND , 0 , 0.1f);
		m_mapAnimationInfo["attack"] = AnimationInfo("attack" , UPPER_BLEND , 1 , 0.1f);
	}

	CharacterObject::~CharacterObject(void)
	{
		destroy();
	}

	void CharacterObject::setupAnimation()
	{
		// ע���������
		m_animationController.registerBoneSelector(UPPER_BLEND , "BU");
		m_animationController.registerBoneSelector(LEFT_HAND_BLEND , "BUL");
		m_animationController.registerBoneSelector(RIGHT_HAND_BLEND , "BUR");
		m_animationController.registerBoneSelector(HEAD_BLEND , "BUH");
	}
	// ���ò���״̬��
	void CharacterObject::setupPFSM()
	{
		//���״̬��
		m_fsmController.setCallBackClass(this);

		// �ƶ���
		m_fsmController.addFSM(FSM_MOVING);
		// ֹͣ�ƶ����
		m_fsmController.addState(FSM_MOVING , STATE_MOVING_STOP , &CharacterObject::onMovingStopEnter , &CharacterObject::onMovingStopLeave , &CharacterObject::onMovingStopState);
		// �����ƶ�
		m_fsmController.addState(FSM_MOVING , STATE_MOVING , &CharacterObject::onMovingEnter , &CharacterObject::onMovingLeave , &CharacterObject::onMovingState);
		// Ĭ���ƶ�
		m_fsmController.changeState(FSM_MOVING , STATE_MOVING_STOP);

		// ��̬��
		m_fsmController.addFSM(FSM_POSE);
		// վ����̬
		m_fsmController.addState(FSM_POSE , STATE_POSE_STAND , &CharacterObject::onPoseStandEnter);
		// ������̬
		m_fsmController.addState(FSM_POSE , STATE_POSE_WALK , &CharacterObject::onPoseWalkEnter);
		// �ܲ���̬
		m_fsmController.addState(FSM_POSE , STATE_POSE_RUN , &CharacterObject::onPoseRunEnter);
		// Ĭ����̬
		m_fsmController.changeState(FSM_POSE , STATE_POSE_STAND);

		// ������
		m_fsmController.addFSM(FSM_ACTION);
		// ���ж���
		m_fsmController.addState(FSM_ACTION , STATE_ACTION_IDLE , &CharacterObject::onActionIdleEnter , &CharacterObject::onActionIdleLeave);
		// ��������
		m_fsmController.addState(FSM_ACTION , STATE_ACTION_ATTACK , &CharacterObject::onActionAttackEnter , &CharacterObject::onActionAttackLeave , &CharacterObject::onActionAttackState);
		// Ĭ�϶���
		m_fsmController.changeState(FSM_ACTION , STATE_ACTION_IDLE);

		// ���뵭����
		m_fsmController.addFSM(FSM_FADE);
		// ֹͣ���뵭��
		m_fsmController.addState(FSM_FADE , STATE_FADE_STOP);
		// ����
		m_fsmController.addState(FSM_FADE , STATE_FADE_OUT , 0 , 0 , &CharacterObject::onFadeOutState);
		// ����
		m_fsmController.addState(FSM_FADE , STATE_FADE_IN , 0 , 0 , &CharacterObject::onFadeInState);
		// ��������
		m_fsmController.addState(FSM_FADE , STATE_FADE_INOUT , 0 , 0 , &CharacterObject::onFadeInOutState);
		// Ĭ�ϵ��뵭��
		m_fsmController.changeState(FSM_FADE , STATE_FADE_STOP);
	}

	// ��������ʵ��
	void CharacterObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();

		EntityObject::create(scene , params);


		// ��ʼ������
		setupAnimation();
		
		// ����Ĭ�϶���
		if(params && params->find("idle") != params->end())
		{
			m_mapAnimationInfo["idle"].aniName = params->find("idle")->second;
		}
		// ���ò���״̬��
		setupPFSM();
	}

	// ���ٶ���ʵ��
	void CharacterObject::destroy()
	{
		if(m_triggerPlayer)
		{
			m_scene->getTriggerWorld()->destroyTriggerEntity(m_triggerPlayer);
			m_triggerPlayer = 0;
		}
		m_fsmController.clearFSM();

		EntityObject::destroy();
	}

	// ����������
	void CharacterObject::setEnableTrigger(bool enable)
	{
		if(m_triggerPlayer && !enable)
		{
			m_node->setListener(0);
			m_scene->getTriggerWorld()->destroyTriggerEntity(m_triggerPlayer);
			m_triggerPlayer = 0;
		}
		if(!m_triggerPlayer && enable)
		{
			// ������������Ҷ���
			m_triggerPlayer = m_scene->getTriggerWorld()->createTriggerPlayer(getFullName());
			m_triggerPlayer->setBox(m_entity->getBoundingBox());
			m_triggerPlayer->addListener(this);
			m_triggerPlayer->setUserAny(Any(static_cast<ISceneObject*>(this)));
			m_node->setListener(this);
			nodeUpdated(m_node);
		}
	}
	// �Ƿ����˴�����
	bool CharacterObject::isEnableTrigger()const
	{
		return m_triggerPlayer != 0;
	}

	// �ڵ����ʱ˳����µ���������
	void CharacterObject::nodeUpdated(const Node *node)
	{
		if(m_triggerPlayer)
		{
			m_triggerPlayer->setPosition(getWorldPosition());
			m_triggerPlayer->setScale(getWorldScale());
			m_triggerPlayer->setOrientation(getWorldOrientation());
			m_triggerPlayer->updateTrigger();
		}
	}

	// ֡��������ÿִ֡��
	bool CharacterObject::frameStarted( const FrameEvent& evt )
	{
		// ÿ֡���²���״̬��
		m_fsmController.update(evt.timeSinceLastFrame);
		// ���¶���
		if(!m_animationController.isNull())
		{
			m_animationController.update(evt.timeSinceLastFrame);
		}

		return true;
	}

	// ���е����λ��
	void  CharacterObject::walkTo(const Vector3 &worldPos , float velocity)
	{
		moveTo(worldPos , velocity);
		//TODO:���Ҫ����ת�����ƶ�����ô������Ҫ��һ��ת��״̬��
		// ת����ǰ����
		setWorldOrientation(m_movingDirection);
		// �л�����·��̬
		m_fsmController.changeState(FSM_POSE , STATE_POSE_WALK);
	}

	// �ܲ������λ��
	void  CharacterObject::runTo(const Vector3 &worldPos , float velocity)
	{
		// ����ƶ�����̫�٣��Ͳ���
		if((worldPos - getWorldPosition()).length() < 0.0001f)
			return;
		moveTo(worldPos , velocity);
		// TODO:���Ҫ����ת�����ƶ�����ô������Ҫ��һ��ת��״̬��
		// ����ת����ǰ����
		setWorldOrientation(m_movingDirection);
		// �л����ܲ���̬
		m_fsmController.changeState(FSM_POSE , STATE_POSE_RUN);
	}

	// �ƶ������λ�ã����ı���̬��
	void CharacterObject::moveTo(const Vector3 &worldPos , float velocity)
	{
		// Ԥ����0�����ƶ�����̫С�Ͳ���
		if(velocity == 0.0f)
			return;
		// ���
		m_moveStartPosition = getWorldPosition();
		// �ٶ�
		m_movingVelocity = velocity;

		// ������Ԫ������
		Vector3 dir(worldPos - getWorldPosition());
		float distance = dir.normalise();
		m_movingDirection = MathEx::DirectionToQuaternion(dir);

		// �ƶ�������ǰ�ĵ�͵���stand
		m_movingTargetDistance = distance;
		// ����Ŀ�ĵ�
		m_movingTarget = worldPos;
		// �л����ƶ�״̬
		m_fsmController.changeState(FSM_MOVING , STATE_MOVING);
	}

	// ת���������
	void CharacterObject::turnTo(const Quaternion &rotate)
	{
		setWorldOrientation(rotate);
		m_movingDirection = rotate;
	}

	// ת��ָ�����λ�õķ����������꣩
	void CharacterObject::turnTo(const Vector3 &targetPoint)
	{
		turnTo(MathEx::PositionToQuaternion(getWorldPosition() , targetPoint));
	}

	// �����ԭ���ķ���ת��һ������
	void CharacterObject::turn(const Quaternion &rotate)
	{
		turnTo(getWorldOrientation() * rotate);
	}
	// ��ǰ����
	void CharacterObject::walk(float velocity , float distance)
	{
		move(velocity , distance);
		// �л�����·��̬
		m_fsmController.changeState(FSM_POSE , STATE_POSE_WALK);
	}

	// ��ǰ�ܲ�
	void CharacterObject::run(float velocity , float distance)
	{
		move(velocity , distance);
		// �л����ܲ���̬
		m_fsmController.changeState(FSM_POSE , STATE_POSE_RUN);
	}

	// ��ǰ�ƶ�
	void CharacterObject::move(float velocity , float distance)
	{
		// Ԥ����0����
		if(velocity == 0.0f)
			return;
		// ���㷽��
		m_movingDirection = getWorldOrientation();
		m_movingVelocity = velocity;
		// ���ƾ���
		m_movingTargetDistance = FLT_MAX;
		// ���
		m_moveStartPosition = getWorldPosition();
		// �л����ƶ�״̬
		m_fsmController.changeState(FSM_MOVING , STATE_MOVING);
	}
	// ��ǰ����
	void CharacterObject::walk(float velocity , const Vector3 &worldPos , float distance)
	{
		turnTo(worldPos);
		walk(velocity , distance);
	}
	// ��ǰ�ܲ�
	void CharacterObject::run(float velocity , const Vector3 &worldPos , float distance)
	{
		turnTo(worldPos);
		run(velocity , distance);
	}
	// ��ǰ�ƶ�
	void CharacterObject::move(float velocity , const Vector3 &worldPos , float distance)
	{
		turnTo(worldPos);
		move(velocity , distance);
	}

	// ��ǰ���У���velocity���ٶ����������������תorientation�ķ����ƶ�distance����
	void CharacterObject::walk(float velocity , const Quaternion &orientation , float distance)
	{
		// �ȱ�����ת
		Quaternion rotateOld(getOrientation());
		// ��ת
		rotate(orientation);
		// �����������
		walk(velocity , distance);
		// �ָ���ת
		setOrientation(rotateOld);
	}

	// ��ǰ�ܲ�����velocity���ٶ����������������תorientation�ķ����ƶ�distance����
	void CharacterObject::run(float velocity , const Quaternion &orientation , float distance)
	{
		// �ȱ�����ת
		Quaternion rotateOld(getOrientation());
		// ��ת
		rotate(orientation);
		// �����������
		run(velocity , distance);
		// �ָ���ת
		setOrientation(rotateOld);
	}

	// ��ǰ�ƶ�����velocity���ٶ����������������תorientation�ķ����ƶ�distance����
	void CharacterObject::move(float velocity , const Quaternion &orientation , float distance )
	{
		// �ȱ�����ת
		Quaternion rotateOld(getOrientation());
		// ��ת
		rotate(orientation);
		// �����������
		move(velocity , distance);
		// �ָ���ת
		setOrientation(rotateOld);
	}


	// ֹͣ�ƶ���վ��
	void CharacterObject::stand()
	{
		// ����Ŀ���ٶ�Ϊ0
		m_movingVelocity = 0.0f;
		// ���
		m_moveStartPosition = getWorldPosition();
		m_movingTargetDistance = 0.0f;
		// ����Ŀ�ĵ�
		m_movingTarget = m_moveStartPosition;
		// ֹͣ�ƶ�
		m_fsmController.changeState(FSM_MOVING , STATE_MOVING_STOP);
	}
	// ���ж���
	void CharacterObject::idle()
	{
		m_fsmController.changeState(FSM_ACTION , STATE_ACTION_IDLE);
	}

	// ��������
	void CharacterObject::attack()
	{
		m_fsmController.changeState(FSM_ACTION , STATE_ACTION_ATTACK);
	}

	// �ƶ���ʼ
	void CharacterObject::onMovingEnter(uint32 state)
	{
		if(m_callback)
		{
			m_callback->onMovingStart();
		}
	}

	// �����ƶ�
	void CharacterObject::onMovingState(float timeSinceLastFrame)
	{
		// �����ƶ�
		moving(timeSinceLastFrame);
		// Ҫ���
		if(m_movingTargetDistance < FLT_MAX)
		{
			// ��Ŀ�ĵأ�����һ������
			if((getWorldPosition() - m_moveStartPosition).length() >= (m_movingTargetDistance - 0.0001f))
			{
				// ֱ�Ӹı�λ�õ�Ŀ�ĵأ�Ԥ�������˹�ȥ
				setWorldPosition(m_movingTarget);
				// ֹͣ�ƶ�
				stand();
				// ���ƶ���
				return;
			}
		}
	};
	// �ƶ�����
	void CharacterObject::moving(float timeSinceLastFrame)
	{
		// ����ʱ��ƽ�ƣ�-ZΪ������
		translate(m_movingDirection * Vector3(0 , 0 , -m_movingVelocity * timeSinceLastFrame));
	}

	// �ƶ�����
	void CharacterObject::onMovingLeave(uint32 state)
	{
		if(m_callback)
		{
			m_callback->onMovingEnd();
		}
	}


	// �ƶ�ֹͣ��Ͽ�ʼ
	void CharacterObject::onMovingStopEnter(uint32 state)
	{
		// վ������
		m_fsmController.changeState(FSM_POSE , STATE_POSE_STAND);
	}

	// �����ƶ�ֹͣ���
	void CharacterObject::onMovingStopState(float timeSinceLastFrame)
	{
	};

	// �ƶ���Ͻ���
	void CharacterObject::onMovingStopLeave(uint32 state)
	{
		if(m_callback)
		{
			m_callback->onMovingStopEnd();
		}
	}



	// վ����̬
	void CharacterObject::onPoseStandEnter(uint32 state)
	{
		m_animationController.play(m_mapAnimationInfo["idle"]);
	};

	// ������̬
	void CharacterObject::onPoseWalkEnter(uint32 state)
	{
		m_animationController.play(m_mapAnimationInfo["walk"]);
	};

	// �ܲ���̬
	void CharacterObject::onPoseRunEnter(uint32 state)
	{
		m_animationController.play(m_mapAnimationInfo["run"]);
	};


	// ���ж���
	void CharacterObject::onActionIdleEnter(uint32 state)
	{
		if(m_callback)
		{
			m_callback->onActionIdleStart();
		}
	};

	// ���ж���
	void CharacterObject::onActionIdleLeave(uint32 state)
	{
		if(m_callback)
		{
			m_callback->onActionIdleEnd();
		}
	};
	// ��������
	void CharacterObject::onActionAttackEnter(uint32 state)
	{
		m_animationController.play(m_mapAnimationInfo["attack"]);
		if(m_callback)
		{
			m_callback->onActionAttackStart();
		}
	};
	// ��������
	void CharacterObject::onActionAttackLeave(uint32 state)
	{
		if(m_callback)
		{
			m_callback->onActionAttackEnd();
		}
	};

	// ��������
	void CharacterObject::onActionAttackState(float timeSinceLastFrame)
	{
		// ����Ѿ�������ϣ����л������ж���
		if(m_animationController.hasEnded(m_mapAnimationInfo["attack"]))
		{
			m_fsmController.changeState(FSM_ACTION , STATE_ACTION_IDLE);
		}
	};


	// ����
	void CharacterObject::onFadeOutState(float timeSinceLastFrame)
	{
		setAlpha(getAlpha() - m_fadeSpeed * timeSinceLastFrame);
		// ���С��0����ֹͣ����
		if(getAlpha() <= 0.0f)
		{
			m_fsmController.changeState(FSM_FADE , STATE_FADE_STOP);
		}
	};

	// ����
	void CharacterObject::onFadeInState(float timeSinceLastFrame)
	{
		setAlpha(getAlpha() + m_fadeSpeed * timeSinceLastFrame);
		// �������1����ֹͣ����
		if(getAlpha() >= 1.0f)
		{
			m_fsmController.changeState(FSM_FADE , STATE_FADE_STOP);
		}
	};

	// ��������
	void CharacterObject::onFadeInOutState(float timeSinceLastFrame)
	{
		setAlpha(getAlpha() + m_fadeSpeed * timeSinceLastFrame);
		// �����������������ͷ�ת
		if(getAlpha() >= 1.0f || getAlpha() <= 0.0f)
		{
			m_fadeSpeed = -m_fadeSpeed;
		}
	};

	// ����
	void  CharacterObject::fadeIn(float fadeSpeed)
	{
		m_fadeSpeed = fadeSpeed;
		m_fsmController.changeState(FSM_FADE , STATE_FADE_IN);
	}

	// ����
	void  CharacterObject::fadeOut(float fadeSpeed)
	{
		m_fadeSpeed = fadeSpeed;
		m_fsmController.changeState(FSM_FADE , STATE_FADE_OUT);
	}

	// ��������
	void  CharacterObject::fadeInOut(float fadeSpeed)
	{
		m_fadeSpeed = fadeSpeed;
		m_fsmController.changeState(FSM_FADE , STATE_FADE_INOUT);
	}

	// ����ֹͣ��������
	void CharacterObject::stopFade()
	{
		m_fsmController.changeState(FSM_FADE , STATE_FADE_STOP);
	}


	// ������һ��������
	// @entity ������Ĵ�����
	// @player ��������
	void CharacterObject::onEnter(ITriggerEntity *entity , ITriggerPlayer *player)
	{
		ISceneObject *triggerObject = obj_cast(entity->getUserAny());
		ISceneObject *playerObject = obj_cast(player->getUserAny());
		if(m_callback && triggerObject && playerObject)
		{
			m_callback->onEnterTrigger(static_cast<TriggerObject*>(triggerObject) , static_cast<CharacterObject*>(playerObject));
		}
	}

	// ��һ����������ȥ
	// @entity ��Ҵ������������ȥ
	// @player ��ȥ�����
	void CharacterObject::onLeave(ITriggerEntity *entity , ITriggerPlayer *player)
	{
		ISceneObject *triggerObject = obj_cast(entity->getUserAny());
		ISceneObject *playerObject = obj_cast(player->getUserAny());
		if(m_callback && triggerObject && playerObject)
		{
			m_callback->onLeaveTrigger(static_cast<TriggerObject*>(triggerObject) , static_cast<CharacterObject*>(playerObject));
		}
	}
};
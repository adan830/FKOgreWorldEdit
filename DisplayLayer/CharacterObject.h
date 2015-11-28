#pragma once
#include "EntityObject.h"
#include "PFSMController.h"
namespace Ogre
{
	class CharacterObject;
	class TriggerObject;
	// ��������Ķ��󹤳�
	class DISPLAYLAYER_API CharacterObjectFactory : public ISceneObjectFactory
	{
	public:
		static const String FACTORY_TYPE;
		// ��øù�������������
		virtual const String &getType() const;
		// ��������Ĵ�������
		virtual ISceneObject *create(const String &name , IScene *scene , const NameValuePairList* params);
		// ������������ٷ���
		virtual void destroy(ISceneObject *obj);
	};


	enum PFSMSTATE
	{
		// ���ö�ٵĵ�һ��ֵ
		CHARACTER_STATE_START,

		// �ƶ���
		FSM_MOVING,
		// ֹͣ�ƶ�
		STATE_MOVING_STOP,
		// �����ƶ�
		STATE_MOVING,

		// ��̬��
		FSM_POSE,
		// վ����̬
		STATE_POSE_STAND,
		// ������̬
		STATE_POSE_WALK,
		// �ܲ���̬
		STATE_POSE_RUN,

		// ������
		FSM_ACTION,
		// ���ж���
		STATE_ACTION_IDLE,
		// ��������
		STATE_ACTION_ATTACK,

		// ���뵭����
		FSM_FADE,
		// ֹͣ���뵭��
		STATE_FADE_STOP,
		// ����
		STATE_FADE_OUT,
		// ����
		STATE_FADE_IN,
		// ��������
		STATE_FADE_INOUT,

		// ���ö�ٵ����һ��ֵ���Ժ�̳еľʹ����￪ʼ
		CHARACTER_STATE_END,
	};
	// ���ﶯ����Ϣ
	typedef std::map<String , AnimationInfo> MapAnimationInfo;

	// ���ﶯ���ص�
	class CharacterCallback
	{
	public:
		// �ƶ���ʼ
		virtual void onMovingStart(){};
		// �ƶ�����
		virtual void onMovingEnd(){};

		// �ƶ�ֹͣ��ʼ
		virtual void onMovingStopStart(){};
		// �ƶ�ֹͣ����
		virtual void onMovingStopEnd(){};

		// ���ж�����ʼ
		virtual void onActionIdleStart(){};
		// ���ж�������
		virtual void onActionIdleEnd(){};

		// ����������ʼ
		virtual void onActionAttackStart(){};
		// ������������
		virtual void onActionAttackEnd(){};


		// ������һ��������
		// @triggerObject ������Ĵ�����
		// @player ��������
		virtual void onEnterTrigger(TriggerObject *triggerObject , CharacterObject *player){};

		// ��һ����������ȥ
		// @triggerObject ��Ҵ������������ȥ
		// @player ��ȥ�����
		virtual void onLeaveTrigger(TriggerObject *triggerObject , CharacterObject *player){};
	};

	enum CharacterBlendType
	{
		UPPER_BLEND = USER_BLEND + 1,
		LEFT_HAND_BLEND,
		RIGHT_HAND_BLEND,
		HEAD_BLEND,
	};

	// �������
	class DISPLAYLAYER_API CharacterObject : public EntityObject
		, public TriggerListener
		, public Node::Listener
	{
	public:
		CharacterObject(const String &name);
		virtual ~CharacterObject(void);
		// ��ö�������
		virtual const String &getType()const
		{
			return CharacterObjectFactory::FACTORY_TYPE;
		}
		// ��������ʵ��
		virtual void create(IScene *scene , const NameValuePairList* params);
		// ���ٶ���ʵ��
		virtual void destroy();

		// ֡��������ÿִ֡��
		virtual bool frameStarted( const FrameEvent& evt );

		// ���е����λ��
		void walkTo(const Vector3 &worldPos , float velocity);
		// �ܲ������λ��
		void runTo(const Vector3 &worldPos , float velocity);
		// �ƶ������λ�ã����ı���̬��
		void moveTo(const Vector3 &worldPos , float velocity);

		// ��ǰ���У���velocity���ٶ��ƶ�distance����
		void walk(float velocity , float distance = FLT_MAX);
		// ��ǰ�ܲ�����velocity���ٶ��ƶ�distance����
		void run(float velocity , float distance = FLT_MAX);
		// ��ǰ�ƶ�����velocity���ٶ��ƶ�distance����
		void move(float velocity , float distance = FLT_MAX);

		// ��ǰ���У���ת����worldPos���꣬Ȼ����velocity���ٶ��ƶ�distance����
		void walk(float velocity , const Vector3 &worldPos , float distance = FLT_MAX);
		// ��ǰ�ܲ�����ת����worldPos���꣬Ȼ����velocity���ٶ��ƶ�distance����
		void run(float velocity , const Vector3 &worldPos , float distance = FLT_MAX);
		// ��ǰ�ƶ�����ת����worldPos���꣬Ȼ����velocity���ٶ��ƶ�distance����
		void move(float velocity , const Vector3 &worldPos , float distance = FLT_MAX);

		// ��ǰ���У���velocity���ٶ����������������תrotate�ķ����ƶ�distance����
		void walk(float velocity , const Quaternion &orientation , float distance = FLT_MAX);
		// ��ǰ�ܲ�����velocity���ٶ����������������תrotate�ķ����ƶ�distance����
		void run(float velocity , const Quaternion &orientation , float distance = FLT_MAX);
		// ��ǰ�ƶ�����velocity���ٶ����������������תrotate�ķ����ƶ�distance����
		void move(float velocity , const Quaternion &orientation , float distance = FLT_MAX);

		// ֹͣ�ƶ���վ��
		void stand();

		// ת���������
		void turnTo(const Quaternion &rotate);
		// ת��ָ�����λ�õķ����������꣩
		void turnTo(const Vector3 &targetPoint);
		// �����ԭ���ķ���ת��һ������
		void turn(const Quaternion &rotate);

		// ���ж���
		void idle();
		// ��������
		void attack();

		// ����
		void fadeIn(float fadeSpeed);
		// ����
		void fadeOut(float fadeSpeed);
		// ��������
		void fadeInOut(float fadeSpeed);
		// ����ֹͣ��������
		void stopFade();

		// �������ﶯ���ص�
		void setCallback(CharacterCallback *callback)
		{
			m_callback = callback;
		}
		CharacterCallback *getCallback()
		{
			return m_callback;
		}

		// �������ﶯ����Ϣ
		void setAnimationInfo(const MapAnimationInfo &mapAnimationInfo)
		{
			m_mapAnimationInfo = mapAnimationInfo;
		}

		// ������ﶯ����Ϣ
		MapAnimationInfo &getAnimationInfo()
		{
			return m_mapAnimationInfo;
		}

		// ��������ƶ��ķ���
		const Quaternion &getMovingDirection()const
		{
			return m_movingDirection;
		}

		// ������һ��������
		// @entity ������Ĵ�����
		// @player ��������
		virtual void onEnter(ITriggerEntity *entity , ITriggerPlayer *player);

		// ��һ����������ȥ
		// @entity ��Ҵ������������ȥ
		// @player ��ȥ�����
		virtual void onLeave(ITriggerEntity *entity , ITriggerPlayer *player);

		// �ڵ����ʱ˳����µ���������
		virtual void nodeUpdated(const Node*);

		// ����������
		virtual void setEnableTrigger(bool enable);
		// �Ƿ����˴�����
		virtual bool isEnableTrigger()const;
	protected:
		// ��������״̬��
		PFSMController<CharacterObject> m_fsmController;
		// ��ʼ������״̬��
		virtual void setupPFSM();

		// ��ʼ������
		virtual void setupAnimation();

		// �ƶ���ʼ
		void onMovingEnter(uint32 state);
		// �����ƶ�
		void onMovingState(float timeSinceLastFrame);
		// �ƶ�����
		void onMovingLeave(uint32 state);

		// �ƶ�����
		void moving(float timeSinceLastFrame);

		// �ƶ�ֹͣ��Ͽ�ʼ
		void onMovingStopEnter(uint32 state);
		// ����ֹͣ����ƶ�
		void onMovingStopState(float timeSinceLastFrame);
		// �ƶ�ֹͣ��Ͻ���
		void onMovingStopLeave(uint32 state);

		// վ����̬
		void onPoseStandEnter(uint32 state);
		// ������̬
		void onPoseWalkEnter(uint32 state);
		// �ܲ���̬
		void onPoseRunEnter(uint32 state);

		// ���ж�����ʼ
		void onActionIdleEnter(uint32 state);
		// ���ж�������
		void onActionIdleLeave(uint32 state);
		// ������������
		void onActionAttackEnter(uint32 state);
		// ������������
		void onActionAttackLeave(uint32 state);
		// ��������ִ����
		void onActionAttackState(float timeSinceLastFrame);

		// ����
		void onFadeOutState(float timeSinceLastFrame);
		// ����
		void onFadeInState(float timeSinceLastFrame);
		// ��������
		void onFadeInOutState(float timeSinceLastFrame);

		// �ƶ��ķ���
		Quaternion m_movingDirection;
		// �ƶ����ٶ�
		float m_movingVelocity;
		// ��ʼ�ƶ���λ��
		Vector3 m_moveStartPosition;
		// ��Ҫ�ƶ��ľ���
		float m_movingTargetDistance;
		// �ƶ���Ŀ�ĵأ�������Ҫ�ƶ��������Ƶ�ʱ������ã�MoveTo��ʱ��
		Vector3 m_movingTarget;

		// ÿ��ĵ��뵭����
		float m_fadeSpeed;

		// ���ﶯ���ص�
		CharacterCallback *m_callback;

		// ���ﶯ����Ϣ
		MapAnimationInfo m_mapAnimationInfo;

		// ��Ҵ�����
		ITriggerPlayer *m_triggerPlayer;
	};


};
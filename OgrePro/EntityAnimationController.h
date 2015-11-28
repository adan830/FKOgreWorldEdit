#pragma once
#include "IEntityAnimationController.h"
#include "AnimationStateController.h"
#include "PlayingAnimationInfo.h"

namespace Ogre
{
	class EntityAnimationController;
	// ����ѡ����
	class OGREPRO_API BoneSelector : public Animation::AnimationCallback
	{
	public:
		BoneSelector(uint blendType)
			: m_blendType(blendType)
		{

		}
		// ��û������
		uint getBlendType()const
		{
			return m_blendType;
		}
		// �Ƿ�Ӱ�������ͷ
		virtual bool isApplyBone(Bone *bone) = 0;
	protected:
		uint m_blendType;
	};

	// ����ѡ����
	class OGREPRO_API NameBoneSelector : public BoneSelector
	{
	public:
		NameBoneSelector(uint blendType , EntityAnimationController *controller , const String &preName = StringUtil::BLANK)
			: BoneSelector(blendType)
			, m_preName(preName)
			, m_controller(controller)
		{

		}
		// ��apply������ʱ��ѡ���Ƿ�apply�������
		// ��ִ�еĹ����У����Զ�̬�޸Ĳ���
		virtual bool applyBone(Bone *&bone , Real &weight , Real &scale);
		// �Ƿ�Ӱ�������ͷ
		virtual bool isApplyBone(Bone *bone);

	protected:
		// ǰ������Ϊ����ĲŲ���
		String m_preName;
		// ����������
		EntityAnimationController *m_controller;
	};

	// ���ڲ��ŵĶ�����Ϣ
	struct OGREPRO_API PlayingAnimation
	{
		PlayingAnimation()
			: selector(0)
		{};
		PlayingAnimation(BoneSelector *sel)
			: selector(sel)
		{

		}
		// ���յĶ���
		PlayingAnimationInfoPtr aniDest;
		// ���ڻ�ϵĶ���
		PlayingAnimationInfoPtr aniBlending;
		// ����ѡ����
		BoneSelector *selector;

		// ��ն���״̬
		void setNull()
		{
			aniDest.setNull();
			aniBlending.setNull();
		}
	};

	// ���ģ��ʵ��Ĺ��������Ĳ��ſ�����ʵ��
	class OGREPRO_API EntityAnimationController : public IEntityAnimationController
	{
	public:
		EntityAnimationController();
		virtual ~EntityAnimationController(void);

		// ����
		void create(Entity *entity);
		// ����
		void destroy();
		// ֹͣһ����������
		virtual void stop(const AnimationInfo &aniInfo);

		// ����һ����������
		virtual void play(const AnimationInfo &aniInfo);
		// ����һ����������
		virtual void play(const String &name
			, uint blend = ALL_REPLACE
			, uint count = 0
			, float blendtime = 1.0f
			, POISTION_FORMAT pfStart = PF_SECOND
			, float posStart = 0.0f
			, POISTION_FORMAT pfEnd = PF_SECOND
			, float posEnd = FLT_MAX
			, float speed = 1.0f
			)
		{
			play(AnimationInfo(name , blend , count , blendtime , pfStart , posStart , pfEnd , posEnd , speed));
		}
		// ���ò����ٶ�
		virtual void setSpeed(float speed);
		// ��ò����ٶ�
		virtual float getSpeed()const;

		// �����Ƿ���ͣ
		virtual void setPaused(bool pause);
		// �Ƿ���ͣ
		virtual bool isPaused();


		// ���ĳ�������Ƿ񲥷Ž�����
		virtual bool hasEnded(const AnimationInfo &aniInfo) const;

		// ���¶���״̬
		virtual void update(float timeSinceLastFrame);

		// �Ƿ��Ѿ���ʼ��
		virtual bool isNull()const
		{
			return m_entity == 0;
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

		// ע��һ������ѡ����
		virtual void registerBoneSelector(uint blendType , const String &preName)
		{
			m_playing[blendType] = PlayingAnimation(new NameBoneSelector(blendType , this , preName));
		}

		// ע��һ������ѡ����
		virtual void registerBoneSelector(BoneSelector *sel)
		{
			m_playing[sel->getBlendType()] = PlayingAnimation(sel);
		}


		typedef std::map<uint , PlayingAnimation> MapPlayingAnimation;
		typedef MapIterator<MapPlayingAnimation> PlayingAnimationIterator;
		// ��ö����б�
		PlayingAnimationIterator getPlayingAnimationIterator()
		{
			return PlayingAnimationIterator(m_playing.begin() , m_playing.end());
		}

		// �������ƻ�����ڲ��ŵĶ���
		PlayingAnimationInfoPtr getPlayingAnimation(const AnimationInfo &aniInfo);
	protected:
		// �������й���ѡ����
		void destroyAllBoneSelector();
		// ��ö���
		PlayingAnimation *getPlayingAnimation(uint blendType);
		const PlayingAnimation *getPlayingAnimation(uint blendType)const;
		// ���Ƶ�ģ��ʵ��
		Entity *m_entity;
		// ��ǰ���ŵĶ���
		MapPlayingAnimation m_playing;
		// �����ٶ�
		float m_speed;
		// ��ͣ
		bool m_paused;
		// ���Żص�
		IEntityAnimationCallback *m_callback;
		// ������в���״̬
		void clearState();
		// ���һ������״̬
		void clearState(const AnimationInfo &aniInfo);
	};
};
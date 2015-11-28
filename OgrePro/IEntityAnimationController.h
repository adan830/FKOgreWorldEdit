#pragma once
namespace Ogre
{
	class PlayingAnimationInfo;
	typedef SharedPtr<PlayingAnimationInfo> PlayingAnimationInfoPtr;

	// �������
	enum ANI_BLEND_TYPE
	{
		ALL_REPLACE = 0,// ȫ���滻��ֻ�����������
		ALL_BLEND,      // ȫ���ϣ����ֵ�ǰ�����ж�����Ȼ�����������
		USER_BLEND,		// �Զ����ϵ������￪ʼ
	};
	// λ�ø�ʽ
	enum POISTION_FORMAT
	{
		// ����3����λ�õĶ�����λ��ʽ��3����ֻ��ѡһ��
		PF_SECOND		= 1 << 1,		// ��λ����
		PF_MILLISECOND	= 1 << 2,		// ��λ�Ǻ��룬ǧ��֮һ��
		PF_PERCENT		= 1 << 3,		// �ٷֱ�

		// ����3����λ�õ����λ�ã�3����ֻ��ѡһ��
		PF_BEGINPOS		= 1 << 4,		// �Ӷ�����ͷλ������
		PF_CURPOS		= 1 << 5,		// �Ӷ�����ǰλ������
		PF_ENDPOS		= 1 << 6,		// �Ӷ���ĩβλ������
	};

	// ��õ�λΪ���λ��
	OGREPRO_API float convertPositionToSecond(const AnimationState *aniState , uchar format , float pos);

	// ������Ϣ
	struct OGREPRO_API AnimationInfo
	{
		AnimationInfo()
			: blendType(ALL_REPLACE)
			, playCount(0)
			, blendTime(1.3f)
			, positionFormatStart(PF_SECOND | PF_BEGINPOS)
			, positionFormatEnd(PF_SECOND | PF_BEGINPOS)
			, positionStart(0)
			, positionEnd(FLT_MAX)
		{

		}
		AnimationInfo(const String &name
			, uint blend
			, uint count = 0
			, float blendtime = 1.0f
			, uchar pfStart = PF_SECOND | PF_BEGINPOS
			, float posStart = 0.0f
			, uchar pfEnd = PF_SECOND | PF_BEGINPOS
			, float posEnd = FLT_MAX
			, float speed = 1.0f
			)
			: aniName(name)
			, blendType(blend)
			, playCount(count)
			, blendTime(blendtime)
			, positionFormatStart(pfStart)
			, positionStart(posStart)
			, positionFormatEnd(pfEnd)
			, positionEnd(posEnd)
			, playSpeed(speed)
		{

		};
		// ��������
		String				aniName;
		// �������
		uint			    blendType;
		// ���Ŵ���
		uint				playCount;
		// �ӱ�Ķ������ȵ�������������ʱ��
		float				blendTime;
		// ��ʼ����λ�õĵ�λ��ʽ
		uchar				positionFormatStart;
		// ��ʼ����λ��
		float				positionStart;
		// ��������λ�õĵ�λ��ʽ
		uchar				positionFormatEnd;
		// ��������λ��
		float				positionEnd;
		// �����ٶȣ�1��ʾ��׼�ٶȣ�2��ʾ2�٣��Դ����ƣ�����Ϊ���𲥷ţ�positionStart��positionEnd����
		float				playSpeed;

		// ��õ�λΪ��Ŀ�ʼ����λ��
		float getPositionStartAsSecond(AnimationState *aniState)
		{
			return convertPositionToSecond(aniState , positionFormatStart , positionStart);
		}

		// ��õ�λΪ��Ľ�������λ��
		float getPositionEndAsSecond(AnimationState *aniState)
		{
			return convertPositionToSecond(aniState , positionFormatEnd , positionEnd);
		}
	};

	// ģ��ʵ�嶯���ص�
	class IEntityAnimationCallback
	{
	public:
		// ������ϻص�
		virtual void onPlayEnd(const AnimationInfo &aniInfo) = 0;
		// ��������һ�Σ���ѭ�������У�ÿ������һ�ζ���ص�������
		virtual void onPlayOnce(const AnimationInfo &aniInfo) = 0;
	};

	// ���ģ��ʵ��Ĺ��������Ĳ��ſ�����
	class IEntityAnimationController
	{
	public:

		IEntityAnimationController(void)
		{
		}

		virtual ~IEntityAnimationController(void)
		{
		}

		// ֹͣһ����������
		virtual void stop(const AnimationInfo &aniInfo) = 0;
		// ����һ����������
		virtual void play(const AnimationInfo &aniInfo) = 0;
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
			) = 0;
		// ���ò����ٶ�
		virtual void setSpeed(float speed) = 0;
		// ��ò����ٶ�
		virtual float getSpeed()const = 0;

		// �����Ƿ���ͣ
		virtual void setPaused(bool pause) = 0;
		// �Ƿ���ͣ
		virtual bool isPaused() = 0;

		// ���ĳ�������Ƿ񲥷Ž�����
		virtual bool hasEnded(const AnimationInfo &aniInfo) const = 0;

		// ���¶���״̬
		virtual void update(float timeSinceLastFrame) = 0;

		// �Ƿ��Ѿ���ʼ��
		virtual bool isNull()const = 0;

		// ���ûص�
		virtual void setCallback(IEntityAnimationCallback *callback) = 0;
		// ��ȡ�ص�
		virtual IEntityAnimationCallback* getCallback() = 0;

		// �������ƻ�����ڲ��ŵĶ���
		virtual PlayingAnimationInfoPtr getPlayingAnimation(const AnimationInfo &aniInfo) = 0;

		// ע��һ������ѡ����
		virtual void registerBoneSelector(uint blendType , const String &preName) = 0;
	};

};
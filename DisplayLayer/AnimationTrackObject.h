#pragma once
#include "NullObject.h"
namespace Ogre
{
	// ��������Ķ��󹤳�
	class DISPLAYLAYER_API AnimationTrackObjectFactory : public ISceneObjectFactory
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

	// ģ��ʵ�����
	class DISPLAYLAYER_API AnimationTrackObject : public NullObject
	{
	public:
		AnimationTrackObject(const String &name);
		virtual ~AnimationTrackObject(void);
		// ��ö�������
		virtual const String &getType()const
		{
			return AnimationTrackObjectFactory::FACTORY_TYPE;
		}
		// ��������ʵ��
		virtual void create(IScene *scene , const NameValuePairList* params);
		// ���ٶ���ʵ��
		virtual void destroy();
		// ���MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return 0;
		}

		// ��������
		virtual bool setProperty(uint id , const Any &value);
		// ��ȡ����ֵ
		virtual Any getProperty(uint id)const;
		// ����������ԣ���������ʽ
		virtual const VTProperty* getPropertiesTree()const;
		// ����������ԣ����б����ʽ
		virtual const MapProperty* getPropertiesList()const;

		// ��ó�������Ŀɼ����������ڱ༭����ѡ��
		virtual ISceneObjectVisible *getSceneObjectVisible();


		// һ���ؼ�֡
		struct KeyFrame
		{
			KeyFrame()
				: m_length(1.0f)
				, m_translate(Vector3::ZERO)
				, m_scale(Vector3::UNIT_SCALE)
				, m_rotate(Quaternion::IDENTITY)
			{

			}
			// �ؼ�֡�Ķ������ȣ���λΪ��
			float m_length;
			// ��ǰ��λ��
			Vector3 m_translate;
			// ��ǰ������
			Vector3 m_scale;
			// ��ǰ����ת
			Quaternion m_rotate;
		};
		typedef std::deque<KeyFrame> ListKeyFrame;
		typedef VectorIterator<ListKeyFrame> KeyFrameIterator;
		typedef ConstVectorIterator<ListKeyFrame> ConstKeyFrameIterator;
		// ��ùؼ�֡�б�
		KeyFrameIterator getKeyFrameIterator()
		{
			return KeyFrameIterator(m_listKeyFrame.begin() , m_listKeyFrame.end());
		}
		// ��ùؼ�֡�б�
		ConstKeyFrameIterator getConstKeyFrameIterator()const
		{
			return ConstKeyFrameIterator(m_listKeyFrame.begin() , m_listKeyFrame.end());
		}

		// ���һ���ؼ�֡����󣬷��عؼ�֡������
		size_t addKeyFrameToBack(float length , const Vector3 &translate , const Vector3 &scale = Vector3::UNIT_SCALE, const Quaternion &rotate = Quaternion::IDENTITY);
		// ���һ���ؼ�֡����ǰ�����عؼ�֡������
		size_t addKeyFrameToFront(float length , const Vector3 &translate , const Vector3 &scale = Vector3::UNIT_SCALE, const Quaternion &rotate = Quaternion::IDENTITY);
		// ��һ���ؼ�֮֡�����һ���ؼ�֡
		size_t insertKeyFrame(size_t index , float length , const Vector3 &translate , const Vector3 &scale = Vector3::UNIT_SCALE, const Quaternion &rotate = Quaternion::IDENTITY);
		// �޸�һ���ؼ�֡
		void modifyKeyFrame(size_t index , float length , const Vector3 &translate , const Vector3 &scale = Vector3::UNIT_SCALE, const Quaternion &rotate = Quaternion::IDENTITY);
		// ��������ɾ��һ���ؼ�֡
		void removeKeyFrame(size_t index);
		// ��ùؼ�֡������
		size_t getKeyFrameCount()const;
		// �����������һ���ؼ�֡
		const KeyFrame &getKeyFrame(size_t index)const;


		// �����Ƿ�ѭ��
		void setLoop(bool loop);
		// ��ȡ�Ƿ�ѭ��
		bool getLoop()const;

		// ��ֵ��ʽ
		enum InterpolationMode
		{
			// ���Բ�ֵ
			IM_LINEAR,
			// ������ֵ
			IM_SPLINE
		};

		// ��ת��ֵ��ʽ
		enum RotationInterpolationMode
		{
			// ���Բ�ֵ
			RIM_LINEAR,
			// ���Ͳ�ֵ
			RIM_SPHERICAL,
			// ����ֵ
			RIM_NONE
		};
		// ���ò�ֵ��ʽ
		void setInterpolationMode(InterpolationMode im);
		// ��ò�ֵ��ʽ
		InterpolationMode getInterpolationMode(void) const;

		// ������ת��ֵ��ʽ
		void setRotationInterpolationMode(RotationInterpolationMode im);
		// �����ת��ֵ��ʽ
		RotationInterpolationMode getRotationInterpolationMode(void) const;


		// �켣������
		class IAnimationTrackController
		{
		public:
			IAnimationTrackController(){};
			virtual ~IAnimationTrackController(){};
			// ���ع켣��������
			virtual AnimationTrackObject *getAnimationTrackObject()const = 0; 
			// ���ع켣�ϵ��������
			virtual ISceneObject *getTrackObject()const = 0;

			// ���Ź켣���������ÿ�ʼ�������Ķ����ؼ�֡����
			virtual void playByIndex(size_t startIndex = 0 , size_t endIndex = 0xFFFFFFFF) = 0;
			// ���Ź켣���������ÿ�ʼ�������Ķ���ʱ��
			virtual void playByTime(float startTime = 0.0f , float endTime = FLT_MAX) = 0;

			// ��ͣ
			virtual void setPaused(bool pause) = 0;
			// �Ƿ���ͣ
			virtual bool getPaused()const = 0;

			// ��ö�������
			virtual float getLength()const = 0;

			// �����Ƿ�ѭ��
			virtual void setLoop(bool loop) = 0;
			// ��ȡ�Ƿ�ѭ��
			virtual bool getLoop()const = 0;

			// ���ò�ֵ��ʽ
			virtual void setInterpolationMode(InterpolationMode im) = 0;
			// ��ò�ֵ��ʽ
			virtual InterpolationMode getInterpolationMode(void) const = 0;

			// ������ת��ֵ��ʽ
			virtual void setRotationInterpolationMode(RotationInterpolationMode im) = 0;
			// �����ת��ֵ��ʽ
			virtual RotationInterpolationMode getRotationInterpolationMode(void) const = 0;


			// �Ƿ��Ѿ��������
			virtual bool hasEnded() const = 0;

			// ���õ�ǰ����λ�ã�ʹ��ʱ��������
			virtual void setPlayPosByTime(float timePos) = 0;
			// ���õ�ǰ����λ�ã�ʹ�ùؼ�֡����������
			virtual void setPlayPosByIndex(size_t index) = 0;

			// ��õ�ǰ���ŵ�λ�ã���λΪ��
			virtual float getTimePosition()const = 0;

			// ������·�ٶ�
			virtual void setSpeed(float speed) = 0;
			// �����·�ٶ�
			virtual float getSpeed()const = 0;
		};
		// ��һ�����������������켣�ܶ������ع켣������
		IAnimationTrackController* createTrack(ISceneObject *trackObject);
		// ���ٸ��ٹ켣
		void destroyTrack(IAnimationTrackController *controller);

		typedef std::list<IAnimationTrackController*> ListAnimationTrackController;
		typedef VectorIterator<ListAnimationTrackController> AnimationTrackControllerIterator;
		typedef ConstVectorIterator<ListAnimationTrackController> ConstAnimationTrackControllerIterator;
		// ������й켣������
		AnimationTrackControllerIterator getAnimationTrackControllerIterator()
		{
			return AnimationTrackControllerIterator(m_controllers.begin() , m_controllers.end());
		}
		// ������й켣������
		ConstAnimationTrackControllerIterator getConstAnimationTrackControllerIterator()
		{
			return ConstAnimationTrackControllerIterator(m_controllers.begin() , m_controllers.end());
		}
	protected:
		// ֪ͨ���ؽ����ж����켣������
		void _notifyRebuild();

		// �������й켣������
		void destroyAllControllers();
		// ö���������ԣ���������ʽ
		virtual void enumPropertiesTree(VTProperty &properties)const;

		// ��ǰ�����켣���������йؼ�֡
		ListKeyFrame m_listKeyFrame;

		// �켣�������б�
		ListAnimationTrackController m_controllers;

		// �Ƿ�ѭ��
		bool m_loop;
		// ��ֵ��ʽ
		InterpolationMode m_interpolationMode;
		// ��ת��ֵ��ʽ
		RotationInterpolationMode m_rotationInterpolationMode;
	};
};
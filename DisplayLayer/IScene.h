#pragma once
#include "TFactorySystem.h"

namespace Ogre
{
	class ISceneObject;
	class IAnimationStateController;


	// ��ײ��Ϣ
	struct RayCheckInfo
	{
		RayCheckInfo()
			: obj(0)
		{

		};
		// ��ײ����
		RayCollisionInfo info;
		// ������ײ�ĳ�����������ж�������ײ�Ļ���
		ISceneObject *obj;

		/// Comparison operator for sorting
		bool operator < (const RayCheckInfo& rhs) const
		{
			return info.distance < rhs.info.distance;
		}
	};
	typedef std::vector<RayCheckInfo> ListRayCheckInfo;


	// һ������
	class IScene
	{
	public:

		IScene(void)
		{
		}

		virtual ~IScene(void)
		{
		}
		// ����������
		struct Listener
		{
			// ������һ��������������
			virtual void onCreateSceneObject(ISceneObject *obj){};
			// ������һ����������ǰ����
			virtual void onDestroySceneObject(ISceneObject *obj){};
		};

		// ���������
		virtual void addListener(Listener *listener , bool toBack = true) = 0;
		// ɾ��������
		virtual void removeListener(Listener *listener) = 0;

		// ֪ͨ������������ĸ��ļ������
		virtual void _notifyOrigin(const String &origin) = 0;
		// ������ڴ�������������ļ���
		virtual const String &getOrigin() const= 0;
		// �������
		virtual const String &getName() const= 0;

		// ����һ������������
		virtual IAnimationStateController* createAnimationStateController(const String &name , const String &animationName) = 0;
		// ����һ������������
		virtual IAnimationStateController* createAnimationStateController(const String &animationName) = 0;
		// ���һ������������
		virtual IAnimationStateController* getAnimationStateController(const String &name) = 0;
		// �Ƿ�����������������
		virtual bool hasAnimationStateController(const String &name) = 0;
		// �Ƿ�����������
		virtual bool hasAnimation(const String &animationName) = 0;
		// ����һ������������
		virtual void destroyAnimationStateController(const String &name) = 0;
		// ����һ������������
		virtual void destroyAnimationStateController(IAnimationStateController *ani) = 0;

		// �����������
		virtual void clear() = 0;

		// �������ͺ����ƴ���һ����������
		virtual ISceneObject *createSceneObject(const String &name , const String &type, const NameValuePairList* params = 0) = 0;
		// �������ͺ����ƴ���һ����������
		virtual ISceneObject *createSceneObject(const String &type, const NameValuePairList* params = 0) = 0;
		// ����һ����������
		virtual void destroySceneObject(const String &name) = 0;
		virtual void destroySceneObject(ISceneObject *obj) = 0;
		// ���һ����������
		virtual ISceneObject *getSceneObject(const String &name) = 0;
		// �Ƿ����ָ�����ֵĳ�������
		virtual bool hasSceneObject(const String &name)const = 0;
		// �Ƿ��obj�����������
		virtual bool hasSceneObject(ISceneObject *obj)const = 0;
		// ���ָ������ڴ˳����Ƿ���Ч
		virtual bool isSceneObjectValid(ISceneObject *obj)const = 0;

		typedef std::map<String , ISceneObject*> MapSceneObject;
		typedef MapIterator<MapSceneObject> SceneObjectIterator;
		// ������ж�����б�
		virtual SceneObjectIterator getSceneObjectIterator() = 0;

		// ���Ogre�ĳ���������
		virtual SceneManager *getOgreSceneManager()const = 0;

		// ���һ��������ΨһID
		virtual uint32 getUniqueID() = 0;
		// ���һ��������Ψһ����
		virtual String getUniqueName() = 0;

		// ��ø���������
		virtual ISceneObject *getRootObject() = 0;

		// �����ײ����
		virtual ICollisionWorld *getCollisionWorld() = 0;

		// ��ô���������
		virtual ITriggerWorld *getTriggerWorld() = 0;

		// ������ײ��⣬�����ص�һ��������ײ����Ϣ
		// @ray ����
		// @collideWorld �Ƿ��������ײ���磺���Σ�
		// @collideObject �Ƿ�������ײ
		// @info ���������ײ�����Ϣ
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , RayCheckInfo& info) = 0;


		// ������ײ��⣬�����ص�һ��������ײ����Ϣ
		// @ray ����
		// @collideWorld �Ƿ��������ײ���磺���Σ�
		// @collideObject �Ƿ�������ײ
		// @intersection �������
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , Vector3& intersection) = 0;

		// ������ײ���
		// @ray ����
		// @collideWorld �Ƿ��������ײ���磺���Σ�
		// @collideObject �Ƿ�������ײ
		// @infoList ���������ײ�����Ϣ
		// @sortByDistance �����infoList����������
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , bool sortByDistance , ListRayCheckInfo& info) = 0;
	};

};
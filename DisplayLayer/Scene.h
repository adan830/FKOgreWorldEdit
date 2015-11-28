#pragma once
#include "IScene.h"
#include "TListener.h"

namespace Ogre
{
	
	class DISPLAYLAYER_API Scene : public IScene
		, public FrameListener
		, public TListener<IScene::Listener>
	{
	public:
		Scene(SceneManager *mgr, const String &name , ICollisionWorld *collisionWorld , ITriggerWorld *triggerWorld);
		virtual ~Scene(void);

		// �������
		virtual const String &getName() const
		{
			return m_name;
		}
		// ֪ͨ������������ĸ��ļ������
		virtual void _notifyOrigin(const String &origin);
		// ������ڴ�������������ļ���
		virtual const String &getOrigin()const;

		// ���������
		virtual void addListener(Listener *listener , bool toBack = true)
		{
			TListener<IScene::Listener>::addListener(listener , toBack);
		}
		// ɾ��������
		virtual void removeListener(Listener *listener)
		{
			TListener<IScene::Listener>::removeListener(listener);
		}


		// ����һ������������
		virtual IAnimationStateController* createAnimationStateController(const String &name , const String &animationName);
		// ����һ������������
		virtual IAnimationStateController* createAnimationStateController(const String &animationName);
		// ���һ������������
		virtual IAnimationStateController* getAnimationStateController(const String &name);
		// �Ƿ�����������������
		virtual bool hasAnimationStateController(const String &name);
		// �Ƿ�����������
		virtual bool hasAnimation(const String &animationName);
		// ����һ������������
		virtual void destroyAnimationStateController(const String &name);
		// ����һ������������
		virtual void destroyAnimationStateController(IAnimationStateController *ani);

		// �����������
		virtual void clear();

		// �������ͺ����ƴ���һ����������
		virtual ISceneObject *createSceneObject(const String &name , const String &type, const NameValuePairList* params = 0);
		// �������ͺ����ƴ���һ����������
		virtual ISceneObject *createSceneObject(const String &type, const NameValuePairList* params = 0);
		// ����һ����������
		virtual void destroySceneObject(const String &name);
		virtual void destroySceneObject(ISceneObject *obj);
		// ���һ����������
		virtual ISceneObject *getSceneObject(const String &name);
		// �Ƿ����ָ�����ֵĳ�������
		virtual bool hasSceneObject(const String &name)const;
		// �Ƿ��obj�����������
		virtual bool hasSceneObject(ISceneObject *obj)const;
		// ���ָ������ڴ˳����Ƿ���Ч
		virtual bool isSceneObjectValid(ISceneObject *obj)const;

		// ���Ogre�ĳ���������
		virtual SceneManager *getOgreSceneManager()const;
		// ������ж�����б�
		virtual SceneObjectIterator getSceneObjectIterator()
		{
			return SceneObjectIterator(m_mapSceneObject.begin() , m_mapSceneObject.end());
		};
		// ����������
		bool renameSceneObject(const String &oldName , const String &newName);

		// ���һ��������ΨһID
		virtual uint32 getUniqueID();
		// ���һ��������Ψһ����
		virtual String getUniqueName();

		// ��ø���������
		virtual ISceneObject *getRootObject()
		{
			return m_rootObject;
		}

		// ֡��������ÿִ֡��
		bool frameStarted( const FrameEvent& evt );
		// �����ײ����
		virtual ICollisionWorld *getCollisionWorld()
		{
			return m_collisionWorld;
		}
		// ��ô���������
		virtual ITriggerWorld *getTriggerWorld()
		{
			return m_triggerWorld;
		}

		// ������ײ���
		// @ray ����
		// @collideWorld �Ƿ��������ײ���磺���Σ�
		// @collideObject �Ƿ�������ײ
		// @info ���������ײ�����Ϣ
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , RayCheckInfo& info);

		// ������ײ��⣬�����ص�һ��������ײ����Ϣ
		// @ray ����
		// @collideWorld �Ƿ��������ײ���磺���Σ�
		// @collideObject �Ƿ�������ײ
		// @intersection �������
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , Vector3& intersection);

		// ������ײ���
		// @ray ����
		// @collideWorld �Ƿ��������ײ���磺���Σ�
		// @collideObject �Ƿ�������ײ
		// @infoList ���������ײ�����Ϣ
		// @sortByDistance �����infoList����������
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , bool sortByDistance , ListRayCheckInfo& info);

		// ������ײ���
		// @ray ����
		// @collideWorld �Ƿ��������ײ���磺���Σ�
		// @collideObject �Ƿ�������ײ
		// @infoList ���������ײ�����Ϣ
		// @sortByDistance �����infoList����������
		// @returnFirstOnly ֻ���ص�һ����ײ����
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , bool sortByDistance , bool returnFirstOnly , ListRayCheckInfo& info);

	protected:
		// ��������
		String m_name;

		// ���������б�
		MapSceneObject m_mapSceneObject;

		// Ogre�ĳ���������
		SceneManager *m_sceneMgr;

		// ������ļ���
		String m_origin;

		// ����ID
		uint32 m_index;

		// ����������
		ISceneObject *m_rootObject;

		// ��ײ����
		ICollisionWorld *m_collisionWorld;
		// ���߲�ѯ��
		RaySceneQuery *m_rayQuery;

		// ����������
		ITriggerWorld *m_triggerWorld;

		typedef std::map<String , IAnimationStateController*> MapAnimationStateController;
		// ���ڲ��ŵĶ����б�
		MapAnimationStateController m_mapAnimationStateController;
	};

};
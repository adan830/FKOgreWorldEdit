#include "ISceneSystem.h"
#pragma once
namespace Ogre
{
	// ����ϵͳʵ��
	class DISPLAYLAYER_API SceneSystem : public ISceneSystem
	{
	public:
		SceneSystem(void);
		virtual ~SceneSystem(void);

		// ���һ��ȫ��ΨһID
		virtual uint32 getUniqueID();
		// ���һ��ȫ��Ψһ����
		virtual String getUniqueName();

		// ����ʵ�����ƺ��ļ�������һ������
		virtual IScene* loadScene(const String &name , const String &fileName);
		// �����ļ�������һ������
		virtual IScene* loadScene(const String &fileName , IScene *scene = 0);
		// �����ļ�������һ������
		virtual bool saveScene(IScene* scene , const String &fileName);
		// ����ʵ�����ƺ��ļ�������һ������
		virtual bool saveScene(const String &name , const String &fileName);

		// ����һ������
		virtual IScene* createScene(const String &name);
		// ����һ������
		virtual IScene* createScene();
		// ���ٳ���
		virtual void destroyScene(const String &name);
		virtual void destroyScene(IScene *scene);
		// ���һ������
		virtual IScene *getScene(const String &name);
		// �Ƿ����ָ�����ֵĳ���
		virtual bool hasScene(const String &name)const;
		// ����ʹ�����Ogre����������
		virtual void setOgreSceneManager(SceneManager *mgr)
		{
			m_sceneMgr = mgr;
		}
		// ���Ogre����������
		virtual SceneManager *getOgreSceneManager()const
		{
			return m_sceneMgr;
		}

		// ����ʹ�������ײ����
		virtual void setCollisionWorld(ICollisionWorld *world)
		{
			m_collisionWorld = world;
		}
		// �����ײ����
		virtual ICollisionWorld *getCollisionWorld()const
		{
			return m_collisionWorld;
		}

		// ����ʹ���������������
		virtual void setTriggerWorld(ITriggerWorld *world)
		{
			m_triggerWorld = world;
		}
		// ��ô���������
		virtual ITriggerWorld *getTriggerWorld()const
		{
			return m_triggerWorld;
		}


		// Ϊһ��Viewport�����˶�ģ��
		virtual void setMotionBlurEnabled(Viewport *vp , bool enable);


	protected:

		// ȷ��һ��������Ѿ���ӵ�vp��
		void ensureCompositorAdd(Viewport *vp , const String &compositor);
		// ��vp����������ĳ��compositor
		void setCompositorEnabled(Viewport *vp , const String &compositor , bool enable);

		typedef std::map<String , IScene*> MapScene;
		// �����б�
		MapScene m_mapScene;
		// Ogre����������
		SceneManager *m_sceneMgr;

		// ��ײ����
		ICollisionWorld *m_collisionWorld;
		// ����������
		ITriggerWorld *m_triggerWorld;

		// �˶�ģ���Ļ����
		CompositorPtr m_motionBlur;
	};
};
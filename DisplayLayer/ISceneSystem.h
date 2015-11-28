#pragma once
#include "ISceneObject.h"
#include "TFactorySystem.h"
namespace Ogre
{
	class IScene;
	class ETTerrain;
	// ����ϵͳ��һ������ϵͳ�����ɶ���������
	class DISPLAYLAYER_API ISceneSystem : public Singleton<ISceneSystem>
		, public TFactorySystem<ISceneObjectFactory>
	{
	public:

		ISceneSystem(void)
		{
		}

		virtual ~ISceneSystem(void)
		{
		}
		// ����ģʽ�ӿ�
		static ISceneSystem& getSingleton(void);
		static ISceneSystem* getSingletonPtr(void);

		// ��ʼ������ϵͳ
		static void init(SceneManager *sceneMgr);
		// ���ٳ���ϵͳ
		static void shutdown();

		// ���һ��ȫ��ΨһID
		virtual uint32 getUniqueID() = 0;
		// ���һ��ȫ��Ψһ����
		virtual String getUniqueName() = 0;

		// �����ļ�������һ������
		virtual bool saveScene(IScene* scene , const String &fileName) = 0;
		// ����ʵ�����ƺ��ļ�������һ������
		virtual bool saveScene(const String &name , const String &fileName) = 0;
		// ����ʵ�����ƺ��ļ�������һ������
		virtual IScene* loadScene(const String &name , const String &fileName) = 0;
		// �����ļ�������һ������
		virtual IScene* loadScene(const String &fileName , IScene *scene = 0) = 0;
		// ����һ������
		virtual IScene* createScene(const String &name) = 0;
		// ����һ������
		virtual IScene* createScene() = 0;
		// ���ٳ���
		virtual void destroyScene(const String &name) = 0;
		virtual void destroyScene(IScene *scene) = 0;
		// ���һ������
		virtual IScene *getScene(const String &name) = 0;
		// �Ƿ����ָ�����ֵĳ���
		virtual bool hasScene(const String &name)const = 0;

		// ����ʹ�����Ogre����������
		virtual void setOgreSceneManager(SceneManager *mgr) = 0;
		// ���Ogre����������
		virtual SceneManager *getOgreSceneManager()const = 0;

		// ����ʹ�������ײ����
		virtual void setCollisionWorld(ICollisionWorld *world) = 0;
		// �����ײ����
		virtual ICollisionWorld *getCollisionWorld()const = 0;

		// ����ʹ���������������
		virtual void setTriggerWorld(ITriggerWorld *world) = 0;
		// ��ô���������
		virtual ITriggerWorld *getTriggerWorld()const = 0;

		// Ϊһ��Viewport�����˶�ģ��
		virtual void setMotionBlurEnabled(Viewport *vp , bool enable) = 0;
	};
};